/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 05:56:17 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/07 18:09:06 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

// int ping_loop()
// {
//     while (g_run) {
//     // 1. Prepare ICMP packet
//     // 2. Send packet using sendto()
//     // 3. Receive reply using recvmsg()
//     // 4. Parse and validate reply
//     // 5. Calculate RTT
//     // 6. Display results
//     // 7. Update statistics
//     // 8. Wait 1 second (usleep(1000000))
// }
// }

static uint16_t get_checksum(void *data, int len)
{
    uint16_t    *buf = data;
    uint32_t    sum = 0;
    
    while (len > 1)
    {
        sum += *buf++;
        len -= 2;
    }
    
    if (len == 1)
        sum += *(uint8_t*)buf;
    
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);

    return ((u_int16_t)(~sum));
}

static bool send_ping(t_conf *conf, t_status *status, int seq)
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
    for (int i = sizeof(tv_now); i < conf->packet_size; i++)
        packet.data[i] = i;

    packet.header.checksum = get_checksum(&packet, sizeof(packet.header) + conf->packet_size);
    
    //send packet
    ssize_t nbytes = sendto(conf->socket_fd, &packet, sizeof(packet.header) + conf->packet_size, 0, (struct sockaddr *)&conf->dest, sizeof(conf->dest));
    if (nbytes < 0)
    {
        if (conf->verbose)
            fprintf(stderr, "send_ping: sendto %s\n", strerror(errno));
        return (false);
    }
    status->sent++;
    
    return (true);
}


static bool recv_ping(t_conf *conf, t_status *status)
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
            if (conf->verbose)
                printf("recv_ping: req timout icmp_seq %d\n", status->sent);
            status->err++;
            return (false);
        }
        if (conf->verbose)
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
        status->recv++;
        sent_tv = (struct timeval *)(buf + ip_hlen +sizeof(struct icmphdr));
        double rtt = get_ms(sent_tv, &now);
        if (status->recv == 1 || rtt < status->min_rtt)
            status->min_rtt = rtt;
        else
            status->max_rtt = rtt;
        status->sum_rtt += rtt;
        
        //reply
        printf("%ld bytes from %s: icmp_seq=%d ttl=%d time=%.1f ms\n",
                nbytes-ip_hlen, conf->res_ip, ntohs(icmp->un.echo.sequence), ip->ttl, rtt);
        return (true);
    }
    else if (conf->verbose)
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


void    ft_ping(t_ping *ping)
{
    int seq = 1;
    
    while (g_run)
    {
        if (!send_ping(&ping->conf, &ping->status, &seq))
        {
            if (ping->conf.verbose)
                fprintf(stderr, "ping loop: failed to send packet\n");
        }
        
        recv_ping(&ping->conf, &ping->status);
        seq++;
        
        if (g_run)
        {
            alarm(PING_INTERVAL);
            pause(); // not sure if allowed checking later
        }
    }
}