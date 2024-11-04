all: program

program: main.o
	@ g++ -Wextra -Werror -Wall main.o -o program

main.o : main.cpp 
	@ g++ -Wextra -Werror -Wall -c main.cpp -o main.o

clean:
	@ rm -rf main.o program

fclean:
	@ rm -f main.o
