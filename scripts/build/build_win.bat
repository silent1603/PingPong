@echo off

echo Checking for cl.exe...
where cl.exe >nul 2>nul
if errorlevel 1 (
    echo cl.exe not found. Attempting to set up environment...
    if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" (
        call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
    ) else (
        echo ERROR: vcvarsall.bat not found!
        exit /b 1
    )
) else (
    echo cl.exe found. Environment already set.
)

pushd %~dp0\..\..
setlocal enabledelayedexpansion
for /r ".\sources" %%f in (*.cpp) do (
    set "cpp_files=!cpp_files! %%f"
)

if not defined cpp_files (
    echo No .cpp files found in the sources directory!
    exit /b 1
)

pushd .\bin
echo Compiling: %cpp_files%
cl %cpp_files% /link /out:pingpong.exe
popd
popd
exit /b
