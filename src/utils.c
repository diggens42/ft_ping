/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 02:45:50 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/06 03:06:22 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

void    ft_error(const char *msg)
{
    fprintf(stderr, "ft_ping: %s\n", msg);
}

void    print_use(const char *exec_name)
{
    printf("Usage: %s [OPTION...] HOST\n", exec_name);
	printf("Options:\n");
	printf("    -v              verbose output\n");
	printf("    -?, --help      give this help list\n");
}

void	print_help(void)
{
	printf("ft_ping - send packets to network hosts\n");
	printf("Usage: ft_ping [OPTION...] HOST\n");
	printf("Options:\n");
	printf("    -v              verbose output\n");
	printf("    -?, --help      give this help list\n");
}