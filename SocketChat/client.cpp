#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string>

bool running = true; 

void* receive(void* arg) {
    int client_sock = *((int*)arg);
    char buffer[1024];
    while (running) {
        ssize_t rv = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
        if (rv <= 0) {
            running = false;
            break;
        }
        buffer[rv] = '\0';
        std::cout << buffer << std::endl;
    }
    return nullptr;
}

int main() {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("can't create a socket");
        exit(errno);
    }
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8888);
    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);

    if (connect(client_socket, (sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("connection error");
        close(client_socket);
        exit(errno);
    }
    std::string name;
    std::cout << "Enter your name: ";
    std::getline(std::cin, name);
    if (send(client_socket, name.c_str(), name.size(), 0) == -1) {
        perror("can't send name");
        close(client_socket);
        exit(errno);
    }

    pthread_t thread;
   if (pthread_create(&thread, nullptr, receive, &client_socket) != 0) {
        perror("can't create thread");
        close(client_socket);
        exit(errno);
    }

    while (running) {
        std::string input;
        std::getline(std::cin, input);
        if (input == "/exit") {
            if (send(client_socket, "/exit", 5, 0) == -1) {
                perror("exit command sending error");
            }
            running = false;
            break;
        }
        if (input == "/list") {
            if (send(client_socket, "/list", 5, 0) == -1) {
                perror("list command sending error");
            }
            continue;
        }
        if (send(client_socket, input.c_str(), input.size(), 0) == -1) {
            perror("message sending error");
            running = false;
            break;
        }
    }
    close(client_socket);
    pthread_join(thread, nullptr);
}
