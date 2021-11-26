/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 21:25:48 by arsciand          #+#    #+#             */
/*   Updated: 2021/11/22 21:26:11 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

static uint16_t retrieve_port(void *buffer)
{
    struct udphdr  *udphdr    = (struct udphdr *)(buffer);

    return (htons(udphdr->dest));
}

static int       find_port(void *current, void *to_find)
{
    t_packet_data   *tmp_current = current;
    uint16_t        tmp_to_find = *(uint16_t *)to_find;

    return (tmp_current->port == tmp_to_find ? TRUE : FALSE);
}

static t_packet_data *validate_packet(t_traceroute *traceroute, void *buffer)
{
    t_lst   *node   = NULL;
    uint16_t port    = 0;

    port = retrieve_port((char *)buffer + IPHDR_SIZE);
    if (!(node = ft_lstfind(traceroute->packets, &port, (int (*)(void*, void*))find_port)))
        return (NULL);
    return ((t_packet_data *)node->content);
}

t_packet_data   *process_packet(t_traceroute *traceroute, void *buffer, struct timeval *time_recv)
{
    t_packet_data   *packet_data = NULL;

    packet_data = validate_packet(traceroute, (char *)buffer + IPHDR_SIZE + ICMPHDR_SIZE);
    if (packet_data)
    {
        ft_memcpy(&packet_data->time_recv, time_recv, sizeof(struct timeval));
    }

    return (packet_data);
}

