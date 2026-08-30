$ErrorActionPreference = "Stop"

& "$PSScriptRoot\Build.ps1"

if ($LASTEXITCODE -ne 0) {
    throw "Build failed."
}

Write-Host "Build successful." -ForegroundColor Green

& "$PSScriptRoot\Run.ps1"
