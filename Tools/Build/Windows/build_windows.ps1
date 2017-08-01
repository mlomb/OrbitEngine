$targetSystem = "Windows"
$ScriptRoot = Split-Path $MyInvocation.MyCommand.Path
. "$ScriptRoot\common.ps1"

echo "********** BUILDING FOR WINDOWS ON WINDOWS **********"

Push-Location $buildPath
cmake.exe "$basePath"
Pop-Location