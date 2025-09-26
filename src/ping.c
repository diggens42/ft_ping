/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 05:56:17 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/26 17:23:11 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

static void handle_numeric(t_conf *conf, struct sockaddr_in *from, char *display_addr, size_t addr_len)
{
    if (HAS_FLAG(conf, FLAG_NUMERIC))
        inet_ntop(AF_INET, &from->sin_addr, display_addr, addr_len);
    else
    {
        struct hostent *host = gethostbyaddr(&from->sin_addr, sizeof(from->sin_addr), AF_INET);
        if (host && host->h_name)
            ft_strlcpy(display_addr, host->h_name, addr_len);
        else
            inet_ntop(AF_INET, &from->sin_addr, display_addr, addr_len);
    }
}

static void handle_verbose(t_ping *ping, t_packet_info *pkt)
{
    t_conf *conf = &ping->conf;

    if (!HAS_FLAG(conf, FLAG_VERBOSE))
        return;

    char display_addr[256];
    handle_numeric(conf, &pkt->from, display_addr, sizeof(display_addr));

    switch (pkt->icmp->type)
    {
        case ICMP_UNREACH:
            printf("%ld bytes from %s: Destination Unreachable", pkt->nbytes, display_addr);
            switch (pkt->icmp->code)
            {
                case ICMP_UNREACH_NET:
                    printf(" (Network Unreachable)");
                    break;
                case ICMP_UNREACH_HOST:
                    printf(" (Host Unreachable)");
                    break;
                case ICMP_UNREACH_PROTOCOL:
                    printf(" (Protocol Unreachable)");
                    break;
                case ICMP_UNREACH_PORT:
                    printf(" (Port Unreachable)");
                    break;
                case ICMP_UNREACH_NEEDFRAG:
                    printf(" (Fragmentation needed)");
                    break;
                case ICMP_UNREACH_SRCFAIL:
                    printf(" (Source Routing failed)");
                    break;
                default:
                    printf(" (Code %d)", pkt->icmp->code);
                    break;
            }
            printf("\n");
            print_ip_hdr_dump(pkt->ip);
            break;

        case ICMP_TIMXCEED:
            printf("%ld bytes from %s: Time to live exceeded", pkt->nbytes, display_addr);
            if (pkt->icmp->code == ICMP_TIMXCEED_INTRANS)
                printf(" (Time to live exceeded in transit)");
            else if (pkt->icmp->code == ICMP_TIMXCEED_REASS)
                printf(" (Fragment reassembly time exceeded)");
            printf("\n");
            print_ip_hdr_dump(pkt->ip);
            break;

        case ICMP_SOURCEQUENCH:
            printf("%ld bytes from %s: Source Quench\n", pkt->nbytes, display_addr);
            print_ip_hdr_dump(pkt->ip);
            break;

        case ICMP_REDIRECT:
            printf("%ld bytes from %s: Redirect", pkt->nbytes, display_addr);
            switch (pkt->icmp->code)
            {
                case ICMP_REDIRECT_NET:
                    printf(" (Redirect Network)");
                    break;
                case ICMP_REDIRECT_HOST:
                    printf(" (Redirect Host)");
                    break;
                case ICMP_REDIRECT_TOSNET:
                    printf(" (Redirect Type of Service and Network)");
                    break;
                case ICMP_REDIRECT_TOSHOST:
                    printf(" (Redirect Type of Service and Host)");
                    break;
            }
            printf("\n");
            print_ip_hdr_dump(pkt->ip);
            break;

        case ICMP_PARAMPROB:
            printf("%ld bytes from %s: Parameter Problem (Pointer = %d)\n",
                   pkt->nbytes, display_addr, pkt->icmp->un.gateway);
            print_ip_hdr_dump(pkt->ip);
            break;

        default:
            printf("%ld bytes from %s: type=%d code=%d\n",
                   pkt->nbytes, display_addr, pkt->icmp->type, pkt->icmp->code);
            if (pkt->icmp->type != ICMP_ECHOREPLY && pkt->icmp->type != ICMP_TSTAMPREPLY)
                print_ip_hdr_dump(pkt->ip);
            break;
    }
}

static bool handle_count(t_conf *conf, uint32_t seq)
{
    return (conf->opts.count > 0 && seq >= conf->opts.count);
}

static bool handle_timeout(t_conf *conf, t_timing *timing)
{
    struct timeval now;

    if (conf->opts.timeout == 0)
        return (false);

    gettimeofday(&now, NULL);
    return ((uint32_t)(now.tv_sec - timing->start.tv_sec) >= conf->opts.timeout);
}

