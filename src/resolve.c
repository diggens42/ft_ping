/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 00:34:39 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/15 15:00:49 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

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

    //store resolved ip
	if (!inet_ntop(AF_INET, &conf->dest.sin_addr, conf->res_ip, INET_ADDRSTRLEN))
	{
		fprintf(stderr, "resolve_hostname: inet_ntop failed\n");
		freeaddrinfo(addr_list);
		return (false);
	}

	if (!HAS_FLAG(conf, FLAG_NUMERIC))
	{
		struct hostent *host = gethostbyaddr(&conf->dest.sin_addr, sizeof(conf->dest.sin_addr), AF_INET);
		if (host && host->h_name)
			ft_strlcpy(conf->hostname, host->h_name, sizeof(conf->hostname));
		else
			ft_strlcpy(conf->hostname, conf->res_ip, sizeof(conf->hostname));
	}
	else
		ft_strlcpy(conf->hostname, conf->res_ip, sizeof(conf->hostname));

	freeaddrinfo(addr_list);
	return (true);
}
