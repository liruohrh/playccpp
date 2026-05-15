# 1. 初始化 VS x64 开发环境
Import-Module "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\Microsoft.VisualStudio.DevShell.dll"
Enter-VsDevShell a2e1d727 -SkipAutomaticLocation -DevCmdArguments "-arch=x64 -host_arch=x64"

$env:PATH = "C:\xampp\apache\bin;$env:PATH"
$env:PATH = "C:\xampp\perl\bin;$env:PATH"
$env:PATH = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer;$env:PATH"
