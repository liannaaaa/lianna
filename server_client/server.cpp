#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

struct args {
    int client_socket; 
    struct sockaddr_in client_address;
  
};

void* handle(void *arg) {
    struct args* arg1 = (args*)arg;
    int client_socket = arg1->client_socket;
    struct sockaddr_in client_address = arg1->client_address;
    std::cout << "Connected client with address: " << inet_ntoa(client_address.sin_addr) << "\n";

      char buffer[1001];
  
      while(true){
      int cs = recv(client_socket, buffer, 1000, 0);
      if (cs == -1) {
        perror("client socket connection error");
        close(client_socket);
      }

      if (cs > 0) {
        std::cout << "Got message:\n";
        buffer[cs] = '\0';
        std::cout << buffer << "\n";
        }
      }
     close(client_socket);
     return NULL;
}

int main() {
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
      perror("socket creation error");
      exit(errno);
  }

  struct sockaddr_in server_address;

  server_address.sin_family = AF_INET
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(8888);
  if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
      perror("bind failed");
      exit(errno);
  }

  if (listen(server_socket, 10) < 0) {
    perror("listen failed");
    exit(errno);
  }
  std::cout << "Waiting for connection\n";

  while(true) {
      int client_socket;
      struct sockaddr_in client_address;
      unsigned int client_addr_len = sizeof(client_address);

      if ((client_socket = accept(server_socket, (struct sockaddr*) &client_address, &client_addr_len)) < 0) {
          perror("accept failed");
          exit(errno);
      }
      pthread_t th;
      struct args argg;
      argg.client_socket = client_socket;
      argg.client_address = client_address;
      if((pthread_create(&th, nullptr, handle, (void*)&argg)) != 0) {
          perror("error creating threads");
          exit(errno);
          close(server_socket);
      }
       pthread_detach(th);
      
  }
  
  close(server_socket);
  return 0;
}
