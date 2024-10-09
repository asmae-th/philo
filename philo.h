/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmae <asmae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 10:23:57 by asmae             #+#    #+#             */
/*   Updated: 2024/10/09 18:18:58 by asmae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef PHILO_H
#define PHILO_H
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct s_args
{
	int	nb_philos;
	int	t_die;
	int	t_eat;
	int	t_sleep;
	int	n_eat;
	int	check_nb_eat;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t mutex_last_eat;
	pthread_mutex_t	mutex_nb_eat;
}	t_args;

typedef struct s_philo
{
	int	id;
	long	last_eat;
	long	start_time;
	int		nb_eat;
	void	*r_fork;
	void	*l_fork;
	int		check;
	t_args	*arg;
	
}	t_philo;

typedef	struct	s_tab
{
	t_philo			*philos;
	pthread_t		*thread;
	pthread_mutex_t	*forks;
}	t_tab;

int	ft_atoi(char *str);


#endif