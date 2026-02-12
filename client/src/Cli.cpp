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

    std::string getValidString(const std::string& prompt)
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

            if (input.length() > MAX_STR_LEN) {
                std::cout << "Error: Input too long (max " << MAX_STR_LEN << " chars\n";
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
                if (value >= 0.0) return value;
                std::cout << "Error: Input cannot be negative or 0\n";
            }
            else std::cout << "Error: Invalid number\n";
        }
    }

    std::vector<char> handleMenuChoice(int reqId, bool& shouldExit)
    {
        int choice;
        std::cin >> choice;
        
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
        std::string currency = getValidString("Enter Currency (e.g. SGD): ");
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
        std::string currency = getValidString("Enter Currency (e.g. SGD): ");
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
