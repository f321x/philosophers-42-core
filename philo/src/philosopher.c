/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ***REMOVED*** <***REMOVED***@student.***REMOVED***.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 09:07:33 by codespace         #+#    #+#             */
/*   Updated: 2023/12/11 13:10:21 by ***REMOVED***            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
 * @brief Function to simulate the philosopher sleeping.
 *
 * This function is responsible for simulating the philosopher sleeping.
 * Will first sleep and then start thinking if the philosopher is still alive.
 *
 * @param philo_data Pointer to the `t_philo_data` structure.
 */
static void	sleeping(t_philo_data *philo_data)
{
	safe_print("is sleeping", philo_data->number,
		get_time() - philo_data->start, philo_data->prt_lck);
	if (check_alive(philo_data))
		better_sleep(philo_data->time_to_sleep);
	if (check_alive(philo_data))
		safe_print("is thinking", philo_data->number,
			get_time() - philo_data->start, philo_data->prt_lck);
}

// @brief Calculates the forks for a philosopher.
//
// This function calculates the forks for a philosopher.
// If the philosopher is odd, the first fork is the one to the left and
// the second one is the one to the right.
// If the philosopher is even, the first fork is the one to the right and
// the second one is the one to the left.
// This is done to prevent deadlocks. There is also a sleep at the beginning
// of the function to prevent starvation.
//
// @param data The pointer to the philosopher data.
// @param forks The pointer to the array of forks.

static void	calculate_forks(t_philo_data *data, unsigned int *forks)
{
	if ((get_time() - data->lm_ts) < data->time_to_die / 2)
		better_sleep(data->time_to_die / 3);
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

// @brief Function to simulate the philosopher eating.
//
// This function is responsible for simulating the philosopher
// eating process.
// It locks the necessary forks, prints the appropriate messages,
// and updates the philosopher's state.
//
// @param data The pointer to the philosopher's data structure.
// @return true if the philosopher successfully eats, false otherwise.
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
		better_sleep(data->time_to_eat);
	}
	pthread_mutex_unlock(&(data->fork_array[forks[0]]));
	pthread_mutex_unlock(&(data->fork_array[forks[1]]));
	return (true);
}

// @brief The philosopher function that represents the behavior
// of a philosopher thread.
//
// @param philo_data A pointer to the philosopher data structure.
// @return void* Returns NULL.
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
