# Economic Forecasting Project

This project demonstrates a small workflow for processing weekly economic data and producing short-term forecasts.

## Prerequisites
Python 3 and its development headers must be available on your system. The
`EconomicForecasting` executable relies on the system interpreter at runtime.
If `numpy` or `matplotlib` are not installed, the program will offer to install
them using `pip` in the user site-packages directory.

## Building with VS Code

1. Install the **CMake Tools** extension.
2. Open the repository folder in VS Code.
3. Run **CMake: Configure** from the command palette and select a generator.
4. Run **CMake: Build** to compile the `EconomicForecasting` target.

## Building with command-line CMake

If you prefer using the command line, configure and build the project with:

```bash
cmake -S . -B build
cmake --build build --config Release
```

After the build completes, run the executable from the `build` directory:

```bash
cd build
cp -r ../data .  # copy input CSV files
./EconomicForecasting
```

## Running

The executable expects the `data/` folder to be in the current directory.
From the `build/` directory run:

```bash
./EconomicForecasting
```

The program writes forecast data to `output/forecast.csv` and generates PNG plots in the `output/` folder.

When executed, the application will attempt to import `numpy` and `matplotlib` using the
system Python. If either package is missing you will be prompted to install them with
`pip --user`. Installation may require additional permissions depending on your Python setup.
