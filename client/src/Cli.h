#pragma once
#include <vector>

namespace Cli
{
    void displayMenu();
    std::vector<char> handleMenuChoice(int reqId, bool& shouldExit);
}
