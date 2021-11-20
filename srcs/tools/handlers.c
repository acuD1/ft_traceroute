/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 15:32:23 by arsciand          #+#    #+#             */
/*   Updated: 2021/11/20 11:35:03 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void        getnameinfo_handler(t_traceroute *traceroute)
{
    int     status = 0;

    ft_memset(traceroute->buff_dns, 0, sizeof(traceroute->buff_dns));
    if (traceroute->conf.mode == AF_INET)
    {
        struct sockaddr_in sa = *(struct sockaddr_in *)&traceroute->target;
        if ((status = getnameinfo((struct sockaddr *)&sa, sizeof(sa), traceroute->buff_dns,
                    sizeof(traceroute->buff_dns), NULL, 0, NI_NAMEREQD)) != 0)
        {
            if (status != EAI_NONAME && status != EAI_FAMILY)
                getnameinfo_error_handler(traceroute, status);
        }
        inet_ntop_handler(traceroute, (uint32_t *)&sa.sin_addr);
    }
    // else
    // {
    //     struct sockaddr_in6 sa6 = *(struct sockaddr_in6 *)&traceroute->target;
    //     if ((status = getnameinfo((struct sockaddr *)&sa6, sizeof(sa6),
    //                 traceroute->buff_dns, sizeof(traceroute->buff_dns), NULL, 0,
    //                 NI_NAMEREQD)) != 0)
    //     {
    //         if (status != EAI_NONAME)
    //             getnameinfo_error_handler(traceroute, status);
    //     }
    //     inet_ntop_handler(traceroute, (uint32_t *)&sa6.sin6_addr);
    // }
    traceroute->conf.dns = TRUE;
}

char        *inet_ntop_handler(t_traceroute *traceroute, uint32_t *addr)
{
    ft_memset(&traceroute->buff_ip, 0, sizeof(traceroute->buff_ip));
    if (!(inet_ntop(traceroute->conf.mode, addr, traceroute->buff_ip, sizeof(traceroute->buff_ip))))
    {
        dprintf(STDERR_FILENO, "ft_ping: inet_ntop(): %s\n", strerror(errno));
        exit_routine(traceroute, FAILURE);
    }

    return (traceroute->buff_ip);
}

uint8_t     inet_pton_handler(t_traceroute *traceroute, char *target)
{
    u_char  buff[sizeof(struct in6_addr)];
    ft_memset(&buff, 0, sizeof(struct in6_addr));

    if (!inet_pton(traceroute->conf.mode, target, buff))
        return FALSE;
    return (TRUE);
}
