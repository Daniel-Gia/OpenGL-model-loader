$ErrorActionPreference = "Stop"

$ProjectRoot = Resolve-Path "$PSScriptRoot\.."
$OutputDir = Join-Path $ProjectRoot "final_build"

Write-Host "Configuring Release build..." -ForegroundColor Cyan

cmake --preset release

if ($LASTEXITCODE -ne 0) {
    throw "Release configuration failed."
}

Write-Host "Building Release version..." -ForegroundColor Cyan

cmake --build --preset build-release

if ($LASTEXITCODE -ne 0) {
    throw "Release build failed."
}

Write-Host "Installing to final_build..." -ForegroundColor Cyan

cmake --install `
    (Join-Path $ProjectRoot "build\release") `
    --prefix $OutputDir

if ($LASTEXITCODE -ne 0) {
    throw "Installation failed."
}

Write-Host "Release build complete." -ForegroundColor Green
