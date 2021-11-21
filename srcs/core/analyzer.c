/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyzer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 15:43:15 by arsciand          #+#    #+#             */
/*   Updated: 2021/11/21 19:16:32 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

// static void    display_ttl(void *packet, ssize_t packet_len)
// {
//     static uint8_t ttl = 0;
//     struct iphdr *ip_header = (struct iphdr *)packet;

//     (void)packet_len;

//     if (ip_header->ttl != ttl)
//     {
//         ttl = ip_header->ttl;
//         dprintf(STDOUT_FILENO, " %hhu  ", ttl);
//     }
// }

void    trace_analyzer(t_traceroute *traceroute, t_loop_data *loop_data)
{
    (void)loop_data;

    fd_set        rfds;
    int           fds;

    ssize_t bytes_read = 0;

    struct timeval       timeout;
    timeout.tv_sec  = 5;
    timeout.tv_usec = 0;

    char            buffer[520];
    struct          sockaddr_in recv;
    socklen_t len = sizeof(recv);

    static uint8_t querie       = 0;
    uint8_t processed    = 0;
    static uint8_t hops         = 0;
    // int8_t current_ttl  = 0;

    t_packet_data *packet_data = NULL;

    while (1)
    {
        FD_ZERO(&rfds);
        FD_SET(traceroute->recv_sockfd, &rfds);


        if (querie == traceroute->conf.n_queries)
        {
            hops++;
            querie = 0;
            dprintf(STDOUT_FILENO, "\n%s%hhu ", hops >= 10 ? "" : " ", hops);
        }
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

                // dprintf(STDERR_FILENO, "[DEBUG] bytes_read: %zd\n", bytes_read);
                // print_bytes((int)bytes_read, buffer);
                // char *icmp_payload = buffer + IPHDR_SIZE + ICMPHDR_SIZE;
                // handler_ttl(buffer + IPHDR_SIZE + ICMPHDR_SIZE, bytes_read - IPHDR_SIZE + ICMPHDR_SIZE - 32);
                // if (current_ttl != packet_ttl)
                // {
                //     current_ttl = packet_ttl;
                //     dprintf(STDOUT_FILENO, "select()\n");
                // }
                // dprintf(STDOUT_FILENO, "#");
                // dprintf(STDERR_FILENO, "[DEBUG] recvfrom() bytes_read: %zu\n", bytes_read);
                // dprintf(STDERR_FILENO, "[DEBUG] recvfrom() recv.sin_addr.s_addr: %s\n", inet_ntoa(recv.sin_addr));
                // dprintf(STDERR_FILENO, "[DEBUG] recvfrom() recv.sin_port: %d\n", recv.sin_port);
                // dprintf(STDERR_FILENO, "[DEBUG] recvfrom() recv.sin_family: %d\n", recv.sin_family);
                // dprintf(STDERR_FILENO, "[DEBUG] N |%zu|\n", recv_len++);

                // display_ttl(buffer + IPHDR_SIZE + ICMPHDR_SIZE, bytes_read - IPHDR_SIZE - ICMPHDR_SIZE);
                processed++;
                querie++;
                dprintf(STDOUT_FILENO, " #");

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
            querie++;
            dprintf(STDOUT_FILENO, " *");
            break;
        }
    }
}
