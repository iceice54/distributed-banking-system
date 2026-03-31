#include "Marshaller.h"
#include "Cli.h"
#include <winsock2.h>
#include <string>
#include <cstring>
#include <cstdint>
#include <vector>

namespace {

    class Buffer
    {
    public:
        void appendInt(int val)
        {
            const uint32_t netVal = htonl(static_cast<uint32_t>(val));
            append(&netVal, sizeof(netVal));
        }

        void appendDouble(double val)
        {
            uint64_t temp;
            std::memcpy(&temp, &val, sizeof(temp));
            const uint64_t netVal = htonll(temp);
            append(&netVal, sizeof(netVal));
        }

        void appendString(const std::string& str)
        {
            appendInt(static_cast<int>(str.size()));
            append(str.data(), str.size());
        }

        [[nodiscard]] std::vector<char> take() { return std::move(data_); }

    private:
        std::vector<char> data_;

        template<typename T>
        void append(const T* src, std::size_t size)
        {
            const auto oldSize = data_.size();
            data_.resize(oldSize + size);
            std::memcpy(data_.data() + oldSize, src, size);
        }
    };

}

enum class MethodId : uint32_t {
    OPEN_ACCOUNT  = 1,
    CLOSE_ACCOUNT = 2,
    WITHDRAW      = 3,
    DEPOSIT       = 4,
    MONITOR       = 5,
    CHECK_BALANCE = 6, // Idempotent
    TRANSFER      = 7  // Non-Idempotent
};

namespace Marshaller {

    namespace {
        void appendMethod(Buffer& buf, MethodId id) {
            buf.appendInt(static_cast<int>(id));
        }
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

        return buf.take();
    };

    std::vector<char> marshallCloseAccount(int reqId, const std::string& name, int accNum, const std::string& password)
    {
        Buffer buf;
        appendMethod(buf, MethodId::CLOSE_ACCOUNT);
        buf.appendInt(reqId);
        buf.appendString(name);
        buf.appendInt(accNum);
        buf.appendString(password);

        return buf.take();
    }

    std::vector<char> marshallWithdrawDeposit(Cli::TransactionType type, int reqId, const std::string& name, int accNum, const std::string& password, const std::string& currency, double amount)
    {
        Buffer buf;
        const MethodId method = (type == Cli::TransactionType::Withdraw)
                              ? MethodId::WITHDRAW
                              : MethodId::DEPOSIT;
        appendMethod(buf, method);
        buf.appendInt(reqId);
        buf.appendString(name);
        buf.appendInt(accNum);
        buf.appendString(password);
        buf.appendString(currency);
        buf.appendDouble(amount);

        return buf.take();
    }
    
    std::vector<char> marshallMonitor(int reqId, int durationInSeconds)
    {
        Buffer buf;
        appendMethod(buf, MethodId::MONITOR);
        buf.appendInt(reqId);
        buf.appendInt(durationInSeconds);

        return buf.take();
    }

    std::vector<char> marshallCheckBalance(int reqId, int accNum) {
        Buffer buf;
        appendMethod(buf, MethodId::CHECK_BALANCE);
        buf.appendInt(reqId);
        buf.appendInt(accNum);

        return buf.take();
    }

    std::vector<char> marshallTransfer(int reqId, const std::string& name, int outAccNum, const std::string& password, int inAccNum, const std::string& currency, double amount) {
        Buffer buf;
        appendMethod(buf, MethodId::TRANSFER);
        buf.appendInt(reqId);
        buf.appendString(name);
        buf.appendInt(outAccNum);
        buf.appendString(password);
        buf.appendInt(inAccNum);
        buf.appendString(currency);
        buf.appendDouble(amount);

        return buf.take();
    }
};
