NAME = source-file
FLAGS = -Wall -Wextra
SRC = source-file.cpp
OBJ = source-file.o
GCC = g++

all: $(NAME)

$(NAME) : $(OBJ)
	@ $(GCC) $(FLAGS) $(OBJ) -o $(NAME)

$(OBJ) : $(SRC)
	@ $(GCC) $(FLAGS) -c $(SRC) -o $(OBJ)

clean:
	@ rm -rf $(OBJ) $(NAME)

fclean:
	@ rm -f $(OBJ)


