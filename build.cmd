@echo off
setlocal EnableDelayedExpansion

set "SOURCE_DIR=%~dp0"

if "%1" == "x64" (
  set ARCHITECTURE=x64
  set GENERATOR=Visual Studio 11 2012 Win64
) else if "%1" == "x86" (
  set ARCHITECTURE=x86
  set GENERATOR=Visual Studio 11 2012
) else (
  echo Usage: build x86^|x64 [zip]
  exit /b 1
)

cmake -G "%GENERATOR%" "%SOURCE_DIR%"
cmake --build . --config Release --target ALL_BUILD --target test
copy Release\BFArchive.exe .

if "%2" == "zip" (
  7z a -tzip bfarchive-win-%ARCHITECTURE%.zip BFArchive.exe
)
