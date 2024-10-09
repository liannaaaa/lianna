#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <cerrno>

void initialize(int argc, char** argv){
  if(argc != 2){
    std::cerr << "invalid arguments" << std::endl;
    exit(1);
}
  
int fd = open(argv[1], O_RDONLY);
  if(fd == -1){
    std::cerr << "can't open the file" << std::endl;
    exit(1);
}

 if(dup2(fd, 0) == -1){
    std::cerr << "can't redirect the file" << std::endl;
    close(fd);
    exit(1);
}
 close(fd); 

}

int main(int argc, char** argv){
 
 initialize(argc, argv);

 std::string input;
 std::string reversed;
 std::cin >> input;

 for(int i = input.size() - 1; i >= 0; i--){
    reversed += input[i];
 }
 std::cout << reversed << std::endl;

 return 0;
}
