#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <vector>
#include <Cli.h>

#define DEFAULT_BUFLEN 1024

int main(int argc, char **argv)
{
	std::cout << "Bank Client Application\n";

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
    if (argc != 3)
    {
        std::cout << "usage: " << argv[0] << " server-name port-number\n";
        return 1;
    }

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0)
    {
        std::cout << "WSAStartup failed " << iResult << "\n";
        return 1;
    }
    
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

    // Resolve server address and port
    iResult = getaddrinfo(argv[1], argv[2], &hints, &result);
    if (iResult != 0)
    {
        std::cout << "getaddrinfo failed: " << iResult << "\n";
        WSACleanup();
        return 1;
    }

    // Create a socket for connecting to server
    ptr=result;

    SendSocket = socket(ptr->ai_family, ptr->ai_socktype, 
        ptr->ai_protocol);

    if (SendSocket == INVALID_SOCKET)
    {
        std::cout << "Error at socket(): " << WSAGetLastError() << "\n";
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    int reqId = 0;
    bool shouldExit = false;

    while (true)
    {
        Cli::displayMenu();

        std::vector<char> payload = Cli::handleMenuChoice(reqId, shouldExit);

        if (shouldExit) {
            break;
        }

        if (payload.empty()) continue;

        // Send to server
        iResult = sendto(SendSocket, payload.data(), payload.size(), 0, 
                            ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR)
        {
            std::cout << "sendto failed: " << WSAGetLastError() << "\n";
        } else
        {
            std::cout << "Sent " << iResult << " bytes to " << argv[1] << ":" << argv[2] << "\n";
        }

        // Receive the response
        struct sockaddr_storage fromAddr;
        int fromLen = sizeof(fromAddr);
        iResult = recvfrom(SendSocket, recvbuf, DEFAULT_BUFLEN, 0, 
                        (struct sockaddr*)&fromAddr, &fromLen);

        if (iResult > 0)
        {
            recvbuf[iResult] = '\0'; // Null terminate string
            std::cout << "Received response: " << recvbuf << "\n";
        }

        reqId++;
    }
    
    // Cleanup
    freeaddrinfo(result);
    closesocket(SendSocket);
    WSACleanup();

    return 0;
}

