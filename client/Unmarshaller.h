#pragma once
#include <vector>
#include <cstdint>

namespace Unmarshaller {
    struct Response {
        uint32_t reqId;
        uint32_t status; // 0 = Fail, 1 = Success
        double balance;
    };

    // Converts raw bytes into a usable Response struct
    Response unmarshall(const std::vector<char>& data);
}
