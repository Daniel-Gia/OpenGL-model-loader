$ErrorActionPreference = "Stop"

$ProjectRoot = Resolve-Path "$PSScriptRoot\.."
$BuildDir = Join-Path $ProjectRoot "build\debug"

if (-not (Test-Path (Join-Path $BuildDir "compile_commands.json"))) {
    Write-Host "compile_commands.json not found." -ForegroundColor Red

    & "$PSScriptRoot\Configure.ps1"

    if ($LASTEXITCODE -ne 0) {
        throw "Configuration failed."
    }
}

if (-not (Get-Command clang-tidy -ErrorAction SilentlyContinue)) {
    Write-Host "clang-tidy not found." -ForegroundColor Red
    Write-Host "Install LLVM and make sure clang-tidy is in PATH." -ForegroundColor Yellow
    exit 1
}

Write-Host "Using clang-tidy:" -ForegroundColor Cyan
Write-Host (Get-Command clang-tidy).Source
Write-Host ""

$Files = Get-ChildItem `
    -Path (Join-Path $ProjectRoot "src") `
    -Recurse `
    -File `
    -Include *.cpp

if (-not $Files) {
    Write-Host "No C++ source files found." -ForegroundColor Yellow
    exit 0
}

$Failed = $false

foreach ($File in $Files) {
    Write-Host "Checking $($File.FullName)" -ForegroundColor Cyan

    clang-tidy `
        $File.FullName `
        -p $BuildDir

    if ($LASTEXITCODE -ne 0) {
        $Failed = $true
    }
}

Write-Host ""

if ($Failed) {
    Write-Host "Linting found issues." -ForegroundColor Red
    exit 1
}

Write-Host "Lint check passed." -ForegroundColor Green
