#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h> // Include this header for inet_ntop

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib

int main() {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock" << std::endl;
        return EXIT_FAILURE;
    }

    // Create a socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return EXIT_FAILURE;
    }

    // Set up the server address structure
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(25565);

    // Bind the socket
    if (bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Error binding socket: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return EXIT_FAILURE;
    }

    // Listen for incoming connections
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Error listening on socket: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return EXIT_FAILURE;
    }

    std::cout << "Waiting for incoming connections..." << std::endl;
    while (1) {

        // Accept incoming connections
        sockaddr_in clientAddress;
        int clientAddressLength = sizeof(clientAddress);
        SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddress, &clientAddressLength);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Error accepting connection: " << WSAGetLastError() << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return EXIT_FAILURE;
        }

        // Display client information
        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(clientAddress.sin_addr), clientIP, INET_ADDRSTRLEN);
        std::cout << "Connection accepted from " << clientIP << ":" << ntohs(clientAddress.sin_port) << std::endl;

        // Handle the connection (you can modify this part based on your application)
        // For example, you can read/write data using recv()/send() functions

        // Proof of concept. Ideally, I'd want to create a system where only authorized clients can 
        // interact with this listener. As it is now, my web browser will create several connections at once, which causes
        // the next three statements to execute.
        std::cout << "Function 1 executed" << std::endl;
        std::cout << "Function 2 executed" << std::endl;
        for (int i = 0; i < 5; i++) std::cout << "Iteration: " << i << " complete. " << std::endl;

        // Close sockets
        closesocket(clientSocket);
    }

    closesocket(serverSocket);
    WSACleanup();

    return EXIT_SUCCESS;
}
