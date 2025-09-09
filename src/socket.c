/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 00:36:41 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/09 21:04:56 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

bool    create_socket(t_conf *conf)
{
    struct timeval  tv;

    //create socket
    conf->socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (conf->socket_fd < 0)
    {
        if (errno == EPERM || errno == EACCES)
            fprintf(stderr, "create_socket: no privilege for raw socket creation\n");
        else
            fprintf(stderr, "create_socket: socker %s\n", strerror(errno));
        return (false);
    }

    tv.tv_sec = 1;
    tv.tv_usec = 0;
    //set recv timout
    if (setsockopt(conf->socket_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, (sizeof(tv) < 0)))
    {
        fprintf(stderr, "create_socket: setsockopt timeout: %s\n", strerror(errno));
        close(conf->socket_fd);
        return (false);
    }

    // -ttl flag
    if (conf->ttl > 0)
    {
        if (setsockopt(conf->socket_fd, IPPROTO_IP, IP_TTL, &conf->ttl, sizeof(conf->ttl)) < 0)
        {
            fprintf(stderr, "create_socket: setsockopt ttl %s\n", strerror(errno));
            close(conf->socket_fd);
            return (false);
        }
    }

    // -r flag
    if (conf->bypass_route)
    {
        int val = 1;
        if (setsockopt(conf->socket_fd, SOL_SOCKET, SO_DONTROUTE, &val, sizeof(val)) < 0)
        {
            fprintf(stderr, "create_socket: setsockopt bypass_route: %s\n", strerror(errno));
            close(conf->socket_fd);
            return (false);
        }
    }

    return (true);
}
