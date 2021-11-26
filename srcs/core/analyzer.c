/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyzer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 15:43:15 by arsciand          #+#    #+#             */
/*   Updated: 2021/11/23 22:15:47 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"


static uint8_t icmp_port_unreach(void *buffer)
{
    struct icmp *icmp_hdr = (struct icmp *)(buffer);

    return (icmp_hdr->icmp_type == ICMP_UNREACH ? TRUE : FALSE);
}

static void display_latency(t_packet_data *packet_data)
{
    struct timeval tmp_t_start  = *(struct timeval *)&packet_data->time_sent;
    struct timeval tmp_t_end    = *(struct timeval *)&packet_data->time_recv;
    double res                  = 0;
    double start;
    double end;

    start   = (double)tmp_t_start.tv_sec * 1000000.0 + (double)tmp_t_start.tv_usec;
    end     = (double)tmp_t_end.tv_sec * 1000000.0 + (double)tmp_t_end.tv_usec;

    res = (end - start) / 1000.0;
    if (res < 0.1)
        dprintf(STDOUT_FILENO, " %.4f ms", res);
    else
        dprintf(STDOUT_FILENO, " %.3f ms", res);
}


static uint8_t display_stats(t_traceroute *traceroute, t_packet_data *packet_data, struct sockaddr_in *recv, uint8_t processed)
{
    (void)traceroute;
    static uint8_t              n_prob              = 1;
    static uint8_t              last_ttl            = 1;
    static char                 last_ip[INET6_ADDRSTRLEN];
    char                        ip[INET6_ADDRSTRLEN];
    int                         status              = 0;
    int                         diff                = 0;
    char                        buff_dns[NI_MAXHOST];
    struct sockaddr_in          sin;

    if (last_ttl != packet_data->ttl)
        n_prob = 1;

    ft_memcpy(&ip, inet_ntoa(recv->sin_addr), sizeof(ip));
    if (last_ttl != packet_data->ttl || (diff = ft_strcmp(ip, last_ip)) != 0 )
    {
        if (diff != 0 && last_ttl == packet_data->ttl && processed > 1)
            dprintf(STDERR_FILENO, " ");
        last_ttl = packet_data->ttl;
        ft_memcpy(&last_ip, inet_ntoa(recv->sin_addr), sizeof(last_ip));
        ft_memset(&sin, 0, sizeof(sin));
        if ((traceroute->opts & N_OPT) == 0 && (status = (inet_pton(AF_INET, inet_ntoa(recv->sin_addr), &(sin.sin_addr)))))
        {
            sin.sin_family = AF_INET;
            sin.sin_port = htons(0);
            ft_memset(buff_dns, 0, NI_MAXHOST);
            if ((status = getnameinfo((struct sockaddr *)&sin, sizeof(sin), buff_dns,
                            sizeof(buff_dns), NULL, 0, NI_NUMERICSERV)) != 0)
            {
                // dprintf(STDERR_FILENO, "??\n");
                // if (status != EAI_NONAME)
                //     getnameinfo_error_handler(traceroute, status);
                // return (FAILURE);
                status = -1;
                dprintf(STDOUT_FILENO, "%s", inet_ntoa(recv->sin_addr));
                dprintf(STDOUT_FILENO, " (%s)", inet_ntoa(recv->sin_addr));
                if (last_ttl == packet_data->ttl)
                {
                    dprintf(STDOUT_FILENO, " ");
                    n_prob++;
                }
                display_latency(packet_data);
                return (SUCCESS);
            }
            dprintf(STDOUT_FILENO, "%s", buff_dns);
        }
        else if (status == -1)
        {
            return (FAILURE);
            // dprintf(STDERR_FILENO, "ft_traceroute: inet_pton(): %s'\n", strerror(errno));
            // exit_routine(traceroute, FAILURE);
        }
        if (traceroute->opts & N_OPT)
            dprintf(STDOUT_FILENO, "%s", inet_ntoa(recv->sin_addr));
        else
            dprintf(STDOUT_FILENO, " (%s)", inet_ntoa(recv->sin_addr));
    }
    if (last_ttl == packet_data->ttl)
    {
        dprintf(STDOUT_FILENO, " ");
        n_prob++;
    }
    display_latency(packet_data);
    return (SUCCESS);
}

static void    display_error(t_traceroute *traceroute, t_analyzer_data *analyzer_data,  uint8_t *processed, uint8_t *queries, uint8_t *hops)
{
    if (*processed % traceroute->conf.probes == 0)
    {
        (*hops)++;
        dprintf(STDOUT_FILENO, "\n%s%hhu ",  *hops >= 10 ? "" : " ", *hops);
    }
    if (*processed % traceroute->conf.probes != 0 && analyzer_data->error == FALSE)
        dprintf(STDOUT_FILENO, " ");
    (*processed)++;
    (*queries)++;
    dprintf(STDOUT_FILENO,  "*");
    if (*processed % traceroute->conf.probes != 0)
    {
        dprintf(STDOUT_FILENO, " ");
    }
    analyzer_data->error = TRUE;
}

static uint8_t recvfrom_handler(t_traceroute *traceroute, t_analyzer_data *analyzer_data, uint8_t *processed, uint8_t *queries, uint8_t *hops)
{
    (void)analyzer_data;
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
            dprintf(STDOUT_FILENO, "%s%hhu  ", *hops >= 10 ? "" : " ", *hops);
        }
        if (display_stats(traceroute, packet_data, &recv, *processed) != SUCCESS)
        {
            display_error(traceroute, analyzer_data, processed, queries, hops);
            return (FAILURE);
        }
        if (icmp_port_unreach(buffer + IPHDR_SIZE) == TRUE)
            traceroute->conf.hops = packet_data->ttl;
        (*processed)++;
        analyzer_data->queries++;
        if (packet_data->ttl == traceroute->conf.hops && *processed % traceroute->conf.probes == 0)
        {
            dprintf(STDOUT_FILENO, "\n");
            exit_routine(traceroute, SUCCESS);
        }
        return (SUCCESS);
    }
    return (FAILURE);
}


void    trace_analyzer(
            t_traceroute *traceroute, t_loop_data *loop_data,
            t_analyzer_data *analyzer_data)
{
    (void)loop_data;
    fd_set          rfds;
    int             fds         = 0;
    uint8_t         queries     = 0;
    static uint8_t  hops        = 0;
    static uint8_t  processed   = 0;
    struct timeval  timeout;

    ft_memset(&timeout, 0, sizeof(timeout));

    timeout.tv_sec  = 5;
    timeout.tv_usec = 0;

    analyzer_data->queries = 0;
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
                if ((recvfrom_handler(traceroute, analyzer_data, &processed, &queries, &hops) != SUCCESS))
                    continue;
                analyzer_data->error = FALSE;
            }
            if (!loop_data || analyzer_data->queries == traceroute->conf.queries)
                break;
            continue ;
        }
        else
        {
            display_error(traceroute, analyzer_data, &processed, &queries, &hops);
            break;
        }
    }
}
