#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include <optional>

namespace Unmarshaller {

    enum class Status : uint8_t { Failure = 0, Success = 1 };

    struct Response {
        int                   reqId;
        Status                status;
        std::string           message;
    };

    // Unmarshalls all replies except for monitor updates
    Response unmarshall(const std::vector<char>& data);
    // Unmarshalls monitor updates
    Response unmarshallUpdate(const std::vector<char>& data);
}
