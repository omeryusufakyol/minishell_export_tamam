# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/28 21:49:43 by oakyol            #+#    #+#              #
#    Updated: 2024/10/28 21:49:45 by oakyol           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	libft.a
CC	=	cc
CFLAGS	=	-Wall -Wextra -Werror
RM	=	rm -f

SRC	=	ft_atoi.c \
ft_putchar_fd.c \
ft_putendl_fd.c \
ft_putnbr_fd.c \
ft_putstr_fd.c \
ft_striteri.c \
ft_isalnum.c \
ft_isalpha.c \
ft_isascii.c \
ft_isdigit.c \
ft_isprint.c \
ft_memmove.c \
ft_strjoin.c \
ft_strlcpy.c \
ft_strmapi.c \
ft_strncmp.c \
ft_strnstr.c \
ft_strrchr.c \
ft_strtrim.c \
ft_strlcat.c \
ft_tolower.c \
ft_toupper.c \
ft_calloc.c \
ft_memchr.c \
ft_memcmp.c \
ft_memcpy.c \
ft_memset.c \
ft_substr.c \
ft_strchr.c \
ft_strdup.c \
ft_strlen.c \
ft_bzero.c \
ft_itoa.c \
ft_split.c \
ft_strcmp.c \
ft_strndup.c

OBJ	=	$(SRC:.c=.o)

all:	$(NAME)

$(NAME):	$(OBJ)
			ar rcs $(NAME) $(OBJ)

clean:
			$(RM) $(OBJ)

fclean:	clean
			$(RM) $(NAME)

re:	fclean $(NAME)

.PHONY: all clean fclean re