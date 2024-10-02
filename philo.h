/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahtouh <atahtouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 10:23:57 by asmae             #+#    #+#             */
/*   Updated: 2024/10/02 10:05:21 by atahtouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef PHILO_H
#define PHILO_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include <sys/time.h>
#include <stdbool.h>

typedef struct args
{
    int nb_philo;
    size_t time_to_die;
    size_t time_to_eat;
    size_t time_to_sleep;
    int num_times_to_eat;
} t_args;

typedef struct philo
{
    int id;
    pthread_t thread;
    pthread_mutex_t *r_fork;
    pthread_mutex_t *l_fork;
    size_t last_eat_time;
    int meals_eaten;
    t_args *args;  // Pointer to args structure
    bool is_alive;  // Track if philosopher is alive
} t_philo;

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
size_t get_time();
int	ft_atoi(char *str);
#endif