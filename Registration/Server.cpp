#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

struct User
{
    std::string login;
    std::string password;
};

std::vector<User> users;

bool registerUser(const std::string& login, const std::string& password) 
{
    for (const auto& user : users) 
    {
        if (user.login == login) 
        {
            return false;
        }
    }
    users.push_back({login, password});
    return true;
}

bool loginUser(const std::string& login, const std::string& password) 
{
    for (const auto& user : users) 
    {
        if (user.login == login && user.password == password) 
        {
            return true;
        }
    }
    return false;
}

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);
    
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);
    
    std::cout << "Server is running on port 8080...\n";
    
    while (true) {
        new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        read(new_socket, buffer, 1024);
        
        std::string request(buffer);
        std::string response;
        
        if (request.find("REGISTER") == 0) {
            size_t sep = request.find(' ');
            size_t sep2 = request.find(' ', sep + 1);
            std::string login = request.substr(sep + 1, sep2 - sep - 1);
            std::string password = request.substr(sep2 + 1);
            
            if (registerUser(login, password)) {
                response = "Registration successful";
            } else {
                response = "Username already exists";
            }
        } else if (request.find("LOGIN") == 0) {
            size_t sep = request.find(' ');
            size_t sep2 = request.find(' ', sep + 1);
            std::string login = request.substr(sep + 1, sep2 - sep - 1);
            std::string password = request.substr(sep2 + 1);
            
            if (loginUser(login, password)) {
                response = "Login successful";
            } else {
                response = "Invalid credentials";
            }
        }
        
        send(new_socket, response.c_str(), response.size(), 0);
        close(new_socket);
    }
    return 0;
}
