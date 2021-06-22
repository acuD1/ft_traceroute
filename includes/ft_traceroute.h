/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/26 15:25:59 by arsciand          #+#    #+#             */
/*   Updated: 2021/06/22 10:00:21 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H

# pragma clang diagnostic ignored "-Wreserved-id-macro"
# define _GNU_SOURCE

#include "libft.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
# include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <sys/select.h>

# define FAILURE                2
# define SUCCESS                0
# define STRINGIZER(arg)        #arg
# define STR_VALUE(arg)         STRINGIZER(arg)
# define BUILD_VERSION_STRING   STR_VALUE(BUILDV)
# define BUILD_RELEASE_STRING   STR_VALUE(BUILDR)
# define BUILD_PATCH_STRING     STR_VALUE(BUILDP)
# define BUILD_DATE_STRING      STR_VALUE(DATE)
/* OPTIONS */
# define UNALLOWED_OPT          1ULL << 63
# define H_OPT_ARRAY            "help"
# define H_OPT                  1ULL << ('h' - 97)
# define V_OPT_ARRAY            "version"
# define V_OPT                  1ULL << ('V' - 39)
# define M_OPT_ARRAY            "max_ttl"
# define M_OPT                  1ULL << ('m' - 97)
# define ALLOWED_OPT            "hmV"
# define ALLOWED_OPT_ARG        "m"
# define ALLOWED_OPT_TAB        ((const char *[])   \
                                {                   \
                                    H_OPT_ARRAY,    \
                                    V_OPT_ARRAY,    \
                                    M_OPT_ARRAY,    \
                                    NULL            \
                                })
# define ALLOWED_OPT_TAB_ARG    ((const char *[])   \
                                {                   \
                                    M_OPT_ARRAY,    \
                                    NULL            \
                                })
/**/
# define DEFAULT_PACKETLEN      60
# define DEFAULT_HOPS           30
# define DEFAULT_SOURCE_PORT    8222
# define DEFAULT_DEST_PORT      33434
# define IPHDR_SIZE             20
# define UDPHDR_SIZE            8

typedef struct                  s_core
{
    t_opts_args                 *opts_args;
    char                        *target;
    uint32_t                    packetlen;
    uint32_t                    hops;
    int                         src_port;
    int                         dst_port;
    struct sockaddr_storage     target_addr;
}                               t_core;

void                            getaddrinfo_error_handler(t_core *core, int8_t status);
void                            free_core(t_core *core);
void                            exit_routine(t_core *core, int8_t status);
void                            print_unallowed_opt(t_opts_args *opts_args);
void                            print_version(void);
void                            print_usage(void);
uint8_t                         get_opts_args_handler(t_core *core, int argc, char **argv);
uint8_t                         init_core(t_core *core);

/* DEV */
void    debug_core(t_core *core);
void    print_bytes(int bytes, void *msg);


#endif
