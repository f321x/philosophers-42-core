/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ***REMOVED*** <***REMOVED***@student.***REMOVED***.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 10:49:22 by codespace         #+#    #+#             */
/*   Updated: 2023/12/10 19:06:24 by ***REMOVED***            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <sys/time.h>
# include <limits.h>

typedef struct philo_data_s {
	unsigned long	time_to_die;
	unsigned long	time_to_eat;
	unsigned long	time_to_sleep;
	unsigned long	lm_ts;
	unsigned long	start;
	long long		min_eat_number;
	unsigned int	number;
	unsigned int	amount;
	bool			alive;
	pthread_mutex_t	*alive_mutex;
	bool			first_dead;
	bool			eaten_enough;
	pthread_mutex_t	*prt_lck;
	pthread_mutex_t	*fork_array;
	pthread_mutex_t	*ts_mutex;
}	t_philo_data;

typedef struct philos_s {
	unsigned int	amount;
	unsigned long	time_to_die;
	unsigned long	time_to_eat;
	unsigned long	time_to_sleep;
	unsigned long	start;
	long long		min_eat_number;
	t_philo_data	*ph_arr;
	pthread_mutex_t	*fork_array;
	pthread_mutex_t	printing_mutex;
	pthread_t		*thread_array;
}	t_philos;

// arg_parsing.c
bool			parse_arguments(int argc, char **argv, t_philos *args);

// parsing_utils.c
bool			ft_is_positive_numeric(char *string);
long long		ft_atol(const char *str);

// thread_handling.c
bool			allocate_threads(t_philos *args);
bool			spawn_philosophers(t_philos *arguments);

// philosophers.c
void			*philo(void *philo_data);

// philo_utils.c
void			safe_print(const char *string, unsigned int number,
					unsigned long timestamp,
					pthread_mutex_t *print_block);
bool			check_alive(t_philo_data *philo);

// time_utils.c
unsigned long	get_time(void);
unsigned long	read_ts(t_philo_data *philo);
void			write_ts(t_philo_data *philo);

// monitoring.c
bool			monitor(t_philos *all_philos);

// thread_utils.c
bool			alloc_monitoring_mutexes(t_philo_data *current_philo);
void			set_philos_dead(t_philos *phils, unsigned int amount);
void			destroy_mutexes(t_philos *args, unsigned int amount);
void			kill_threads(t_philos *args, unsigned int amount);

#endif
