/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 15:04:42 by arsciand          #+#    #+#             */
/*   Updated: 2021/11/21 12:30:52 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

// static void resolve_local(t_traceroute *traceroute)
// {
//     struct ifaddrs  *ifap       = NULL;
//     void            *in_addr    = NULL;

//     if (getifaddrs(&ifap) == -1)
//     {
//         dprintf(STDERR_FILENO, "ft_ping: getifaddrs(): %s\n", strerror(errno));
//         exit_routine(traceroute, FAILURE);
//     }

//     /**/
//     #pragma clang diagnostic ignored "-Wcast-align"
//     /**/

//     for (struct ifaddrs *ifa = ifap; ifa; ifa = ifa->ifa_next)
//     {
//         if (ifa->ifa_addr == NULL)
//             continue;
//         if (!(ifa->ifa_flags & IFF_UP))
//             continue;
//         if (ifa->ifa_addr->sa_family == traceroute->conf.mode)
//         {
//             switch (ifa->ifa_addr->sa_family)
//             {
//                 case AF_INET:
//                 {
//                     struct sockaddr_in *s4
//                         = (struct sockaddr_in *)ifa->ifa_addr;
//                     if (s4->sin_addr.s_addr == htonl(INADDR_LOOPBACK))
//                         in_addr = &s4->sin_addr;
//                     break;
//                 }
//                 // case AF_INET6:
//                 // {
//                 //     struct sockaddr_in6 *s6
//                 //         = (struct sockaddr_in6 *)ifa->ifa_addr;
//                 //     if (IN6_IS_ADDR_LOOPBACK(&s6->sin6_addr))
//                 //         in_addr = &s6->sin6_addr;
//                 //     break;
//                 // }
//                 default:
//                     continue;
//             }
//         }
//     }
//     if (!inet_ntop(traceroute->conf.mode, in_addr, traceroute->buff_ip, sizeof(traceroute->buff_ip)))
//     {
//         dprintf(STDERR_FILENO, "ft_traceroute: inet_ntop(): %s\n", strerror(errno));
//         freeifaddrs(ifap);
//         exit_routine(traceroute, FAILURE);
//     }
//     freeifaddrs(ifap);
// }

uint8_t     resolve_target(t_traceroute *traceroute, char *target, int argc)
{
    struct  addrinfo    hints;
    struct  addrinfo    *res    = NULL;
    int                 status  = 0;

    ft_memset(&hints, 0, sizeof(struct addrinfo));

    /* Preparing getaddrinfo struct */
    hints.ai_flags      = AI_V4MAPPED | AI_ALL | AI_ADDRCONFIG;
    hints.ai_family     = AF_UNSPEC;
    hints.ai_socktype   = SOCK_RAW;
    // hints.ai_protocol   = IPPROTO_ICMP;

    if ((status = getaddrinfo(target, NULL, &hints, &res)) != SUCCESS)
    {
        getaddrinfo_error_handler(target, argc, status);
        return (FAILURE);
    }

    /**/
    #pragma clang diagnostic ignored "-Wcast-align"
    /**/

    for (struct addrinfo *tmp = res; tmp != NULL; tmp = tmp->ai_next)
    {
        if (tmp->ai_family == AF_INET)
        {
            traceroute->conf.mode |= AF_INET;
            ((struct sockaddr_in *)&traceroute->target)->sin_addr.s_addr
                = ((struct sockaddr_in *)res->ai_addr)->sin_addr.s_addr;
            // ((struct sockaddr_in *)&traceroute->target)->sin_port
            //     = htons(0);
            ((struct sockaddr_in *)&traceroute->target)->sin_family
                = (sa_family_t)res->ai_family;
            if (inet_pton_handler(traceroute, target) != TRUE)
            {
                getnameinfo_handler(traceroute);
                if (ft_strequ(target, traceroute->buff_dns) == FALSE)
                    traceroute->conf.diff_dns = TRUE;
            }
            break ;
        }
        // if (tmp->ai_family == AF_INET6)
        // {
        //     traceroute->conf.mode |= AF_INET6;
        //     ((struct sockaddr_in6 *)&traceroute->target)->sin6_addr
        //         = ((struct sockaddr_in6 *)res->ai_addr)->sin6_addr;
        //     ((struct sockaddr_in6 *)&traceroute->target)->sin6_port
        //         = htons(0);
        //     ((struct sockaddr_in6 *)&traceroute->target)->sin6_family
        //         = (sa_family_t)res->ai_family;
        //     if (inet_pton_handler(traceroute, target) != TRUE)
        //     {
        //         getnameinfo_handler(traceroute);
        //         if (ft_strequ(target, traceroute->buff_dns) == FALSE)
        //             traceroute->conf.diff_dns = TRUE;
        //     }
        //     break ;
        // }
    }
    ft_strcpy(traceroute->buff_target, target);

    for (struct addrinfo *tmp = NULL; res; res = tmp)
    {
        tmp = res->ai_next;
        ft_memdel((void **)&res);
    }

    /* Correctly resolve local IP ADDRESS like 0.0.0.0 from interfaces and loopback */

    // if (traceroute->conf.mode == AF_INET && !*((uint32_t *)&((struct sockaddr_in *)&traceroute->target)->sin_addr))
    // {
    //     resolve_local(traceroute);
    //     if (inet_pton_handler(traceroute, target) != TRUE)
    //         traceroute->conf.diff_dns = TRUE;
    //     traceroute->conf.local = TRUE;
    // }

    // dprintf(STDERR_FILENO, "[DEBUG] TARGET\t|%s|\n", traceroute->buff_target);
    // dprintf(STDERR_FILENO, "[DEBUG] DNS\t|%s|\n", traceroute->buff_dns);
    // dprintf(STDERR_FILENO, "[DEBUG] IP\t|%s|\n", traceroute->buff_ip);

    return (SUCCESS);
}
