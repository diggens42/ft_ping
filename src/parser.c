/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 01:15:51 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/13 00:32:05 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

static int  parse_ttl(const char *val, t_conf *conf)
{
    char    *endptr;
    long    n;
    
    if (!val || !*val)
    {
        FT_ERROR();
        return (-1);
    }
    errno = 0;
    n = ft_strtol(val, &endptr, 10);
    if (errno == ERANGE || n < 0 || n > UINT8_MAX || *endptr != '\0')
    {
        FT_ERROR();
        return (-1);
    }
    conf->opts.ttl = (u_int8_t)n;
    return (0);
}

static int  parse_count(const char *val, t_conf *conf)
{
    char            *endptr;
    unsigned long   n;
    
    if (!val || !*val)
    {
        FT_ERROR();
        return (-1);
    }
    errno = 0;
    n = ft_strtoul(val, &endptr, 10);
    if (errno == ERANGE || val > UINT32_MAX)
        conf->opts.count = UINT32_MAX;
    else
        conf->opts.count = (u_int32_t)n;
    return (0);
}

static int  parse_type(const char *val, t_conf *conf)
{
    char    *endptr;
    long    n;
    
    if (!val || !*val)
    {
        FT_ERROR();
        return (-1);
    }
    errno = 0;
    n = ft_strtol(val, &endptr, 10);
    if (errno == ERANGE || n < 0 || n > UINT8_MAX || !ft_is_icmp_type(n) || *endptr != '\0')
    {
        FT_ERROR();
        return (-1);
    }
    conf->opts.packet_type = (uint8_t)n;
    return (0);
}

static int  parse_interval(const char *val, t_conf *conf)
{
    char    *endptr;
    double  n;
    
    if (!val || !*val)
        return (-1);
    n = ft_strtod(val, &endptr);
    if (endptr != val && n >= 0.01)
        conf->opts.interval = n;
    return (0);
}


static const t_opt_def    opt_table[] = 
{
    {"-v", "--verbose",        OPT_NO_ARG, FLAG_VERBOSE, NULL},
    {"-?", "--help",           OPT_NO_ARG, FLAG_HELP,    NULL},
    {"-r", "--ignore-routing", OPT_NO_ARG, FLAG_NOROUTE, NULL},
    
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

static const t_opt_def   *get_opt(const char *arg)
{
    char    *val = NULL;
    char    *opt = (char *)arg;

    if (arg[0] == '-' && arg[1] == '-' && (val = get_opt_val(arg)))
    {
        size_t  opt_len = val - arg - 1; // -1 for = separator
        opt = ft_strndup(arg, opt_len);
        if (!opt)
            return (NULL);
    }
    
    for (const t_opt_def *opt_def = opt_table; opt_def->long_opt || opt_def->short_opt; opt_def++)
    {
        if (opt_def->short_opt && ft_strcmp(opt, opt_def->short_opt == 0) ||
            opt_def->long_opt  && ft_strcmp(opt, opt_def->long_opt) == 0)
        {
            if (opt != arg)
                free(opt);
            return (opt_def);
        }
        
    }
    
    if (opt != arg)
        free(opt);
    return (NULL);
}

static int consume_opt(const t_opt_def *opt, const char *arg, const char *next_arg, t_conf *conf)
{
    if (opt->type == OPT_NO_ARG)
    {
        if (opt->flag_bit)
           SET_FLAG(conf, opt->flag_bit); 
        return (1);
    }
    else
    {
        char *val = NULL;
        int consumed = 1;
        if (arg[0] == '-' && arg[1] == '-' && (val = get_opt_val(arg)))
            consumed = 1;
        else
        {
            if (!next_arg)
            {
                fprintf(stderr, "ft_ping: option '%s' requires an argument\n", arg);
                return (-1);
            }
            val = (char*)next_arg;
            consumed = 2;
        }

        if (opt->parser && opt->parser(val, conf) != 0)
            return (-1);
        return (consumed);
    }
}

static bool parse_target(const char *arg, t_conf *conf)
{
    if (!conf->tar)
    {
        conf->tar = ft_strdup(arg);
        if (!conf->tar)
        {
            FT_ERROR();
            return (false);
        }
        return (true);
    }
    fprintf(stderr, "ft_ping: extra operand '%s'\n", arg);
    return (false);
}

bool    parse(int argc, char **argv, t_conf *conf)
{
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            const t_opt_def *opt = get_opt(argv[i]);
            if (!opt)
            {
                fprintf(stderr, "ft_ping: invalid option -- '%s'\n", argv[i]);
                fprintf(stderr, "Try 'ft_ping --help' for more information.\n");
                return (false);
            }
            
            const char *next_arg = (i + 1 < argc) ? argv[i + 1] : NULL;
            int consumed = consume_opt(opt, argv[i], next_arg, conf);

            if (consumed < 0)
                return (false);
            
            i += (consumed - 1);
        }
        else
        {
            if (!parse_target(argv[i], conf))
                return (false);
        }
    }
    if (!HAS_FLAG(conf, FLAG_HELP) && !conf->tar)
    {
        fprintf(stderr, "ft_ping: usage error: Destination address required\n");
        return (false);
    }

    return (true);
}