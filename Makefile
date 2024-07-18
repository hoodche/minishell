# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gtaza-ca <gtaza-ca@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/24 10:30:44 by igcastil          #+#    #+#              #
#    Updated: 2024/07/18 21:19:36 by gtaza-ca         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = gcc
CCFLAGS = -Wall -Wextra -Werror
MINISHELL_SRC = children.c debug.c errors.c expansions.c expansions_bis.c\
				free.c general_utils.c heredoc.c init.c matrix_utils.c\
				mini_env.c minishell.c parse.c parse_bis.c parse_bis_bis.c \
				pipes.c prompt.c ptr_recalculation.c quotation.c \
				redirections.c redirections_bis.c redirections_bis_bis.c\
				signal.c dlist.c\
				commands/commands.c commands/selector.c  commands/utils.c \
				redirections/heredoc.c redirections/input.c redirections/heredoc_expansion.c\
				redirections/output.c builtins/cd.c builtins/cd_extended.c \
				builtins/echo.c builtins/env.c builtins/exit.c \
				builtins/export.c builtins/pwd.c builtins/unset.c \
				builtins/utils.c
SRCDIR = src
MINISHELL_SRCS = $(addprefix $(SRCDIR)/, $(MINISHELL_SRC))
MINISHELL_OBJS = $(MINISHELL_SRCS:.c=.o)
INCLUDE = libft/libft.h inc/minishell.h
SUPPRSSION_FILE=$(shell realpath -P readline.supp)
ifdef DEBUG_MODE
	CCFLAGS += -DDEBUG_MODE=$(DEBUG_MODE)
endif

all: $(NAME)

$(NAME): $(MINISHELL_OBJS) $(INCLUDE) libft/libft.a
	$(CC) $(CCFLAGS) -g -o minishell $(MINISHELL_OBJS) -L./libft -lft -lreadline

libft/libft.a:
	make -C ./libft bonus

src/%.o: src/%.c
	$(CC) $(CCFLAGS) -c -o $@ $<

valgrind: $(NAME)
	valgrind --suppressions=$(SUPPRSSION_FILE) --track-fds=yes --trace-children=yes  --leak-check=full --show-leak-kinds=all --track-origins=yes ./minishell
#--track-fds=yes --trace-children=yes 
clean:
	(cd libft; make fclean; cd ..)
	rm -f $(MINISHELL_OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all bonus clean fclean re