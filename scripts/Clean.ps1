$ErrorActionPreference = "Stop"

$ProjectRoot = Resolve-Path "$PSScriptRoot\.."

$Directories = @(
    "build",
    "final_build"
)

Write-Host "The following directories will be deleted:" -ForegroundColor Yellow

$Found = $false

foreach ($Directory in $Directories) {
    $Path = Join-Path $ProjectRoot $Directory

    if (Test-Path $Path) {
        Write-Host "  $Directory"
        $Found = $true
    }
}

if (-not $Found) {
    Write-Host "Nothing to clean." -ForegroundColor Green
} else {
    $Confirmation = Read-Host "Continue? (y/N)"

    if ($Confirmation -notmatch "^[Yy]$") {
        Write-Host "Canceled." -ForegroundColor Yellow
        exit 0
    }

    foreach ($Directory in $Directories) {
        $Path = Join-Path $ProjectRoot $Directory

        if (Test-Path $Path) {
            Write-Host "Deleting $Directory..." -ForegroundColor Cyan
            Remove-Item $Path -Recurse -Force
        }
    }
}

$VcpkgDir = Join-Path $ProjectRoot "vcpkg"

if (Test-Path $VcpkgDir) {
    Write-Host ""
    $DeleteVcpkg = Read-Host "Also delete vcpkg? (y/N)"

    if ($DeleteVcpkg -match "^[Yy]$") {
        Write-Host "Deleting vcpkg..." -ForegroundColor Cyan
        Remove-Item $VcpkgDir -Recurse -Force
        Write-Host "vcpkg deleted." -ForegroundColor Green
    } else {
        Write-Host "Keeping vcpkg." -ForegroundColor Green
    }
}

Write-Host ""
Write-Host "Cleanup complete." -ForegroundColor Green
