/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ***REMOVED*** <***REMOVED***@student.***REMOVED***.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 09:07:33 by codespace         #+#    #+#             */
/*   Updated: 2023/12/10 18:48:06 by ***REMOVED***            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	sleeping(t_philo_data *philo_data)
{
	safe_print("is sleeping", philo_data->number,
		get_time() - philo_data->start, philo_data->prt_lck);
	if (check_alive(philo_data))
		usleep(philo_data->time_to_sleep * 1000);
	if (check_alive(philo_data))
		safe_print("is thinking", philo_data->number,
			get_time() - philo_data->start, philo_data->prt_lck);
}

static void	calculate_forks(t_philo_data *data, unsigned int *forks)
{
	if ((get_time() - data->lm_ts) < data->time_to_die / 2)
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

static bool	eating(t_philo_data *data)
{
	unsigned int	forks[2];

	calculate_forks(data, forks);
	if (pthread_mutex_lock(&(data->fork_array[forks[0]])))
		return (false);
	safe_print("has taken a fork", data->number, get_time()
		- data->start, data->prt_lck);
	if (!check_alive(data) || pthread_mutex_lock(&(data->fork_array[forks[1]])))
	{
		pthread_mutex_unlock(&(data->fork_array[forks[0]]));
		return (false);
	}
	if (check_alive(data))
	{
		safe_print("has taken a fork", data->number, get_time()
			- data->start, data->prt_lck);
		safe_print("is eating", data->number, get_time()
			- data->start, data->prt_lck);
		write_ts(data);
		usleep(data->time_to_eat * 1000);
	}
	pthread_mutex_unlock(&(data->fork_array[forks[0]]));
	pthread_mutex_unlock(&(data->fork_array[forks[1]]));
	return (true);
}

void	*philo(void *philo_data)
{
	t_philo_data	*data;

	data = (t_philo_data *)philo_data;
	if (data->number % 2)
		usleep(100);
	write_ts(data);
	while (check_alive(data) && data->amount > 1 && data->min_eat_number != 0)
	{
		if (!eating(philo_data) || !check_alive(data))
			break ;
		if (data->min_eat_number > 0)
			data->min_eat_number--;
		if (check_alive(data) && data->min_eat_number != 0)
			sleeping(data);
	}
	if ((data->min_eat_number != 0 && data->first_dead) || data->amount == 1)
	{
		pthread_mutex_lock(data->alive_mutex);
		data->alive = false;
		pthread_mutex_unlock(data->alive_mutex);
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
