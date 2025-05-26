#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstdlib>
#include <semaphore.h>
#include <string>
#include <sstream>
#include <sys/stat.h>
#include <cstddef>

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

    void help() {
        std::cout << "Available commands:\n"
                  << "  print <acc>\n"
                  << "  transfer <acc1> <acc2> <amount>\n"
                  << "  add <amount>\n"
                  << "  sub <amount>\n"
                  << "  freeze <acc>\n"
                  << "  unfreeze <acc>\n"
                  << "  setmin <acc> <value>\n"
                  << "  setmax <acc> <value>\n"
                  << "  help\n";
    }

    void print_info(size_t i) {
        if (i >= num_accounts) {
            std::cout << "Invalid account number\n";
            return;
        }
        std::cout << "Account " << i << ": balance=" << acc[i].current_balance
                  << ", min=" << acc[i].min_balance << ", max=" << acc[i].max_balance
                  << ", frozen=" << (acc[i].is_frozen ? "yes" : "no") << "\n";
    }

    bool transfer(size_t acc1, size_t acc2, int amount) {
        if (acc1 >= num_accounts || acc2 >= num_accounts || amount <= 0){
            return false;
	}
        Account& from = acc[acc1];
        Account& to = acc[acc2];
        if (from.is_frozen || to.is_frozen){
            return false;
	}

        if (from.current_balance - amount < from.min_balance ||
            to.current_balance + amount > to.max_balance){
            return false;
	}
        from.current_balance -= amount;
        to.current_balance += amount;
        return true;
    }

    bool add_all(int sum) {
        for (size_t i = 0; i < num_accounts; ++i) {
            if (acc[i].is_frozen ||
                acc[i].current_balance + sum < acc[i].min_balance ||
                acc[i].current_balance + sum > acc[i].max_balance){
                return false;
	    }
        }
        for (size_t i = 0; i < num_accounts; ++i){
            acc[i].current_balance += sum;
	}
        return true;
    }

    bool sub_all(int sub) {
        for (size_t i = 0; i < num_accounts; ++i) {
            if (acc[i].is_frozen ||
                acc[i].current_balance - sub < acc[i].min_balance ||
                acc[i].current_balance - sub > acc[i].max_balance){
                return false;
	    }
        }
        for (size_t i = 0; i < num_accounts; ++i){
            acc[i].current_balance -= sub;
	}
        return true;
    }

    bool set_limit(size_t num, bool min, int value) {
        if (num >= num_accounts){
            return false;
	}
        if (min){
            acc[num].min_balance = value;
	}
	else{
            acc[num].max_balance = value;
	}
        return true;
    }

    bool freeze(size_t num, bool yes) {
        if (num >= num_accounts){
            return false;
	}
        acc[num].is_frozen = yes;
        return true;
    }
};

int main() {
    const char* shm_name = "/TBank";

    int fd = shm_open(shm_name, O_RDWR, 0666);
    if (fd < 0) {
        std::cerr << "Can't open shared memory.\n";
        exit(errno);
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        std::cerr << "fstat failed\n";
        close(fd);
        exit(errno);
    }
    size_t map_size = sb.st_size;

    void* ptr = mmap(nullptr, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        std::cerr << "mmap failed\n";
        close(fd);
        exit(errno);
    }

    Bank* bank = reinterpret_cast<Bank*>(ptr);
    sem_t* sem = &bank->sem;

    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        std::string command;
        iss >> command;

        sem_wait(sem);

        if (command == "print") {
            size_t acc_1;
            iss >> acc_1;
            bank->print_info(acc_1);
        } else if (command == "transfer") {
            size_t acc_1, acc_2;
            int money;
            iss >> acc_1 >> acc_2 >> money;
            std::cout << (bank->transfer(acc_1, acc_2, money) ? "Transfered successfully\n" : "Transfer failed\n");
        } else if (command == "help") {
            bank->help();
        } else if (command == "add") {
            int money;
            iss >> money;
            std::cout << (bank->add_all(money) ? "Added successfully\n" : "failed to add\n");
        } else if (command == "sub") {
            int money;
            iss >> money;
            std::cout << (bank->sub_all(money) ? "Subtructed succesfully\n" : "failed to sub\n");
        } else if (command == "freeze") {
            size_t acc_1;
            iss >> acc_1;
            std::cout << (bank->freeze(acc_1, true) ? "Account is frozen\n" : "freezing failed\n");
        } else if (command == "unfreeze") {
            size_t acc_1;
            iss >> acc_1;
            std::cout << (bank->freeze(acc_1, false) ? "Account is unfrozen\n" : "Operation failed\n");
        } else if (command == "setmin") {
            size_t acc_1;
            int money;
            iss >> acc_1 >> money;
            std::cout << (bank->set_limit(acc_1, true, money) ? "Min is set\n" : "Failed to set min\n");
        } else if (command == "setmax") {
            size_t acc_1;
            int money;
            iss >> acc_1 >> money;
            std::cout << (bank->set_limit(acc_1, false, money) ? "Max is set\n" : "Failed to set max\n");
        } else {
            std::cout << "Unknown command\n";
        }

        sem_post(sem);
    }

    munmap(ptr, map_size);
    close(fd);
    return 0;
}

