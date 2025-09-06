/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 02:45:39 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/07 01:07:57 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"


int main(int argc, char **argv)
{
    t_ping ping;

    if (!init(&ping))
        return (EXIT_ERROR);
    
    if (!parse(argc, argv, &ping.conf))
    {
        //cleanup()
        return (EXIT_FAILURE);
    }

    if (ping.conf.help)
    {
        print_help();
        //cleanup()
        return (EXIT_SUCCESS);
    }

    if (!resolve_hostname(&ping.conf))
    {
        //cleanup()
        return (EXIT_FAILURE);
    }
    
    if (!create_socket(&ping.conf.socket_fd))
    {
        //cleanup()
        return (EXIT_FAILURE);
    }
    // signalhandler
    // start time
    // ping loop
    // free all

    return (EXIT_SUCCESS);
}