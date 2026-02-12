#pragma once
#include <vector>

namespace Cli
{
    void displayMenu();
    std::vector<char> handleMenuChoice(int reqId, bool& shouldExit);
    std::vector<char> handleOpenAccount(int reqId);
    std::vector<char> handleCloseAccount(int reqId);
    std::vector<char> handleWithdrawDeposit(int reqId, int type);
    std::vector<char> handleMonitor(int reqId);
}
