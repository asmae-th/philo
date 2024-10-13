/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmae <asmae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 10:22:12 by asmae             #+#    #+#             */
/*   Updated: 2024/10/13 10:24:24 by asmae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_print_log(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->arg->print_mutex);
	printf("%ld %d %s\n", (ft_time() - philo->start_time), philo->id, str);
	pthread_mutex_unlock(&philo->arg->print_mutex);
}

void	*ft_routine(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
	if (philo->id % 2 != 0)
		usleep(1000);
	while (!philo->check)
	{
		ft_print_log(philo, "is thinking");
		pthread_mutex_lock(philo->r_fork);
		ft_print_log(philo, "has taken a fork");
		pthread_mutex_lock(philo->l_fork);
		ft_print_log(philo, "has taken a fork");
		ft_print_log(philo, "is eating");
		pthread_mutex_lock(&philo->arg->mutex_last_eat);
		philo->last_eat = ft_time();
		pthread_mutex_unlock(&philo->arg->mutex_last_eat);
		usleep(philo->arg->t_eat * 1000);
		ft_count(philo);
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
		ft_print_log(philo, "is sleeping");
		usleep(philo->arg->t_sleep * 1000);
	}
	return (NULL);
}

int	ft_died(t_tab *arr, t_args *arg)
{
	int	i;
	int	n;

	while (1)
	{
		i = 0;
		n = 0;
		while (i < arg->nb_philos)
		{
			if ((ft_time() - arr->philos[i].last_eat) > arg->t_die)
			{
				ft_print_die(arr, i);
				return (1);
			}
			pthread_mutex_lock(&arg->print_mutex);
			if (arg->n_eat != -1 && arr->philos[i].nb_eat >= arg->n_eat)
			{
				arr->philos[i].check = 1;
				n += 1;
			}
			pthread_mutex_unlock(&arg->print_mutex);
			i++;
		}
		if (n == arg->nb_philos)
		{
			// ft_join(arr, arg);
			usleep(1000);
			return (1);
		}
			
	}
}

void	ft_join(t_tab *arr, t_args *arg)
{
	int	i;

	i = 0;
	while (i < arg->nb_philos)
	{
		pthread_detach(arr->thread[i]);
		i++;
	}
}

int	main(int ac, char **av)
{
	t_tab	arr;
	t_args	arg;

	if (ft_check_args(ac, av, &arg))
		return (1);
	ft_init_mutex(&arr, &arg);
	ft_create(&arr, &arg);
	if (ft_died(&arr, &arg))
	{
		ft_join(&arr, &arg);
		ft_free(&arr, &arg);
		return (0);
	}
	ft_join(&arr, &arg);
	ft_free(&arr, &arg);
	return (0);
}
