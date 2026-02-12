#include "Cli.h"
#include "Marshaller.h"
#include <iostream>
#include <string>
#include <vector>

#define MAX_STR_LEN 20

namespace Cli
{
    void displayMenu()
    {
        std::cout << "----- MENU -----\n";
        std::cout << "1. Open account\n";
        std::cout << "2. Close account\n";
        std::cout << "3. Withdraw\n";
        std::cout << "4. Deposit\n";
        std::cout << "5. Monitor\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your option: \n";
    }

    void cleanBuffer() {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }

    std::string getValidString(const std::string& prompt, int minLen=0, int maxLen=MAX_STR_LEN)
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
                std::cout << "Error: Input too short (min " << minLen << " chars\n";
                continue;
            }

            if (input.length() > maxLen) {
                std::cout << "Error: Input too long (max " << maxLen << " chars\n";
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
                std::cout << "Error: Input cannot be negative or 0\n";
            }
            else std::cout << "Error: Input must be an integer\n";
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

    std::vector<char> handleMenuChoice(int reqId, bool& shouldExit)
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
            case 1: return handleOpenAccount(reqId);
            case 2: return handleCloseAccount(reqId);
            case 3: return handleWithdrawDeposit(reqId, 0);
            case 4: return handleWithdrawDeposit(reqId, 1);
            case 5: return handleMonitor(reqId);

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
        std::string name = getValidString("Enter Name: ");
        std::string password = getValidString("Enter Password: ");
        std::string currency = getValidString("Enter Currency (e.g. SGD): ", 3, 3);
        double initialBalance   = getValidDouble("Enter Initial Balance: ");

        return Marshaller::marshallOpenAccount(reqId, name, password, currency, initialBalance);
    }

    std::vector<char> handleCloseAccount(int reqId)
    {
        std::cout << "----- Close Account -----\n";
        std::string name = getValidString("Enter Name: ");
        int accNum = getValidInt("Enter account number: ");
        std::string password = getValidString("Enter Password: ");

        return Marshaller::marshallCloseAccount(reqId, name, accNum, password);
    }

    std::vector<char> handleWithdrawDeposit(int reqId, int type)
    {
        if (type == 0) std::cout << "----- Withdraw -----\n";
        else std::cout << "----- Deposit -----\n";
        std::string name = getValidString("Enter Name: ");
        int accNum = getValidInt("Enter account number: ");
        std::string password = getValidString("Enter Password: ");
        std::string currency = getValidString("Enter Currency (e.g. SGD): ", 3, 3);
        double amount = getValidDouble("Enter amount: ");

        return Marshaller::marshallWithdrawDeposit(type, reqId, name, accNum, password, currency, amount);
    }

    std::vector<char> handleMonitor(int reqId)
    {
        std::cout << "----- Monitor -----\n";
        int duration = getValidInt("Enter monitoring duration in seconds: ");

        return Marshaller::marshallMonitor(reqId, duration);
    }
}
