/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 05:56:17 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/25 18:57:21 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

static void handle_verbose(t_conf *conf, struct icmphdr *icmp, struct sockaddr_in *from)
{
    if (!HAS_FLAG(conf, FLAG_VERBOSE))
        return ;
    char addr_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &from->sin_addr, addr_str, sizeof(addr_str));

    switch (icmp->type)
    {
        case (ICMP_UNREACH):
            printf("From %s: Destination Unreachable\n", addr_str);
            break;
        case (ICMP_TIMXCEED):
            printf("From %s: Time to live exceeded\n", addr_str);
            break;
        case (ICMP_SOURCEQUENCH):
            printf("From %s: Source Quench\n", addr_str);
            break;
        case (ICMP_REDIRECT):
            printf("From %s: Redirect (type=%d, code=%d)\n",
                   addr_str, icmp->type, icmp->code);
            break;
        default:
            printf("From %s: type=%d code=%d\n",
                   addr_str, icmp->type, icmp->code);
    }
}

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
    return (now.tv_sec - timing->start.tv_sec >= conf->opts.timeout);
}

static void handle_interval(t_conf *conf, t_timing *timing)
{
    struct timeval  now, diff;
    long            elapsed_us;
    long            wait_us;

    if (!g_run)
        return ;

    gettimeofday(&now, NULL);
    ft_time_substract(&diff, &now, &timing->last_send);
    elapsed_us = ft_time_to_us(&diff);
    wait_us = (conf->opts.interval * 1000000) - elapsed_us;

    if (wait_us > 0)
        usleep(wait_us);

    gettimeofday(&timing->last_send, NULL);
}

static void handle_pattern(t_conf *conf, char *data, int offset)
{
    if (conf->opts.pattern_len > 0)
    {
        // pattern flag on
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
        //default pattern
        for (int i = offset;  i < conf->opts.packet_size; i++)
            data[i] = i & 0xFF;
    }

}

static bool send_ping(t_conf *conf, t_stat *stat, int seq)
{
    size_t packet_size;
    t_packet *packet;

    //packet size based on req type
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
        timestamps[1] = 0;                         // Receive time (filled by responder)
        timestamps[2] = 0;                         // Transmit time (filled by responder)
    }
    else
    {
        // echo req
        struct timeval tv_now;
        gettimeofday(&tv_now, NULL);
        ft_memcpy(packet->data, &tv_now, sizeof(tv_now));
        handle_pattern(conf, packet->data, sizeof(tv_now));
    }
    packet->header.checksum = get_checksum(packet, packet_size);
    // send packet
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

static bool recv_ping(t_conf *conf, t_stat *stat)
{
    char                buf[1024];
    struct iovec        iov;
    struct msghdr       msg;
    struct sockaddr_in  from;
    struct timeval      now, *sent_tv, diff;
    char                display_addr[256];
    ssize_t             nbytes;
    struct iphdr        *ip;
    struct icmphdr      *icmp;
    int                 ip_hlen;
    double              rtt;

    // Setup message structure for recvmsg
    ft_memset(&msg, 0, sizeof(msg));
    msg.msg_name = &from;
    msg.msg_namelen = sizeof(from);
    iov.iov_base = buf;
    iov.iov_len = sizeof(buf);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    // MSG_DONTWAIT to make this non-blocking
    nbytes = recvmsg(conf->socket_fd, &msg, MSG_DONTWAIT);
    if (nbytes < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return (false);
        if (HAS_FLAG(conf, FLAG_VERBOSE))
            fprintf(stderr, "recvmsg: %s\n", strerror(errno));
        return (false);
    }

    // Get current time and parse packet headers
    gettimeofday(&now, NULL);
    ip = (struct iphdr *)buf;
    ip_hlen = ip->ihl * 4;
    icmp = (struct icmphdr *)(buf + ip_hlen);

    //check if its our reply package
    if (ntohs(icmp->un.echo.id) != (conf->pid & 0xFFFF))
    {
        handle_verbose(conf, icmp, &from);
        return (false);
    }

    // Handle Timestamp Reply
    if (icmp->type == ICMP_TSTAMPREPLY)
    {
        uint32_t *timestamps = (uint32_t *)(icmp + 1);
        uint32_t otime = ntohl(timestamps[0]);
        uint32_t rtime = ntohl(timestamps[1]);
        uint32_t ttime = ntohl(timestamps[2]);

        stat->recv++;
        handle_numeric(conf, &from, display_addr, sizeof(display_addr));

        printf("%ld bytes from %s: icmp_seq=%d\n", nbytes, display_addr, ntohs(icmp->un.echo.sequence));
        printf("icmp_otime = %u\n", otime);
        printf("icmp_rtime = %u\n", rtime);
        printf("icmp_ttime = %u\n", ttime);

        return (true);
    }

    // Handle Echo Reply
    if (icmp->type == ICMP_ECHOREPLY)
    {
        stat->recv++;

        // Extract timestamp and calculate RTT
        sent_tv = (struct timeval *)(buf + ip_hlen + sizeof(struct icmphdr));
        ft_time_substract(&diff, &now, sent_tv);
        rtt = ft_time_to_ms(&diff);

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
        handle_numeric(conf, &from, display_addr, sizeof(display_addr));
        if (!HAS_FLAG(conf, FLAG_QUIET))
            printf("%ld bytes from %s: icmp_seq=%d ttl=%d time=%.1f ms\n", nbytes - ip_hlen, display_addr, ntohs(icmp->un.echo.sequence), ip->ttl, rtt);
        return (true);
    }

    handle_verbose(conf, icmp, &from);
    return (true);
}


void ft_ping(t_ping *ping)
{
    u_int32_t   seq = 0;
    t_timing    timing;

    gettimeofday(&timing.start, NULL);
    gettimeofday(&timing.last_send, NULL);
    ping->stat.start = timing.start;
    print_ping_header(&ping->conf);

    while (g_run)
    {
        if (handle_timeout(&ping->conf, &timing)) //-w flag
            break ;

        if (!send_ping(&ping->conf, &ping->stat, seq))
        {
            if (HAS_FLAG(&ping->conf, FLAG_VERBOSE))
                fprintf(stderr, "ping: failed to send packet\n");
        }

        // Keep receiving until socket buffer is empty
        while (recv_ping(&ping->conf, &ping->stat))
        {
            // recv_ping will return false when no more packets available
        }

        seq++;
        if (handle_count(&ping->conf, seq)) //-c flag
            break ;
        handle_interval(&ping->conf, &timing); // -i flag
    }
}

