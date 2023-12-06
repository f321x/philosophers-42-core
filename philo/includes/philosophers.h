/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <***REMOVED***@student.***REMOVED***.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 10:49:22 by codespace         #+#    #+#             */
/*   Updated: 2023/12/05 14:29:15 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/time.h>
#include <limits.h>

typedef	struct philo_data_s {
	unsigned long	time_to_die;
	unsigned long	time_to_eat;
	unsigned long	time_to_sleep;
	unsigned long	last_meal_ts;
	unsigned long	start_time;
	long long		min_eat_number;
	unsigned int	number;
	unsigned int	amount;
	bool			alive;
	bool			first_dead;
	bool			eaten_enough;
	pthread_mutex_t	*print_mutex;
	pthread_mutex_t	*fork_array;
} philo_data_t;

typedef struct philos_s {
	unsigned int	amount;
	unsigned long	time_to_die;
	unsigned long	time_to_eat;
	unsigned long	time_to_sleep;
	unsigned long	start_time;
	long long		min_eat_number;
	philo_data_t	*philo_data_array;
	pthread_mutex_t	*fork_array;
	pthread_mutex_t	printing_mutex;
	pthread_t		*thread_array;
}	philos_t;

// arg_parsing.c
bool			parse_arguments(int argc, char **argv, philos_t *args);

// parsing_utils.c
bool			ft_is_positive_numeric(char *string);
long long		ft_atol(const char *str);

// thread_handling.c
bool			allocate_threads(philos_t *args);
void			destroy_mutexes(philos_t *args, unsigned int amount);
void			kill_threads(philos_t *args, unsigned int amount);
bool			spawn_philosophers(philos_t *arguments);

// philosophers.c
void			*philo(void *philo_data);

// philo_utils.c
unsigned long	get_time();
void			safe_print(const char *string, unsigned int number, unsigned long timestamp,
							pthread_mutex_t *print_block);
bool			monitor(philos_t *all_philos);
void 			better_sleep(unsigned long sleep_ms);

#endif
