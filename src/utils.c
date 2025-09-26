/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 02:45:50 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/26 20:20:22 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

void print_help(void)
{
    printf("Usage: ft_ping [OPTION...] HOST ...\n");
    printf("Send ICMP ECHO_REQUEST packets to network hosts.\n\n");

    printf("Options controlling ICMP request types:\n");
    printf("  -t, --type=TYPE            send TYPE packets\n\n");

    printf("Options valid for all request types:\n");
    printf("  -c, --count=NUMBER         stop after sending NUMBER packets\n");
    // printf("  -d, --debug                set the SO_DEBUG option\n");
    printf("  -i, --interval=NUMBER      wait NUMBER seconds between sending each packet\n");
    printf("  -n, --numeric              do not resolve host address\n");
    // printf("  -r, --ignore-routing       send directly to a host on an attached network\n");
    printf("      --ttl=N                specify N as time-to-live\n");
    printf("  -T, --tos=NUM              set Type-of-Service (TOS) to NUM\n");
    printf("  -v, --verbose              verbose output\n");
    printf("  -w, --timeout=N            stop after N seconds\n");
    // printf("  -W, --linger=N             number of seconds to wait for response\n\n");

    printf("Options valid for --echo requests:\n\n");
    // printf("  -p, --pattern=PATTERN      fill ICMP packet with given pattern (hex)\n");
    printf("  -q, --quiet                quiet output\n");
    printf("  -s, --size=NUMBER          send NUMBER of data octets\n");
    printf("  -?, --help                 display this help list\n\n");

    printf("Mandatory or optional arguments to long options are also mandatory or optional\n");
    printf("for any corresponding short options.\n\n");
    printf("Options marked with (root only) are available only to superuser\n\n");
    printf("Report bugs to <fwahl@student.42heilbronn.de>\n");
}

void print_stats(t_ping *ping)
{
    struct timeval end, diff;
    gettimeofday(&end, NULL);
    ft_time_substract(&diff, &end, &ping->stat.start);
    long duration_ms = ft_time_to_ms(&diff);

    printf("\n--- %s ping statistics ---\n", ping->conf.tar);

    ping->stat.lost = ping->stat.sent - ping->stat.recv;
    float loss_percent = ping->stat.sent > 0 ? (100.0 * ping->stat.lost / ping->stat.sent) : 0;

    printf("%d packets transmitted, %d received, ",
           ping->stat.sent, ping->stat.recv);

    if (ping->stat.lost > 0)
        printf("+%d errors, ", ping->stat.lost);

    printf("%.0f%% packet loss, time %ldms\n", loss_percent, duration_ms);

    if (ping->stat.recv > 0)
    {
        double avg_rtt = ping->stat.sum_rtt / ping->stat.recv;
        double sum_rtt_sq = ping->stat.sum_rtt * ping->stat.sum_rtt;
        double variance = (sum_rtt_sq / ping->stat.recv) - (avg_rtt * avg_rtt);
        if (variance < 0)
            variance = 0;
        double stddev = sqrt(variance);

        printf("round trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n",
               ping->stat.min_rtt, avg_rtt, ping->stat.max_rtt, stddev);
    }

}

void print_ping_header(t_conf *conf)
{
    if (conf->opts.packet_type == ICMP_ECHO)
    {
        printf("PING %s (%s) %d data bytes",
            conf->tar,
            conf->res_ip,
            conf->opts.packet_size);
        if (HAS_FLAG(conf, FLAG_VERBOSE))
            printf(", id 0x%04x = %u\n", conf->pid, conf->pid);
        else
            printf("\n");
    }
    else
    {
        printf("PING %s (%s) ", conf->tar, conf->res_ip);
        if (conf->opts.packet_type == ICMP_TSTAMP)
            printf("sending timestamp request\n");
        if (conf->opts.packet_type == ICMP_MASKREQ)
            printf("sending address mask request\n");
    }
}

void    print_ip_hdr_dump(struct iphdr *ip)
{
    uint8_t *hdr = (uint8_t *)ip;
    int     hdr_len = ip->ihl * 4;

    printf("IP Hdr Dump:\n ");
    for (int i = 0; i < hdr_len; i+=2)
    {
        if (i + 1 < hdr_len)
            printf("%02x%02x ", hdr[i], hdr[i + 1]);
        else
            printf("%02x00 ", hdr[i]);
    }
    printf("\n");
    printf("Vr HL TOS  Len   ID  Flg  off  TTL Pro  cks      Src       Dst\n");
    printf(" %x  %x  %02x %04x %04x   %x %04x  %02x  %02x %04x %s  %s\n",
           (ip->version),
           (ip->ihl),
           (ip->tos),
           ntohs(ip->tot_len),
           ntohs(ip->id),
           (ntohs(ip->frag_off) >> 13) & 0x7,  // flags
           ntohs(ip->frag_off) & 0x1fff,       // fragment offset
           (ip->ttl),
           (ip->protocol),
           ntohs(ip->check),
           inet_ntoa(*(struct in_addr *)&ip->saddr),
           inet_ntoa(*(struct in_addr *)&ip->daddr));
}

double	get_ms(struct timeval *sent, struct timeval *recv)
{
	struct timeval	diff;

	ft_time_substract(&diff, recv, sent);
	return (ft_time_to_ms(&diff));
}

uint16_t get_checksum(void *data, int len)
{
    uint16_t *buf = (uint16_t*)data;
    uint32_t sum = 0;

    // Sum all 16-bit words
    while (len > 1)
    {
        sum += *buf++;
        len -= 2;
    }

    // Add left-over byte, if any
    if (len == 1)
        sum += *(uint8_t*)buf;

    // Add the carry
    while (sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);

    // One's complement
    return (uint16_t)(~sum);
}
