/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 01:15:51 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/10 03:57:05 by fwahl            ###   ########.fr       */
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

static char *get_opt_val(const char *arg)
{
    char *sep = ft_strchr(arg, '=');
    return (sep ? (sep + 1) : NULL);
}

static const t_opt_def   *get_opt_def(const char *arg)
{
    char    *val = NULL;
    char    *opt = (char *)arg;

    if (arg[0] == '-' && arg[1] == '-' && (val = get_opt_val(arg))
    {
        size_t  opt_len = opt_val - arg - 1; // -1 for = separator
        opt = ft_strndup(arg, opt_len);
        if (!opt)
            return (NULL);
    }
    
    for (const t_opt_def *opt_def = opt_table; opt_def->long_opt || opt_def->short_opt; opt_def++)
    {
        if (opt_def->short_opt && ft_strcmp(opt, opt_def->short_opt == 0) 
            || opt_def->long_opt && ft_strcmp(opt, opt_def->long_opt) == 0)
        {
            if (opt != arg)
                free(opt);
            return (opt)
        }
    }
    
    if (opt != arg)
        free(opt);
    return (NULL);
}

bool    parse(int argc, char **argv, t_conf *conf)
{
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            const t_opt_def *opt = get_opt_def(argv[i]);
            if (!parse)
            {
                fprintf(stderr, "ft_ping: invalid option -- '%s'\n", argv[i]);
                fprintf(stderr, "Try 'ft_ping --help' for more information.\n");
                return (false);
            }
        }
    }
}