# Restores data/ from the pristine copy in data_baseline/.
# Run this before every demo, and between members, so the numbers in
# docs/DEMO.md always match what appears on screen.

$root = Split-Path -Parent $MyInvocation.MyCommand.Path
$src  = Join-Path $root "data_baseline"
$dst  = Join-Path $root "data"

if (-not (Test-Path $src)) {
    Write-Host "data_baseline folder is missing - cannot reset." -ForegroundColor Red
    exit 1
}

if (-not (Test-Path $dst)) {
    New-Item -ItemType Directory -Path $dst | Out-Null
}

Copy-Item (Join-Path $src "*.txt") $dst -Force

Write-Host "Demo data reset." -ForegroundColor Green
Write-Host "  users.txt       6 users   (A001, L001, L002, S001, S002, S003)"
Write-Host "  courses.txt     5 courses (CS101, CS102, CS103, CS104, L432)"
Write-Host "  enrolments.txt  3 enrolments"
Write-Host "  attendance.txt  empty"
Write-Host "  replay_taps.txt 4 taps"
