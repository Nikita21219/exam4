#ifndef MICROSH_H
#define MICROSH_H

# include <stdio.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>

# define ERROR 1
# define PIPE 1
# define SEMICOLON 2

typedef struct s_data
{
	char			*comm;
	char			**args;
	int				oper;
	int				arg_len;
	struct s_data	*next;
}	t_data;


#endif
