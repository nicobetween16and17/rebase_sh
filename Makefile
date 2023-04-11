NAME	=	minishell

SRCDIR	=	./

SRC	=	$(wildcard *.c)

SRCS	=	 $(addprefix $(SRCDIR), $(SRC))

OBJS	=	$(SRCS:.c=.o)

CC	= gcc

CFLAGS	= -Wall -Werror -Wextra -I /Users/$(USER)/.brew/Cellar/readline/8.2.1/include

all:	$(NAME)

$(NAME):	$(SRCS) $(OBJS)
		@make -C libft bonus
		@$(CC) -lreadline libft/libft.a $(CFLAGS) -L /Users/$(USER)/.brew/Cellar/readline/8.2.1/lib  $(OBJS) -o $(NAME)

%.o:%.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)

fclean:	clean
	@make -C libft fclean
	@rm -f $(NAME)

re:	fclean all