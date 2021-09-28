@echo off
chcp 65001>nul

setlocal EnableExtensions EnableDelayedExpansion

pushd "%~dp0"

set module_cursor=Debug\CursorMower.exe





if not exist "%module_cursor%" (
  echo.^(i^) CursorMower module not found
  echo.    Compile the project at first
  pause>nul
  exit
)





:loop
call :prepareMainLayout

set counter=0
for /f "delims=" %%i in ('type "layout.conf" 2^>nul') do set /a counter+=1
set /a counter+=9

%module_cursor% "layout.conf"

(
  echo.screen_width=120
  echo.screen_height=40
  echo.screen_margin=1

  echo.cursor_to=4 38
  echo.text=!counter! operations. That's a lot^!
)>"layout.conf"
%module_cursor% "layout.conf"



(
  echo.screen_width=120
  echo.screen_height=40
  echo.screen_margin=1

  echo.cursor_to=52 24
)>"layout.conf"
%module_cursor% "layout.conf"

set /p input="> "

       if "!input!" == "0" ( exit
) else if "!input!" == "1" ( set module_cursor=Debug\CursorMower.exe
) else if "!input!" == "2"   set module_cursor=Release\CursorMower.exe
goto :loop





:prepareMainLayout
(
  echo.screen_width=120
  echo.screen_height=40
  echo.screen_margin=1

  echo.clear=screen

  for %%i in (1 120) do (
    for /l %%j in (2,1,39) do (
      echo.cursor_to=%%i %%j
      echo.text=│
    )
  )
)>"layout.conf"
exit /b