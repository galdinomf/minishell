NAME 		= minishell
LIBFT 		= libft.a


PRINTF_PATH = ./ft_printf/
LFT_PATH 	= ./libft/
INCLUDES 	= ./includes/
SRCDIR 		= ./src/
OBJ_PATH	= ./obj/

SRC 		= 	main.c \
				treat_line.c\
				get_ind.c\
				ft_pow.c\
				hash_table_utils.c\
				ft_lstnew.c\
				ft_lstadd_back.c\
				ft_lstclear.c\
				ft_lstlast.c\
				ft_strcmp.c\
			



OBJ= $(addprefix $(OBJ_PATH), $(notdir $(SRC:.c=.o)))


#compilation
CF 			= -Wall -Wextra -Werror -g
CC 			= cc
CFI 		= -I$(INCLUDES)
LEAKS 		= valgrind
LEAKS_FILE	= valgrind-out.txt



#common commands
RM =rm -f

#rules
all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	@printf "\n$(CY)Generating Push_swap executable...$(RC)\n"
	@$(CC) $(CF) -o $(NAME) $(OBJ) -L $(LFT_PATH) -lft -lreadline
	@printf "$(GR)Done!$(RC)\n\n"

$(OBJ_PATH)%.o: $(SRCDIR)%.c
	@mkdir -p $(OBJ_PATH)
	@printf "\n$(CY)Compiling source file...$(RC)\n"
	@$(CC) $(CF) -g -c -I ./includes/ $< -o $@
	@printf "$(GR)Object $@ ready!$(RC)\n\n"

$(LIBFT):
	@printf "\n$(GR)Generating Libft...$(RC)\n"
	@make -C $(LFT_PATH)
	@printf "$(GR)Libft created!$(RC)\n\n"

bonus: all

re: fclean all

rebonus: fclean bonus

leaks: 
	@$(LEAKS) ./$(NAME) 4 5 1 2 0 3
	@printf "$(GR)Leaks log ready! Check valgrind-out.txt $(RC)\n\n"

cleanleaks: 
	$(RM) $(LEAKS_FILE)
	@printf "$(GR)Leaks log file deleted.$(RC)\n\n"

clean:
	@printf "\n$(YE)Cleaning all object files from libft...$(RC)\n"
	@make clean -C $(LFT_PATH)
	@printf "$(GR)Libft objects removed!$(RC)\n\n"
	$(RM) -rf $(OBJ) $(BONUS_OBJ) objs/

fclean: clean
	@printf "\n$(YE)Cleaning all additional objects and libraries...$(RC)\n"
	$(RM) -rf $(NAME) $(BONUS_OBJ) objs/
	@make fclean -C $(LFT_PATH)
	@printf "$(GR)All libraries removed!$(RC)\n\n"
	make cleanleaks

.PHONY: clean fclean re rebonus all bonus

# Colors
GR	= \033[32;1m
RE	= \033[31;1m
YE	= \033[33;1m
CY	= \033[36;1m
RC	= \033[0m
