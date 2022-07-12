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

void	ft_exit(int code)
{
	if (code == MALLOC_ERR)
		put_str("error: fatal\n");
	exit(1);
}

t_data	*init_new_comm(char *comm)
{
	t_data	*new;

	new = malloc(sizeof(t_data));
	if (new == NULL)
		ft_exit(MALLOC_ERR);
	new->comm = comm;
	new->arg = NULL;
	new->next = NULL;
	return (new);
}

void	add_new_command(t_data **data, t_data *new)
{
	t_data	*tmp;

	if (*data == NULL)
		*data = new;
	else
	{
		tmp = *data;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

t_arg	*init_new_arg(char *arg)
{
	t_arg	*new;

	new = malloc(sizeof(t_arg));
	if (new == NULL)
		ft_exit(MALLOC_ERR);
	new->arg = arg;
	new->next = NULL;
	return (new);
}

void	add_new_arg(t_data *data, t_arg *new)
{
	t_arg	*tmp;

	while (data->next)
		data = data->next;
	if (data->arg == NULL)
		data->arg = new;
	else
	{
		tmp = data->arg;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void	parse_commands(t_data **data, int argc, char **argv)
{
	(void) argc;
	(void) argv;
	int	i;

	i = 0;
	while (++i < argc - 1)
	{
		add_new_command(data, init_new_comm(argv[i]));
		while (++i < argc && strcmp(argv[i], ";") != 0)
			add_new_arg(*data, init_new_arg(argv[i]));
	}
}

void	print_data(t_data *data)
{
	while (data)
	{
		printf("command = %s\n", data->comm);
		while (data->arg)
		{
			printf("arg = %s\n", data->arg->arg);
			data->arg = data->arg->next;
		}
		printf("\n");
		data = data->next;
	}
}

void	main_logic(t_data *data, char **env)
{
	(void) data;
	(void) env;
}

int	main(int argc, char **argv, char **env)
{
	t_data	*data;

	data = NULL;
	parse_commands(&data, argc, argv);
	// print_data(data);
	main_logic(data, env);
	return (0);
}
