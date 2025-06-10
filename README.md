# Economic Forecasting Project

This project demonstrates a small workflow for processing weekly economic data and producing short-term forecasts.

## Building with VS Code

1. Install the **CMake Tools** extension.
2. Open the repository folder in VS Code.
3. Run **CMake: Configure** from the command palette and select a generator.
4. Run **CMake: Build** to compile the `EconomicForecasting` target.

## Running

From the `build/` directory run:

```bash
./EconomicForecasting
```

The program writes forecast data to `output/forecast.csv` and generates PNG plots in the `output/` folder.
