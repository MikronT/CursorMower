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

  for /l %%i in (3,1,116) do (
    for /l %%j in (2,1,37) do (
      echo.goto`%%i`%%j
      echo.text`=
    )
  )

  echo.clear

  for /l %%i in (3,1,116) do (
    for /l %%j in (2,1,37) do (
      echo.goto`%%i`%%j
      echo.text`/
    )
  )

  echo.clear

  for /l %%i in (3,1,116) do (
    for /l %%j in (2,1,37) do (
      echo.goto`%%i`%%j
      echo.text`\
    )
  )

  echo.clear

  for /l %%i in (3,1,116) do (
    for /l %%j in (2,1,37) do (
      echo.goto`%%i`%%j
      echo.text`^|
    )
  )

  echo.clear

  for /l %%i in (0,1, 39) do (
    echo.goto`0`%%i
    echo.text`^|
  )
  for /l %%i in (0,1, 39) do (
    echo.goto`119`%%i
    echo.text`^|
  )
  for /l %%i in (0,1,119) do (
    echo.goto`%%i`0
    echo.text`-
  )
  for /l %%i in (0,1,119) do (
    echo.goto`%%i`39
    echo.text`-
  )

  for /l %%i in (3,1,116) do (
    for /l %%j in (2,1,37) do (
      echo.goto`%%i`%%j
      echo.text`+
    )
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

  echo.goto`51`16
  echo.text`CursorMower v3.2
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