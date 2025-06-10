
#ifndef FORECAST_HPP
#define FORECAST_HPP

#include <vector>

// Simple moving average forecast
std::vector<double> forecast_sma(const std::vector<double>& series,
                                 std::size_t window,
                                 std::size_t steps);

// Simple exponential smoothing forecast
std::vector<double> forecast_exp(const std::vector<double>& series,
                                 double alpha,
                                 std::size_t steps);

#endif
