# include "microsh.h"

void	add_command(void)
{
	return ;
}

int	ft_strlen(char *str)
{
	int	count;

	count = 0;
	while (*str++)
		count++;
	return (count);
}

void	put_str(char *buf)
{
	write(2, buf, ft_strlen(buf));
}

void	ft_exit(int code, char *str)
{
	if (code == ERROR)
		put_str("error: fatal\n");
	if (code == ERROR_EXECVE)
	{
		put_str("error: cannot execute ");
		put_str(str);
		put_str("\n");
	}
	exit(1);
}

t_data	*init_new_comm(char *comm)
{
	t_data	*new;

	new = malloc(sizeof(t_data));
	if (new == NULL)
		ft_exit(ERROR, NULL);
	new->comm = comm;
	new->arg_len = 0;
	new->oper = 0;
	new->args = malloc(sizeof(char *) * 2);
	if (new->args == NULL)
		ft_exit(ERROR, NULL);
	new->args[0] = "./microshell";
	new->args[1] = NULL;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	add_new_command(t_data **data, t_data *new)
{
	t_data	*tmp;
	t_data	*tmp2;

	if (*data == NULL)
		*data = new;
	else
	{
		tmp = *data;
		while (tmp->next)
			tmp = tmp->next;
		tmp2 = tmp;
		tmp->next = new;
		new->prev = tmp2;
	}
}

void	add_new_arg(t_data *data, char *arg)
{
	char	**tmp;
	int		i;

	tmp = malloc(data->arg_len + 3 * sizeof(char *) * 1000);
	if (tmp == NULL)
		ft_exit(ERROR, NULL);
	i = 0;
	while (data->next)
		data = data->next;
	while (data->args && data->args[i])
	{
		tmp[i] = data->args[i];
		i++;
	}
	tmp[i++] = arg;
	tmp[i] = NULL;
	free(data->args);
	data->args = tmp;
}

void	add_oper(t_data *data, int oper)
{
	while (data->next)
		data = data->next;
	data->oper = oper;
}

void	parse_commands(t_data **data, int argc, char **argv)
{
	(void) argc;
	(void) argv;
	int	i;

	i = 0;
	while (++i < argc)
	{
		add_new_command(data, init_new_comm(argv[i]));
		while (++i < argc && (strcmp(argv[i], ";") != 0))
		{
			if (strcmp(argv[i], "|") == 0)
			{
				add_oper(*data, PIPE);
				break ;
			}
			add_new_arg(*data, argv[i]);
			(*data)->arg_len++;
		}
	}
}

void	print_data(t_data *data)
{
	int i;

	while (data)
	{
		printf("command = %s, oper = %d\n", data->comm, data->oper);
		i = -1;
		while (data->args && data->args[++i])
			printf("arg = %s\n", data->args[i]);
		printf("\n");
		data = data->next;
	}
}

void	do_pipe(t_data *data)
{
	if (pipe(data->fd) == -1)
		ft_exit(ERROR, NULL);
}

void	duplicate_fds(t_data *data)
{
	if (data && data->oper == PIPE)
	{
		if (dup2(data->fd[1], STDOUT_FILENO) == -1)
			ft_exit(ERROR, NULL);
	}
	else if (data && data->prev && data->prev->oper == PIPE)
	{
		if (dup2(data->prev->fd[0], STDIN_FILENO) == -1)
			ft_exit(ERROR, NULL);
	}
}

int	get_count_commands(t_data *data)
{
	int	count;

	count = 0;
	while (data->next)
	{
		count++;
		data = data->next;
	}
	return (count);
}

void	main_logic(t_data *data, char **env)
{
	int	i;
	int	*pids;

	i = 0;
	pids = malloc(sizeof(int) * get_count_commands(data) + sizeof(int));
	if (pids == NULL)
		ft_exit(ERROR, NULL);
	while (data)
	{
		if (data->oper == PIPE)
			do_pipe(data);
		pids[i] = fork();
		if (pids[i] == -1)
			ft_exit(ERROR, NULL);
		else if (pids[i] == 0)
		{
			duplicate_fds(data);
			if (execve(data->comm, data->args, env) == -1)
				ft_exit(ERROR_EXECVE, data->comm);
		}
		else
		{
			if (waitpid(pids[i], NULL, WNOHANG) == -1)
				ft_exit(ERROR, NULL);
			if (data->oper == PIPE)
				if (close(data->fd[1]) == -1)
					ft_exit(ERROR, NULL);
			if (data->prev && data->prev->oper == PIPE)
				if (close(data->prev->fd[0]) == -1)
					ft_exit(ERROR, NULL);
		}
		data = data->next;
		i++;
	}
	pids[i] = 0;
	// i = -1;
	// while (pids[++i])
	// 	if (waitpid(pids[i], NULL, WNOHANG) == -1)
	// 		ft_exit(ERROR, NULL);
}

int	main(int argc, char **argv, char **env)
{
	t_data	*data;
	(void) env;

	data = NULL;
	parse_commands(&data, argc, argv);
	// print_data(data);
	main_logic(data, env);
	return (0);
}
