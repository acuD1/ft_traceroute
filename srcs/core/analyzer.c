/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyzer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 15:43:15 by arsciand          #+#    #+#             */
/*   Updated: 2021/11/21 20:07:22 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

// static uint8_t owned_packet_v4(t_traceroute *traceroute, void *buffer)
// {
//     struct iphdr  *iphdr    = (struct iphdr *)(buffer);
//     uint16_t        id      = htons(iphdr->id);

//     return (id == 0 || (id >= DEFAULT_SRC_PORT && id <= (DEFAULT_SRC_PORT + traceroute->packet_sent) ? TRUE : FALSE));
// }

// static uint8_t owned_packet_v6(t_traceroute *traceroute, void *icmp_area)
// {
//     struct icmp6_hdr *response     = (struct icmp6_hdr *)(icmp_area);
//     uint16_t          id            = htons(response->icmp6_id);

//     return (id == 0 || id == traceroute->conf.pid ? TRUE : FALSE);
// }

// static uint8_t owned_packet_handler(t_traceroute *traceroute, void *buffer)
// {
//     if (traceroute->conf.mode == AF_INET)
//         return (owned_packet_v4(traceroute, (char *)buffer + IPHDR_SIZE + ICMPHDR_SIZE));
//     // else
//     //     return (owned_packet_v6(traceroute, buffer));
//     return (0);
// }

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

    port = retrieve_port((char *)buffer + IPHDR_SIZE + ICMPHDR_SIZE + IPHDR_SIZE);
    dprintf(STDERR_FILENO, "port: %d\n", port);
    if (!(node = ft_lstfind(traceroute->packets, &port, (int (*)(void*, void*))find_port)))
        return (NULL);
    return ((t_packet_data *)node->content);
}

static t_packet_data   *process_packet(t_traceroute *traceroute, void *buffer, struct timeval *time_recv)
{
    t_packet_data   *packet_data = NULL;

    // if (owned_packet_handler(traceroute, buffer) == TRUE)
    // {
        packet_data = validate_packet(traceroute, (char *)buffer + IPHDR_SIZE + ICMPHDR_SIZE);
        if (packet_data)
        {
            dprintf(STDERR_FILENO, "??\n");
            ft_memcpy(&packet_data->time_recv, time_recv, sizeof(struct timeval));
        }

    // }
    return (packet_data);
}

void    trace_analyzer(t_traceroute *traceroute, t_loop_data *loop_data)
{
    (void)loop_data;

    fd_set        rfds;
    int           fds;

    ssize_t bytes_read = 0;

    struct timeval       timeout;
    struct timeval       time_recv;

    timeout.tv_sec  = 5;
    timeout.tv_usec = 0;

    char            buffer[520];
    struct          sockaddr_in recv;
    socklen_t len = sizeof(recv);

    // static uint8_t querie       = 0;
    uint8_t processed    = 0;
    static uint8_t hops         = 0;
    // int8_t current_ttl  = 0;

    t_packet_data *packet_data = NULL;

    while (1)
    {
        FD_ZERO(&rfds);
        FD_SET(traceroute->recv_sockfd, &rfds);


        // if (querie == traceroute->conf.n_queries)
        // {
        //     hops++;
        //     querie = 0;
        //     dprintf(STDOUT_FILENO, "\n%s%hhu ", hops >= 10 ? "" : " ", hops);
        // }
        if ((fds = select(traceroute->recv_sockfd + 1, &rfds, NULL, NULL, &timeout)) == -1)
        {
            dprintf(STDERR_FILENO, "ft_traceroute: select(): %s\n", strerror(errno));
            exit_routine(traceroute, FAILURE);
        }
        else if (fds)
        {

            if (FD_ISSET(traceroute->recv_sockfd, &rfds))
            {
                bytes_read = recvfrom(traceroute->recv_sockfd, &buffer, sizeof(buffer), MSG_DONTWAIT, (struct sockaddr*)&recv, &len);
                if (bytes_read == -1)
                {
                    printf("recvfrom(): ERROR: %s , errno %d\n", strerror(errno), errno);
                    exit_routine(traceroute, FAILURE);
                }
                gettimeofday_handler(traceroute, &time_recv);
                if ((packet_data = process_packet(traceroute, buffer, &time_recv)) != NULL)
                {
                    if (packet_data->ttl > hops)
                    {
                        hops = packet_data->ttl;
                        dprintf(STDOUT_FILENO, "\n%s%hhu ", hops >= 10 ? "" : " ", hops);
                    }
                    processed++;
                    // querie++;
                }
                else
                {
                    dprintf(STDOUT_FILENO, "[DEBUG] /!\\ INVALID PACKET\n");
                    continue;
                }
            }
            if (!loop_data || processed == traceroute->conf.n_queries)
            {
                break;
            }
            continue ;
        }
        else
        {
            processed++;
            // querie++;
            dprintf(STDOUT_FILENO, " *");
            break;
        }
    }
}
