#include <iostream>
#include "utils.hpp"

int main() {
    auto data = utils::load_fred_csv("data/ICSA.csv");
    int printed = 0;
    for (const auto& [date, value] : data) {
        std::cout << date << "," << value << '\n';
        if (++printed >= 5) break;
    }
    return 0;
}
