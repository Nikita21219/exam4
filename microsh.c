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
	if (code == ERROR)
		put_str("error: fatal\n");
	exit(1);
}

t_data	*init_new_comm(char *comm)
{
	t_data	*new;

	new = malloc(sizeof(t_data));
	if (new == NULL)
		ft_exit(ERROR);
	new->comm = comm;
	new->arg_len = 0;
	new->oper = 0;
	new->args = NULL;
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

void	add_new_arg(t_data *data, char *arg)
{
	char	**tmp;
	int		i;

	tmp = malloc(data->arg_len + 2 * sizeof(char *) * 1000);
	if (tmp == NULL)
		ft_exit(ERROR);
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
	while (++i < argc - 1)
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
	print_data(data);
	main_logic(data, env);
	return (0);
}
