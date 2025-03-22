#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

int main() 
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    
    std::string command, login, password;
    std::cout << "Enter command (REGISTER/LOGIN): ";
    std::cin >> command;
    std::cout << "Enter login: ";
    std::cin >> login;
    std::cout << "Enter password: ";
    std::cin >> password;
    
    std::string request = command + " " + login + " " + password;
    send(sock, request.c_str(), request.size(), 0);
    
    read(sock, buffer, 1024);
    std::cout << "Server response: " << buffer << std::endl;
    
    close(sock);
    return 0;
}

