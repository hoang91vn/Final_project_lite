#include <iostream>
#include "data_cleaner.hpp"
#include "forecast.hpp"
#include "plot.hpp"
#include "utils.hpp"
#include "output_writer.hpp"
#include <fstream>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <vector>

int main() {
    auto table = merge_data("data/ICSA.csv", "data/UNRATE.csv", "data/JTSJOL.csv",
                           "2020-01-01", "2025-06-01");
    std::cout << "Merged " << table.size() << " rows to data/weekly_data.csv\n";

    std::vector<double> icsaVec;
    std::vector<std::string> dates;
    icsaVec.reserve(table.size());
    dates.reserve(table.size());
    std::string lastDate;
    for (const auto& [date, row] : table) {
        icsaVec.push_back(row.icsa);
        dates.push_back(date);
        lastDate = date;
    }

    auto to_tm = [](const std::string& d) {
        std::tm tm{};
        tm.tm_year = std::stoi(d.substr(0, 4)) - 1900;
        tm.tm_mon = std::stoi(d.substr(5, 2)) - 1;
        tm.tm_mday = std::stoi(d.substr(8, 2));
        tm.tm_isdst = -1;
        return tm;
    };
    auto to_string = [](const std::tm& tm) {
        char buf[11];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d", &tm);
        return std::string(buf);
    };
    std::tm lt = to_tm(lastDate);
    std::time_t t = std::mktime(&lt);
    std::vector<std::string> forecastWeeks;
    for (int i = 1; i <= 3; ++i) {
        t += 7 * 24 * 60 * 60;
        std::tm *nt = std::localtime(&t);
        forecastWeeks.push_back(to_string(*nt));
    }

    std::vector<double> icsaTrend = moving_average(icsaVec, 4);

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

    bool ok1 = write_forecast_csv("output/forecast.csv", "ICSA", "SMA", forecastWeeks, smaPred);
    bool ok2 = false;
    if (forecastWeeks.size() == expPred.size()) {
        std::ofstream out("output/forecast.csv", std::ios::app);
        if (out.is_open()) {
            for (std::size_t i = 0; i < forecastWeeks.size(); ++i) {
                out << "ICSA,EXP," << forecastWeeks[i] << ',' << expPred[i] << '\n';
            }
            ok2 = true;
        }
    }

    if (ok1 && ok2) {
        std::cout << "Forecast CSV written to output/forecast.csv\n";
    } else {
        std::cout << "Failed to write forecast CSV\n";
    }

    plot_series("ICSA Weekly Claims",
                dates,
                icsaVec,
                icsaTrend,
                forecastWeeks,
                smaPred,
                "output/icsa.png");

    ascii_sparkline("ICSA Raw Values", icsaVec);
    ascii_sparkline("ICSA Trend (window=4)", icsaTrend);
    ascii_sparkline("SMA Forecast (3 weeks)", smaPred);
    ascii_sparkline("EXP Forecast (3 weeks)", expPred);

    return 0;
}
