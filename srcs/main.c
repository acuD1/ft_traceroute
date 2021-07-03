/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 13:19:29 by arsciand          #+#    #+#             */
/*   Updated: 2021/06/22 21:03:14 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"
# include <errno.h>
#include <string.h>
int       errno;

uint16_t    checksum(size_t len, void *icmp_packet)
{
    uint16_t    *buff   =   icmp_packet;
    uint32_t    sum     =   0;

    while (len) {
        sum += *buff++;
        len -= 2;
    }
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return (~sum);
}

void    exec_ft_traceroute(t_core *core)
{
    size_t  payload_size = core->packetlen - IPHDR_SIZE - UDPHDR_SIZE;
    char    payload[payload_size];

    dprintf(STDOUT_FILENO, "ft_traceroute to %s, %d hops max, %d byte packets\n", core->target, core->hops, core->packetlen);

    /* Payload */

    for (size_t i = 0; i < payload_size; i++)
        payload[i] = 0x42;

    print_bytes(payload_size, &payload);

    int sockfd = 0;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        printf("socket(): ERROR: %s , errno %d\n", strerror(errno), errno);
        exit_routine(core, FAILURE);
    }

    /*  Loop send */
    int listen_sd = 0;

    #define SERVER_PORT 8442
    for (size_t ttl = 1; ttl <= core->hops; ttl++)
    {
        if ((setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl))) != SUCCESS)
        {
            printf("setsockopt(): ERROR: %s , errno %d\n", strerror(errno), errno);
            exit_routine(core, FAILURE);
        }

        for (size_t i = 0; i < core->probes; i++)
        {

            ((struct sockaddr_in *)&core->target_addr)->sin_port = htons(core->dst_port);

            listen_sd = socket(AF_INET, SOCK_DGRAM, 0);
            if (listen_sd < 0)
            {
                perror("socket() failed");
                exit(-1);
            }
            int on = 1;
            setsockopt(sockfd, SOL_SOCKET,  SO_REUSEADDR,
                   (char *)&on, sizeof(on));
            struct sockaddr_in addr;
            memset(&addr, 0, sizeof(addr));
            addr.sin_family      = AF_INET;
            // // memcpy(&addr.sin_addr, INADDR_ANY, sizeof(INADDR_ANY));
            addr.sin_addr.s_addr = htonl(INADDR_ANY);
            addr.sin_port        = htons(core->dst_port);
            // int rc = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
            // if (rc < 0)
            // {
            //     printf("bind(): ERROR: %s , errno %d\n", strerror(errno), errno);
            //     exit_routine(core, FAILURE);
            // }
            // struct sockaddr empty;
            // ft_memset(&empty, 0, sizeof(empty));
            // // if ((bind(sockfd, &em, sizeof(empty)) != SUCCESS))
            // // if ((bind(sockfd, (struct sockaddr *)&core->target_addr, sizeof(core->target_addr))) != SUCCESS)
            // {
            //     printf("bind(): ERROR: %s , errno %d\n", strerror(errno), errno);
            //     exit_routine(core, FAILURE);
            // }

            ssize_t bytes_sent = 0;
            bytes_sent = sendto(sockfd, payload, payload_size, 0, (struct sockaddr_in *)&core->target_addr, sizeof(core->target_addr));
            printf("BYTES_SENT |%zd|\n", bytes_sent);
        }
    }


    char            buffer[520];
    ssize_t         bytes_received = 0;
    struct          sockaddr_in recv;

    ft_memset(&buffer, 0, sizeof(buffer));
    socklen_t len = sizeof(recv);


    // int sockfdr1;

    // if ((sockfdr1 = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1)
    // {
    //     printf("socket(): ERROR: %s , errno %d\n", strerror(errno), errno);
    //     exit_routine(core, FAILURE);
    // }
    // int sockfdr2;

    // if ((sockfdr2 = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP)) == -1)
    // {
    //     printf("socket(): ERROR: %s , errno %d\n", strerror(errno), errno);
    //     exit_routine(core, FAILURE);
    // }


    fd_set rfds;
    struct timeval tv;
    int retval;
    FD_ZERO(&rfds);
    FD_SET(sockfd, &rfds);
    // FD_SET(sockfdr2, &rfds);
    // FD_SET(listen_sd, &rfds);

    tv.tv_sec = 5;
    tv.tv_usec = 0;

    retval = select(listen_sd + 1, &rfds, NULL, NULL, &tv);
           /* Don't rely on the value of tv now! */

    if (retval == -1)
        perror("select()");
    else if (retval)
    {
        printf("Data is available now.\n");
        if (FD_ISSET(sockfd, &rfds))
        {
            printf("[DEBUG] SOCKET 1\n");
            bytes_received = recvfrom(sockfd, &buffer, sizeof(buffer), MSG_DONTWAIT, (struct sockaddr*)&recv, &len);
            if (bytes_received == -1)
            {
                printf("recvfrom(): ERROR: %s , errno %d\n", strerror(errno), errno);
                exit_routine(core, FAILURE);
            }
        }
        // if (FD_ISSET(listen_sd, &rfds))
        //     printf("SOCKET 2\n");
    }
        /* FD_ISSET(0, &rfds) will be true. */
    else
        printf("No data within five seconds.\n");


    printf("bytes_received| %zu\n", bytes_received);
    print_bytes(256, buffer);
    printf("ID %d\n", htons(((struct iphdr *)buffer)->id));
    // printf("id : %d\n", (struct iphdr))
    char                buff_ipv4[INET_ADDRSTRLEN];
    ft_memset(&buff_ipv4, 0, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &((struct iphdr *)buffer)->daddr, buff_ipv4, sizeof(buff_ipv4));
    printf("To %s\n", buff_ipv4);
    ft_memset(&buff_ipv4, 0, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &((struct iphdr *)buffer)->saddr, buff_ipv4, sizeof(buff_ipv4));
    printf("From %s\n", buff_ipv4);


}

int     main(int argc, char *argv[])
{
    t_core  core;

    ft_memset(&core, 0, sizeof(core));

    core.packetlen  = DEFAULT_PACKETLEN;
    core.hops       = DEFAULT_HOPS;
    core.probes     = DEFAULT_PROBES;
    core.dst_port   = DEFAULT_DST_PORT;

    if (get_opts_args_handler(&core, argc, argv) != SUCCESS)
        return (EXIT_FAILURE);

        /* Check if ft_ping executed as root */
    if (getuid())
    {
        dprintf(STDERR_FILENO, "ft_traceroute: socket: Operation not permitted\n");
        exit_routine(&core, FAILURE);
    }

    if (init_core(&core) != SUCCESS)
        exit_routine(&core, FAILURE);
    else
        exec_ft_traceroute(&core);
    debug_core(&core);
    debug_opts_args(core.opts_args);
    free_core(&core);
    return (EXIT_SUCCESS);
}
