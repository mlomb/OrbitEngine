$basePath = (Get-Item $ScriptRoot/../../../).FullName
$buildPath = $basePath + "Build\" + $targetSystem

New-Item -ItemType Directory -Force -Path $buildPath | Out-Null

echo "Base Path: $basePath"
echo "Build Path: $buildPath"