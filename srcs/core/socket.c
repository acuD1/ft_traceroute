/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/20 11:59:25 by arsciand          #+#    #+#             */
/*   Updated: 2021/11/21 15:51:31 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void    setup_send_sockfd(t_traceroute *traceroute, int ttl)
{
    int enabled = 1;

    switch (traceroute->conf.mode)
    {
        case AF_INET:
        {
            if ((traceroute->send_sockfd = socket(PF_INET, SOCK_RAW, IPPROTO_UDP)) == -1)
            {
                dprintf(STDERR_FILENO, "ft_traceroute: socket(): %s\n", strerror(errno));
                exit_routine(traceroute, FAILURE);
            }
            if ((setsockopt(traceroute->send_sockfd, IPPROTO_IP, IP_HDRINCL, &enabled, sizeof(enabled))) != SUCCESS)
            {
                dprintf(STDERR_FILENO, "ft_traceroute: setsockopt(): %s\n", strerror(errno));
                exit_routine(traceroute, FAILURE);
            }
            if ((setsockopt(traceroute->send_sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl))) != SUCCESS)
            {
                dprintf(STDERR_FILENO, "ft_traceroute: setsockopt(): %s\n", strerror(errno));
                exit_routine(traceroute, FAILURE);
            }
            break;
        }

    default:
        break;
    }
}

void    setup_recv_sockfd(t_traceroute *traceroute)
{
    int enabled = 1;

    switch (traceroute->conf.mode)
    {
        case AF_INET:
        {
            if ((traceroute->recv_sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1)
            {
                dprintf(STDERR_FILENO, "ft_traceroute: socket(): %s\n", strerror(errno));
                exit_routine(traceroute, FAILURE);
            }
            if ((setsockopt(traceroute->recv_sockfd, SOL_SOCKET,  SO_REUSEADDR,&enabled, sizeof(enabled))) != SUCCESS)
            {
                dprintf(STDERR_FILENO, "ft_traceroute: setsockopt(): %s\n", strerror(errno));
                exit_routine(traceroute, FAILURE);
            }
            break;
        }

    default:
        break;
    }

    bind_socket(traceroute);
}
