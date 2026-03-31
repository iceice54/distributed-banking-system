#pragma once

#include "Cli.h"
#include <vector>
#include <string>

namespace Marshaller
{
    // Marshalls data for opening a new account
    // Returns a byte buffer ready for network transmission
    std::vector<char> marshallOpenAccount(
        int reqId, 
        const std::string& name, 
        const std::string& password, 
        const std::string& currency, 
        double initialBalance
    );

    // Marshalls data for closing an existing account
    std::vector<char> marshallCloseAccount(
        int reqId, 
        const std::string& name, 
        int accNum, 
        const std::string& password
    );

    // Marshalls data for withdrawing or depositing funds
    std::vector<char> marshallWithdrawDeposit(
        Cli::TransactionType type, 
        int reqId, 
        const std::string& name, 
        int accNum, 
        const std::string& password, 
        const std::string& currency, 
        double amount
    );

    // Marshalls data for monitoring updates
    std::vector<char> marshallMonitor(
        int reqId, 
        int durationInSeconds
    );

    // Marshalls data for checking balance
    std::vector<char> marshallCheckBalance(
        int reqId,
        int accNum
    );

    // Marshalls data for transfers
    std::vector<char> marshallTransfer(
        int reqId,
        const std::string& name, 
        int outAccNum,
        const std::string& password, 
        int inAccNum,
        const std::string& currency, 
        double amount
    );
}
