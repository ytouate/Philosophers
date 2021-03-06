/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/13 01:49:23 by ytouate           #+#    #+#             */
/*   Updated: 2022/04/19 16:38:46 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_eat(t_data *data)
{
	if (sem_wait(data->args.forks) == -1)
		exit(1);
	print_message(data, FORK);
	if (sem_wait(data->args.forks) == -1)
		exit(1);
	print_message(data, FORK);
	data->last_meal_time = current_timestamp();
	print_message(data, EATING);
	ft_usleep(data->args.time_to_eat);
	data->meals_track += 1;
	if (sem_post(data->args.forks) == -1)
		exit(1);
	if (sem_post(data->args.forks) == -1)
		exit(1);
}

void	*routine(void *arg)
{
	t_data	*data;

	data = arg;
	while (1)
	{
		ft_eat(data);
		print_message(data, SLEEPING);
		ft_usleep(data->args.time_to_sleep);
		print_message(data, THINKING);
	}
	return (NULL);
}

void	start_routine(int ac, t_args args)
{
	int			i;
	pid_t		*pid;
	t_data		**philos;

	i = -1;
	philos = malloc(sizeof(t_data *) * args.num_of_philos);
	pid = malloc(sizeof(pid_t) * args.num_of_philos);
	if (!pid || !philos)
		return ;
	args.start_time = current_timestamp();
	while (++i < args.num_of_philos)
	{
		philos[i] = init_needed_data(args, philos, i);
		philos[i]->last_meal_time = current_timestamp();
		pid[i] = fork();
		if (pid[i] == -1)
			exit(1);
		if (pid[i] == 0)
		{
			set_task(philos[i]);
			exit(EXIT_SUCCESS);
		}
		usleep(50);
	}
	ft_wait(ac, pid, args);
}

void	set_task(t_data *philo)
{
	pthread_t	start;
	pthread_t	check;

	if (pthread_create(&start, NULL, routine, philo) != 0)
		exit(0);
	if (pthread_create(&check, NULL, ft_end, philo) != 0)
		exit(1);
	if (pthread_join(start, NULL) != 0)
		exit(1);
	if (pthread_join(check, NULL) != 0)
		exit(1);
}

int	main(int ac, char **av)
{
	t_args		args;

	if (check_args(ac, av))
		if (data_init(&args, ac, av))
			start_routine(ac, args);
}
