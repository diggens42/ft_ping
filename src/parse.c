/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 02:47:36 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/07 23:30:18 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

static bool parse_verbose(const char *arg, t_conf *conf)
{
    if (ft_strcmp(arg, "-v") == 0)
    {
        conf->verbose = 1;
        return (true);
    }

    return (false);
}

static bool parse_help(const char *arg, t_conf *conf)
{
    if (ft_strcmp(arg, "-?") == 0 || ft_strcmp(arg, "--help") == 0 || ft_strcmp(arg, "-h") == 0)
    {
        conf->help = true;
        return (true);
    }
    
    return (false);
}

static bool parse_bypass_route(const char *arg, t_conf *conf)
{
    if (ft_strcmp(arg,"-r") == 0)
    {
        conf->bypass_route = true;
        return (true);
    }
    return (false);
}

static int parse_ttl(const char *arg, const char *next_arg, t_conf *conf)
{
    if (ft_strcmp(arg,"-ttl") == 0 || ft_strcmp(arg, "--ttl") == 0)
    {
        if(!next_arg)
        {
            ft_error("parse: opt '-ttl' requires arg\n");
            return (-1);
        }
        
        int ttl = ft_atoi(next_arg);
        if (ttl <= 0 || ttl > 255 )
        {
            fprintf("parse: invalid TTL value -- '%s'\n", next_arg);
            return (-1);
        }

        conf->ttl = ttl;
        return (1); //arg consumed
    }
    return (0);
}

static bool parse_no_arg_flag(const char *arg, t_conf *conf)
{
    return (parse_verbose(arg, conf) 
         || parse_help(arg, conf) 
         || parse_bypass_route(arg, conf));
}

static int  parse_arg_flag(const char *arg, const char *next_arg, t_conf *conf)
{
    int res;

    if ((res = parse_ttl(arg, next_arg, conf)) != 0)
        return (res);
    
    return (0);
}

static bool parse_target(const char *arg, t_conf *conf)
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


bool    parse(int argc, char **argv, t_conf *conf)
{
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            if (parse_no_arg_flag(argv[i], conf))
                continue ;

            const char *next_arg = (i + 1 < argc) ? argv[i + 1] : NULL;
            int arg_res = parse_arg_flag(argv[i], next_arg, conf);

            if (arg_res == 1)
            {
                i++;
                continue ;
            }
            else if (arg_res == -1)
                return (false);
            fprintf(stderr, "parse: invalid option -- '%s'\n", argv[i]);
            return (false);
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