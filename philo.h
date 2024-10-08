/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmae <asmae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 10:23:57 by asmae             #+#    #+#             */
/*   Updated: 2024/10/07 18:51:32 by asmae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef PHILO_H
#define PHILO_H
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct s_philo
{
    int id;
    int t_eaten;
    long last_meal_time;
    long start_time;
    pthread_t thread;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    struct s_data *data;
} t_philo;

typedef struct s_data
{
    int nb_of_philo;
    long t_to_die;
    long time_to_eat;
    long time_to_sleep;
    int num_eat;
    int stop_simulation;

    pthread_mutex_t *forks;
    pthread_mutex_t print_mutex;
    pthread_mutex_t stop_mutex;
    t_philo *philosophers;
} t_data;
// const char *colors[] = {
//     "\033[0;31m", // Red
//     "\033[0;32m", // Green
//     "\033[0;33m", // Yellow
//     "\033[0;34m", // Blue
//     "\033[0;35m", // Magenta
//     "\033[0;36m", // Cyan
//     "\033[0;37m", // White
//     "\033[0;90m"  // Bright Black
// };

//utils
// size_t get_time();
// int	ft_atoi(char *str);

long ft_time(void);
int ft_atoi(char *str);
void ft_sleep(long time);
void *ft_thread(void *void_philo);
int ft_check_args(int ac, char **av, t_data *data);
void ft_create_philos(t_data *data);
void ft_init_mutex(t_data *data);
void ft_destroy_mutex(t_data *data);
void ft_free(t_data *data);
int ft_died(t_data *data);

#endif