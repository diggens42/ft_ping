/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 02:45:50 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/13 04:40:19 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

void	print_help(void)
{
	printf("\nUsage: ft+ping [OPTION...] HOST ...\n");
	printf("Send ICMP ECHO_REQUEST packets to network hosts.\n\n");
	printf("Options valid for all request types:\n");
	printf("  -c, --count=COUNT   stop after sending (and receiving) COUNT ECHO_RESPONSE packets\n");
	printf("  -r, --ignore-routing\n");
    printf("                       bypass normal routing and send directly to a host on an attached network\n");
	printf("  -v, --verbose       verbose output; lists non-ECHO_RESPONSE ICMP packets received\n");
	printf("\n");
	printf("  -?, --help          display this help list\n");
	
}

void print_ping_header(t_conf *conf)
{
	printf("PING %s (%s) %d(%d) bytes of data.\n",
		   conf->tar, 
		   conf->res_ip,
		   conf->opts.packet_size,
		   conf->opts.packet_size + 28);  // +20 IP +8 ICMP headers
}

double	get_ms(struct timeval *sent, struct timeval *recv)
{
	struct timeval	diff;
	
	ft_time_substract(&diff, recv, sent);
	return (ft_time_to_ms(&diff));
}

uint16_t get_checksum(void *data, int len)
{
    uint32_t    *buf32 = (uint32_t*)data;  // Process 4 bytes at a time
    uint32_t    sum = 0;
    
    // Process 32-bit chunks for speed
    while (len >= 4) {
        sum += *buf32++;
        len -= 4;
    }
    
    // Handle remaining bytes
    uint16_t *buf16 = (uint16_t*)buf32;
    if (len >= 2) {
        sum += *buf16++;
        len -= 2;
    }
    
    if (len == 1)
        sum += *(uint8_t*)buf16;
    
    // Fold 32-bit sum to 16 bits
    while (sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);
    
    return ((uint16_t)(~sum));
}
