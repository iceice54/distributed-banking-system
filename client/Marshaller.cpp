#include "Marshaller.h"
#include <winsock2.h>
#include <cstring>
#include <cstdint>

namespace {
    std::vector<char> pack(int methodId, int reqId, int accNum, double amount) {
        uint32_t netM = htonl(static_cast<uint32_t>(methodId));
        uint32_t netR = htonl(static_cast<uint32_t>(reqId));
        uint32_t netA = htonl(static_cast<uint32_t>(accNum));

        std::vector<char> buf(20);

        char* ptr = buf.data();

        std::memcpy(ptr + 0,  &netM, 4);
        std::memcpy(ptr + 4,  &netR, 4);
        std::memcpy(ptr + 8,  &netA, 4);
        std::memcpy(ptr + 12, &amount, 8);

        return buf;
    }
}

namespace Marshaller {
    std::vector<char> marshallCheckBalance(int reqId, int accId) {
        return pack(1, reqId, accId, 0.0);
    }
    
    std::vector<char> marshallDeposit(int reqId, int accId, double amount) {
        return pack(2, reqId, accId, amount);
    }
}
