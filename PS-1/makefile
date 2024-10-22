all: do-command

do-command : main.o
	@ g++ -Wextra -Werror -Wall main.o -o do-command

main.o : main.cpp 
	@ g++ -Wextra -Werror -Wall -c main.cpp -o main.o

clean:
	@ rm -rf main.o do-command

fclean:
	@ rm -f main.o
