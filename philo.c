/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahtouh <atahtouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 10:22:12 by asmae             #+#    #+#             */
/*   Updated: 2024/10/02 10:16:54 by atahtouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


int ft_check_args(int ac, char **av, t_args *args)
{
	if(ac >= 5)
	{
		args->nb_philo = ft_atoi(av[1]);
		args->time_to_eat = ft_atoi(av[2]);
		args->time_to_die = ft_atoi(av[3]);
		args->time_to_sleep = ft_atoi(av[4]);
		if(ac == 6)
			args->num_times_to_eat = ft_atoi(av[5]);
		else
			args->num_times_to_eat = -1;
		return(0);
	}
	return(1);
}

void init_forks_mutex(pthread_mutex_t *forks,t_args args)
{
	int i;
	i = 0;
	while (i < args.nb_philo)
	{
		pthread_mutex_init(&forks[i],NULL);
		i++;
	}
}
void *ft_monitor(void *args)
{
	t_philo *philo;
	int i;
	int j;
	size_t current_time;

	philo = (t_philo *)args;
	i = 0;
	while (true)
	{
		i = 0;
		while (i < philo->args->nb_philo)
		{
			if(!philo[i].is_alive)
				continue;
			current_time = get_time();
			if((current_time - philo[i].last_eat_time) > (philo[i].args->time_to_die))
			{
				printf("%d  Philosopher %d has died after %zu milliseconds without eating\n",philo[i].id , philo[i].id,current_time - philo[i].last_eat_time);
				j = 0;
				while (j < philo->args->nb_philo)
				{
					philo[j].is_alive = false;
					j++;
				}
				return (NULL);
			}
		}
		usleep(1);
	}
	
}
void ft_print_log(int id,char *str)
{
	size_t time;
	time = get_time();
	printf("%zu %d %s\n",time, id, str);
}
void *ft_routine(void *philo)
{
	t_philo *philos;
	int nb_meals;

	philos = (t_philo *)philo;
	philos->last_eat_time = get_time();
	nb_meals = philos->args->num_times_to_eat;
	while(nb_meals > 0 && philos->is_alive == true)
	{
		pthread_mutex_lock(philos->l_fork);
		ft_print_log(philos->id,"has taken a fork");
		pthread_mutex_lock(philos->r_fork);
		ft_print_log(philos->id, "has taken a fork");
		philos->last_eat_time = get_time();
		ft_print_log(philos->id, "is eating");
		usleep(philos->args->time_to_eat * 1000);
		pthread_mutex_unlock(philos->l_fork);
		pthread_mutex_unlock(philos->r_fork);
		ft_print_log(philos->id, "is sleeping");
		usleep(philos->args->time_to_sleep * 1000);
		ft_print_log(philos->id, "is thinking");
		usleep(philos->args->time_to_die * 1000);
		nb_meals--;
	}
	return(NULL);
}

void	ft_creat_philo(t_philo *philos, pthread_mutex_t *forks,t_args args)
{
	pthread_t monitor;
	int	i;
	i = 0;
	while (i < args.nb_philo)
	{
		philos[i].id = i + 1;
		philos[i].r_fork = &forks[i];
		philos[i].l_fork = &forks[(i + 1) % args.nb_philo];
		philos[i].args = &args;
		philos[i].meals_eaten = 0;
		philos[i].is_alive = true;
		pthread_create(&philos[i].thread,NULL,ft_routine,&philos[i]);
		i++;
	}
	pthread_create(&monitor, NULL, ft_monitor,philos);
	
	i = 0;
	while (i < args.nb_philo)
		 pthread_join(philos[i].thread, NULL);
	i = 0;
	while(i < args.nb_philo)
		pthread_mutex_destroy(&forks[i]);
	pthread_join(monitor, NULL);
}
int main(int ac, char **av)
{
	t_args args;
	if(ft_check_args(ac,av,&args))
		return(1);
	pthread_mutex_t forks[args.nb_philo];
	t_philo philos[args.nb_philo];
	
	init_forks_mutex(forks,args);
	ft_creat_philo(philos,forks,args);
	return(0);
}
