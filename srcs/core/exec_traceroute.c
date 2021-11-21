/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_traceroute.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 15:06:14 by arsciand          #+#    #+#             */
/*   Updated: 2021/11/21 19:19:05 by arsciand         ###   ########.fr       */
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
    setup_iphdr(traceroute, packet_data, traceroute->packet);
    setup_payload(traceroute, traceroute->packet + IPHDR_SIZE + UDPHDR_SIZE);
    // Need handling ICMP
    setup_udphdr(traceroute, packet_data, traceroute->packet + IPHDR_SIZE);
    setup_send_sockfd(traceroute, packet_data->ttl);

    sendto(traceroute->send_sockfd, traceroute->packet, traceroute->conf.packetlen, 0,
        (struct sockaddr *)&traceroute->target, sizeof(traceroute->target));

    if (!(ft_lstappend(&traceroute->packets, ft_lstnew(packet_data, sizeof(t_packet_data)))))
        exit_routine(traceroute, FAILURE);
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

    // dprintf(STDERR_FILENO, "[DEBUG] Ending ...\n");

    // int rc = 0;
    // int on = 1;
    // int listen_sd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    //     if (listen_sd < 0)
    // {
    //     perror("socket() failed");
    //     exit(-1);
    // }

    // rc = setsockopt(listen_sd, SOL_SOCKET,  SO_REUSEADDR,
    //             (char *)&on, sizeof(on));
    // if (rc < 0)
    // {
    //     perror("setsockopt() failed");
    //     close(listen_sd);
    //     exit(-1);
    // }

    // struct sockaddr_in addr;
    // addr.sin_family = AF_INET;
    // addr.sin_port = htons(4242);
    // addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // rc = bind(listen_sd,
    //          (struct sockaddr *)&addr, sizeof(addr));
    // if (rc < 0)
    // {
    //     perror("bind() failed");
    //     close(listen_sd);
    //     exit(-1);
    // }

    // fd_set        master_set;
    // int max_sd = listen_sd;
    // int retval = 0;
    // ssize_t bytes_read = 0;

    // struct timeval       timeout;
    // timeout.tv_sec  = 3 * 60;
    // timeout.tv_usec = 0;
    // char            buffer[520];
    // struct          sockaddr_in recv;
    // socklen_t len = sizeof(recv);
    // size_t recv_len = 0;
    // while (1)
    // {
    //     FD_ZERO(&master_set);
    //     FD_SET(listen_sd, &master_set);
    //     retval = select(max_sd + 1, &master_set, NULL, NULL, NULL);
    //     if (retval == -1)
    //         perror("select()");
    //     else if (retval)
    //     {
    //         printf("Data is available now.\n");
    //         if (FD_ISSET(listen_sd, &master_set))
    //         {
    //             printf("[DEBUG] SOCKET LISTEN\n");
    //             bytes_read = recvfrom(listen_sd, &buffer, sizeof(buffer), MSG_DONTWAIT, (struct sockaddr*)&recv, &len);
    //             if (bytes_read == -1)
    //             {
    //                 printf("recvfrom(): ERROR: %s , errno %d\n", strerror(errno), errno);
    //                 exit_routine(traceroute, FAILURE);
    //             }
    //             dprintf(STDERR_FILENO, "[DEBUG] recvfrom() bytes_read: %zu\n", bytes_read);
    //             print_bytes((int)bytes_read, buffer);
    //             dprintf(STDERR_FILENO, "[DEBUG] recvfrom() recv.sin_addr.s_addr: %s\n", inet_ntoa(recv.sin_addr));
    //             dprintf(STDERR_FILENO, "[DEBUG] recvfrom() recv.sin_port: %d\n", ntohs(recv.sin_port));
    //             dprintf(STDERR_FILENO, "[DEBUG] recvfrom() recv.sin_family: %d\n", recv.sin_family);
    //             dprintf(STDERR_FILENO, "[DEBUG] N |%zu|\n", recv_len++);
    //             // return ;
    //         }
    //         // if (FD_ISSET(listen_sd, &rfds))
    //         //     printf("SOCKET 2\n");
    //     }
    //         /* FD_ISSET(0, &rfds) will be true. */
    //     else
    //     {
    //         printf("No data within five seconds.\n");
    //         return;
    //     }
    // }

    // ft_memset(&packet_data, 0, sizeof(t_packet_data));

    // int listensocket = setup_recv_sockfd(traceroute);
    // packet_data.sockfd = listensocket;
    // // bind_socket_handler(traceroute, &packet_data);

    // // char            buffer[520];
    // // struct          sockaddr_in recv;
    // // ft_memset(&buffer, 0, sizeof(buffer));
    // // socklen_t len = sizeof(recv);
    // // ssize_t bytes_received;

    // // FILLING PACKET_DATA
    // packet_data.ttl = (uint8_t)10;
    // packet_data.protocol = traceroute->conf.protocol;
    // setup_send_sockfd(traceroute, (int)1);

    // packet_data.probe = (int)1;
    // packet_data.port = DEFAULT_DST_PORT;
    // send_packet_handler(traceroute, &packet_data);

    // struct timeval tv;
    // tv.tv_sec = 5;
    // tv.tv_usec = 0;
    // int retval;

    // fd_set rfds;
    // printf("SOCKETLISTEN |%d|\n", listensocket);
    // while (1)
    // {
    //     FD_ZERO(&rfds);
    //     FD_SET(listensocket, &rfds);
    //     FD_SET(traceroute->sockfd, &rfds);
    //     retval = select(2, &rfds, NULL, NULL, NULL);
    //     if (retval == -1)
    //         perror("select()");
    //     else if (retval)
    //     {
    //         printf("Data is available now.\n");
    //         if (FD_ISSET(listensocket, &rfds))
    //         {
    //             printf("[DEBUG] SOCKET LISTEN\n");
    //             // bytes_received = recvfrom(sock1, &buffer, sizeof(buffer), MSG_DONTWAIT, (struct sockaddr*)&recv, &len);
    //             // if (bytes_received == -1)
    //             // {
    //             //     printf("recvfrom(): ERROR: %s , errno %d\n", strerror(errno), errno);
    //             //     exit_routine(traceroute, FAILURE);
    //             // }
    //         }
    //         if (FD_ISSET(traceroute->sockfd, &rfds))
    //         {
    //             printf("[DEBUG] SOCKET SEND\n");
    //             // bytes_received = recvfrom(sock2, &buffer, sizeof(buffer), MSG_DONTWAIT, (struct sockaddr*)&recv, &len);
    //             // if (bytes_received == -1)
    //             // {
    //             //     printf("recvfrom(): ERROR: %s , errno %d\n", strerror(errno), errno);
    //             //     exit_routine(traceroute, FAILURE);
    //             // }
    //         }
    //         // if (FD_ISSET(listen_sd, &rfds))
    //         //     printf("SOCKET 2\n");
    //     }
    //         /* FD_ISSET(0, &rfds) will be true. */
    //     else
    //     {
    //         printf("No data within five seconds.\n");
    //         return;
    //     }
    // }
}
