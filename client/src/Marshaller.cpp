#include "Marshaller.h"
#include <winsock2.h>
#include <string>
#include <cstring>
#include <cstdint>

class Buffer
{
public:
    std::vector<char> data;

    void appendInt(int val)
    {
        uint32_t netVal = htonl(static_cast<uint32_t>(val));
        size_t oldSize = data.size();
        data.resize(oldSize + 4);
        std::memcpy(data.data() + oldSize, &netVal, 4);
    }

    void appendDouble(double val)
    {
        uint64_t temp;
        std::memcpy(&temp, &val, 8);
        uint64_t netVal = htonll(temp);

        size_t oldSize = data.size();
        data.resize(oldSize + 8);
        std::memcpy(data.data() + oldSize, &netVal, 8);
    }

    void appendString(const std::string& str)
    {
        appendInt(static_cast<int>(str.size()));

        size_t oldSize = data.size();
        data.resize(oldSize + str.size());
        std::memcpy(data.data() + oldSize, str.data(), str.size());
    }
};

enum class MethodId : uint32_t {
    OPEN_ACCOUNT  = 1,
    CLOSE_ACCOUNT = 2,
    DEPOSIT       = 3,
    WITHDRAW      = 4,
    MONITOR       = 5,
    // GET_DETAILS   = 6, // Idempotent
    // TRANSFER      = 7  // Non-Idempotent
};

namespace Marshaller {

    void appendMethod(Buffer& buf, MethodId id) {
        buf.appendInt(static_cast<int>(id));
    }

    std::vector<char> marshallOpenAccount(int reqId, const std::string& name, const std::string& password, const std::string& currency, double initialBalance)
    {
        Buffer buf;
        appendMethod(buf, MethodId::OPEN_ACCOUNT);
        buf.appendInt(reqId);
        
        buf.appendString(name);
        buf.appendString(password);
        buf.appendString(currency);
        buf.appendDouble(initialBalance);

        return buf.data;
    };

    std::vector<char> marshallCloseAccount(int reqId, const std::string& name, int accNum, const std::string& password)
    {
        Buffer buf;
        appendMethod(buf, MethodId::CLOSE_ACCOUNT);
        buf.appendInt(reqId);

        buf.appendString(name);
        buf.appendInt(accNum);
        buf.appendString(password);

        return buf.data;
    }

    std::vector<char> marshallWithdrawDeposit(int type, int reqId, const std::string& name, int accNum, const std::string& password, const std::string& currency, double amount)
    {
        Buffer buf;
        MethodId method = (type == 0) ? MethodId::WITHDRAW : MethodId::DEPOSIT;
        appendMethod(buf, method);
        buf.appendInt(reqId);

        buf.appendString(name);
        buf.appendInt(accNum);
        buf.appendString(password);
        buf.appendString(currency);
        buf.appendDouble(amount);

        return buf.data;
    }
    
    std::vector<char> marshallMonitor(int reqId, int durationInSeconds)
    {
        Buffer buf;
        appendMethod(buf, MethodId::MONITOR);
        buf.appendInt(reqId);

        buf.appendInt(durationInSeconds);

        return buf.data;
    }

    // std::vector<char> marshallCheckBalance(int reqId, int accId) {
    //     return pack(1, reqId, accId, 0.0, "");
    // }
};