static void handle_interval(t_conf *conf, t_timing *timing)
{
    struct timeval now, diff;
    long elapsed_us;
    long wait_us;

    if (!g_run)
        return;

    gettimeofday(&now, NULL);
    ft_time_substract(&diff, &now, &timing->last_send);
    elapsed_us = ft_time_to_us(&diff);
    wait_us = (long)(conf->opts.interval * 1000000) - elapsed_us;

    if (wait_us > 0)
        usleep(wait_us);

    gettimeofday(&timing->last_send, NULL);
}

static void handle_pattern(t_conf *conf, char *data, int offset)
{
    if (conf->opts.pattern_len > 0)
    {
        // Pattern flag on
        while (offset < conf->opts.packet_size)
        {
            int tmp = conf->opts.pattern_len;
            if (offset + tmp > conf->opts.packet_size)
                tmp = conf->opts.packet_size - offset;
            ft_memcpy(data + offset, conf->opts.pattern, tmp);
            offset += tmp;
        }
    }
    else
    {
        // Default pattern
        for (int i = offset; i < conf->opts.packet_size; i++)
            data[i] = i & 0xFF;
    }
}

static void handle_echo_reply(t_ping *ping, t_packet_info *pkt, char *buf)
{
    t_conf *conf = &ping->conf;
    t_stat *stat = &ping->stat;
    struct timeval now, *sent_tv, diff;
    double rtt;
    char display_addr[256];

    stat->recv++;

    // Extract timestamp and calculate RTT
    gettimeofday(&now, NULL);
    sent_tv = (struct timeval *)(buf + pkt->ip_hlen + sizeof(struct icmphdr));
    ft_time_substract(&diff, &now, sent_tv);
    rtt = ft_time_to_ms(&diff);

    // Update rtt statistics
    if (stat->recv == 1)
    {
        stat->min_rtt = rtt;
        stat->max_rtt = rtt;
    }
    else
    {
        if (rtt < stat->min_rtt)
            stat->min_rtt = rtt;
        if (rtt > stat->max_rtt)
            stat->max_rtt = rtt;
    }
    stat->sum_rtt += rtt;

    // Print echo reply (respects quiet flag)
    handle_numeric(conf, &pkt->from, display_addr, sizeof(display_addr));
    if (!HAS_FLAG(conf, FLAG_QUIET))
    {
        printf("%ld bytes from %s: icmp_seq=%d ttl=%d time=%.1f ms\n",
               pkt->nbytes - pkt->ip_hlen, display_addr,
               ntohs(pkt->icmp->un.echo.sequence), pkt->ip->ttl, rtt);
    }
}

static void handle_timestamp_reply(t_ping *ping, t_packet_info *pkt)
{
    t_conf *conf = &ping->conf;
    t_stat *stat = &ping->stat;
    char display_addr[256];

    uint32_t *timestamps = (uint32_t *)(pkt->icmp + 1);
    uint32_t otime = ntohl(timestamps[0]);
    uint32_t rtime = ntohl(timestamps[1]);
    uint32_t ttime = ntohl(timestamps[2]);

    stat->recv++;
    handle_numeric(conf, &pkt->from, display_addr, sizeof(display_addr));

    printf("%ld bytes from %s: icmp_seq=%d\n",
           pkt->nbytes, display_addr, ntohs(pkt->icmp->un.echo.sequence));
    printf("icmp_otime = %u\n", otime);
    printf("icmp_rtime = %u\n", rtime);
    printf("icmp_ttime = %u\n", ttime);
}

