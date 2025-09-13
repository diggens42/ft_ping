/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 05:56:17 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/13 04:40:50 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

static bool handle_count(t_conf *conf, int seq)
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
    
    if (!g_run || HAS_FLAG(conf, FLAG_FLOOD))
        return;
    
    gettimeofday(&now, NULL);
    ft_timesub(&diff, &now, &timing->last_send);
    elapsed_us = ft_time_to_us(&diff);
    wait_us = (conf->opts.interval * 1000000) - elapsed_us;
    
    if (wait_us > 0)
        usleep(wait_us);
    
    gettimeofday(&timing->last_send, NULL);
}

static bool send_ping(t_conf *conf, t_stat *stat, int seq)
{
    t_packet        packet;
    struct timeval  tv_now;

    //init packet & icmp header
    ft_memset(&packet, 0, sizeof(packet));
    packet.header.type = ICMP_ECHO;
    packet.header.code = 0;
    packet.header.un.echo.id = htons(conf->pid);
    packet.header.un.echo.sequence = htons(seq);
    packet.header.checksum = 0;

    // fill packet.data with timestamp and leftover bytes with pattern
    gettimeofday(&tv_now, NULL);
    memcpy(packet.data, &tv_now, sizeof(tv_now));
    for (int i = sizeof(tv_now); i < conf->opts.packet_size; i++)
        packet.data[i] = i;

    packet.header.checksum = get_checksum(&packet, sizeof(packet.header) + conf->opts.packet_size);

    //send packet
    ssize_t nbytes = sendto(conf->socket_fd, &packet, sizeof(packet.header) + conf->opts.packet_size, 0, (struct sockaddr *)&conf->dest, sizeof(conf->dest));
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
    struct timeval      now, *sent_tv;


    ft_memset(&msg, 0, sizeof(msg));
    msg.msg_name = &from;
    msg.msg_namelen = sizeof(from);
    iov.iov_base = buf;
    iov.iov_len = sizeof(buf);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    ssize_t nbytes = recvmsg(conf->socket_fd, &msg, 0);
    if (nbytes < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            if (HAS_FLAG(conf, FLAG_VERBOSE))
                printf("recv_ping: req timeout icmp_seq %d\n", stat->sent);
            stat->lost++;
            return (false);
        }
        if (HAS_FLAG(conf, FLAG_VERBOSE))
            fprintf(stderr, "recv_ping: recvmsg %s\n", strerror(errno));
        return (false);
    }

    gettimeofday(&now, NULL);

    struct iphdr *ip = (struct iphdr *)buf;
    int ip_hlen = ip->ihl * 4;

    struct icmphdr *icmp = (struct icmphdr *)(buf + ip_hlen);

    //check if its correct echo reply
    if (ntohs(icmp->un.echo.id) == conf->pid && icmp->type == ICMP_ECHOREPLY)
    {
        stat->recv++;
        sent_tv = (struct timeval *)(buf + ip_hlen +sizeof(struct icmphdr));
        double rtt = get_ms(sent_tv, &now);
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

        //reply
        printf("%ld bytes from %s: icmp_seq=%d ttl=%d time=%.1f ms\n",
                nbytes-ip_hlen, conf->res_ip, ntohs(icmp->un.echo.sequence), ip->ttl, rtt);
        return (true);
    }
    else if (HAS_FLAG(conf, FLAG_VERBOSE))
    {
        //other icmp types with verbose flag
        if (icmp->type == ICMP_DEST_UNREACH)
            printf("From %s: dest unreachable\n", inet_ntoa(from.sin_addr));
        else if (icmp->type == ICMP_TIME_EXCEEDED)
            printf("From %s: ttl exceeded\n", inet_ntoa(from.sin_addr));
        else
            printf("From %s: type=%d code=%d\n", inet_ntoa(from.sin_addr),icmp->type, icmp->code);
    }
    return (false);
}


void ft_ping(t_ping *ping)
{
    int         seq = 0;
    t_timing    timing;
    
    gettimeofday(&timing.start, NULL);
    gettimeofday(&timing.last_send, NULL);
    ping->stat.start = timing.start;
    
    if (!HAS_FLAG(&ping->conf, FLAG_QUIET))
        print_ping_header(&ping->conf);
    
    while (g_run)
    {
        if (handle_timeout(&ping->conf, &timing)) //-w flag
            break;
        if (!send_ping(&ping->conf, &ping->stat, seq))
        {
            if (HAS_FLAG(&ping->conf, FLAG_VERBOSE))
                fprintf(stderr, "ping: failed to send packet\n");
        }
        
        recv_ping(&ping->conf, &ping->stat);
        seq++;
        if (handle_count(&ping->conf, seq)) //-c flag
            break;
        handle_interval(&ping->conf, &timing); // -i flag
    }
}

