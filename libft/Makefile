# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rluari <rluari@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/10 15:38:38 by rluari            #+#    #+#              #
#    Updated: 2024/01/24 19:11:58 by rluari           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS			=	ft_isalnum.c ft_isprint.c ft_memcmp.c  ft_putchar_fd.c ft_split.c \
					ft_strlcat.c ft_strncmp.c ft_strcmp.c ft_substr.c ft_atoi.c ft_isalpha.c \
					ft_itoa.c ft_memcpy.c  ft_putendl_fd.c ft_strchr.c  ft_strlcpy.c \
					ft_strnstr.c ft_tolower.c ft_bzero.c   ft_isascii.c	ft_strncpy.c \
					ft_memmove.c ft_putnbr_fd.c  ft_strdup.c  ft_strlen.c  ft_strrchr.c \
					ft_toupper.c ft_calloc.c  ft_isdigit.c ft_memchr.c  ft_memset.c  \
					ft_putstr_fd.c  ft_strjoin.c ft_strjoin_free.c ft_strmapi.c ft_strtrim.c \
					ft_get_next_line.c ft_get_next_line_utils.c ft_putnbr_base.c ft_atoi_base.c \
					ft_striteri.c ft_strlcpy.c ft_strcpy.c \
					printf/ft_putnbr_fd_int.c		\
					printf/ft_print_dec.c		\
					printf/ft_print_hex.c		\
					printf/ft_print_char.c		\
					printf/ft_print_pointer.c	\
					printf/ft_print_string.c	\
					printf/ft_print_uns.c		\
					printf/ft_do_the_comparison.c	\
					ft_get_next_line_utils2.c

BONUS_SRCS		=	ft_lstadd_back.c ft_lstadd_front.c ft_lstclear.c \
					ft_lstdelone.c ft_lstlast.c ft_lstnew.c ft_lstsize.c ft_lstiter.c \
					ft_lstmap.c \

BONUS_OBJS		=	$(BONUS_SRCS:.c=.o)

OBJS			=	$(SRCS:.c=.o)

CC				=	cc

RM				=	rm -f

CFLAGS			=	-Wall -Wextra -Werror #-g -fsanitize=address,undefined,leak

NAME			=	libft.a

all:			$(NAME)

$(NAME):		$(OBJS)
					ar rcs $(NAME) $(OBJS)

clean:
					$(RM) $(OBJS) $(BONUS_OBJS)

fclean:			clean
					$(RM) $(NAME)

re:				fclean all

bonus:			$(OBJS) $(BONUS_OBJS)
					ar rcs $(NAME) $(OBJS) $(BONUS_OBJS)

.PHONY:			all clean fclean re bonus
