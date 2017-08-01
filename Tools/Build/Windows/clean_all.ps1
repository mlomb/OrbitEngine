$ScriptRoot = Split-Path $MyInvocation.MyCommand.Path
. "$ScriptRoot\common.ps1"

echo "********** CLEAN ALL **********"

Remove-Item $buildPath -Recurse