/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bind.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/20 15:58:58 by arsciand          #+#    #+#             */
/*   Updated: 2021/11/22 18:55:25 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

// static void     resolve_source(t_traceroute *traceroute, struct sockaddr_in *source)
// {
//     (void)traceroute;
//     struct ifaddrs  *ifap   = NULL;

//     if (getifaddrs(&ifap) == -1)
//     {
//         printf("ft_ping: getifaddrs(): %s\n", strerror(errno));
//         exit_routine(traceroute, FAILURE);
//     }

//     #pragma clang diagnostic ignored "-Wcast-align"

//     for (struct ifaddrs *ifa = ifap; ifa; ifa = ifa->ifa_next)
//     {
//         if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET
//             && !(ifa->ifa_flags & (IFF_LOOPBACK))
//             && (ifa->ifa_flags & (IFF_RUNNING)))
//         {
//             source->sin_addr =
//                 ((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
//             break ;
//         }
//     }

//     freeifaddrs(ifap);
// }

void    bind_socket(t_traceroute *traceroute)
{
    int                 status = 0;
    struct sockaddr_in  sockaddr_in;
    // struct sockaddr_in  sockaddr_in2;

    switch (traceroute->conf.mode)
    {
        case AF_INET:
        {
            ft_memset(&sockaddr_in, 0, sizeof(sockaddr_in));
            sockaddr_in.sin_family      = AF_INET;
            sockaddr_in.sin_port        = htons(DEFAULT_DST_PORT);
            sockaddr_in.sin_addr.s_addr = htonl(INADDR_ANY);
            // resolve_source(traceroute, &sockaddr_in2);
            // sockaddr_in.sin_addr.s_addr = sockaddr_in2.sin_addr.s_addr;

            if ((status = bind(traceroute->recv_sockfd, (struct sockaddr *)&sockaddr_in, sizeof(struct sockaddr))) < 0)
            {
                dprintf(STDERR_FILENO, "ft_traceroute: bind(): %s\n", strerror(errno));
                exit_routine(traceroute, FAILURE);
            }
            break;
        }

    default:
        break;
    }
}
