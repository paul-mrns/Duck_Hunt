##
## Paul Mourens Project
## Duck Hunt cpp
## Makefile
##

GREEN        = \033[0;32m
RESET         = \033[0m
BLUE          = \033[94m
RED 		  = \033[31m
BOLD 		  = \033[1m
YELLOW        = \033[0;33m

SRC =	src/main.cpp 	\
		src/Core.cpp	\
		src/Score.cpp	\
		src/Audio.cpp 	\
		src/game_states/Menu.cpp	\
		src/game_states/Play.cpp \
		src/assets/MenuAssets.cpp	\
		src/assets/PlayAssets.cpp	\
		src/animation/Dog.cpp	\
		src/animation/AAnimation.cpp	\
		src/animation/Duck.cpp

OBJ = $(SRC:.cpp=.o)
OBJ := $(addprefix obj/, $(OBJ))

NAME = Duck_Hunt

CXXFLAGS = -I ./include -Wall -Wextra -Werror -std=c++17

LDFLAGS = -lsfml-system -lsfml-window -lsfml-network -lsfml-graphics -lsfml-audio

all: $(NAME)

obj/%.o: %.cpp
	@mkdir -p $(dir $@)
	@printf "$(GREEN)[OK]$(BLUE)Compilation de $<...$(RESET)\n"
	@g++ $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	@printf "$(GREEN)[OK]$(BLUE)Édition des liens et création de l'exécutable $(NAME)...$(RESET)\n"
	@g++ -g $(OBJ) -o $(NAME) $(LDFLAGS) -lm
	@printf "$(GREEN)Compilation terminée avec succès !$(RESET)\n"
	@printf "$(BOLD)Exécutez : $(YELLOW)$(BOLD)./$(NAME)$(RESET)$(BOLD) pour lancer l'application.$(RESET)\n"

clean:
	@printf "$(GREEN)[OK]$(RED)Suppression des fichiers objets...$(RESET)\n"
	@rm -f $(OBJ)
	@printf "$(GREEN)[OK]$(RED)Suppression des répertoires créés...$(RESET)\n"
	@rm -rf obj/

fclean: clean
	@printf "$(GREEN)[OK]$(RED)Suppression de l'exécutable $(NAME)...$(RESET)\n"
	@rm -f $(NAME)

re: fclean all
