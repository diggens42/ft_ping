/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 00:36:41 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/07 01:06:11 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

bool    create_socket(int socket_fd)
{
    int             ttl = 64; //time to live
    struct timeval  tv;

    //create socket
    socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (socket_fd < 0)
    {
        if (errno == EPERM || errno == EACCES)
            fprintf(stderr, "create_socket: no privilege for raw socket creation\n");
        else
            fprintf(stderr, "create_socket: socker %s\n", strerror(errno));
        return (false);
    }

    //set ttl
    if (setsockopt(socket_fd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0)
    {
        fprintf(stderr, "create_socket: setsockopt ttl %s\n", strerror(errno));
        close(socket_fd);
        return (false);
    }
    
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv) < 0))
    {
        fprintf(stderr, "create_socket: setsockopt timeout: %s\n", strerror(errno));
        close(socket_fd);
        return (false);
    }

    return (true);
}