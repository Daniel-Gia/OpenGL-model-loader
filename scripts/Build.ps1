$ErrorActionPreference = "Stop"

Write-Host "Building Debug configuration..." -ForegroundColor Cyan

cmake --build --preset build-debug

if ($LASTEXITCODE -ne 0) {
    throw "Build failed."
}

Write-Host "Build complete." -ForegroundColor Green
