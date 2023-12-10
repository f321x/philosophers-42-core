# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ***REMOVED*** <***REMOVED***@student.***REMOVED***.de>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/28 10:54:26 by codespace         #+#    #+#              #
#    Updated: 2023/12/10 18:49:41 by ***REMOVED***            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	:= philo
CC		:= cc
CFLAGS	:= -Wall -Werror -Wextra
DFLAGS 	:= -g3 # -fsanitize=thread # -fsanitize=address

HEADERS	:= -I ./includes
LIBS	:= -lpthread
SRCS	:= main.c arg_parsing.c philosopher.c thread_handling.c philo_utils.c time_utils.c monitoring.c thread_utils.c

OBJDIR	:= objs
SRCDIR	:= src

OBJS	:= $(addprefix $(OBJDIR)/, $(notdir ${SRCS:.c=.o}))
DOBJS   := $(addprefix $(OBJDIR)/, $(notdir ${SRCS:.c=.d.o}))

$(shell mkdir -p $(OBJDIR))

all: $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS)

$(OBJDIR)/%.d.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(DFLAGS) -o $@ -c $< $(HEADERS)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(HEADERS) -o $(NAME)

debug: $(DOBJS)
	$(CC) $(CFLAGS) $(DFLAGS) $(DOBJS) $(LIBS) $(HEADERS) -o philo_debug

clean:
	rm	-rf $(OBJDIR)

fclean: clean
	rm -rf $(NAME) philo_debug

re: clean all

.PHONY: all, clean, fclean, re, debug
