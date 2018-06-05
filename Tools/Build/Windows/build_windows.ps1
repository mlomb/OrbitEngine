$targetSystem = "Windows"
$ScriptRoot = Split-Path $MyInvocation.MyCommand.Path
. "$ScriptRoot\common.ps1"

echo "********** BUILDING FOR WINDOWS ON WINDOWS **********"

Push-Location $buildPath
cmake -DCMAKE_BUILD_TYPE=Release "$basePath"
Pop-Location