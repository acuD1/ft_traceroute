/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_udp_ipv4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 21:16:05 by arsciand          #+#    #+#             */
/*   Updated: 2021/11/22 21:21:17 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

static void setup_payload(t_traceroute *traceroute, void *packet)
{
    char    *payload    = (char *)packet;

    for (size_t i = 0; i < traceroute->conf.packetlen - IPHDR_SIZE - UDPHDR_SIZE; i++)
        payload[i] = 0x42;
}

static void setup_iphdr(t_traceroute *traceroute, t_packet_data *packet_data, void *packet)
{
    struct iphdr *iphdr = (struct iphdr *)packet;

    iphdr->version      = 4;
    iphdr->ihl          = IPHDR_SIZE / 4;
    iphdr->tos          = 0;
    iphdr->tot_len      = htons(traceroute->conf.packetlen);
    iphdr->id           = htons((uint16_t)(DEFAULT_SRC_PORT + packet_data->probe));
    iphdr->frag_off     = htons(0);
    iphdr->ttl          = packet_data->ttl;
    iphdr->protocol     = packet_data->protocol;
    iphdr->check        = 0;
    iphdr->saddr        = INADDR_ANY;
    iphdr->daddr        = ((struct sockaddr_in *)&traceroute->target)->sin_addr.s_addr;
}

static void setup_udphdr(t_traceroute *traceroute, t_packet_data *packet_data, void *packet)
{
    struct udphdr *udphdr = (struct udphdr *)(packet);

    udphdr->source       = htons(DEFAULT_SRC_PORT);
    udphdr->dest         = htons(packet_data->port);
    udphdr->len          = htons(traceroute->conf.packetlen - IPHDR_SIZE);
    udphdr->check        = 0;
}

void        send_udp_ipv4(t_traceroute *traceroute, t_packet_data *packet_data)
{
    struct timeval  time_sent;

    gettimeofday_handler(traceroute, &time_sent);

    setup_iphdr(traceroute, packet_data, traceroute->packet);
    setup_payload(traceroute, traceroute->packet + IPHDR_SIZE + UDPHDR_SIZE);
    setup_udphdr(traceroute, packet_data, traceroute->packet + IPHDR_SIZE);
    setup_send_sockfd(traceroute, packet_data->ttl);

    sendto(traceroute->send_sockfd, traceroute->packet, traceroute->conf.packetlen, 0,
        (struct sockaddr *)&traceroute->target, sizeof(traceroute->target));

    ft_memcpy(&packet_data->time_sent, &time_sent, sizeof(struct timeval));

    if (!(ft_lstappend(&traceroute->packets, ft_lstnew(packet_data, sizeof(t_packet_data)))))
        exit_routine(traceroute, FAILURE);

    traceroute->packet_sent++;
}
