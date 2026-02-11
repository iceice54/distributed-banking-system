#pragma once

#include <vector>
#include <string>

namespace Marshaller {
    
    std::vector<char> marshallOpenAccount(int reqId, const std::string& name, const std::string& password, const std::string& currency, double initialBalance);

    std::vector<char> marshallCloseAccount(int reqId, const std::string& name, int accNum, const std::string& password);

    std::vector<char> marshallDeposit(int reqId, const std::string& name, int accNum, const std::string& password, const std::string& currency, double amount);

    std::vector<char> marshallWithdraw(int reqId, const std::string& name, int accNum, const std::string& password, const std::string& currency, double amount);
    
    std::vector<char> marshallMonitor(int reqId, int durationInSeconds);
}
