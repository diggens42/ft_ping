/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 00:36:41 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/14 18:59:13 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

// -T, --tos=NUM --- sete Type-of-Service to NUM (IP level)
static void handle_tos(t_conf *conf)
{
    if (conf->opts.tos == 0)
        return ;
    
    if (setsockopt(conf->socket_fd, IPPROTO_IP, IP_TOS, &conf->opts.tos, sizeof(conf->opts.tos)) < 0)
        fprintf(stderr, "Warning: IP_TOS: %s",strerror(errno));
}

// -d, --debug ---  enables debugging info (socket level), set SO_DEBUG option on socket being used
static void handle_debug(t_conf *conf)
{
    if (!HAS_FLAG(conf, FLAG_DEBUG))
        return ;
    
    int val = 1;
    if (setsockopt(conf->socket_fd, SOL_SOCKET, SO_DEBUG, &val, sizeof(val)) < 0)
        fprintf(stderr, "Warning: SO_DEBUG: %s\n", strerror(errno));
}

// --ttl=N --- set packet time-to-live to N (IP level)
static void handle_ttl(t_conf *conf)
{
    if (conf->opts.ttl == 0)
        return ;
    if (setsockopt(conf->socket_fd, IPPROTO_IP, IP_TTL, &conf->opts.ttl, sizeof(conf->opts.ttl)) < 0)
        fprintf(stderr, "Warning: IP_TTL: %s\n", strerror(errno));
}


// -W , --linger --- SO_RCVTIMEO socket option is per-packet receive timeout (socket level)
static void handle_linger(t_conf *conf)
{
    struct timeval tv;
    tv.tv_sec = conf->opts.linger > 0 ? conf->opts.linger : 1;
    tv.tv_usec = 0;

    if (setsockopt(conf->socket_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0)
        fprintf(stderr, "Warning: SO_RCVTIMEO: %s\n", strerror(errno));
    
}

// -r, --ignore-routing --- bypass routing tables (socket level) and send diectly to host on attached network
static bool handle_noroute(t_conf *conf)
{
    if (!HAS_FLAG(conf, FLAG_NOROUTE))
        return (true);
    
    int val = 1;
    if (setsockopt(conf->socket_fd, SOL_SOCKET, SO_DONTROUTE, &val, sizeof(val)) < 0)
    {
        fprintf(stderr, "Error: SO_DONTROUTE %s\n", strerror(errno));
        close (conf->socket_fd);
        return (false);
    }
    return (true);
}

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
            fprintf(stderr, "create_socket: socket %s\n", strerror(errno));
        return (false);
    }

    handle_linger(conf);
    handle_ttl(conf);
    handle_tos(conf);
    handle_debug(conf);

    //only noroute can be fatal
    if (!handle_noroute(conf))
        return (false);

    return (true);
}
