$ErrorActionPreference = "Stop"

$ProjectRoot = Resolve-Path "$PSScriptRoot\.."
$BuildDir = Join-Path $ProjectRoot "build\debug"

if (-not (Test-Path (Join-Path $BuildDir "compile_commands.json"))) {
    Write-Host "compile_commands.json not found." -ForegroundColor Red
    Write-Host "Configuring Debug build..." -ForegroundColor Yellow

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

if (-not (Get-Command g++.exe -ErrorAction SilentlyContinue)) {
    Write-Host "g++.exe not found. Install MinGW and make sure it is in PATH." -ForegroundColor Red
    exit 1
}

$ClangTidyArguments = @(
    "--extra-arg-before=--target=x86_64-w64-windows-gnu"
)

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
    Write-Host "Fixing $($File.FullName)" -ForegroundColor Cyan

    clang-tidy `
        $File.FullName `
        -p $BuildDir `
        --fix `
        $ClangTidyArguments

    if ($LASTEXITCODE -ne 0) {
        $Failed = $true
    }
}

Write-Host ""

if ($Failed) {
    Write-Host "clang-tidy encountered errors." -ForegroundColor Red
    exit 1
}

Write-Host "Lint fixes complete." -ForegroundColor Green
