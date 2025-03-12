@echo off

pushd %~dp0/../..
echo %cd%
if not exist bin\ (
    mkdir bin
)
pushd workspaces
if not exist vs2022\ (
    mkdir vs2022
)
pushd vs2022
cmake -G "Visual Studio 17 2022" ../.. -DPLATFORMS=windows 
popd
popd
popd