$ErrorActionPreference = "Stop"

$ProjectRoot = Resolve-Path "$PSScriptRoot\.."
$VcpkgDir = Join-Path $ProjectRoot "vcpkg"

if (-not (Test-Path $VcpkgDir)) {
    Write-Host "Cloning vcpkg..." -ForegroundColor Cyan

    git clone https://github.com/microsoft/vcpkg.git $VcpkgDir

    if ($LASTEXITCODE -ne 0) {
        throw "Failed to clone vcpkg."
    }
} else {
    Write-Host "vcpkg already exists. Skipping clone." -ForegroundColor Yellow
}

Push-Location $VcpkgDir

try {
    Write-Host "Using vcpkg 2026.07.29..." -ForegroundColor Cyan

    git fetch --tags

    if ($LASTEXITCODE -ne 0) {
        throw "Failed to fetch vcpkg tags."
    }

    git checkout 2026.07.29

    if ($LASTEXITCODE -ne 0) {
        throw "Failed to checkout vcpkg 2026.07.29."
    }
} finally {
    Pop-Location
}

$Bootstrap = Join-Path $VcpkgDir "bootstrap-vcpkg.bat"

Write-Host "Bootstrapping vcpkg..." -ForegroundColor Cyan

& $Bootstrap

if ($LASTEXITCODE -ne 0) {
    throw "vcpkg bootstrap failed."
}

Write-Host "Setup complete." -ForegroundColor Green
