#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <cstdlib>

#define MAX_ACCOUNTS 20

struct Account {
    int current_balance;
    int min_balance;
    int max_balance;
    bool is_frozen;
};

struct Bank {
    size_t num_accounts;
    sem_t sem;
    Account acc[];
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <num_accounts>\n";
        return 1;
    }

    int num_accounts = std::stoi(argv[1]);
    if (num_accounts <= 0 || num_accounts > MAX_ACCOUNTS) {
        std::cerr << "Invalid number of accounts\n";
        return 1;
    }

    size_t total_size = sizeof(Bank) + num_accounts * sizeof(Account);

    int fd = shm_open("/TBank", O_CREAT | O_TRUNC | O_RDWR, 0666);
    if (fd == -1) {
        std::cerr << "Failed to open shared memory\n";
        return 1;
    }

    if (ftruncate(fd, total_size) == -1) {
        std::cerr << "ftruncate error\n";
        return 1;
    }

    void* ptr = mmap(nullptr, total_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        std::cerr << "mmap error\n";
        return 1;
    }
    close(fd);

    Bank* bank = reinterpret_cast<Bank*>(ptr);
    bank->num_accounts = num_accounts;

    if (sem_init(&bank->sem, 1, 1) == -1) {
        std::cerr << "sem_init failed\n";
        return 1;
    }

    for (int i = 0; i < num_accounts; i++) {
        bank->acc[i].current_balance = 0;
        bank->acc[i].min_balance = 0;
        bank->acc[i].max_balance = 10000;
        bank->acc[i].is_frozen = false;
    }
    return 0;
}

