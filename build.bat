@echo off
setlocal

echo ==========================================
echo AutoLangSwitcher Build Script (MSVC)
echo ==========================================

REM Check if cl.exe is in the path
where cl >nul 2>nul
IF %ERRORLEVEL% NEQ 0 GOTO NO_CL

GOTO CHECK_DIRS

:NO_CL
echo Error: MSVC compiler (cl.exe) not found.
echo Please run this script from a "Developer Command Prompt for VS".
echo or ensure cl.exe is in your PATH.
pause
exit /b 1

:CHECK_DIRS
IF NOT EXIST "bin" mkdir "bin"
IF NOT EXIST "obj" mkdir "obj"
IF NOT EXIST "release" mkdir "release"

:BUILD
echo Compiling...
REM /Fo"obj\\" forces object files to obj directory (Note: double backslash might be needed for escaping in some contexts, but usually /Fo"obj/" or /Fo"obj\\" works)
cl /nologo /O2 /LD /DUNICODE /D_UNICODE /Fo"obj\\" /Fe"bin\AutoLangSwitcher.dll" src\*.cpp /I src user32.lib kernel32.lib shlwapi.lib shell32.lib comctl32.lib

IF %ERRORLEVEL% NEQ 0 GOTO BUILD_FAIL

echo.
echo Build SUCCESSFUL.
echo Plugin is located at: %~dp0bin\AutoLangSwitcher.dll

echo Copying to release folder...
copy /Y "bin\AutoLangSwitcher.dll" "release\AutoLangSwitcher.dll" >nul

echo.
echo Release available at: %~dp0release\AutoLangSwitcher.dll
echo.
pause
exit /b 0

:BUILD_FAIL
echo.
echo Build FAILED.
pause
exit /b 1
