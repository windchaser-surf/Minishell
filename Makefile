NAME		= minishell

SRC			= main.c ft_parser_utils.c ft_basic_error_checker.c ft_lexer.c ft_parser.c

OBJS		= $(SRC:.c=.o)

CC			= cc

LDFLAGS 	= -lreadline

CFLAGS		= -Wall -Werror -Wextra -g #-fsanitize=address,undefined,leak -fno-omit-frame-pointer

RM			= rm -rf

all: $(NAME) 

$(NAME): $(OBJS)
	$(MAKE) -C libft all bonus
	$(CC) $(CFLAGS) $(OBJS) -L. libft/libft.a -o $(NAME) $(LDFLAGS)

clean:
	$(RM) $(OBJS)
	$(MAKE) -C libft fclean

fclean:	clean
	$(RM) $(NAME)

re:	fclean all

.PHONY:	all clean fclean re