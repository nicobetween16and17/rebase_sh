#include "minishell.h"

int	print_err(int nb_arg, ...)
{
	va_list	lst;
	int		i;

	i = -1;
	va_start(lst, nb_arg);
	while (++i < nb_arg)
		ft_putstr_fd((char *)va_arg(lst, char *), 2);
	va_end(lst);
	return (1);
}