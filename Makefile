NAME		= minishell

SRC			=	main.c ft_basic_error_checker.c ft_lexer.c ft_parser.c env.c ft_signals.c\
				parser/ft_getting_path.c parser/ft_parser_handlers.c parser/ft_parser_utils_1.c parser/ft_parser_utils_2.c parser/ft_parser_utils_3.c\
				parser/ft_parser_utils_4.c parser/ft_parser_handlers_2.c lexer/ft_lexer_utils_1.c lexer/ft_lexer_utils_2.c \
				expander/ft_expander_handlers.c  expander/ft_expander_handlers_2.c \
				ft_expander.c builtin/cd.c builtin/cd2.c builtin/check_builtin.c builtin/echo.c builtin/exit.c builtin/exit2.c builtin/export.c builtin/export2.c builtin/unset.c \
				execution/exec1.c execution/exec1_1.c execution/exec2.c execution/exec2_2.c execution/exec2_3.c execution/exec_main.c execution/exec_utils.c \
				expander/ft_expander_utils.c expander/ft_expander_utils2.c expander/ft_expander_utils3.c

OBJS		= $(SRC:.c=.o)

CC			= cc

LDFLAGS 	= -lreadline

CFLAGS		= -Wall -Werror -Wextra #-g -fsanitize=address,undefined,leak -fno-omit-frame-pointer

RM			= rm -rf

all: $(NAME) 

$(NAME): $(OBJS) 
	$(MAKE) -C libft all bonus
	$(CC) $(CFLAGS) $(OBJS) -L. libft/libft.a -o $(NAME) $(LDFLAGS)

clean:
	$(RM) $(OBJS)
	$(MAKE) -C libft clean

fclean:	clean
	$(RM) $(NAME)
	$(MAKE) -C libft fclean


re:	fclean all

.PHONY:	all clean fclean re
