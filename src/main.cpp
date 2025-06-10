#include <iostream>
#include "data_cleaner.hpp"
#include "forecast.hpp"
#include "plot.hpp"
#include "utils.hpp"

int main() {
    auto table = merge_data("data/ICSA.csv", "data/UNRATE.csv", "data/JTSJOL.csv",
                           "2020-01-01", "2025-06-01");
    std::cout << "Merged " << table.size() << " rows to data/weekly_data.csv\n";
    return 0;
}
