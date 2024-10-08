/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmae <asmae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 10:22:12 by asmae             #+#    #+#             */
/*   Updated: 2024/10/08 12:10:36 by asmae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


void *ft_thread(void *void_philo)
{
    t_philo *philo = (t_philo *)void_philo;

    if (philo->id % 2 == 0)
        usleep(2000);  // Adjust timing for odd philosophers
    while (!philo->data->stop_simulation )
	{
        // Thinking
        pthread_mutex_lock(&philo->data->print_mutex);
        printf("%ld %d is thinking\n", ft_time() - philo->start_time, philo->id);
        pthread_mutex_unlock(&philo->data->print_mutex);

        // Taking forks
        pthread_mutex_lock(philo->right_fork);
        pthread_mutex_lock(&philo->data->print_mutex);
        printf("%ld %d has taken a fork\n", ft_time() - philo->start_time, philo->id);
        pthread_mutex_unlock(&philo->data->print_mutex);

        pthread_mutex_lock(philo->left_fork);
        pthread_mutex_lock(&philo->data->print_mutex);
        printf("%ld %d has taken a fork\n", ft_time() - philo->start_time, philo->id);
        pthread_mutex_unlock(&philo->data->print_mutex);

        // Eating
        pthread_mutex_lock(&philo->data->print_mutex);
        printf("%ld %d is eating\n", ft_time() - philo->start_time, philo->id);
        pthread_mutex_unlock(&philo->data->print_mutex);
        
        philo->last_meal_time = ft_time();
        philo->t_eaten++;
		
		// printf("%ld %d has eaten %d times\n", ft_time() - philo->start_time, philo->id, philo->t_eaten);
		
        ft_sleep(philo->data->time_to_eat);
        
        pthread_mutex_unlock(philo->left_fork);
        pthread_mutex_unlock(philo->right_fork);

        // Sleeping
        pthread_mutex_lock(&philo->data->print_mutex);
        printf("%ld %d is sleeping\n", ft_time() - philo->start_time, philo->id);
        pthread_mutex_unlock(&philo->data->print_mutex);

        ft_sleep(philo->data->time_to_sleep);
    }
    return (NULL);
}

int ft_check_args(int ac, char **av, t_data *data)
{
    if (ac < 5)
		return (1);
	if(ac >=5 && ac <= 6)
	{
		data->nb_of_philo = ft_atoi(av[1]);
		data->t_to_die = ft_atoi(av[2]);
		data->time_to_eat = ft_atoi(av[3]);
		data->time_to_sleep = ft_atoi(av[4]);
		if (ac == 6)
			data->num_eat = ft_atoi(av[5]);
		else
			data->num_eat = -1;
		data->stop_simulation = 0;	
	}
	return (0);
}

void ft_create_philos(t_data *data)
{
	int i = 0;
    data->philosophers = malloc(sizeof(t_philo) * data->nb_of_philo);

	i = 0;
    while (i < data->nb_of_philo)
	{
        data->philosophers[i].id = i + 1;
        data->philosophers[i].t_eaten = 0;
        data->philosophers[i].last_meal_time = ft_time();
        data->philosophers[i].start_time = ft_time();
        data->philosophers[i].left_fork = &data->forks[i];
        data->philosophers[i].right_fork = &data->forks[(i + 1) % data->nb_of_philo];
        data->philosophers[i].data = data;
        pthread_create(&data->philosophers[i].thread, NULL, ft_thread, &data->philosophers[i]);
		i++;
    }
}

void ft_destroy_mutex(t_data *data)
{
	int i;

	i = 0;
    while (i < data->nb_of_philo)
	{
        pthread_mutex_destroy(&data->forks[i]);
		i++;
    }
    pthread_mutex_destroy(&data->print_mutex);
    pthread_mutex_destroy(&data->stop_mutex);
}

int ft_died(t_data *data)
{
	int i;
	
    while (1)
	{
		i = 0;
        while (i < data->nb_of_philo)
		{
            if (ft_time() - data->philosophers[i].last_meal_time > data->t_to_die)
			{
                data->stop_simulation = 1;
                pthread_mutex_lock(&data->print_mutex);
                printf("%ld %d died\n", ft_time() - data->philosophers[i].start_time, data->philosophers[i].id);
                pthread_mutex_unlock(&data->print_mutex);
                return (1);
            }
			i++;
        }
    }
}

int main(int ac, char **av)
{
    t_data data;
	int i;

    if (ft_check_args(ac, av, &data))
        return (1);
    ft_init_mutex(&data);
    ft_create_philos(&data);
    if (ft_died(&data))
	{
        ft_free(&data);
        return (0);
    }
    i = 0;
    while (i < data.nb_of_philo)
	{
        pthread_join(data.philosophers[i].thread, NULL);
		i++;
    }
    ft_free(&data);
    return (0);
}
