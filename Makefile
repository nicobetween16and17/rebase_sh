NAME			=	minishell

SRCDIR =	./

SRC				=	builtins.c\
						builtins2.c\
						dollar_signs.c\
						env.c\
						env_get.c\
						env_lst.c\
						env_utils.c\
						errno.c\
						exec.c\
						forbidden.c\
						heredoc.c\
						main.c\
						parse_utils.c\
						parsing.c\
						pipe.c\
						redirect.c\
						signal.c\
						split_pipe.c\
						utils.c

SRCS	=	$(addprefix $(SRCDIR), $(SRC))

OBJS	=	$(SRCS:.c=.o)

CC	=	gcc

CFLAGS	=	-Wall -Werror -Wextra -I /Users/$(USER)/.brew/Cellar/readline/8.2.1/include

all:	$(NAME)

$(NAME):	$(SRCS) $(OBJS)
			@make -C libft bonus
			@$(CC) -lreadline libft/libft.a $(CFLAGS) -L /Users/$(USER)/.brew/Cellar/readline/8.2.1/lib						$(OBJS)						-o						$(NAME)

%.o:%.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
			@rm -f $(OBJS)

fclean:	clean
				@make -C libft fclean
				@rm -f $(NAME)

re: fclean all builtins.c
