/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 13:19:29 by arsciand          #+#    #+#             */
/*   Updated: 2021/06/21 14:08:53 by arsciand         ###   ########.fr       */
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
    t_udp_packet_v4 *packet         = NULL;
    size_t          udp_area_size   = core->packetlen - IPHDR_SIZE - UDPHDR_SIZE;

    dprintf(STDOUT_FILENO, "ft_traceroute to %s, %d hops max, %d byte packets\n", core->target, core->hops, core->packetlen);


    if (!(packet = ft_memalloc(core->packetlen)))
        exit_routine(core, FAILURE);

    /* IP Header */
    packet->iphdr.version           = 4;
    packet->iphdr.ihl               = IPHDR_SIZE / 4;
    packet->iphdr.tos               = 0;
    packet->iphdr.tot_len           = htons(core->packetlen);
    packet->iphdr.id                = 0;
    packet->iphdr.frag_off          = htons(0);
    packet->iphdr.ttl               = 1;
    packet->iphdr.protocol          = IPPROTO_UDP;
    packet->iphdr.check             = 0;
    packet->iphdr.saddr             = INADDR_ANY;
    packet->iphdr.daddr             = ((struct sockaddr_in *)&core->target_addr)->sin_addr.s_addr;


    /* UDP Header */

    packet->udp_area.udphdr.source   = htons(core->src_port);
    packet->udp_area.udphdr.dest     = htons(core->dst_port);
    packet->udp_area.udphdr.len      = htons(UDPHDR_SIZE + udp_area_size);
    packet->udp_area.udphdr.check    = 0;

    /* Payload */

    for (size_t i = 1; i < udp_area_size; i++)
        packet->udp_area.payload[i] = 0x42;

    /* Checksums */

    packet->udp_area.udphdr.check   = checksum(udp_area_size, &packet->udp_area);
    packet->iphdr.check             = checksum(core->packetlen, packet);

    print_bytes(core->packetlen, packet);


    int sockfd = 0;
    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) == -1)
    {
        printf("socket(): ERROR: %s , errno %d\n", strerror(errno), errno);
        exit_routine(core, FAILURE);
    }

    int un = 1;
    if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &un, sizeof(un)) != SUCCESS)
    {
        printf("setsockopt(): ERROR: %s , errno %d\n", strerror(errno), errno);
        exit_routine(core, FAILURE);
    }

    ssize_t bytes_sent = 0;
    bytes_sent = sendto(sockfd, packet,
                    core->packetlen, 0,
                    ( struct sockaddr_in *)&core->target_addr,
                    sizeof(core->target_addr));
    printf("%zd\n", bytes_sent);

    char            buffer[256];
    ssize_t         bytes_received = 0;
    struct          sockaddr_in recv;

    ft_memset(&buffer, 0, sizeof(buffer));
    socklen_t len = sizeof(recv);

    int sockfdr;

    if ((sockfdr = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1)
    {
        printf("socket(): ERROR: %s , errno %d\n", strerror(errno), errno);
        exit_routine(core, FAILURE);
    }

    fd_set rfds;
    struct timeval tv;
    int retval;
    FD_ZERO(&rfds);
    FD_SET(sockfdr, &rfds);

    tv.tv_sec = 5;
    tv.tv_usec = 0;

    retval = select(sockfdr + 1, &rfds, NULL, NULL, &tv);
           /* Don't rely on the value of tv now! */

    if (retval == -1)
        perror("select()");
    else if (retval)
    {
        printf("Data is available now.\n");
        if (FD_ISSET(sockfdr, &rfds))
        {
            bytes_received = recvfrom(sockfdr, &buffer, sizeof(buffer), MSG_DONTWAIT, (struct sockaddr*)&recv, &len);
            if (bytes_received == -1)
            {
                printf("recvfrom(): ERROR: %s , errno %d\n", strerror(errno), errno);
                exit_routine(core, FAILURE);
            }
        }
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
    core.dst_port   = DEFAULT_DEST_PORT;
    core.src_port   = DEFAULT_SOURCE_PORT;

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
