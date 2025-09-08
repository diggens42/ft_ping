/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 02:45:50 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/09 01:01:06 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

void	print_help(void)
{
	printf("\nUsage\n");
	printf("  ft_ping [options] <destination>\n\n");
	printf("Options:\n");
	printf("  <destination>      dns name or ip address\n");
	printf("  -v              	verbose output\n");
	printf("  -h      			give this help list\n");
}

double get_ms(struct timeval *sent, struct timeval *recv)
{
	double sent_ms, recv_ms;
	
	sent_ms = sent->tv_sec * 1000.0 + sent->tv_usec / 1000.0;
	recv_ms = recv->tv_sec * 1000.0 + recv->tv_usec / 1000.0;
	
	return (recv_ms - sent_ms);
}