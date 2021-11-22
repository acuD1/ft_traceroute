/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/26 15:25:59 by arsciand          #+#    #+#             */
/*   Updated: 2021/11/22 21:36:49 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H

# pragma clang diagnostic ignored "-Wreserved-id-macro"
# define _GNU_SOURCE

#include "libft.h"

# include <netdb.h>
# include <netinet/in.h>
# include <errno.h>
# include <netinet/ip.h>
# include <netinet/udp.h>
# include <netinet/ip_icmp.h>
# include <arpa/inet.h>
# include <sys/select.h>
# include <string.h>
# include <ifaddrs.h>
# include <net/if.h>
# include <sys/time.h>

# define errno                  (*__errno_location ())

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
# define DEFAULT_HOPS           30
# define DEFAULT_START_HOPS     1
# define MIN_HOPS               1
# define MAX_HOPS               255
# define DEFAULT_PROBES         3
# define DEFAULT_START_PROBE    1
# define MIN_PROBES             1
# define MAX_PROBES             10
# define DEFAULT_DST_PORT       33437
# define DEFAULT_SRC_PORT       42000
# define DEFAULT_N_QUERIES      1
# define IPHDR_SIZE             20
# define ICMPHDR_SIZE           8
# define UDPHDR_SIZE            8
# define DEFAULT_PROTOCOL       IPPROTO_UDP

typedef struct                  s_conf
{
    uint16_t                    packetlen;
    uint8_t                     protocol;
    uint8_t                     hops;
    uint8_t                     probes;
    uint8_t                     n_queries;
    uint8_t                     mode;
    uint8_t                     dns;
    uint8_t                     diff_dns;
    uint8_t                     local;
    uint8_t                     start_ttl;
    uint8_t                     start_probe;
}                               t_conf;

typedef struct                  s_packet_data
{
    int                         probe;
    uint16_t                    port;
    uint8_t                     ttl;
    uint8_t                     protocol;
    struct timeval              time_sent;
    struct timeval              time_recv;
}                               t_packet_data;

typedef struct                  s_loop_data
{
    uint8_t                     ttl;
    uint8_t                     probe;
    uint8_t                     n_queries;
}                               t_loop_data;

typedef struct                  s_traceroute
{
    t_lst                       *packets;
    char                        *packet;
    uint64_t                    opts;
    size_t                      packet_sent;
    int                         send_sockfd;
    int                         recv_sockfd;
    char                        buff_ip[INET6_ADDRSTRLEN];
    char                        buff_dns[NI_MAXHOST];
    char                        buff_target[NI_MAXHOST];
    t_conf                       conf;
    char                        _PADDING(4);
    struct sockaddr_storage     target;
}                               t_traceroute;

void                            exit_routine(t_traceroute *traceroute, int8_t status);
void                            init_traceroute(t_traceroute *traceroute);
uint8_t                         set_opts_args(t_traceroute *traceroute, int argc, char **argv);
void                            print_usage(void);
void                            print_unallowed_opt(t_opts_args *opts_args);
void                            free_traceroute(t_traceroute *traceroute);
void                            print_version(void);
void                            exec_traceroute(t_traceroute *traceroute);
void                            getnameinfo_handler(t_traceroute *traceroute);
void                            getnameinfo_error_handler(t_traceroute *traceroute, int status);
char                            *inet_ntop_handler(t_traceroute *traceroute, uint32_t *addr);
uint8_t                         resolve_target(t_traceroute *traceroute, char *target, int argc);
void                            getaddrinfo_error_handler(char *target, int argc, int status);
uint8_t                         inet_pton_handler(t_traceroute *traceroute, char *target);
void                            setup_send_sockfd(t_traceroute *traceroute, int ttl);
void                            setup_recv_sockfd(t_traceroute *traceroute);
void                            bind_socket(t_traceroute *traceroute);
void                            print_init(t_traceroute *traceroute);
void                            trace_analyzer(t_traceroute *traceroute, t_loop_data *loop_data);
void                            send_packet_handler(t_traceroute *traceroute, t_packet_data *packet_data);
void                            gettimeofday_handler(t_traceroute *traceroute, void *time);
void                            send_udp_ipv4(t_traceroute *traceroute, t_packet_data *packet_data);
t_packet_data                   *process_packet(t_traceroute *traceroute, void *buffer, struct timeval *time_recv);

/* DEBUG */
void                            print_bytes(int bytes, void *msg);

#endif
