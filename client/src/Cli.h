#pragma once
#include <string>
#include <vector>

namespace Cli
{
    constexpr size_t kMaxStrLen = 20;
    constexpr size_t kPasswordLen = 4;

    enum class TransactionType { Withdraw, Deposit };

    struct MenuResult {
        std::vector<char> payload;
        int choice   = 0;
        int duration = 0;
    };

    void              displayMenu();
    MenuResult        handleMenuChoice(int reqId, bool& shouldExit);

    // Helpers
    std::string       getValidString(const std::string& prompt, size_t minLen = 0, size_t maxLen = kMaxStrLen);
    int               getValidInt(const std::string& prompt);
    double            getValidDouble(const std::string& prompt);

    // Handlers
    std::vector<char> handleOpenAccount(int reqId);
    std::vector<char> handleCloseAccount(int reqId);
    std::vector<char> handleWithdrawDeposit(int reqId, TransactionType type);
    std::vector<char> handleCheckBalance(int reqId);
    std::vector<char> handleTransfer(int reqId);
}
