/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <***REMOVED***@student.***REMOVED***.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 09:07:33 by codespace         #+#    #+#             */
/*   Updated: 2023/12/06 12:12:51 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	sleeping(philo_data_t *philo_data)
{
	safe_print("is sleeping", philo_data->number, get_time() - philo_data->start_time, philo_data->print_mutex);
	if (philo_data->alive)
		usleep(philo_data->time_to_sleep * 1000);
	if (philo_data->alive)
		safe_print("is thinking", philo_data->number, get_time() - philo_data->start_time, philo_data->print_mutex);
}

static void calculate_forks(philo_data_t *data, unsigned int *forks)
{
	if ((get_time() - data->last_meal_ts) < data->time_to_die / 2)
		usleep(data->time_to_die / 3);
	if (data->amount % 2)
	{
		forks[0] = data->number - 1;
		forks[1] = (data->number) % data->amount;
	}
	else
	{
		forks[0] = (data->number) % data->amount;
		forks[1] = data->number - 1;
	}
}

static bool	eating(philo_data_t *data)
{
	unsigned int	forks[2];

	calculate_forks(data, forks);
	if (pthread_mutex_lock(&(data->fork_array[forks[0]])))
		return (false);
	safe_print("has taken a fork", data->number, get_time() - data->start_time, data->print_mutex);
	if (!data->alive || pthread_mutex_lock(&(data->fork_array[forks[1]])))
	{
		pthread_mutex_unlock(&(data->fork_array[forks[0]]));
		return (false);
	}
	if (data->alive)
	{
		safe_print("has taken a fork", data->number, get_time() - data->start_time, data->print_mutex);
		safe_print("is eating", data->number, get_time() - data->start_time, data->print_mutex);
		data->last_meal_ts = get_time();
		usleep(data->time_to_eat * 1000);
		// data->last_meal_ts = get_time();
	}
	pthread_mutex_unlock(&(data->fork_array[forks[0]]));
	pthread_mutex_unlock(&(data->fork_array[forks[1]]));
	return (true);
}

void	*philo(void *philo_data)
{
	philo_data_t	*data;

	data = (philo_data_t *)philo_data;

	// delaying some time to prevent deadlock
	if (data->number % 2)
		usleep(100);

	// setting last meal to start time
	data->last_meal_ts = get_time();

	// main loop
	while (data->alive && data->amount > 1 && data->min_eat_number != 0)
	{
		if (!eating(philo_data) || !data->alive)
			break ;
		if (data->min_eat_number > 0)
			data->min_eat_number--;
		if (data->alive && data->min_eat_number != 0)
			sleeping(data);
	}
	if ((data->min_eat_number != 0 && data->first_dead) || data->amount == 1)
	{
		data->alive = false;
		// usleep(2000);
		// safe_print("died", data->number, get_time() - data->start_time, data->print_mutex);
	}
	if (data->min_eat_number == 0)
		data->eaten_enough = true;
	return (NULL);
}

// ◦timestamp_in_ms X has taken a fork
// ◦timestamp_in_ms X is eating
// ◦timestamp_in_ms X is sleeping
// ◦timestamp_in_ms X is thinking
// ◦timestamp_in_ms X died
