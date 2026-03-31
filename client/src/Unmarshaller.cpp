#include "Unmarshaller.h"
#include <string>
#include <cstring>
#include <winsock2.h>

namespace {

    uint32_t readUInt32(const char*& ptr) {
        uint32_t net;
        std::memcpy(&net, ptr, sizeof(net));
        ptr += sizeof(net);
        return ntohl(net);
    }

    int readInt(const char*& ptr) {
        return static_cast<int>(readUInt32(ptr));
    }

    std::string readString(const char*& ptr) {
        const auto len = readUInt32(ptr);
        std::string str(ptr, len);
        ptr += len;
        return str;
    }
}

namespace Unmarshaller {
    Response unmarshall(const std::vector<char>& data)
    {
        constexpr size_t kMinSize = sizeof(uint32_t)  // reqId
                                  + sizeof(uint32_t)  // status
                                  + sizeof(uint32_t); // msgLen
        
        if (data.size() < kMinSize) return {}; 

        const char* ptr = data.data();

        Response resp;
        resp.reqId   = readInt(ptr);
        resp.status  = static_cast<Status>(readUInt32(ptr));
        resp.message = readString(ptr);

        return resp;
    }

    Response unmarshallUpdate(const std::vector<char>& data) {
        constexpr size_t kMinSize = sizeof(uint32_t); // msgLen
        
        if (data.size() < kMinSize) return {};

        const char* ptr = data.data();

        Response resp;
        resp.message = readString(ptr);

        return resp;
    }
}
