CC = g++
FLAGS = -Wextra -Wall -Werror
NAME = readfile
SRC = main.cpp
OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@ $(CC) $(FLAGS) $(OBJ) -o $(NAME)

$(OBJ): $(SRC)
	@ $(CC) $(FLAGS) -c $< -o $@

clean:
	@ rm -f $(NAME) $(OBJ)
