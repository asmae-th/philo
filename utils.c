/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmae <asmae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 09:56:31 by asmae             #+#    #+#             */
/*   Updated: 2024/09/26 10:04:00 by asmae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


size_t get_time()
{
    struct timeval tv;
    if(gettimeofday(&tv,NULL))
        return(0);
    return((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	ft_atoi(char *str)
{
	int	result;
	int	sign;

	sign = 1;
	result = 0;
	while (*str == ' ' || *str == '\t')
		str++;
	if (*str == '+')
		str++;
	else if (*str == '-')
	{
		sign = -1;
		str++;
	}
	while (*str != '\0' && (*str >= '0' && *str <= '9'))
	{
		result *= 10;
		result += *str - '0';
		str++;
	}
	return (result * sign);
}