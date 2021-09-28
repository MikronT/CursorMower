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

set counter=9
for /f "delims=" %%i in ('type "layout.conf" 2^>nul') do set /a counter+=1

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

  for %%x in (1 120) do (
    for /l %%y in (2,1,39) do (
      echo.cursor1=%%x %%y
      echo.text=│
    )
  )
  for %%y in (1 40) do (
    for /l %%x in (2,1,119) do (
      echo.cursor1=%%x %%y
      echo.text=─
    )
  )
  echo.cursor1=1 1
  echo.text=┌
  echo.cursor1=120 1
  echo.text=┐
  echo.cursor1=1 40
  echo.text=└
  echo.cursor1=120 40
  echo.text=┘

  for %%x in (4 117) do (
    for /l %%y in (2,1,39) do (
      echo.cursor1=%%x %%y
      echo.text=●
    )
  )
  for %%y in (3 38) do (
    for /l %%x in (2,1,119) do (
      echo.cursor1=%%x %%y
      echo.text=●
    )
  )

  for /l %%x in (7,2,113) do (
    for /l %%y in (5,1,36) do (
      echo.cursor1=%%x %%y
      echo.text=┼─
    )
  )

  for /l %%x in (9,1,111) do (
    set /a x=%%x-60
    set /a y1=35-!x!*!x!/120
    set /a y2= 6+!x!*!x!/120

    if !y1! GTR 4 if !y1! LSS 40 (
      echo.cursor1=%%x !y1!
      echo.text=█

      set /a y1-=1
      echo.cursor1=%%x !y1!
      echo.text=█
    )

    if !y2! GTR 4 if !y2! LSS 40 (
      echo.cursor1=%%x !y2!
      echo.text=█

      set /a y2+=1
      echo.cursor1=%%x !y2!
      echo.text=█
    )
  )

  echo.cursor1=40 15
  echo.cursor2=80 27
  echo.clear

  echo.cursor1=49 17
  echo.text=CursorMower Alpha v4.0

  rem echo.cursor2=54 19
  echo.text=1  Check debug build
  echo.text=2  Check release build
  rem echo.skip
  echo.text=0  Exit
)>"layout.conf"
exit /b