/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmae <asmae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 09:56:31 by asmae             #+#    #+#             */
/*   Updated: 2024/10/08 10:31:48 by asmae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


long ft_time(void)
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000) + (time.tv_usec / 1000);
}

void ft_sleep(long time)
{
    usleep(time * 1000);
}
void ft_init_mutex(t_data *data)
{
	int i;

    data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_of_philo);
	i = 0;
    while (i < data->nb_of_philo)
	{
        pthread_mutex_init(&data->forks[i], NULL);
		i++;
    }
    pthread_mutex_init(&data->print_mutex, NULL);
    pthread_mutex_init(&data->stop_mutex, NULL);
}

void ft_free(t_data *data)
{
    ft_destroy_mutex(data);
    free(data->forks);
    free(data->philosophers);
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
