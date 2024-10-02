#include <cerrno>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

  int main(int argc, char** argv) {
      if (argc != 3) {
          std::cout << "Invalid arguments" << std::endl;
          
	  return 1;
  }

  int source = open(argv[1], O_RDONLY);
      if (source == -1) {
          std::cout << "Error opening source file" << std::endl;
    
	  return errno;
  }

  int destination = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY , 0666);
      if (destination == -1) {
          std::cout << "Error opening destination file" << std::endl;
          close(source);
    
	  return errno;
  }

  const int buf_size = 100;
  char BUF[buf_size];
  ssize_t bytes_read;

  
  while ((bytes_read = read(source, BUF, sizeof(BUF) - 1)) > 0) {
    BUF[bytes_read] = '\0'; 
    ssize_t written_bytes = write(destination, BUF, bytes_read);
    
      if (written_bytes != bytes_read) {
          std::cout << "Error writing destination file" << std::endl;
          close(source);
          close(destination);
       
	  return errno;
    }
    
      bytes_read = read(source, BUF, sizeof(BUF) - 1);
  }

      if (bytes_read == -1) {
          std::cout << "Error reading source file" << std::endl;
          close(source);
          close(destination);
    
	  return errno;
  }

    close(source);
    close(destination);

    std::cout << "The destination file has been copied" << std::endl;
    return 0;
  }
