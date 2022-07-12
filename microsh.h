#ifndef MICROSH_H
#define MICROSH_H

# include <stdio.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>

# define ERROR 1

typedef struct s_arg
{
	char			*arg;
	struct s_arg	*next;
}	t_arg;

typedef struct s_data
{
	char			*comm;
	t_arg			*arg;
	struct s_data	*next;
}	t_data;


#endif
