/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 02:45:50 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/09 03:58:49 by fwahl            ###   ########.fr       */
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

double get_ms(struct timeval *sent, struct timeval *recv)
{
	double sent_ms, recv_ms;
	
	sent_ms = sent->tv_sec * 1000.0 + sent->tv_usec / 1000.0;
	recv_ms = recv->tv_sec * 1000.0 + recv->tv_usec / 1000.0;
	
	return (recv_ms - sent_ms);
}