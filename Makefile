CC = gcc #-fsanitize=address -g3
CFLAGS = -Wall -Wextra -Werror 
NAME = philo
SRCF = philo.c utils.c utils1.c
OBJF = $(SRCF:.c=.o)

all : $(NAME)

$(NAME) : $(OBJF)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJF)

clean :
	@rm -rf $(OBJF)
fclean : clean
	@rm -rf $(NAME)
re : clean all

.PHONY : all clean fclean re