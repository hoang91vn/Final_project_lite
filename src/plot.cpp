#include "plot.hpp"
#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;

void plot_series(const std::string& title,
                 const std::vector<std::string>& dates,
                 const std::vector<double>& raw,
                 const std::vector<double>& trend,
                 const std::vector<std::string>& forecastDates,
                 const std::vector<double>& forecastVals,
                 const std::string& outputPng) {
    // TODO: implement matplotlib-cpp plotting
}

void ascii_sparkline(const std::string& title,
                     const std::vector<double>& data) {
    // TODO: implement console sparkline
}
