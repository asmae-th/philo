#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include <sys/time.h>
#include <stdbool.h>

typedef struct args {
    int nb_philo;
    size_t time_to_die;
    size_t time_to_eat;
    size_t time_to_sleep;
    int num_times_to_eat;
} t_args;

typedef struct philo {
    int id;
    pthread_t thread;
    pthread_mutex_t *r_fork;
    pthread_mutex_t *l_fork;
    size_t last_eat_time;
    int meals_eaten;
    t_args *args;
    bool is_alive;
} t_philo;

size_t get_time() {
    struct timeval tv;
    if (gettimeofday(&tv, NULL))
        return (0);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int ft_atoi(char *str) {
    int result = 0;
    int sign = 1;

    while (*str == ' ' || *str == '\t') str++;
    if (*str == '+') str++;
    else if (*str == '-') {
        sign = -1;
        str++;
    }
    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }
    return result * sign;
}

int ft_check_args(int ac, char **av, t_args *args) {
    if (ac >= 5) {
        args->nb_philo = ft_atoi(av[1]);
        args->time_to_eat = ft_atoi(av[2]);
        args->time_to_die = ft_atoi(av[3]);
        args->time_to_sleep = ft_atoi(av[4]);
        args->num_times_to_eat = (ac == 6) ? ft_atoi(av[5]) : -1;
        return 0;
    }
    return 1;
}

void init_forks_mutex(pthread_mutex_t *forks, t_args args) {
    for (int i = 0; i < args.nb_philo; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }
}

void *ft_monitor(void *args) {
    t_philo *philo;
    size_t current_time;

    philo = (t_philo *)args;
    while (true) {
        for (int i = 0; i < philo->args->nb_philo; i++) {
            if (!philo[i].is_alive) continue;
            current_time = get_time();
            if ((current_time - philo[i].last_eat_time) > philo[i].args->time_to_die) {
                printf("%zu Philosopher %d has died after %zu milliseconds without eating\n",
                       current_time, philo[i].id, current_time - philo[i].last_eat_time);
                for (int j = 0; j < philo->args->nb_philo; j++) {
                    philo[j].is_alive = false;
                }
                return NULL;
            }
        }
        usleep(100); // Sleep for a short time to reduce CPU usage
    }
}

void ft_print_log(int id, const char *str) {
    size_t time = get_time();
    printf("%zu %d %s\n", time, id, str);
}

void *ft_routine(void *philo) {
    t_philo *philos = (t_philo *)philo;
    int nb_meals = philos->args->num_times_to_eat;

    while (nb_meals != 0 && philos->is_alive) {
        // Think before trying to take forks
        usleep(100); // Small delay to reduce contention

        // Pick up forks in a specific order
        pthread_mutex_lock(philos->l_fork);
        ft_print_log(philos->id, "has taken a fork");
        pthread_mutex_lock(philos->r_fork);
        ft_print_log(philos->id, "has taken a fork");

        // Eating
        philos->last_eat_time = get_time();
        ft_print_log(philos->id, "is eating");
        usleep(philos->args->time_to_eat * 1000);

        // Release forks
        pthread_mutex_unlock(philos->l_fork);
        pthread_mutex_unlock(philos->r_fork);

        // Sleeping
        ft_print_log(philos->id, "is sleeping");
        usleep(philos->args->time_to_sleep * 1000);

        nb_meals--;
    }
    return NULL;
}

void ft_create_philo(t_philo *philos, pthread_mutex_t *forks, t_args args) {
    pthread_t monitor;
    for (int i = 0; i < args.nb_philo; i++) {
        philos[i].id = i + 1;
        philos[i].r_fork = &forks[i];
        philos[i].l_fork = &forks[(i + 1) % args.nb_philo];
        philos[i].args = &args;
        philos[i].meals_eaten = 0;
        philos[i].is_alive = true;
        pthread_create(&philos[i].thread, NULL, ft_routine, &philos[i]);
    }
    pthread_create(&monitor, NULL, ft_monitor, philos);

    for (int i = 0; i < args.nb_philo; i++) {
        pthread_join(philos[i].thread, NULL);
    }
    pthread_join(monitor, NULL);
    for (int i = 0; i < args.nb_philo; i++) {
        pthread_mutex_destroy(&forks[i]);
    }
}

int main(int ac, char **av) {
    t_args args;
    if (ft_check_args(ac, av, &args)) return 1;
    
    pthread_mutex_t forks[args.nb_philo];
    t_philo philos[args.nb_philo];

    init_forks_mutex(forks, args);
    ft_create_philo(philos, forks, args);
    return 0;
}