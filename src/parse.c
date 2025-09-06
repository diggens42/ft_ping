/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 02:47:36 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/06 03:42:32 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

static bool parse_verbose(const char *arg, t_ping_conf *conf)
{
    if (ft_strcmp(arg, "-v") == 0)
    {
        conf->verbose = 1;
        return (true);
    }

    return (false);
}

static bool parse_help(const char *arg, t_ping_conf *conf)
{
    if (ft_strcmp(arg, "-?") == 0 || ft_strcmp(arg, "--help") == 0 || ft_strcmp(arg, "-h") == 0)
    {
        conf->help = true;
        return (true);
    }
    
    return (false);
}

static bool parse_flag(const char *arg, t_ping_conf *conf)
{
    return (parse_verbose(arg, conf) || parse_help(arg, conf));
}

static bool parse_target(const char *arg, t_ping_conf *conf)
{
    if (!conf->tar)
    {
        conf->tar = ft_strdup(arg);
        if (!conf->tar)
        {
            ft_error("parse: parse target malloc failed\n");
            return (false);
        }
        return (true);
    }
    
    ft_error("parse: parse target extra operand\n");
    return (false);
}


bool    parse(int argc, char **argv, t_ping_conf *conf)
{
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            if (!parse_flag(argv[i], conf))
            {
                fprintf(stderr, "parse: invalid option -- '%s'\n", argv[i]);
                return (false);
            }
        }
        else
        {
            if (!parse_target(argv[i], conf))
                return (false);
        }
    }

    if (!conf->help && !conf->tar)
	{
		ft_error("parse: missing destination\n");
		print_usage(argv[0]);
		return (false);
	}
    
    return (true);
}