#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#define DEFAULT_BUFLEN 512
// #define DEFAULT_PORT "2222"

int main(int argc, char **argv)
{
	std::cout << "Bank Client Application" << std::endl;

    WSADATA wsaData;
    SOCKET SendSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                *ptr = NULL,
                hints;
    const char *sendbuf = "hello";
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;

    // Validate params
    if (argc != 3) {
        printf("usage: %s server-name port-number\n", argv[0]);
        return 1;
    }

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }
    
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

    // Resolve server address and port
    iResult = getaddrinfo(argv[1], argv[2], &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ptr=result;

    SendSocket = socket(ptr->ai_family, ptr->ai_socktype, 
        ptr->ai_protocol);

    if (SendSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Send to server
    iResult = sendto(SendSocket, sendbuf, (int)strlen(sendbuf), 0, 
                         ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("sendto failed: %d\n", WSAGetLastError());
    } else {
        printf("Sent %d bytes to %s:%s\n", iResult, argv[1], argv[2]);
    }

   // Receive the echo back
    struct sockaddr_storage fromAddr;
    int fromLen = sizeof(fromAddr);
    iResult = recvfrom(SendSocket, recvbuf, DEFAULT_BUFLEN, 0, 
                       (struct sockaddr*)&fromAddr, &fromLen);

    if (iResult > 0) {
        recvbuf[iResult] = '\0'; // Null terminate string
        printf("Received echo: %s\n", recvbuf);
    }

    // Cleanup
    freeaddrinfo(result);
    closesocket(SendSocket);
    WSACleanup();

    return 0;
}

