#include "Cli.h"
#include "Marshaller.h"
#include <iostream>
#include <format>
#include <string>
#include <vector>

namespace Cli
{

    void displayMenu()
    {
        std::cout << "----- MENU -----\n"
                  << "1. Open account\n"
                  << "2. Close account\n"
                  << "3. Withdraw\n"
                  << "4. Deposit\n"
                  << "5. Monitor\n"
                  << "6. Check balance\n"
                  << "7. Transfer\n"
                  << "0. Exit\n"
                  << "Enter your option: \n";
    }

    void cleanBuffer() {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }

    std::string getValidString(const std::string& prompt, size_t minLen, size_t maxLen)
    {
        std::string input;
        while (true)
        {
            std::cout << prompt;
            std::getline(std::cin, input);

            if (input.empty()) {
                std::cout << "Error: Input cannot be empty\n";
                continue;
            }

            if (input.length() < minLen) {
                std::cout << "Error: Input too short (min " << minLen << " chars)\n";
                continue;
            }

            if (input.length() > maxLen) {
                std::cout << "Error: Input too long (max " << maxLen << " chars)\n";
                continue;
            }

            return input;
        }
    }

    int getValidInt(const std::string& prompt)
    {
        int value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value)
            {
                cleanBuffer();
                if (value >= 0) return value;
                std::cout << "Error: Input cannot be negative\n";
            }
            else {
                std::cout << "Error: Input must be an integer\n";
                cleanBuffer();
            }
        }
    }

    double getValidDouble(const std::string& prompt)
    {
        double value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value)
            {
                cleanBuffer();
                if (value >= 0.0) return value;
                std::cout << "Error: Input cannot be negative or 0\n";
            }
            else {
                std::cout << "Error: Invalid number\n";
                cleanBuffer();
            }
        }
    }

    bool getConfirmation(const std::string& prompt) {
        char input;
        while (true) {
            std::cout << prompt << " (Y/N): \n";
            std::cin >> input;
            cleanBuffer();
            if (input == 'Y') return true;
            if (input == 'N') return false;
            std::cout << "Invalid input. Please enter 'Y' or 'N' only.\n";
        }
    }

    MenuResult handleMenuChoice(int reqId, bool& shouldExit)
    {
        int choice;
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input.\n";
            cleanBuffer();
            return {};
        }
        cleanBuffer();
        
        switch (choice)
        {
            case 1: return { handleOpenAccount(reqId),   choice };
            case 2: return { handleCloseAccount(reqId),  choice };
            case 3: return { handleWithdrawDeposit(reqId, TransactionType::Withdraw), choice };
            case 4: return { handleWithdrawDeposit(reqId, TransactionType::Deposit),  choice };
            case 5: {
                std::cout << "----- Monitor -----\n";
                const auto duration = getValidInt("Enter monitoring duration in seconds: ");
                return { Marshaller::marshallMonitor(reqId, duration), choice, duration };
            }
            case 6: return { handleCheckBalance(reqId), choice };
            case 7: return { handleTransfer(reqId), choice };
            case 0:
                std::cout << "Exiting\n";
                shouldExit = true;
                return {};
            default:
                std::cout << "Invalid choice.\n";
                return {};
        }
    }

    std::vector<char> handleOpenAccount(int reqId)
    {
        std::cout << "----- Open New Account -----\n";
        const auto name           = getValidString("Enter Name: ");
        const auto password       = getValidString("Enter Password: ", kPasswordLen, kPasswordLen);
        const auto currency       = getValidString("Enter Currency (e.g. SGD): ", 3, 3);
        const auto initialBalance = getValidDouble("Enter Initial Balance: ");
        
        std::string confirmation = std::format("Open new account with Currency: {} and Balance: {:.2f}?", 
                                       currency, initialBalance);
        if (!getConfirmation(confirmation)) return {};

        return Marshaller::marshallOpenAccount(reqId, name, password, currency, initialBalance);
    }

    std::vector<char> handleCloseAccount(int reqId)
    {
        std::cout << "----- Close Account -----\n";
        const auto name     = getValidString("Enter Name: ");
        const auto accNum   = getValidInt("Enter Account Number: ");
        const auto password = getValidString("Enter Password: ", kPasswordLen, kPasswordLen);

        std::string confirmation = std::format("Close account with Name: {} and Account Number: {}?", 
                                       name, accNum);
        if (!getConfirmation(confirmation)) return {};

        return Marshaller::marshallCloseAccount(reqId, name, accNum, password);
    }

    std::vector<char> handleWithdrawDeposit(int reqId, TransactionType type)
    {
        std::cout << (type == TransactionType::Withdraw ? "----- Withdraw -----\n"
                                                        : "----- Deposit -----\n");

        const auto name     = getValidString("Enter Name: ");
        const auto accNum   = getValidInt("Enter Account Number: ");
        const auto password = getValidString("Enter Password: ", kPasswordLen, kPasswordLen);
        const auto currency = getValidString("Enter Currency (e.g. SGD): ", 3, 3);
        const auto amount   = getValidDouble("Enter Amount: ");

        std::string confirmation = std::format("{} {:.2f} {} {} account with Account Number: {}?", 
                                       type == TransactionType::Withdraw ? "Withdraw" : "Deposit", amount, currency, type == TransactionType::Withdraw ? "from" : "into", accNum);
        if (!getConfirmation(confirmation)) return {};

        return Marshaller::marshallWithdrawDeposit(type, reqId, name, accNum, password, currency, amount);
    }

    std::vector<char> handleCheckBalance(int reqId) {
        std::cout << "----- Check Balance -----\n";
        const auto accNum = getValidInt("Enter Account Number: ");

        return Marshaller::marshallCheckBalance(reqId, accNum);
    }

    std::vector<char> handleTransfer(int reqId) {
        std::cout << "----- Transfer -----\n";
        const auto name      = getValidString("Enter Name: ");
        const auto outAccNum = getValidInt("Enter Sending Account Number: ");
        const auto password  = getValidString("Enter Password: ", kPasswordLen, kPasswordLen);
        const auto inAccNum  = getValidInt("Enter Receiving Account Number: ");
        const auto currency  = getValidString("Enter Currency (e.g. SGD): ", 3, 3);
        const auto amount    = getValidDouble("Enter Amount: ");

        std::string confirmation = std::format("Transfer {:.2f} {} from account with Account Number: {} to account with Account Number: {}?",
                                        amount, currency, outAccNum, inAccNum);
        if (!getConfirmation(confirmation)) return {};

        return Marshaller::marshallTransfer(reqId, name, outAccNum, password, inAccNum, currency, amount);
    }
}
