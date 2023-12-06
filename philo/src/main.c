/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ***REMOVED*** <***REMOVED***@student.***REMOVED***.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 10:49:08 by codespace         #+#    #+#             */
/*   Updated: 2023/11/28 12:35:33 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int argc, char **argv)
{
	t_philos		args;

	if (!parse_arguments(argc, argv, &args))
		return (1);
	if (!spawn_philosophers(&args))
		return (1);
	destroy_mutexes(&args, args.amount);
	free(args.ph_arr);
	free(args.thread_array);
	return (0);
}

// valgrind --tool=hellgrind
// -g -fsanitize=thread
// -g3
// printf("number_of_philosophers: %lu\n", args.number_of_philosophers);
// printf("time_to_die: %lu\n", args.time_to_die);
// printf("time_to_eat: %lu\n", args.time_to_eat);
// printf("time_to_sleep: %lu\n", args.time_to_sleep);
// printf("min_eat_number: %lu\n", args.min_eat_number);

// static void	wait_for_threads(t_philos *args)
// {
// 	unsigned int	index;

// 	index = 0;
// 	while (index < args->amount)
// 	{
// 		pthread_join(args->thread_array[index], NULL);
// 		index++;
// 	}
// 	destroy_mutexes(args, args->amount);
// 	free(args->ph_arr);
// 	free(args->thread_array);
// }

// void leak_checker(void)
// {
// 	system("leaks philo");
// }
