#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

int main(int argc, char** argv){
   if(argc != 2){
     std::cerr << "invalid arguments" << std::endl;
     exit(1); 
 }

 int fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, 0644);
   if(fd == -1){
     std::cerr << "Can't open/create the file" << std::endl;
     exit(1);
 }

 int new_fd = dup(fd);
   if(new_fd == -1){
     std::cerr << "can't dublicate the file" << std::endl;
     close(fd);
     exit(1);
 }

 if(write(fd, "first line\n",11) == -1){
     std::cerr << "Can't write the file" << std::endl;
     close(fd);
     exit(1);
 }

 if(write(new_fd, "second line\n",12) == -1){
       std::cerr << "Can't write the file" << std::endl;
       close(fd);
       close(new_fd);
       exit(1);
  }

 close(fd);
 close(new_fd);

}

