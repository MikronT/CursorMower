@echo off
chcp 65001>nul

setlocal EnableExtensions EnableDelayedExpansion

pushd "%~dp0"

set module_cursor=Debug\CursorMower.exe





(
  echo.dims`120`40
  echo.margins`2`1

  for /l %%i in (0,1, 39) do echo.line`0`%%i`^|
  for /l %%i in (0,1, 39) do echo.line`119`%%i`^|
  for /l %%i in (0,1,119) do echo.line`%%i`0`-
  for /l %%i in (0,1,119) do echo.line`%%i`39`-

  for /l %%i in (3,1,116) do (
    for /l %%j in (2,1,37) do (
      echo.line`%%i`%%j`+
    )
  )

  echo.finally`1`39
)>"layout.conf"



:loop
cls
%module_cursor% "layout.conf"
set /p input="> "
goto :loop