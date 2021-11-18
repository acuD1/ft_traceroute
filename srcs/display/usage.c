/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usage.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 14:29:51 by arsciand          #+#    #+#             */
/*   Updated: 2021/11/18 14:30:21 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void    print_usage(void)
{
    dprintf(STDOUT_FILENO,
        "Usage: ft_traceroute [-h] [--help] <destination>\n");
}
