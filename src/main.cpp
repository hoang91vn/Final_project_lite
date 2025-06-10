#include <Eigen/Dense>
#include <matplot/matplot.h>
#include <vector>
#include <iostream>

int main() {
    // Sample GDP growth rates for demonstration
    std::vector<double> years{2018, 2019, 2020, 2021, 2022, 2023};
    std::vector<double> gdpGrowth{2.9, 2.3, -3.5, 5.7, 2.6, 1.9};

    // Map data to an Eigen vector
    Eigen::Map<Eigen::VectorXd> data(gdpGrowth.data(), gdpGrowth.size());

    // Compute 3-point moving average using Eigen segments
    Eigen::VectorXd avg(data.size() - 2);
    for (int i = 0; i < avg.size(); ++i) {
        avg(i) = data.segment(i, 3).mean();
    }

    // Convert results back to std::vector for plotting
    std::vector<double> maVec(avg.data(), avg.data() + avg.size());
    std::vector<double> maYears(years.begin() + 2, years.end());

    using namespace matplot;

    // Plot raw GDP growth
    plot(years, gdpGrowth, "-o")->display_name("GDP Growth");
    hold(on);
    // Plot smoothed series
    plot(maYears, maVec, "--")->display_name("3-pt Moving Avg");

    title("GDP Growth Example");
    xlabel("Year");
    ylabel("Growth (%)");
    legend();
    save("gdp_growth.png");

    return 0;
}
