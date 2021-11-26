/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_traceroute.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 15:06:14 by arsciand          #+#    #+#             */
/*   Updated: 2021/11/23 22:03:45 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

static void     send_handler(t_traceroute *traceroute, t_packet_data *packet_data)
{
    switch (traceroute->conf.mode)
    {
        case AF_INET:
        {
            switch (traceroute->conf.protocol)
            {
                case IPPROTO_UDP:
                {
                    send_udp_ipv4(traceroute, packet_data);
                    break;
                }
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }
}

static uint8_t  trace_recursive_loop(
                    t_traceroute *traceroute,
                    t_loop_data *loop_data,
                    t_analyzer_data *analyzer_data,
                    t_packet_data *packet_data)
{
    if (loop_data->ttl >= traceroute->conf.hops && loop_data->probe > traceroute->conf.probes)
        return (0);

    while (loop_data->queries)
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

                trace_analyzer(traceroute, loop_data, analyzer_data);
                return (0);
            }
            return (trace_recursive_loop(traceroute, loop_data, analyzer_data, packet_data));
        }

        // dprintf(STDERR_FILENO, "[DEBUG] [TTL %hhu] Sending probe |%d| ...\n", loop_data->ttl, loop_data->probe);

        send_handler(traceroute, packet_data);

        loop_data->queries--;
        loop_data->probe++;
        packet_data->port++;
        packet_data->probe = loop_data->probe;

    }

    // dprintf(STDERR_FILENO, "[DEBUG] Analyzing ...\n");

    trace_analyzer(traceroute, loop_data, analyzer_data);

    loop_data->queries = traceroute->conf.queries;

    return (trace_recursive_loop(traceroute, loop_data, analyzer_data, packet_data));
}

void    exec_traceroute(t_traceroute *traceroute)
{
    t_packet_data   packet_data;
    t_loop_data     loop_data;
    t_analyzer_data analyzer_data;

    if (!(traceroute->packet = ft_memalloc(traceroute->conf.packetlen)))
        exit_routine(traceroute, FAILURE);

    print_init(traceroute);

    setup_recv_sockfd(traceroute);

    ft_memset(&packet_data, 0, sizeof(packet_data));
    ft_memset(&loop_data, 0, sizeof(loop_data));
    ft_memset(&analyzer_data, 0, sizeof(analyzer_data));

    loop_data.ttl        = traceroute->conf.start_ttl;
    loop_data.probe      = traceroute->conf.start_probe;
    loop_data.queries    = traceroute->conf.queries;

    packet_data.ttl      = loop_data.ttl;
    packet_data.probe    = loop_data.probe;
    packet_data.protocol = traceroute->conf.protocol;
    packet_data.port     = DEFAULT_DST_PORT;

    if (traceroute->conf.hops == 1 && traceroute->conf.probes == 1)
    {
        // dprintf(STDERR_FILENO, "[DEBUG] [TTL %hhu] Sending probe |%d| ...\n", loop_data.ttl, loop_data.probe);
        send_handler(traceroute, &packet_data);
        // dprintf(STDERR_FILENO, "[DEBUG] Analyzing ...\n");
        trace_analyzer(traceroute, NULL, &analyzer_data);
    }
    else
        trace_recursive_loop(traceroute, &loop_data, &analyzer_data, &packet_data);
}
