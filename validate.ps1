# PowerShell validation script for EconomicForecasting project
#
# This script performs the following steps:
# 1. Clean and recreate the build directory
# 2. Configure and build the CMake project
# 3. Run the resulting executable
# 4. Validate that forecast.csv and PNG outputs exist and are well formed

# Step 1: Clean build directory
if (Test-Path 'build') {
    Remove-Item -Recurse -Force 'build'
}
New-Item -ItemType Directory -Path 'build' | Out-Null

$buildSuccess = $false
$runSuccess   = $false
$outputSuccess = $false

try {
    # Step 2: Configure with CMake
    Write-Output 'Configuring CMake...'
    cmake -S . -B build
    if ($LASTEXITCODE -ne 0) { throw 'CMake configuration failed.' }

    # Build in Release mode
    Write-Output 'Building project...'
    cmake --build build --config Release
    if ($LASTEXITCODE -ne 0) { throw 'Build failed.' }
    $buildSuccess = $true
    Write-Output 'Build SUCCESS'

    # Step 3: locate and run the executable
    $exe = if (Test-Path 'build/Release/EconomicForecasting.exe') {
        'build/Release/EconomicForecasting.exe'
    } elseif (Test-Path 'build/EconomicForecasting.exe') {
        'build/EconomicForecasting.exe'
    } elseif (Test-Path 'build/EconomicForecasting') {
        'build/EconomicForecasting'
    } else {
        throw 'Executable not found after build.'
    }

    Write-Output "Running executable $exe ..."
    & $exe
    if ($LASTEXITCODE -ne 0) { throw 'Executable returned nonzero exit code.' }
    $runSuccess = $true
    Write-Output 'Run SUCCESS'

    # Step 4: Validate output files
    $csvPath = 'output/forecast.csv'
    if (-not (Test-Path $csvPath)) { throw 'forecast.csv not found in output/' }

    $png1 = 'output/icsa.png'
    $png2 = 'output/job_openings.png'
    if (-not ((Test-Path $png1) -or (Test-Path $png2))) {
        throw 'No expected PNG files found in output/'
    }

    $lines = Get-Content $csvPath
    if ($lines.Count -lt 7) { throw 'forecast.csv does not contain enough data lines.' }
    if ($lines[0].Trim() -ne 'Series,Method,Week,ForecastValue') { throw 'forecast.csv header line incorrect.' }

    $first = $lines[1].Split(',')
    if ($first.Length -lt 4) { throw 'First data row malformed.' }
    if (-not ($first[2] -match '^\d{4}-\d{2}-\d{2}$')) { throw 'First data row date not in YYYY-MM-DD format.' }
    [double]$value = 0
    if (-not ([double]::TryParse($first[3], [ref]$value))) { throw 'First data row forecast value is not numeric.' }

    $outputSuccess = $true
    Write-Output 'Output files OK'
}
catch {
    Write-Output $_
}
finally {
    if (-not $buildSuccess) { Write-Output 'Build FAILURE' }
    if ($buildSuccess -and -not $runSuccess) { Write-Output 'Run FAILURE' }
    if ($buildSuccess -and $runSuccess -and -not $outputSuccess) { Write-Output 'Output check FAILURE' }

    if ($buildSuccess -and $runSuccess -and $outputSuccess) {
        exit 0
    } else {
        exit 1
    }
}
