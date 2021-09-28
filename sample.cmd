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

  for /l %%i in (1,1, 38) do (
    echo.goto`0`%%i
    echo.text`│
  )
  for /l %%i in (1,1, 38) do (
    echo.goto`119`%%i
    echo.text`│
  )
  for /l %%i in (1,1,118) do (
    echo.goto`%%i`0
    echo.text`─
  )
  for /l %%i in (1,1,118) do (
    echo.goto`%%i`39
    echo.text`─
  )
  echo.goto`0`0
  echo.text`┌
  echo.goto`119`0
  echo.text`┐
  echo.goto`0`39
  echo.text`└
  echo.goto`119`39
  echo.text`┘

  for /l %%i in (1,1, 38) do (
    echo.goto`3`%%i
    echo.text`▪
  )
  for /l %%i in (1,1, 38) do (
    echo.goto`116`%%i
    echo.text`▪
  )
  for /l %%i in (1,1,118) do (
    echo.goto`%%i`2
    echo.text`▪
  )
  for /l %%i in (1,1,118) do (
    echo.goto`%%i`37
    echo.text`▪
  )

  echo.goto`39`14
  echo.clear`40
  echo.clear`40
  echo.clear`40
  echo.clear`40
  echo.clear`40
  echo.clear`40
  echo.clear`40
  echo.clear`40
  echo.clear`40
  echo.clear`40
  echo.clear`40
  echo.clear`40

  echo.goto`48`16
  echo.text`CursorMower Alpha v3.2
  echo.goto`53`18
  echo.text`1  Debug
  echo.text`2  Release
  echo.goto`53`21
  echo.text`0  Exit
)>"layout.conf"

set counter=0
for /f "delims=" %%i in ('type "layout.conf" 2^>nul') do set /a counter+=1
set /a counter+=7

%module_cursor% "layout.conf"

(
  echo.dims`120`40
  echo.margins`2`1

  echo.goto`3`37
  echo.text`!counter! operations. That's a lot^!
)>"layout.conf"
%module_cursor% "layout.conf"



(
  echo.dims`120`40
  echo.margins`2`1

  echo.goto`51`23
)>"layout.conf"
%module_cursor% "layout.conf"

set /p input="> "

       if "!input!" == "0" ( exit
) else if "!input!" == "1" ( set module_cursor=Debug\CursorMower.exe
) else if "!input!" == "2"   set module_cursor=Release\CursorMower.exe
goto :loop