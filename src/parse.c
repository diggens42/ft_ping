/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 02:47:36 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/09 19:13:32 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

//-v, --verbose in inetutils2.0 ping ref
static bool parse_verbose(const char *arg, t_conf *conf)
{
    if (ft_strcmp(arg, "-v") == 0 || ft_strcmp(arg, "--verbose"))
    {
        conf->verbose = 1;
        return (true);
    }

    return (false);
}
//-? , --help in inetutils2.0 ping ref
static bool parse_help(const char *arg, t_conf *conf)
{
    if (ft_strcmp(arg, "-?") == 0 || ft_strcmp(arg, "--help"))
    {
        conf->help = true;
        return (true);
    }

    return (false);
}
//-r, --ignore-routing in inetutils2.0 ping ref
static bool parse_bypass_route(const char *arg, t_conf *conf)
{
    if (ft_strcmp(arg, "-r") == 0 || ft_strcmp(arg, "--ignore-routing"))
    {
        conf->bypass_route = true;
        return (true);
    }
    return (false);
}

static int parse_ttl(const char *arg, const char *next_arg, t_conf *conf)
{
    if (ft_strcmp(arg,"-ttl") == 0)
    {
        if(!next_arg)
        {
            print_help();
            return (-1);
        }

        int ttl = ft_atoi(next_arg);
        if (ttl == 0)
            fprintf(stderr, "ft_ping: cannot set unicast time-to-live: Invalid argument\n");
        if (ttl < 0 || ttl > 255 )
        {
            fprintf(stderr, "ft_ping: invalid argument: '%s': out of range: 0 <= value <= 255\n", next_arg);
            return (-1);
        }

        conf->ttl = ttl;
        return (1); //arg consumed
    }
    return (0);
}
//-c / --count=N
static int parse_count(const char *arg, const char *next_arg, t_conf *conf)
{
    char    *val = NULL;
    int     used_args = 0;
    if (ft_strcmp(arg, "-c") == 0 || ft_strcmp(arg, "--count") == 0)
    {
        if (!next_arg)
        {
            fprintf(stderr, "ft_ping: option '%s' requires an argument \n", arg);
            return (-1);
        }
        val = (char *)next_arg;
        used_args = 1;
    }
    else if ((val = get_long_opt_val(arg, "--count")) != 0)
        used_args = 0;
    else
        return (0);
    if (!val || !*val)
    {
        fprintf(stderr, "ft_ping: option requires an argument -- count\n");
        return (-1);
    }
    //need to check this again because it behaves differently in inetutils 2.0 ping
    long long count = ft_atoll(next_arg);
    if (count <= 0 || count >= __LONG_LONG_MAX__)
    {
        fprintf(stderr, "ping: invalid argument: '%s': out of range: 1 <= value <= 9223372036854775807", next_arg);
        return (-1);
    }
    //
    conf->count = count;
    return (++used_args);
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
    if ((res = parse_count(arg, next_arg, conf)) != 0)
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
            fprintf(stderr, "parse: parse target malloc failed\n");
            return (false);
        }
        return (true);
    }
    fprintf(stderr, "parse: parse target extra operand\n");
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
            else if (arg_res > 1)
            {
                i += (arg_res - 1);
                continue ;
            }
            else if (arg_res == -1)
                return (false);
            fprintf(stderr, "parse: invalid option -- '%s'\n", argv[i]);
            fprintf(stderr, "Try 'ft_ping --help' or 'ft_ping --usage' for more information.\n");
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
		fprintf(stderr, "ft_ping: usage error: Destination address required\n");
		return (false);
	}

    return (true);
}
