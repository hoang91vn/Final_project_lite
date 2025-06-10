# Economic Forecasting Project

This project demonstrates a small workflow for processing weekly economic data and producing short-term forecasts.

## Prerequisites
Install Python 3 and the required packages before configuring:
```bash
python -m pip install numpy matplotlib
```
CMake copies the Python standard library and these packages into `build/python` during the build.

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

## Bundled Python Runtime

The application embeds the Python interpreter to produce plots via
Matplotlib. To run on a system without Python installed, place a
minimal Python distribution in a `python/` folder next to the
executable. `src/main.cpp` sets `PYTHONHOME` to this directory before
initializing the interpreter.
