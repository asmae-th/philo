/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahtouh <atahtouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 11:17:43 by atahtouh          #+#    #+#             */
/*   Updated: 2024/10/11 11:25:53 by atahtouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_create(t_tab *arr, t_args *arg)
{
	int	i;

	i = 0;
	while (i < arg->nb_philos)
	{
		arr->philos[i].check = 0;
		arr->philos[i].nb_eat = 0;
		arr->philos[i].id = i + 1;
		arr->philos[i].start_time = ft_time();
		arr->philos[i].last_eat = ft_time();
		pthread_create(&arr->thread[i], NULL, &ft_routine, &arr->philos[i]);
		i++;
	}
}

void	ft_init_mutex(t_tab *arr, t_args *arg)
{
	int	i;

	i = 0;
	arr->philos = malloc(sizeof(t_philo) * arg->nb_philos);
	arr->thread = malloc(sizeof(pthread_t) * arg->nb_philos);
	arr->forks = malloc(sizeof(pthread_mutex_t) * arg->nb_philos);
	while (i < arg->nb_philos)
	{
		pthread_mutex_init(&arr->forks[i], NULL);
		arr->philos[i].r_fork = &arr->forks[i];
		arr->philos[i].l_fork = &arr->forks[(i + 1) % arg->nb_philos];
		arr->philos[i].arg = arg;
		i++;
	}
	pthread_mutex_init(&arg->print_mutex, NULL);
	pthread_mutex_init(&arg->mutex_nb_eat, NULL);
	pthread_mutex_init(&arg->mutex_last_eat, NULL);
}

void	ft_destroy(t_tab *arr, t_args *arg)
{
	int	i;

	i = 0;
	while (i < arg->nb_philos)
	{
		pthread_mutex_destroy(&arr->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&arg->print_mutex);
	pthread_mutex_destroy(&arg->mutex_nb_eat);
	pthread_mutex_destroy(&arg->mutex_last_eat);
}

void	ft_free(t_tab *arr, t_args *arg)
{
	ft_destroy(arr, arg);
	free(arr->forks);
	free(arr->philos);
	free(arr->thread);
}
