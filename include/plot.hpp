#ifndef PLOT_HPP
#define PLOT_HPP

#include <vector>
#include <string>

void plot_series(const std::string& title,
                 const std::vector<std::string>& dates,
                 const std::vector<double>& raw,
                 const std::vector<double>& trend,
                 const std::vector<std::string>& forecastDates,
                 const std::vector<double>& forecastVals,
                 const std::string& outputPng);

void ascii_sparkline(const std::string& title,
                     const std::vector<double>& data);

#endif
