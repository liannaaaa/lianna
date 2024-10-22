#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

void do_command(char** argv){
pid_t child = fork();

if(child == 0){
	execvp(argv[0], argv);
	std::cerr << "Exec failed" << std::endl;
	exit(1);
}
else if(child < 0){
	std::cerr << "Fork failed" << std::endl;
	exit(1);
}
else{
	wait(nullptr);
}
}

int main(int argc, char** argv){
if(argc < 2){
	std::cerr << "Invalid arguments" << std::endl;
	exit(1);
}

do_command(argv + 1);
}
