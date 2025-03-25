#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <cstring>
int main()
{
    struct sockaddr_in server_addr;
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket == -1) {
        std::cerr << "socket creation error";
        exit(errno);
    } 
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    server_addr.sin_addr.s_addr = inet_addr("146.190.62.39");
    int connection = connect(client_socket, (struct sockaddr*)&server_addr, sizeof(seCrver_addr));
    if(connection == -1) {
        std::cerr << "connection error";
        exit(errno); 
    }
    std::string message = "GET / HTTP/1.1\r\nHost: httpforever.com\r\nConnection: close\r\n\r\n";
    int sent = send(client_socket, message.c_str(), message.size(), 0);
    if(sent == -1) {
        std::cerr << "sending error";
        exit(errno); 
    }
    char buff[1024];
    int rv = 0;
    while((rv = recv(client_socket, buff, 1001, 0)) > 0) {
        buff[rv] = '\0';
        std::cout << buff << std::endl;
    }
    if(rv == -1) {
        std::cerr << "receiving error";
        exit(errno); 
    }
    close(client_socket);
    
}
