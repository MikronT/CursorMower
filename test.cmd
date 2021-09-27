@echo off
chcp 65001>nul

setlocal EnableExtensions EnableDelayedExpansion

pushd "%~dp0"

set module_cursor=Debug\CursorMower.exe





:loop
cls
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

  echo.line`39`17`                                        
  echo.line`39`18`                                        
  echo.line`39`19`                                        
  echo.line`39`20`                                        
  echo.line`39`21`                                        
  echo.line`39`22`                                        
  echo.line`39`23`                                        
  echo.line`39`24`                                        

  echo.line`51`18`CursorMower v2.0
  echo.line`53`20`1  Debug
  echo.line`53`21`2  Release

  echo.end`51`23
)>"layout.conf"



%module_cursor% "layout.conf"
set /p input="> "

       if "!input!" == "1" ( set module_cursor=Debug\CursorMower.exe
) else if "!input!" == "2"   set module_cursor=Release\CursorMower.exe
goto :loop