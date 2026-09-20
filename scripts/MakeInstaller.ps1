$ErrorActionPreference = "Stop"

$BuildDir = "final_build"
$IssScript = "installer.iss"

Write-Host "Making the installer..." -ForegroundColor Cyan

if (-not (Get-Command "ISCC.exe" -ErrorAction SilentlyContinue)) {
    Write-Host "Error: 'ISCC.exe' was not found in your PATH" -ForegroundColor Red
    Write-Host "Please ensure Inno Setup is installed and added to your PATH" -ForegroundColor Yellow
    exit 1
}

if (-not (Test-Path -Path $BuildDir -PathType Container)) {
    Write-Host "Error: '$BuildDir' folder not found" -ForegroundColor Red
    exit 1
}

Write-Host "Packaging '$BuildDir' into an installer..." -ForegroundColor Cyan

& ISCC.exe $IssScript

Write-Host "Success! Created: OpenGL-Model-Loader-Installer.exe" -ForegroundColor Green
