/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 02:45:39 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/07 00:27:12 by fwahl            ###   ########.fr       */
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
        //add cleanup function here
        return (EXIT_FAILURE);
    }

    if (ping.conf.help)
    {
        print_help();
        //add  cleanup
        return (EXIT_SUCCESS);
    }

    if (!resolve_hostname(&ping.conf))
    {
        //cleanup()
        return (EXIT_FAILURE);
    }
    
    // create socket
    // signalhandler
    // start time
    // ping loop
    // free all

    return (EXIT_SUCCESS);
}