@echo off
chcp 65001>nul
setlocal EnableExtensions EnableDelayedExpansion

pushd "%~dp0"
if not exist "cache" md "cache"


set programName=CursorMower
set programName_joke=КурсороКосарка

set cursorMower=%1
if "!cursorMower!" == "" set cursorMower=cursorMower.exe

set layout=cache\layout_loop


title %programName% Sample
cls







:loop
  if not exist "!cursorMower!" (
    echo.^(i^) CursorMower executable wasn't found
    echo.    Ensure the binary is near the sample
    echo.
    echo.    Current dir: %cd%
    echo.    Module path: !cursorMower!
    pause
    exit /b
  )


  call :prepareLayout_main

  set _operations=15
  for /f "delims=" %%i in ('type "%layout%" 2^>nul') do set /a _operations+=1

  %cursorMower% "%layout%"


  call :prepareLayout_info !_operations!
  %cursorMower% "%layout%"


  call :prepareLayout_input
  %cursorMower% "%layout%"
  set /p input=

         if "!input!" == "0" ( exit /b
  ) else if "!input!" == "1" ( set cursorMower=..\Debug\cursorMower.exe
  ) else if "!input!" == "2" ( set cursorMower=..\MinSizeRelease\cursorMower.exe
  ) else if "!input!" == "3"   set cursorMower=..\Release\cursorMower.exe
goto :loop







:prepareLayout_main
  (
    echo.console_width=120
    echo.console_height=40
    echo.console_margin=1
    echo.console_color=black         23262e
    echo.console_color=blue          0321d7
    echo.console_color=green         8fd46d
    echo.console_color=cyan          03d6b8
    echo.console_color=red           f0266f
    echo.console_color=purple        ee5d43
    echo.console_color=yellow        ffe66d
    echo.console_color=white         ffca28
    echo.console_color=bright-black  292e38
    echo.console_color=bright-blue   03d6b8
    echo.console_color=bright-green  8fd46d
    echo.console_color=bright-cyan   03d6b8
    echo.console_color=bright-red    f92672
    echo.console_color=bright-purple ee5d43
    echo.console_color=bright-yellow ffe66d
    echo.console_color=bright-white  c74ded
    echo.clear=screen


    rem Draw window frame
    echo.color

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


    rem Draw another frame
    echo.color=c

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


    rem Draw net
    echo.color=7

    for /l %%x in (7,2,113) do (
      for /l %%y in (5,1,36) do (
        echo.cursor1=%%x %%y
        echo.text=┼─
      )
    )


    rem Draw 2 parabolas
    echo.color=c

    for /l %%x in (9,1,111) do (
      set /a x=%%x-60
      set /a y1=35-!x!*!x!/120
      set /a y2= 6+!x!*!x!/120

      if !y1! GTR 4 if !y1! LSS 40 (
        echo.cursor1=%%x !y1!
        echo.text=♦

        set /a y1-=1
        echo.cursor1=%%x !y1!
        echo.text=♦
      )

      if !y2! GTR 4 if !y2! LSS 40 (
        echo.cursor1=%%x !y2!
        echo.text=♦

        set /a y2+=1
        echo.cursor1=%%x !y2!
        echo.text=♦
      )
    )


    rem Draw a box
    echo.cursor1=10 6
    echo.cursor2=76 12
    echo.color=70
    echo.clear

    rem Write some text
    echo.right=2
    echo.down
    echo.text=Build
    echo.down

    rem Put a variable into another variable to test recursive runtime variable expansion
    set cursorMower_path=%%cursorMower%%
    if "!cursorMower_path!" == "." set cursorMower_path=Current

    echo.text=Layout write-time expansion: !cursorMower_path!
    echo.text=Render-time expansion:       %%cursorMower_path%%

    rem Draw a box
    echo.cursor1=40 15
    echo.cursor2=80 28
    echo.color=70
    echo.clear

    rem Create your interface
    echo.cursor1=46 17
    echo.text=%programName% %%programName_joke%%
    echo.down
    echo.text=1  Check debug збірку
    echo.text=2  Перевірити min-size build
    echo.text=3  Check релізний build
    echo.down
    echo.text=0  Exit
  )>"%layout%"
exit /b



:prepareLayout_info
  (
    echo.console_width=120
    echo.console_height=40
    echo.console_margin=1

    echo.cursor1=1 39
    echo.right=6
    echo.text=Layout rendered with %1 operations^^^! That's a lot^^^!
  )>"%layout%"
exit /b



:prepareLayout_input
  (
    echo.console_width=120
    echo.console_height=40
    echo.console_margin=1

    rem Prepare user input
    echo.cursor1=46 26
    echo.color=70
    rem Don't forget to escape certain characters
    echo.text=^> 
    rem Set the caret position
    echo.up
    echo.right=2
    echo.caret
  )>"%layout%"
exit /b