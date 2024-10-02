#include <cerrno>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

int main(int argc, char** argv){
  if(argc!= 2){
    std::cout <<"Invalid arguments" << std::endl;
    return 1;
  }
  int desc = open(argv[1],O_RDWR);
  if(desc == -1){
    std::cout << "Error opening file" << std::endl;
    return errno;
  }
  
  const int BUF = 256;
  char buffer[BUF];
  ssize_t bytes_read = read(desc, buffer, BUF);

  if(bytes_read == -1){
      std::cout << "Error reading file" << std::endl;
      return errno;
    }

  while((bytes_read = read(desc, buffer, BUF)) > 0){
      buffer[bytes_read] = '\0';
      lseek(desc, -bytes_read, SEEK_CUR);
      
      ssize_t written_bytes = write(desc, "\0", bytes_read);
      if (written_bytes == -1) {
        std::cout << "Error writing file" << std::endl;
        close(desc);
        return errno;
      }
    }

  close(desc);

  if(unlink(argv[1]) != 0)
    {
      std::cout << "Error:cant delete file" << std::endl;
      return errno;
    }
  
  std::cout << "File is deleted" << std::endl;
  return 0;
}
