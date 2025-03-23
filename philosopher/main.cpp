#include <iostream>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <cstdlib>  

sem_t* sticks[5];

void philosopher(int id) {
    int left_stick = id;              
    int right_stick = (id + 1) % 5;
    if (id == 0) {
	std::swap(left_stick, right_stick);
    }
    while (true) {
        std::cout << "Philosopher " << id << " is thinking\n";
        sleep(3); 

        sem_wait(sticks[left_stick]);
        sem_wait(sticks[right_stick]);

        std::cout << "Philosopher " << id << " is eating.\n";
        sleep(3); 
        sem_post(sticks[left_stick]);
        sem_post(sticks[right_stick]);
    }
}

int main() {
    for (int i = 0; i < 5; i++) {
        std::string semaphore_name = "/stick #" + std::to_string(i);
        sticks[i] = sem_open(semaphore_name.c_str(), O_CREAT, 0666, 1);
        if (sticks[i] == SEM_FAILED) {
            std::cerr << "Error: can't create the semaphore " << semaphore_name << std::endl;
            exit(1);
        }
    }

    pid_t stick_pid[5];
    for (int i = 0; i < 5; i++) {
        stick_pid[i] = fork();
	if (stick_pid[i] < 0) {
            std::cerr << "Error: can't fork for philosopher " << i << std::endl;
            exit(1);
        }else if(stick_pid[i] == 0){
            philosopher(i);
            exit(0);
	}
    }
    for (int i = 0; i < 5; i++) {
        waitpid(stick_pid[i], nullptr, 0);
    }

    for (int i = 0; i < 5; i++) {
        sem_close(sticks[i]);
        std::string semaphore_name = "/stick #" + std::to_string(i);
        sem_unlink(semaphore_name.c_str());
    }

    return 0;
}
