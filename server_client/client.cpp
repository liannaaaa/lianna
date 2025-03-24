#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>

int main() {
    struct sockaddr_in server_address;
    
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation error");
        exit(errno);
    }

    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8888);

    int connected = connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    if (connected == -1) {
        perror("Connection failed");
        exit(errno);
    }

    std::string mess;
    while (true) {
        std::cout << "Enter message (or 'exit' to quit): ";
        std::getline(std::cin, mess);
    if (mess == "exit"){ 
	break;
    }
    int sent = send(client_socket, mess.c_str(), mess.size(), 0);
    if (sent == -1) {
        perror("Sending failed");
        break;
    }
    }

    close(client_socket);
    return 0;
}

