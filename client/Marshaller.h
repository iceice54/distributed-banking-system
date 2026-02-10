#pragma once
#include <vector>

namespace Marshaller {
    std::vector<char> marshallCheckBalance(int reqId, int accId);
    std::vector<char> marshallDeposit(int reqId, int accId, double amount);
};
