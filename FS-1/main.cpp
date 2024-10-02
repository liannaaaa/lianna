#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <cerrno>

int main(int argc, char **argv){
	if (argc == 2)
	{
		char buffer[100];
		int fd = open(argv[1], O_RDONLY);
		if(fd == -1)
		{
			perror("oshibka");
			return errno;
		}
		ssize_t bytes_read;
		while((bytes_read = read(fd, buffer, sizeof(buffer))) > 0)
		{
			std::cout.write(buffer, bytes_read);
		}
    		if (bytes_read == -1) 
		{
        		perror("Ошибка при чтении файла");
			close(fd);
    			return errno;
		}

    		buffer[bytes_read] = '\0';
    		std::cout <<"FILE:" << buffer;
 		close(fd);
	}
	else
	{
		std::cout<<"Invalid arguments"<<std::endl;
	}
}

