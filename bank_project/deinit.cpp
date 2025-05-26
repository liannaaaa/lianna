#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <semaphore.h>

int main() {
    const char* shm_name = "/TBank";
    const char* sem_name = "/TBankSem";

    if (shm_unlink(shm_name) == 0) {
        std::cout << "Shared memory successfully unlinked.\n";
    } else {
	    std::cerr << "Can't unlink shared memory";
    }
    if (sem_unlink(sem_name) == 0) {
        std::cout << "Semaphore successfully unlinked.\n";
    } else {
	    std::cerr << "Can't unlink semaphore";
    }

    return 0;
}

