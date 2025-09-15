/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 02:45:50 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/15 17:40:43 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

void print_help(void)
{
    printf("Usage: ft_ping [OPTION...] HOST ...\n");
    printf("Send ICMP ECHO_REQUEST packets to network hosts.\n\n");

    printf("Options controlling ICMP request types:\n");
    printf("  -t, --type=TYPE     send TYPE packets\n\n");

    printf("Options valid for all request types:\n");
    printf("  -c, --count=COUNT   stop after sending (and receiving) COUNT ECHO_RESPONSE\n");
    printf("                       packets\n");
    printf("  -d, --debug         set the SO_DEBUG option on the socket being used\n");
    printf("  -i, --interval=WAIT wait WAIT seconds between sending each packet\n");
    printf("  -n, --numeric       numeric output only; no attempt will be made to lookup\n");
    printf("                       symbolic names for host addresses\n");
    printf("  -p, --pattern=PATTERN\n");
    printf("                       specify up to 16 \"pad\" bytes to fill out the packet\n");
    printf("                       you send\n");
    printf("  -q, --quiet         quiet output; only summary is printed at start and\n");
    printf("                       finish\n");
    printf("  -r, --ignore-routing\n");
    printf("                       bypass normal routing and send directly to a host on\n");
    printf("                       an attached network\n");
    printf("  -s, --size=PACKETSIZE\n");
    printf("                       specify the number of data bytes to send\n");
    printf("  -v, --verbose       verbose output; lists non-ECHO_RESPONSE ICMP packets\n");
    printf("                       received\n");
    printf("  --ttl=N             set packet time-to-live to N\n");
    printf("  -T, --tos=NUM       set Type-of-Service (TOS) to NUM\n");
    printf("  -w, --timeout=N     stop after N seconds of sending packets\n");
    printf("  -W, --linger=N      number of seconds to wait for response before quitting\n\n");

    printf("  -?, --help          display this help list\n");
}

void print_stats(t_ping *ping)
{
    struct timeval end, diff;
    gettimeofday(&end, NULL);
    ft_time_substract(&diff, &end, &ping->stat.start);
    long duration_ms = ft_time_to_ms(&diff);

    printf("\n--- %s ping statistics ---\n", ping->conf.tar);

    uint32_t lost = ping->stat.sent - ping->stat.recv;
    float loss_percent = ping->stat.sent > 0 ? (100.0 * lost / ping->stat.sent) : 0;

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
        if (variance < 0) variance = 0;
        double stddev = sqrt(variance);

        printf("round trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n",
               ping->stat.min_rtt, avg_rtt, ping->stat.max_rtt, stddev);
    }

}

void print_ping_header(t_conf *conf)
{
	printf("PING %s (%s) %d(%d) bytes of data.",
		   conf->tar,
		   conf->res_ip,
		   conf->opts.packet_size,
		   conf->opts.packet_size + 28);  // +20 IP +8 ICMP headers
    if (HAS_FLAG(conf, FLAG_VERBOSE))
        printf("id 0x%04x = %u\n", conf->pid, conf->pid);
    else
        printf("\n");

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
    while (len > 1) {
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
