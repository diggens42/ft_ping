/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 02:45:50 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/06 03:15:49 by fwahl            ###   ########.fr       */
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
	printf("Usage:\n");
	printf("	ft_ping [options] <destination>\n\n");
	printf("Options:\n");
	printf("	<destination>      dns name or ip address");
	printf("    -v              verbose output\n");
	printf("    -?, --help      give this help list\n");
}