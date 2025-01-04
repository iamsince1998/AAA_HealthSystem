@echo off
setlocal

rem Define the registry path and value name for File 1
set "regPath=HKEY_CLASSES_ROOT\Unreal.ProjectFile\shell\rungenproj"
set "valueName=Icon"

rem Initialize variables for File 1
set "uprojectFile="
set "uprojectFileWithPath="
set "regValue="
set "command="

rem Use PowerShell to get the registry value
for /f "delims=" %%A in ('powershell -command "Get-ItemProperty -Path Registry::%regPath% -Name %valueName% | Select-Object -ExpandProperty %valueName%" 2^>nul') do (
    echo Debug: Retrieved value from PowerShell: %%A
    set "regValue=%%A"
)

rem Remove leading and trailing quotes from the registry value
set "regValue=%regValue:"=%"

rem Check if the value was retrieved
if "%regValue%"=="" (
    echo Could not retrieve the registry value or the path/value may not exist.
    goto :eof
)

rem Display the retrieved registry value
echo Registry Value: %regValue%

rem Fetch and store the current directory path in a variable
set "currentDir=%CD%"

rem Use a for loop to find the .uproject file
for %%f in (*.uproject) do (
    set "uprojectFile=%%f"
    set "uprojectFileWithPath=%currentDir%\%%f"
    goto :foundUProject
)

rem Check if the .uproject file was found
if "%uprojectFile%"=="" (
    echo No .uproject file found in the current directory.
    goto :eof
)

:foundUProject
rem Display the retrieved .uproject file and its full path
echo Found .uproject file: %uprojectFile%
echo Full path of .uproject file: %uprojectFileWithPath%

rem Construct the full command string with proper quoting
set "command="%regValue%" -projectfiles "%uprojectFileWithPath%""

rem Display the full command string
echo Full Command: %command%

rem Execute the command
echo Running Command...
call %command%