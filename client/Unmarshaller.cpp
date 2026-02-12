#include "Unmarshaller.h"
#include <cstring>
#include <winsock2.h> // For ntohl

namespace Unmarshaller {
    Response unmarshall(const std::vector<char>& data)
    {
        Response resp = {0, 0, 0.0};
        
        // Safety check: Is the packet big enough?
        if (data.size() < 16) return resp; 

        // 1. Copy raw bytes into temp variables
        uint32_t netReqId, netStatus, netBalance;
        
        // Pointer arithmetic to read at offsets 0 and 4
        std::memcpy(&netReqId, data.data() + 0, 4);
        std::memcpy(&netStatus, data.data() + 4, 4);
        std::memcpy(&netBalance, data.data() + 8, 8);
        
        // 2. Convert from Network (Big Endian) to Host (Little Endian)
        resp.reqId = ntohl(netReqId);
        resp.status = ntohl(netStatus);
        resp.balance = ntohl(netBalance);

        return resp;
    }
}
