$ErrorActionPreference = "Stop"

$ProjectRoot = Resolve-Path "$PSScriptRoot\.."

$SourceDirectories = @(
    Join-Path $ProjectRoot "src"
    Join-Path $ProjectRoot "include"
)

$Files = foreach ($Directory in $SourceDirectories) {
    if (Test-Path $Directory) {
        Get-ChildItem `
            -Path $Directory `
            -Recurse `
            -File `
            -Include *.cpp, *.h, *.hpp
    }
}

$Failed = $false

foreach ($File in $Files) {
    clang-format `
        --dry-run `
        --Werror `
        $File.FullName

    if ($LASTEXITCODE -ne 0) {
        Write-Host "Formatting violation: $($File.FullName)" -ForegroundColor Red
        $Failed = $true
    }
}

if ($Failed) {
    Write-Host ""
    Write-Host "Formatting check failed." -ForegroundColor Red
    Write-Host "Run .\scripts\Format.ps1 to fix formatting." -ForegroundColor Yellow
    exit 1
}

Write-Host "Formatting check passed." -ForegroundColor Green
