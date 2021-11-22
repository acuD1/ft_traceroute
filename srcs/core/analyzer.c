/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyzer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 15:43:15 by arsciand          #+#    #+#             */
/*   Updated: 2021/11/22 18:58:58 by arsciand         ###   ########.fr       */
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

static t_packet_data   *process_packet(t_traceroute *traceroute, void *buffer, struct timeval *time_recv)
{
    t_packet_data   *packet_data = NULL;

    packet_data = validate_packet(traceroute, (char *)buffer + IPHDR_SIZE + ICMPHDR_SIZE);
    if (packet_data)
    {
        ft_memcpy(&packet_data->time_recv, time_recv, sizeof(struct timeval));
    }

    return (packet_data);
}

static uint8_t icmp_port_unreach(void *buffer)
{
    struct icmp *icmp_hdr = (struct icmp *)(buffer);

    return (icmp_hdr->icmp_type == ICMP_UNREACH ? TRUE : FALSE);
}

static void display_stats(t_traceroute *traceroute, void *buffer, struct sockaddr_in *recv)
{
    (void)buffer;
    (void)traceroute;

    dprintf(STDOUT_FILENO, "%s ", inet_ntoa(recv->sin_addr));

}

static uint8_t recvfrom_handler(t_traceroute *traceroute, uint8_t *processed, uint8_t *queries, uint8_t *hops)
{
    t_packet_data       *packet_data    = NULL;
    ssize_t             bytes_read      = 0;
    char                buffer[520];
    struct timeval      time_recv;
    struct sockaddr_in  recv;
    socklen_t           len = sizeof(recv);

    ft_memset(&recv, 0, sizeof(recv));
    ft_memset(buffer, 0, sizeof(buffer));
    ft_memset(&time_recv, 0, sizeof(time_recv));

    if ((bytes_read = recvfrom(traceroute->recv_sockfd, &buffer, sizeof(buffer), MSG_DONTWAIT, (struct sockaddr*)&recv, &len)) == -1)
    {
        printf("recvfrom(): ERROR: %s , errno %d\n", strerror(errno), errno);
        exit_routine(traceroute, FAILURE);
    }

    gettimeofday_handler(traceroute, &time_recv);
    if ((packet_data = process_packet(traceroute, buffer, &time_recv)) != NULL)
    {
        if (packet_data->ttl > *hops)
        {
            if (*hops != 0)
                dprintf(STDOUT_FILENO, "\n");
            *hops = packet_data->ttl;
            dprintf(STDOUT_FILENO, "%s%hhu ", *hops >= 10 ? "" : " ", *hops);
        }
        display_stats(traceroute, packet_data, &recv);
        if (icmp_port_unreach(buffer + IPHDR_SIZE) == TRUE)
        {
            dprintf(STDOUT_FILENO, "\n");
            exit_routine(traceroute, SUCCESS);
        }
        (*processed)++;
        (*queries)++;
        return (SUCCESS);
    }
    return (FAILURE);
}

static void    display_error(t_traceroute *traceroute, uint8_t *processed, uint8_t *queries, uint8_t *hops)
{
    if (*processed % traceroute->conf.probes == 0)
    {
        (*hops)++;
        dprintf(STDOUT_FILENO, "\n%s%hhu ",  *hops >= 10 ? "" : " ", *hops);
    }
    (*processed)++;
    (*queries)++;
    dprintf(STDOUT_FILENO, "* ");
}

void    trace_analyzer(t_traceroute *traceroute, t_loop_data *loop_data)
{
    (void)loop_data;
    fd_set          rfds;
    int             fds         = 0;
    uint8_t         queries     = 0;
    static uint8_t  hops        = 0;
    static uint8_t  processed   = 0;
    struct timeval       timeout;

    timeout.tv_sec  = 5;
    timeout.tv_usec = 0;

    while (1)
    {
        FD_ZERO(&rfds);
        FD_SET(traceroute->recv_sockfd, &rfds);

        if ((fds = select(traceroute->recv_sockfd + 1, &rfds, NULL, NULL, &timeout)) == -1)
        {
            dprintf(STDERR_FILENO, "ft_traceroute: select(): %s\n", strerror(errno));
            exit_routine(traceroute, FAILURE);
        }
        else if (fds)
        {
            if (FD_ISSET(traceroute->recv_sockfd, &rfds))
            {
                if ((recvfrom_handler(traceroute, &processed, &queries, &hops) != SUCCESS))
                    continue;
            }
            if (!loop_data || queries == traceroute->conf.n_queries)
                break;
            continue ;
        }
        else
        {
            display_error(traceroute, &processed, &queries, &hops);
            break;
        }
    }
}
