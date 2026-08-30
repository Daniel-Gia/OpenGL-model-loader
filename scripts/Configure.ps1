$ErrorActionPreference = "Stop"

Write-Host "Configuring Debug build..." -ForegroundColor Cyan

cmake --preset debug

if ($LASTEXITCODE -ne 0) {
    throw "CMake configuration failed."
}

Write-Host "Configuration complete." -ForegroundColor Green
