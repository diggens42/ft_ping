/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 01:15:51 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/26 20:19:16 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

static int parse_size(const char *val, t_conf *conf)
{
    char *endptr;
    long n;

    errno = 0;
    n = ft_strtol(val, &endptr, 10);
    // ICMP data size limits: 8 bytes min (timestamp), 65507 max (IP limit)
    if (errno == ERANGE || n < 8 || n > 65507 || *endptr != '\0')
    {
        fprintf(stderr, "ft_ping: packet size %s is invalid\n", val);
        return (PARSE_ERROR);
    }
    conf->opts.packet_size = (uint16_t)n;
    return (0);
}

static int parse_timeout(const char *val, t_conf *conf)
{
    char *endptr;
    long n;

    errno = 0;
    n = ft_strtol(val, &endptr, 10);
    if (errno == ERANGE || n < 0 || n > UINT32_MAX || *endptr != '\0')
    {
        fprintf(stderr, "ft_ping: bad timeout value\n");
        return (PARSE_ERROR);
    }
    conf->opts.timeout = (uint32_t)n;
    return (0);
}

// static int parse_linger(const char *val, t_conf *conf)
// {
//     char *endptr;
//     long n;

//     errno = 0;
//     n = ft_strtol(val, &endptr, 10);
//     if (errno == ERANGE || n < 0 || n > 3600 || *endptr != '\0')
//     {
//         fprintf(stderr, "ft_ping: bad linger time\n");
//         return (PARSE_ERROR);
//     }
//     conf->opts.linger = (uint32_t)n;
//     return (0);
// }

static int  parse_ttl(const char *val, t_conf *conf)
{
    char    *endptr;
    long    n;

    if (!val || !*val)
    {
        FT_ERROR();
        return (PARSE_ERROR);
    }
    errno = 0;
    n = ft_strtol(val, &endptr, 10);
    if (errno == ERANGE || n < 0 || n > UINT8_MAX || *endptr != '\0')
    {
        FT_ERROR();
        return (PARSE_ERROR);
    }
    conf->opts.ttl = (u_int8_t)n;
    return (0);
}

// static int  parse_pattern(const char *val, t_conf *conf)
// {
//     size_t  len = ft_strlen(val);
//     if (len > 32) // 16 bytes --> 32 hex chars
//     {
//         fprintf(stderr, "ft_ping: pattern too long (max 16 bytes)\n");
//         return (PARSE_ERROR);
//     }
//     //converthex string to bytes

//     for (size_t i = 0; i < len && conf->opts.pattern_len < 16; i += 2)
//     {
//         char hex[3] = {val[i], val[i + 1] ? val[i + 1] : '0', '\0'};
//         char *endptr;
//         long nbyte = ft_strtol(hex, &endptr, 16);
//         if (*endptr != '\0' || nbyte < 0 || nbyte > UINT8_MAX)
//         {
//             fprintf(stderr, "ft_ping: bad pattern\n");
//             return (PARSE_ERROR);
//         }
//         conf->opts.pattern[conf->opts.pattern_len++] = (uint8_t)nbyte;
//     }
//     return (0);
// }

static int parse_tos(const char *val, t_conf *conf)
{
    char *endptr;
    long n;

    errno = 0;
    n = ft_strtol(val, &endptr, 10);
    if (errno == ERANGE || n < 0 || n > UINT8_MAX || *endptr != '\0')
    {
        fprintf(stderr, "ft_ping: bad tos value\n");
        return (PARSE_ERROR);
    }
    conf->opts.tos = (uint8_t)n;
    return (0);
}

static int  parse_count(const char *val, t_conf *conf)
{
    char            *endptr;
    unsigned long   n;

    if (!val || !*val)
    {
        FT_ERROR();
        return (PARSE_ERROR);
    }
    errno = 0;
    n = ft_strtoul(val, &endptr, 10);
    if (errno == ERANGE || n > UINT32_MAX)
        conf->opts.count = UINT32_MAX;
    else
        conf->opts.count = (u_int32_t)n;
    return (0);
}

static int parse_type(const char *val, t_conf *conf)
{
    if (!val || !*val)
    {
        FT_ERROR();
        return (PARSE_ERROR);
    }

    if (ft_strcasecmp(val, "echo") == 0)
        conf->opts.packet_type = ICMP_ECHO;
    else if (ft_strcasecmp(val, "address") == 0)
        conf->opts.packet_type = ICMP_MASKREQ;
    else if (ft_strcasecmp(val, "mask") == 0)
        conf->opts.packet_type = ICMP_MASKREQ;
    else if (ft_strcasecmp(val, "timestamp") == 0)
        conf->opts.packet_type = ICMP_TSTAMP;
    else
    {
        fprintf(stderr, "ft_ping: unsupported packet type: %s\n", val);
        return (PARSE_ERROR);
    }

    return (0);
}

static int  parse_interval(const char *val, t_conf *conf)
{
    char    *endptr;
    double  n;

    if (!val || !*val)
        return (PARSE_ERROR);
    n = ft_strtod(val, &endptr);
    if (endptr != val && n >= 0.01)
        conf->opts.interval = n;
    return (0);
}


static const t_opt_def opt_table[] =
{
    {"-v", "--verbose",        false,  FLAG_VERBOSE, NULL},
    {"-?", "--help",           false,  FLAG_HELP,    NULL},
    // {"-r", "--ignore-routing", false,  FLAG_NOROUTE, NULL},
    {"-n", "--numeric",        false,  FLAG_NUMERIC, NULL},
    {"-q", "--quiet",          false,  FLAG_QUIET,   NULL},
    // {"-f", "--flood",          false,  FLAG_FLOOD,   NULL},
    // {"-d", "--debug",          false,  FLAG_DEBUG,   NULL},

    {"-c", "--count",          true, 0, parse_count},
    {"-i", "--interval",       true, 0, parse_interval},
    {"-s", "--size",           true, 0, parse_size},
    {"-w", "--timeout",        true, 0, parse_timeout},
    // {"-W", "--linger",         true, 0, parse_linger},
    // {"-p", "--pattern",        true, 0, parse_pattern},
    {"-t", "--type",           true, 0, parse_type},
    {"-T", "--tos",            true, 0, parse_tos},
    {NULL, "--ttl",            true, 0, parse_ttl},

    {NULL, NULL, false, 0, NULL} // Sentinel
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
        if ((opt_def->short_opt && ft_strcmp(opt, opt_def->short_opt) == 0) ||
            (opt_def->long_opt  && ft_strcmp(opt, opt_def->long_opt) == 0))
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
    if (!opt->has_arg)
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
    if (conf->tar[0] == '\0')
    {
        if (ft_strlen(arg) >= sizeof(conf->tar))
        {
            fprintf(stderr, "ft_ping: hostname too long\n");
            return (false);
        }
        ft_strlcpy(conf->tar, arg, sizeof(conf->tar));
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
    if (!HAS_FLAG(conf, FLAG_HELP) && conf->tar[0] == '\0')
    {
        fprintf(stderr, "ft_ping: usage error: Destination address required\n");
        return (false);
    }

    return (true);
}
