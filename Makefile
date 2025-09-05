CC		=	cc
CFLAGS	=	-Wall -Wextra -Werror
# CFLAGS	+=	-Ofast
# CFLAGS	+=	-fsanitize=address -g
LDFLAGS	=	-pthread -lm
RM		=	rm -rf

NAME	=	ft_ping
SRCS	=	./src/main.c
OBJS	=	$(SRCS:.c=.o)
HEADERS	=	./include/ft_ping.h

LIBFT	=	./libft/libft.a

RESET	=	\x1b[0m
RED		=	\x1b[31m
YELLOW	=	\x1b[33m
BLUE		=	\x1b[34m

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS) $(HEADERS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $@
	@echo "$(BLUE) $(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $@ $(RESET)"
	@echo "$(YELLOW)Creating executable finished.$(RESET)"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(BLUE) $(CC) $(CFLAGS) -c $< -o $@ $(RESET)"
	@echo "$(YELLOW)Creating object file finished.$(RESET)"

$(LIBFT):
	@git submodule init
	@git submodule update --remote
	@$(MAKE) -C ./libft

clean:
	@$(RM) $(OBJS)
	@echo "$(RED) $(RM) $(OBJS) $(RESET)"
	@echo "$(YELLOW)Deleting object files finished.$(RESET)"

	@$(MAKE) clean -C ./libft

fclean: clean
	@$(RM) $(NAME)
	@echo "$(RED) $(RM) $(NAME) $(RESET)"
	@echo "$(YELLOW)Deleting executable finished.$(RESET)"

	@$(MAKE) fclean -C ./libft

re: fclean
	@$(MAKE) all

.PHONY: all clean fclean re
