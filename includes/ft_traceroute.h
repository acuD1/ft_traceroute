/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/26 15:25:59 by arsciand          #+#    #+#             */
/*   Updated: 2021/11/19 14:57:07 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H

# pragma clang diagnostic ignored "-Wreserved-id-macro"
# define _GNU_SOURCE

#include "libft.h"

// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netdb.h>
// #include <netinet/ip.h>
// #include <netinet/udp.h>
// # include <netinet/ip_icmp.h>
// #include <arpa/inet.h>
// #include <sys/select.h>

/* GENERIC */
# define STRINGIZER(arg)        #arg
# define STR_VALUE(arg)         STRINGIZER(arg)
# define BUILD_VERSION_STRING   STR_VALUE(BUILDV)
# define BUILD_RELEASE_STRING   STR_VALUE(BUILDR)
# define BUILD_PATCH_STRING     STR_VALUE(BUILDP)
# define BUILD_DATE_STRING      STR_VALUE(DATE)

/* OPTIONS */
# define UNALLOWED_OPT          1ULL << 63
# define VV_OPT                 1ULL << ('V' - 39)
# define M_OPT                  1ULL << ('m' - 97)
# define Q_OPT                  1ULL << ('q' - 97)
# define H_OPT                  1ULL << ('h' - 97)
# define A_OPT                  1ULL << ('a' - 97)
# define H_OPT_STR              "help"
# define V_OPT_STR              "version"
# define M_OPT_STR              "max-hops"
# define Q_OPT_STR              "queries"
# define ALLOWED_OPT            "mVqh"
# define ALLOWED_OPT_ARG        "mq"
# define ALLOWED_OPT_TAB        ((const char *[])   \
                                {                   \
                                    H_OPT_STR,      \
                                    V_OPT_STR,      \
                                    M_OPT_STR,      \
                                    Q_OPT_STR,      \
                                    NULL            \
                                })
# define ALLOWED_OPT_TAB_ARG    NULL
# define ALLOWED_OPT_ASSIGN     NULL
# define ALLOWED_OPT_ASSIGN_TAB ((const char *[])   \
                                {                   \
                                    M_OPT_STR,      \
                                    Q_OPT_STR,      \
                                    NULL            \
                                })

/* TRACEROUTE */
# define DEFAULT_PACKETLEN      60
# define MIN_PACKETLEN          28
# define MAX_PACKETLEN          65000
// # define DEFAULT_HOPS           30
# define MIN_HOPS               1
# define MAX_HOPS               255
# define DEFAULT_PROBES         3
# define MIN_PROBES             1
# define MAX_PROBES             10
// # define DEFAULT_DST_PORT       33434
// # define IPHDR_SIZE             20
// # define UDPHDR_SIZE            8

typedef struct                  s_conf
{
    uint32_t                    hops;
    uint32_t                    probes;
    uint32_t                    packetlen;
}                               t_conf;
typedef struct                  s_traceroute
{
    // t_opts_args                 *opts_args;
    // char                        *target;
    // uint32_t                    packetlen;
    // uint32_t                    probes;
    // int                         dst_port;
    // struct sockaddr_storage     target_addr;
    t_conf                       conf;
}                               t_traceroute;

void                            exit_routine(t_traceroute *traceroute, int8_t status);
void                            init_traceroute(t_traceroute *traceroute);
uint8_t                         set_opts_args(t_traceroute *traceroute, int argc, char **argv);
void                            print_usage(void);
void                            print_unallowed_opt(t_opts_args *opts_args);
void                            free_traceroute(t_traceroute *traceroute);
void                            print_version(void);
// void                            getaddrinfo_error_handler(t_core *core, int8_t status);
// void                            free_core(t_core *core);
// void                            print_unallowed_opt(t_opts_args *opts_args);
// void                            print_version(void);
// void                            print_usage(void);
// uint8_t                         get_opts_args_handler(t_core *core, int argc, char **argv);
// uint8_t                         init_core(t_core *core);

/* DEV */
// void    debug_core(t_core *core);
// void    print_bytes(int bytes, void *msg);

#endif
