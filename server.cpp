#include <sys/types.h>
#include <iostream>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sstream>

#define PORT 5003

//This file will contain the server side code for the chatroom

int main(){

    struct sockaddr_in saddr = {
        .sin_family = AF_INET, //Ipv4 setting
        .sin_addr.s_addr = INADDR_ANY, //INADDR_ANY means socket can be bound to any IP available on host
        .sin_port = htons(PORT) //Converts PORT to ns and then sets it
    };

    //Creates socket
    int option = 1;
    int srv_addr_size = sizeof(saddr); //Server address size
    int socketServer = socket(AF_INET, SOCK_STREAM, 0); //SOCK_STREAM -> TCP  |   SOCK_DGRAM -> UDP
    setsockopt(socketServer, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option));

    if (socketServer == -1){
        std::cerr << "[ERROR]: Socket unable to be created.";
        return 0;
    }

    //Client setup
    struct sockaddr_in clientAdrress; //sockaddr_in datatype represents a ipv4 socket address
    socklen_t clientAdSize = sizeof(clientAdrress);
    int socketClient;

    //Binding our socket to the IP+Port
    bind(socketServer, (struct sockaddr*)&saddr, srv_addr_size);

    //Listening
    int checkListening = listen(socketServer, SOMAXCONN);
    std::stringstream ss;
    ss << PORT;
    if (checkListening == 0){ //Case of successful listening
        std::cout << "[SERVER]: Now listening on port " << ss.str() << ".\n";
    } else {
        std::cerr << "[SERVER]: Error listening on Port " << ss.str() << ".\n";
        return 0;
    }

    char buff[4096]; //Buffer -> Holds data before processing
    int recievedDataSize;

    std::cout << "Welcome to the room! Enjoy your conversations\n---------------------------------------\n";
    
    //Waiting for clients
    while (true){
        
        //Clear the buffer
        memset(&buff, 0, sizeof(buff));

        socketClient = accept(socketServer, (struct sockaddr*)&clientAdrress, &clientAdSize);

        if (socketClient < 0){
            std::cerr << "[SERVER]: Error accepting client connection.\n";
            continue;
        }

        recievedDataSize = recv(socketClient, buff, sizeof(buff), 0);

        if (recievedDataSize <= 0){
            std::cerr << "[SERVER]: Client disconnected or there was an error receiving data.\n";
            close(socketClient);
            continue;
        }

        std::cout << "Client: " << buff << '\n';
    }






}