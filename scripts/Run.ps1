$ErrorActionPreference = "Stop"

$ProjectRoot = Resolve-Path "$PSScriptRoot\.."
$Executable = Join-Path $ProjectRoot "build\debug\OpenGL-Model-Loader.exe"

if (-not (Test-Path $Executable)) {
    Write-Host "Executable not found." -ForegroundColor Red
    Write-Host "Run .\scripts\Build.ps1 first." -ForegroundColor Yellow
    exit 1
}

Write-Host "Running OpenGL-Model-Loader..." -ForegroundColor Cyan

& $Executable

if ($LASTEXITCODE -ne 0) {
    throw "Application exited with code $LASTEXITCODE."
}
