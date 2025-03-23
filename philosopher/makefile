all: program
program: main.o
	g++ -Wall -Wextra -Werror -pthread main.o -o program
main.o: main.cpp
	g++ -Wall -Wextra -Werror -pthread -c main.cpp -o main.o
clean:
	rm -f main.o program
