/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 01:15:51 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/10 03:21:00 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

static int  parse_ttl(const char *val, t_conf *conf)
{
    
    return (0);
}

static int  parse_count(const char *val, t_conf *conf)
{
    
    return (0);
}

static int  parse_type(const char *val, t_conf *conf)
{
    
    return (0);
}

static int  parse_interval(const char *val, t_conf *conf)
{
    
    return (0);
}


static const t_opt_def    opt_table[] = 
{
    {"-v", "--verbose",        OPT_NO_ARG, offsetof(t_conf, flags.verbose), NULL},
    {"-?", "--help",           OPT_NO_ARG, offsetof(t_conf, flags.help),    NULL},
    {"-r", "--ignore-routing", OPT_NO_ARG, offsetof(t_conf, flags.noroute), NULL},
    
    {NULL, "--ttl",        OPT_HAS_ARG, NULL, parse_ttl},
    {"-c", "--count",      OPT_HAS_ARG, NULL, parse_count},
    {"-t", "--type",       OPT_HAS_ARG, NULL, parse_type},
    {"-i", "--interval",   OPT_HAS_ARG, NULL, parse_interval},
    
    {NULL, NULL, OPT_NO_ARG, NULL, NULL} // Sentinel
};

static const *t_opt_def   lookup_opt(const char *arg)
{
    
    return (NULL);
}

bool    parse(int argc, char **argv, t_conf *conf)
{
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            const t_opt_def *opt = lookup_opt(argv[i]);
            if (!parse)
            {
                fprintf(stderr, "ft_ping: invalid option -- '%s'\n", argv[i]);
                fprintf(stderr, "Try 'ft_ping --help' for more information.\n");
                return (false);
            }
        }
    }
}