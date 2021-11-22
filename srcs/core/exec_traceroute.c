/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_traceroute.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 15:06:14 by arsciand          #+#    #+#             */
/*   Updated: 2021/11/22 17:24:29 by arsciand         ###   ########.fr       */
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
static void send_packet_ipv4(t_traceroute *traceroute, t_packet_data *packet_data)
{
    struct timeval  time_sent;

    gettimeofday_handler(traceroute, &time_sent);

    setup_iphdr(traceroute, packet_data, traceroute->packet);
    setup_payload(traceroute, traceroute->packet + IPHDR_SIZE + UDPHDR_SIZE);
    // Need handling ICMP
    setup_udphdr(traceroute, packet_data, traceroute->packet + IPHDR_SIZE);
    setup_send_sockfd(traceroute, packet_data->ttl);

    sendto(traceroute->send_sockfd, traceroute->packet, traceroute->conf.packetlen, 0,
        (struct sockaddr *)&traceroute->target, sizeof(traceroute->target));

    ft_memcpy(&packet_data->time_sent, &time_sent, sizeof(struct timeval));

    if (!(ft_lstappend(&traceroute->packets, ft_lstnew(packet_data, sizeof(t_packet_data)))))
        exit_routine(traceroute, FAILURE);

    traceroute->packet_sent++;
}

void send_packet_handler(t_traceroute *traceroute, t_packet_data *packet_data)
{
    switch (traceroute->conf.mode)
    {
        case AF_INET:
            send_packet_ipv4(traceroute, packet_data);
            break;
        default:
            break;
    }
}

static uint8_t  trace_recursive_loop(t_traceroute *traceroute,
                    t_loop_data *loop_data, t_packet_data *packet_data)
{
    if (loop_data->ttl >= traceroute->conf.hops && loop_data->probe > traceroute->conf.probes)
        return (0);

    while (loop_data->n_queries)
    {
        if (loop_data->probe > traceroute->conf.probes)
        {
            // dprintf(STDOUT_FILENO, "[DEBUG] Reached max prob, increasing TTL ...\n");

            loop_data->ttl++;
            loop_data->probe = traceroute->conf.start_probe;
            packet_data->ttl = loop_data->ttl;

            if (loop_data->ttl > traceroute->conf.hops)
            {
                // dprintf(STDERR_FILENO, "[DEBUG] Analyzing ...\n");
                trace_analyzer(traceroute, loop_data);
                return (0);
            }
            return (trace_recursive_loop(traceroute, loop_data, packet_data));
        }

        // dprintf(STDERR_FILENO, "[DEBUG] [TTL %hhu] Sending probe |%d| ...\n", loop_data->ttl, loop_data->probe);

        send_packet_handler(traceroute, packet_data);

        loop_data->n_queries--;
        loop_data->probe++;
        packet_data->port++;
        packet_data->probe = loop_data->probe;

        // print_bytes(traceroute->conf.packetlen, traceroute->packet);
    }

    // dprintf(STDERR_FILENO, "[DEBUG] Analyzing ...\n");

    trace_analyzer(traceroute, loop_data);

    loop_data->n_queries = traceroute->conf.n_queries;

    return (trace_recursive_loop(traceroute, loop_data, packet_data));
}

void    exec_traceroute(t_traceroute *traceroute)
{
    t_packet_data   packet_data;    
    t_loop_data     loop_data;

    if (!(traceroute->packet = ft_memalloc(traceroute->conf.packetlen)))
        exit_routine(traceroute, FAILURE);

    print_init(traceroute);

    setup_recv_sockfd(traceroute);

    // dprintf(STDERR_FILENO, "[DEBUG] CONF PROBES |%hhu|\n", traceroute->conf.probes);
    // dprintf(STDERR_FILENO, "[DEBUG] CONF HOPS |%hhu|\n", traceroute->conf.hops);
    // dprintf(STDERR_FILENO, "[DEBUG] CONF N_QUERIES |%hhu|\n", traceroute->conf.n_queries);

    ft_memset(&packet_data, 0, sizeof(packet_data));
    ft_memset(&loop_data, 0, sizeof(loop_data));

    loop_data.ttl        = traceroute->conf.start_ttl;
    loop_data.probe      = traceroute->conf.start_probe;
    loop_data.n_queries  = traceroute->conf.n_queries;

    packet_data.ttl      = loop_data.ttl;
    packet_data.probe    = loop_data.probe;
    packet_data.protocol = traceroute->conf.protocol;
    packet_data.port     = DEFAULT_DST_PORT;

    if (traceroute->conf.hops == 1 && traceroute->conf.probes == 1)
    {
        // dprintf(STDERR_FILENO, "[DEBUG] [TTL %hhu] Sending probe |%d| ...\n", loop_data.ttl, loop_data.probe);
        send_packet_handler(traceroute, &packet_data);
        // dprintf(STDERR_FILENO, "[DEBUG] Analyzing ...\n");
        trace_analyzer(traceroute, NULL);
    }
    else
        trace_recursive_loop(traceroute, &loop_data, &packet_data);
}
