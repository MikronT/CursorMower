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
cls
(
  echo.screen_width=120
  echo.screen_height=40
  echo.screen_margin=1

  for %%i in (1 120) do (
    for /l %%j in (2,1,39) do (
      echo.cursor_to=%%i %%j
      echo.text=│
    )
  )
  for %%i in (1 40) do (
    for /l %%j in (2,1,119) do (
      echo.cursor_to=%%j %%i
      echo.text=─
    )
  )
  echo.cursor_to=1 1
  echo.text=┌
  echo.cursor_to=120 1
  echo.text=┐
  echo.cursor_to=1 40
  echo.text=└
  echo.cursor_to=120 40
  echo.text=┘

  for %%i in (4 117) do (
    for /l %%j in (2,1,39) do (
      echo.cursor_to=%%i %%j
      echo.text=▪
    )
  )
  for %%i in (3 38) do (
    for /l %%j in (2,1,119) do (
      echo.cursor_to=%%j %%i
      echo.text=▪
    )
  )

  echo.cursor_from=40 15
  echo.cursor_to=60 27
  echo.clear

  echo.cursor_to=49 17
  echo.text=CursorMower Alpha v4.0
  echo.cursor_to=54 19
  echo.text=1  Check debug build
  echo.text=2  Check release build
  echo.skip
  echo.text=0  Exit
)>"layout.conf"

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