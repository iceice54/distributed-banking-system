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

    Response unmarshall(const std::vector<char>& data);
    Response unmarshallUpdate(const std::vector<char>& data);
}
