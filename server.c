// Verry Kurniawan
// 21120122130062

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    int port = atoi(argv[1]);
    printf("Port: %d\n", port);

    // Create a socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Error creating socket");
        exit(1);
    }

    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    // Bind the socket to the specified port
    if (bind(server_fd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Error binding socket");
        close(server_fd);
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_fd, 5) < 0) {
        perror("Error listening on socket");
        close(server_fd);
        exit(1);
    }

    printf("[+] Listening for connections...\n");

    struct sockaddr_in clientAddress;
    socklen_t clientAddressLen = sizeof(clientAddress);

    // Accept a connection from the client
    int client_fd = accept(server_fd, (struct sockaddr*)&clientAddress, &clientAddressLen);
    if (client_fd < 0) {
        perror("Error accepting connection");
        close(server_fd);
        exit(1);
    }

    printf("[+] Client connected: Verry Kurniawan, NIM: 2112012213002\n");

    // Send current server time to the client
    time_t currentTime = time(NULL);
    char* timeString = ctime(&currentTime);

    if (send(client_fd, timeString, strlen(timeString), 0) < 0) {
        perror("Error sending data to client");
        close(client_fd);
        close(server_fd);
        exit(1);
    }

    printf("Time sent to client: %s", timeString);

    // Close the sockets
    close(client_fd);
    close(server_fd);

    return 0;
}
