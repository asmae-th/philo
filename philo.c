/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmae <asmae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 10:22:12 by asmae             #+#    #+#             */
/*   Updated: 2024/10/09 18:21:09 by asmae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


long	ft_time(void)
{
	struct timeval	time;
	long			x;

	gettimeofday(&time, NULL);
	x = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (x);
}

int	ft_check_args(int ac, char **av, t_args *arg)
{
	(void)arg;
	if(ac >= 5 && ac <= 6)
	{
		arg->nb_philos = ft_atoi(av[1]);
		arg->t_die = ft_atoi(av[2]);
		arg->t_eat = ft_atoi(av[3]);
		arg->t_sleep = ft_atoi(av[4]);
		if(ac == 6)
			arg->n_eat = ft_atoi(av[5]);
		else
			arg->n_eat = -1;
		return(0);
	}
	return (1);
}

void	ft_print_log(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->arg->print_mutex);
	// printf("iddddddddddddddddd%d\n",philo->id);
	printf("%ld %d %s\n",(ft_time() - philo->start_time), philo->id, str);
	pthread_mutex_unlock(&philo->arg->print_mutex);
}

void	*ft_routine(void *p)
{
	t_philo *philo;

	philo = (t_philo *)p;
	if(philo->id % 2 != 0)
		usleep(2000);
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
		pthread_mutex_lock(&philo->arg->mutex_nb_eat);
		philo->nb_eat++;
		pthread_mutex_unlock(&philo->arg->mutex_nb_eat);
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
		ft_print_log(philo, "is sleeping");
		usleep(philo->arg->t_sleep * 1000);
	}
	return(NULL);
}


void	ft_create(t_tab *arr,t_args *arg)
{
	int i;

	i = 0;
	while (i < arg->nb_philos)
	{
		arr->philos[i].check = 0;
		arr->philos[i].nb_eat = 0;
		arr->philos[i].id = i + 1;
		arr->philos[i].start_time = ft_time();
		arr->philos[i].last_eat = ft_time();
		pthread_create(&arr->thread[i],NULL,&ft_routine,&arr->philos[i]);
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
		pthread_mutex_init(&arr->forks[i],NULL);
		arr->philos[i].r_fork = &arr->forks[i];
		arr->philos[i].l_fork = &arr->forks[(i + 1) % arg->nb_philos];
		arr->philos[i].arg = arg;
		i++;
	}
	pthread_mutex_init(&arg->print_mutex,NULL);
	pthread_mutex_init(&arg->mutex_nb_eat,NULL);
	pthread_mutex_init(&arg->mutex_last_eat,NULL);
}

int	ft_died(t_tab *arr, t_args *arg)
{
	int i;
	int n;
	
	while (1)
	{
		i = 0;
		n = 0;
		while (i < arg->nb_philos)
		{
			// pthread_mutex_lock(&arg->mutex_nb_eat);
			if((ft_time() - arr->philos[i].last_eat) > arg->t_die)
			{
				// pthread_mutex_unlock(&arg->mutex_nb_eat);
				// pthread_mutex_lock(&arg->mutex_nb_eat);
				printf("%ld %d is died\n", (ft_time() - arr->philos[i].start_time),arr->philos[i].id);
				return(1);
			}
			// pthread_mutex_unlock(&arg->mutex_nb_eat);

			pthread_mutex_lock(&arg->print_mutex);
			if(arg->n_eat != -1 && arr->philos[i].nb_eat >= arg->n_eat)
			{
				arr->philos[i].check = 1;
				n += 1;
			}
			pthread_mutex_unlock(&arg->print_mutex);
			i++;
		}
		if (n == arg->nb_philos)
			return(1);
	}
	
}

void ft_destroy(t_tab *arr, t_args *arg)
{
	int i;
	
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

void ft_join(t_tab *arr, t_args *arg)
{
	int i;
	
	i = 0;
	while(i < arg->nb_philos)
	{
		pthread_detach(arr->thread[i]);
		i++;
	}
}
int main(int ac, char **av)
{
	t_tab   arr;
	t_args  arg;

	if(ft_check_args(ac, av, &arg))
		return (1);
	ft_init_mutex(&arr, &arg);
	ft_create(&arr, &arg);
	if(ft_died(&arr, &arg))
	{
		ft_free(&arr, &arg);
		return(0);
	}
	ft_join(&arr, &arg);
	ft_free(&arr, &arg);
	return(0);
}