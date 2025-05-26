all: init deinit client

init: init.o
	g++ -Wall -Wextra -Werror init.o -o init

deinit: deinit.o
	g++ -Wall -Wextra -Werror deinit.o -o deinit

client: client.o
	g++ -Wall -Wextra -Werror client.o -o client

init.o: init.cpp
	g++ -Wall -Wextra -Werror -c init.cpp -o init.o

deinit.o: deinit.cpp
	g++ -Wall -Wextra -Werror -c deinit.cpp -o deinit.o

client.o: client.cpp
	g++ -Wall -Wextra -Werror -c client.cpp -o client.o

clean:
	rm -f *.o init deinit client