static bool send_ping(t_ping *ping, int seq)
{
    t_conf *conf = &ping->conf;
    t_stat *stat = &ping->stat;
    size_t packet_size;
    t_packet *packet;

    // Packet size based on request type
    if (conf->opts.packet_type == ICMP_TSTAMP)
        packet_size = sizeof(struct icmphdr) + 12; // Fixed 12 bytes for timestamps
    else
        packet_size = sizeof(struct icmphdr) + conf->opts.packet_size;

    packet = ft_calloc(1, packet_size);
    if (packet == NULL)
    {
        FT_ERROR();
        return (false);
    }

    packet->header.type = conf->opts.packet_type;
    packet->header.code = 0;
    packet->header.un.echo.id = htons(conf->pid & 0xFFFF);
    packet->header.un.echo.sequence = htons(seq);
    packet->header.checksum = 0;

    if (conf->opts.packet_type == ICMP_TSTAMP)
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);

        uint32_t ms_since_midnight = ((tv.tv_sec % 86400) * 1000) + (tv.tv_usec / 1000);
        uint32_t *timestamps = (uint32_t *)packet->data;
        timestamps[0] = htonl(ms_since_midnight); // Originate time
        timestamps[1] = 0;                        // Receive time (filled by responder)
        timestamps[2] = 0;                        // Transmit time (filled by responder)
    }
    else
    {
        // Echo request
        struct timeval tv_now;
        gettimeofday(&tv_now, NULL);
        ft_memcpy(packet->data, &tv_now, sizeof(tv_now));
        handle_pattern(conf, packet->data, sizeof(tv_now));
    }

    packet->header.checksum = get_checksum(packet, packet_size);

    // Send packet
    ssize_t nbytes = sendto(conf->socket_fd, packet, packet_size, 0,
                           (struct sockaddr *)&conf->dest, sizeof(conf->dest));
    free(packet);

    if (nbytes < 0)
    {
        if (HAS_FLAG(conf, FLAG_VERBOSE))
            fprintf(stderr, "send_ping: sendto %s\n", strerror(errno));
        return (false);
    }
    stat->sent++;
    return (true);
}

static bool recv_ping(t_ping *ping)
{
    t_conf *conf = &ping->conf;
    char buf[1024];
    struct iovec iov;
    struct msghdr msg;
    t_packet_info pkt;

    // Setup message structure for recvmsg
    ft_memset(&msg, 0, sizeof(msg));
    ft_memset(&pkt, 0, sizeof(pkt));
    msg.msg_name = &pkt.from;
    msg.msg_namelen = sizeof(pkt.from);
    iov.iov_base = buf;
    iov.iov_len = sizeof(buf);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    // MSG_DONTWAIT to make this non-blocking
    pkt.nbytes = recvmsg(conf->socket_fd, &msg, MSG_DONTWAIT);
    if (pkt.nbytes < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return (false);
        if (HAS_FLAG(conf, FLAG_VERBOSE))
            fprintf(stderr, "recvmsg: %s\n", strerror(errno));
        return (false);
    }

    // Parse packet headers
    pkt.ip = (struct iphdr *)buf;
    pkt.ip_hlen = pkt.ip->ihl * 4;
    pkt.icmp = (struct icmphdr *)(buf + pkt.ip_hlen);

    // Handle error messages
    if (pkt.icmp->type != ICMP_ECHOREPLY && pkt.icmp->type != ICMP_TSTAMPREPLY)
    {
        struct iphdr *og_ip_hdr = (struct iphdr *)(pkt.icmp + 1);
        struct icmphdr *og_icmp_hdr = (struct icmphdr *)((char *)og_ip_hdr + (og_ip_hdr->ihl * 4));

        if (ntohs(og_icmp_hdr->un.echo.id) == (conf->pid & 0xFFFF))
        {
            handle_verbose(ping, &pkt);
            return (true);
        }
        else
        {
            if (HAS_FLAG(conf, FLAG_VERBOSE))
                handle_verbose(ping, &pkt);
            return (false);
        }
    }

    // Check if it's our reply package
    if (ntohs(pkt.icmp->un.echo.id) != (conf->pid & 0xFFFF))
    {
        handle_verbose(ping, &pkt);
        return (false);
    }

    // Handle Timestamp Reply
    if (pkt.icmp->type == ICMP_TSTAMPREPLY)
    {
        handle_timestamp_reply(ping, &pkt);
        return (true);
    }

    // Handle Echo Reply
    if (pkt.icmp->type == ICMP_ECHOREPLY)
    {
        handle_echo_reply(ping, &pkt, buf);
        return (true);
    }

    handle_verbose(ping, &pkt);
    return (true);
}

void ft_ping(t_ping *ping)
{
    uint32_t seq = 0;
    t_timing timing;

    gettimeofday(&timing.start, NULL);
    gettimeofday(&timing.last_send, NULL);
    ping->stat.start = timing.start;
    print_ping_header(&ping->conf);

    while (g_run)
    {
        if (handle_timeout(&ping->conf, &timing)) // -w flag
            break;

        if (!send_ping(ping, seq))
        {
            if (HAS_FLAG(&ping->conf, FLAG_VERBOSE))
                fprintf(stderr, "ping: failed to send packet\n");
        }

        // Keep receiving until socket buffer is empty
        while (recv_ping(ping))
        {
            // recv_ping will return false when no more packets available
        }

        seq++;
        if (handle_count(&ping->conf, seq)) // -c flag
            break;
        handle_interval(&ping->conf, &timing); // -i flag
    }
}
