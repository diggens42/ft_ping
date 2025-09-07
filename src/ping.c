/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 05:56:17 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/07 02:29:14 by fwahl            ###   ########.fr       */
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
    struct timeval  *tv;
    
    //init packet & icmp header
    ft_memset(&packet, 0, sizeof(packet));
    packet.header.type = ICMP_ECHO;
    packet.header.code = 0;
    packet.header.un.echo.id = htons(conf->pid);
    packet.header.un.echo.sequence = htons(seq);
    packet.header.checksum = 0;

    // fill packet.data with timestamp and leftover bytes with pattern
    tv = (struct timeval *)packet.data;
    gettimeofday(tv, NULL);
    for (int i = sizeof(struct timeval); i < conf->packet_size; i++)
        packet.data[i] = i;

    packet.header.checksum = get_checksum(&packet, sizeof(packet.header) + conf->packet_size);
    
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
    
}


void    ping(t_ping *ping)
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