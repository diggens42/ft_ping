/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 02:45:50 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/07 00:23:53 by fwahl            ###   ########.fr       */
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

bool	resolve_hostname(t_conf *conf)
{
	struct addrinfo		addr_hints, *addr_list;

	ft_memset(&addr_hints, 0, sizeof(addr_hints));
	addr_hints.ai_family = AF_INET;
	addr_hints.ai_socktype = SOCK_RAW;

	int ret = getaddrinfo(conf->tar, NULL, &addr_hints, &addr_list);
	if (ret != 0)
	{
		fprintf(stderr, "resolve_hostname: cannot resolve %s: %s\n", conf->tar, gai_strerror(ret));
		return (false);
	}
	
	ft_memcpy(&conf->dest, addr_list->ai_addr, sizeof(struct sockaddr_in));
	
	if (!inet_ntop(AF_INET, &conf->dest.sin_addr, conf->res_ip, INET_ADDRSTRLEN))
	{
		ft_error("resolve_hostname: inet_ntop failed");
		freeaddrinfo(addr_list);
		return false;
	}

	freeaddrinfo(addr_list);
	
	if(!conf->res_ip)
	{
		ft_error("resolve_hostname: malloc failed");
		return (false);
	}
	
	return (true);
}