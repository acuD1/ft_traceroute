/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyzer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 15:43:15 by arsciand          #+#    #+#             */
/*   Updated: 2021/11/22 22:32:06 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"


static uint8_t icmp_port_unreach(void *buffer)
{
    struct icmp *icmp_hdr = (struct icmp *)(buffer);

    return (icmp_hdr->icmp_type == ICMP_UNREACH ? TRUE : FALSE);
}

// static void display_latency(t_packet_data *packet_data)
// {
//     struct timeval tmp_t_start  = *(struct timeval *)&packet_data->time_sent;
//     struct timeval tmp_t_end    = *(struct timeval *)&packet_data->time_recv;
//     double res                  = 0;
//     double start;
//     double end;

//     start   = (double)tmp_t_start.tv_sec * 1000000.0 + (double)tmp_t_start.tv_usec;
//     end     = (double)tmp_t_end.tv_sec * 1000000.0 + (double)tmp_t_end.tv_usec;

//     res = (end - start) / 1000.0;
//     if (res < 0.1)
//         dprintf(STDOUT_FILENO, "  %.3f ms", res);
//     else
//         dprintf(STDOUT_FILENO, "  %.2f ms", res);
// }


static void display_stats(t_traceroute *traceroute, t_packet_data *packet_data, uint8_t processed, struct sockaddr_in *recv)
{
    (void)packet_data;
    (void)traceroute;
    (void)processed;
    static struct sockaddr_in   last_recv;
    static uint8_t              last_ttl            = 1;
    static char                 last_ip[INET6_ADDRSTRLEN];
    char                        ip[INET6_ADDRSTRLEN];
    // int                         status              = 0;
    // char                        buff_dns[NI_MAXHOST];
    // struct sockaddr_in          sin;
    // char *str1 = NULL;
    // char *str2 = NULL;


    if (processed == 1 || last_ttl != packet_data->ttl) != 0)
    {
        last_ttl = packet_data->ttl;
        ft_memcpy(&last_recv, recv, sizeof(struct sockaddr_in));
        dprintf(STDOUT_FILENO, "CHANGE\n");
    }

    dprintf(STDOUT_FILENO, "%s\n", inet_ntoa(recv->sin_addr));
        // str2 = inet_ntoa(recv->sin_addr);
        // dprintf(STDOUT_FILENO, "STR2 %s", str2);
        // dprintf(STDOUT_FILENO, "  STRCMP |%d|\n", strcmp(str1, str2));

    // dprintf(STDOUT_FILENO, "%s\n", inet_ntoa(recv->sin_addr));
    // if (last_recv != NULL)
        // dprintf(STDOUT_FILENO, "\n --- TTL %hhu --- %s - %s |%d| \n", packet_data->ttl, inet_ntoa(recv->sin_addr), inet_ntoa(recv->sin_addr), ft_strcmp(inet_ntoa(recv->sin_addr), inet_ntoa(last_recv->sin_addr)));
    // if (last_recv == NULL || (last_ttl != packet_data->ttl && ft_strcmp(inet_ntoa(recv->sin_addr), inet_ntoa(last_recv->sin_addr)) != 0))
    // {
    //     if ((status = (inet_pton(AF_INET, inet_ntoa(recv->sin_addr), &(sin.sin_addr)))))
    //     {
    //         sin.sin_family = AF_INET;
    //         sin.sin_port = htons(0);
    //         ft_memset(buff_dns, 0, NI_MAXHOST);
    //         if ((status = getnameinfo((struct sockaddr *)&sin, sizeof(sin), buff_dns,
    //                         sizeof(buff_dns), NULL, 0, NI_NUMERICSERV)) != 0)
    //         {
    //             if (status != EAI_NONAME)
    //                 getnameinfo_error_handler(traceroute, status);
    //         }
    //         dprintf(STDOUT_FILENO, "%s", buff_dns);
    //     }
    //     else if (status == -1)
    //     {
    //         dprintf(STDERR_FILENO, "ft_traceroute: inet_pton(): %s'\n", strerror(errno));
    //         exit_routine(traceroute, FAILURE);
    //     }
    //     dprintf(STDOUT_FILENO, " (%s)", inet_ntoa(recv->sin_addr));
    // }

    // last_recv = recv;
    // last_ttl = packet_data->ttl;

    // display_latency(packet_data);
    if (processed % traceroute->conf.probes == 0)
        dprintf(STDOUT_FILENO, " ");
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
        display_stats(traceroute, packet_data, *processed, &recv);
        if (icmp_port_unreach(buffer + IPHDR_SIZE) == TRUE
            || (packet_data->ttl == traceroute->conf.hops && *processed % traceroute->conf.probes == 0))
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
    struct timeval  timeout;

    ft_memset(&timeout, 0, sizeof(timeout));

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
