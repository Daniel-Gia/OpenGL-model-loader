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

if (-not $Files) {
    Write-Host "No C++ files found." -ForegroundColor Yellow
    exit 0
}

foreach ($File in $Files) {
    Write-Host "Formatting $($File.FullName)" -ForegroundColor Cyan

    clang-format -i $File.FullName

    if ($LASTEXITCODE -ne 0) {
        throw "clang-format failed for $($File.FullName)"
    }
}

Write-Host ""
Write-Host "Formatting complete." -ForegroundColor Green
