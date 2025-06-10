#include <iostream>
#include "data_cleaner.hpp"
#include "forecast.hpp"
#include "plot.hpp"
#include "utils.hpp"
#include <chrono>
#include <vector>

int main() {
    auto table = merge_data("data/ICSA.csv", "data/UNRATE.csv", "data/JTSJOL.csv",
                           "2020-01-01", "2025-06-01");
    std::cout << "Merged " << table.size() << " rows to data/weekly_data.csv\n";

    std::vector<double> icsaVec;
    icsaVec.reserve(table.size());
    for (const auto& [date, row] : table) {
        icsaVec.push_back(row.icsa);
    }

    auto start = std::chrono::high_resolution_clock::now();
    auto smaPred = forecast_sma(icsaVec, 4, 3);
    auto end = std::chrono::high_resolution_clock::now();
    auto smaTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    auto expPred = forecast_exp(icsaVec, 0.2, 3);
    end = std::chrono::high_resolution_clock::now();
    auto expTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "SMA forecast (" << smaTime << " us): ";
    for (double v : smaPred) std::cout << v << ' ';
    std::cout << "\n";

    std::cout << "EXP forecast (" << expTime << " us): ";
    for (double v : expPred) std::cout << v << ' ';
    std::cout << "\n";

    return 0;
}
