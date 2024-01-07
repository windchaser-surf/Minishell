NAME		= minishell

SRC			=	main.c ft_basic_error_checker.c ft_lexer.c ft_parser.c env.c \
				parser/ft_getting_path.c parser/ft_parser_handlers.c parser/ft_parser_utils_1.c parser/ft_parser_utils_2.c parser/ft_parser_utils_3.c\
				lexer/ft_lexer_utils_1.c lexer/ft_lexer_utils_2.c \
				expander/ft_expander_handlers.c \
				ft_expander.c builtin/cd.c builtin/check_builtin.c builtin/echo.c builtin/exit.c builtin/export.c builtin/unset.c \
				execution/exec1.c execution/exec2.c execution/exec_main.c execution/exec_utils.c

OBJS		= $(SRC:.c=.o)

CC			= cc

LDFLAGS 	= -lreadline

CFLAGS		= -Wall -Werror -Wextra -Wno-error=unused-result -g #-fsanitize=address,undefined,leak -fno-omit-frame-pointer

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
