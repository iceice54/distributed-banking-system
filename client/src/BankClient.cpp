#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <array>
#include <memory>
#include "Unmarshaller.h"
#include "Cli.h"

constexpr int kDefaultBufLen = 1024;
constexpr double kDropProbability = 0.1;
constexpr int kTimeoutMs = 1000;
constexpr int kMaxRetries = 10;

bool shouldDrop() {
    return (static_cast<double>(rand()) / RAND_MAX) < kDropProbability;
}

class WinsockContext {
public:
    WinsockContext() {
        if (const auto rc = WSAStartup(MAKEWORD(2,2), &_wsaData); rc != 0)
            throw std::runtime_error("WSAStartup failed: " + std::to_string(rc));
    }
    ~WinsockContext() { WSACleanup(); }

    WinsockContext(const WinsockContext&)             = delete;
    WinsockContext& operator=(const WinsockContext&) = delete;

private:
    WSADATA _wsaData{};
};

class UdpSocket {
public:
    explicit UdpSocket(addrinfo* addr) 
        : socket_(::socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol))
    {
        if (socket_ == INVALID_SOCKET)
            throw std::runtime_error("socket() failed: " + std::to_string(WSAGetLastError()));
    }

    ~UdpSocket() { closesocket(socket_); }

    UdpSocket(const UdpSocket&)            = delete;
    UdpSocket& operator=(const UdpSocket&) = delete;

    void sendTo(const std::vector<char>& payload, addrinfo* addr) const {
        const auto sent = ::sendto(
            socket_,
            payload.data(), static_cast<int>(payload.size()),
            0,
            addr->ai_addr, static_cast<int>(addr->ai_addrlen)
        );

        if (sent == SOCKET_ERROR)
            throw std::runtime_error("sendto() failed: " + std::to_string(WSAGetLastError()));
        
        std::cout << "Sent " << sent << " bytes\n";
    }

    [[nodiscard]] std::vector<char> receiveFrom() const {
        std::array<char, kDefaultBufLen> buf{};
        sockaddr_storage fromAddr{};
        int fromLen = sizeof(fromAddr);

        const auto received = ::recvfrom(
            socket_,
            buf.data(), static_cast<int>(buf.size()),
            0,
            reinterpret_cast<sockaddr*>(&fromAddr), &fromLen
        );

        if (received == SOCKET_ERROR)
            throw std::runtime_error("recvfrom() failed: " + std::to_string(WSAGetLastError()));
        
        return { buf.data(), buf.data() + received };
    }

    void setTimeout(int timeoutMs) const {
        setsockopt(socket_, SOL_SOCKET, SO_RCVTIMEO,
            reinterpret_cast<const char*>(&timeoutMs), sizeof(timeoutMs));
    }

    [[nodiscard]] std::optional<std::vector<char>> tryReceiveFrom() const {
        std::array<char, kDefaultBufLen> buf{};
        sockaddr_storage fromAddr{};
        int fromLen = sizeof(fromAddr);

        const auto received = ::recvfrom(
            socket_,
            buf.data(), static_cast<int>(buf.size()),
            0,
            reinterpret_cast<sockaddr*>(&fromAddr), &fromLen
        );

        if (received == SOCKET_ERROR) return std::nullopt;
        return std::vector<char>{ buf.data(), buf.data() + received };
    }

private:
    SOCKET socket_;
};

struct AddrInfoDeleter {
    void operator()(addrinfo* p) const { freeaddrinfo(p); }
};
using AddrInfoPtr = std::unique_ptr<addrinfo, AddrInfoDeleter>;

[[nodiscard]] AddrInfoPtr resolveAddress(const std::string& host, const std::string& port) {
    addrinfo hints{};
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

    addrinfo* result = nullptr;
    if (const auto rc = getaddrinfo(host.c_str(), port.c_str(), &hints, &result); rc != 0)
        throw std::runtime_error("getaddrinfo() failed: " + std::to_string(rc));

    return AddrInfoPtr{ result };
}

[[nodiscard]] std::vector<char> sendWithRetry(
    const UdpSocket& socket,
    const std::vector<char>& payload,
    addrinfo* addr)
{
    socket.setTimeout(kTimeoutMs);

    for (int attempt = 1; attempt <= kMaxRetries; ++attempt) {
        std::cout << "Attempt " << attempt << "...\n";

        if (!shouldDrop()) {
            socket.sendTo(payload, addr);
        } else {
            std::cout << "[SIMULATE] Dropped outgoing request\n";
        }

        try {
            const auto data = socket.receiveFrom(); // throws on timeout

            if (shouldDrop()) {
                std::cout << "[SIMULATE] Dropped incoming reply\n";
                continue; // discard and retry
            }

            return data;
        } catch (const std::runtime_error&) {
            std::cout << "Timeout, retrying...\n";
        }
        
    }

    throw std::runtime_error("No response after " 
        + std::to_string(kMaxRetries) + " attempts");
}

void handleMonitorResponse(const UdpSocket& socket, int durationSeconds,
                           const std::vector<char>& payload, addrinfo* addr) {
    // Use sendWithRetry for the initial registration — handles drops and timeouts
    const auto data = sendWithRetry(socket, payload, addr);
    const auto resp = Unmarshaller::unmarshall(data);
    std::cout << resp.message << "\n";

    // Set timeout to monitor duration
    socket.setTimeout(durationSeconds * 1000);
    std::cout << "Monitoring for " << durationSeconds << "s...\n";

    // Keep receiving updates until timeout
    while (true) {
        const auto update = socket.tryReceiveFrom();
        if (!update.has_value()) {
            // recvfrom returned SOCKET_ERROR — timeout expired
            std::cout << "Monitoring period ended.\n";
            break;
        }
        const auto updateResp = Unmarshaller::unmarshallUpdate(update.value());
        std::cout << "Update: " << updateResp.message << "\n";
    }

    // Reset timeout to infinite for normal operation
    socket.setTimeout(0);
}

int main(int argc, char **argv)
{
	std::cout << "Bank Client Application\n";

    if (argc != 3)
    {
        std::cout << "usage: " << argv[0] << " <server> <port>\n";
        return EXIT_FAILURE;
    }

    try {
        const WinsockContext winsock;
        const auto           addrInfo = resolveAddress(argv[1], argv[2]);
        const UdpSocket      socket{ addrInfo.get() };

        int reqId = 0;
        bool shouldExit = false;

        while (!shouldExit) {
            Cli::displayMenu();
            const auto result = Cli::handleMenuChoice(reqId, shouldExit);

            if (shouldExit || result.payload.empty()) continue;

            if (result.choice == 5) {
                handleMonitorResponse(socket, result.duration, result.payload, addrInfo.get());
            } else {
                const auto data = sendWithRetry(socket, result.payload, addrInfo.get());
                const auto resp = Unmarshaller::unmarshall(data);
                std::cout << (resp.status == Unmarshaller::Status::Success ? "Success: " : "")
                        << resp.message << "\n";
            }

            ++reqId;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

