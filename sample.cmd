@echo off
chcp 65001>nul

setlocal EnableExtensions EnableDelayedExpansion

pushd "%~dp0"



set program_name=CursorMower
set program_name_pun=КурсороКосарка

set targetDir=%1
if "!targetDir!" == "" set targetDir=.







:loop
  if not exist "!targetDir!\cursorMower.exe" (
    echo.^(i^) CursorMower module not found
    echo.    Ensure the binary is in the same location as the sample
    pause
    exit /b
  )
  pushd "!targetDir!"


  call :prepareLayout_main

  set _operations=13
  for /f "delims=" %%i in ('type "layout.conf" 2^>nul') do set /a _operations+=1

  cursorMower "layout.conf"

  call :prepareLayout_info !_operations!
  cursorMower "layout.conf"


  call :prepareLayout_input
  cursorMower "layout.conf"
  set /p input=

         if "!input!" == "0" ( exit /b
  ) else if "!input!" == "1" ( set targetDir=Debug
  ) else if "!input!" == "2" ( set targetDir=MinSizeRelease
  ) else if "!input!" == "3"   set targetDir=Release

  popd
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
    echo.cursor2=60 12
    echo.color=70
    echo.clear

    rem Write some text
    echo.right=2
    echo.down
    echo.text=Build
    echo.down

    rem Put a variable into another variable to test recursive runtime variable expansion
    set displayDir=dir=%%targetDir%%
    if "!displayDir!" == "." set displayDir=Current

    echo.text=Layout write-time expansion: !displayDir!
    echo.text=Render-time expansion:       %%displayDir%%

    rem Draw a box
    echo.cursor1=40 15
    echo.cursor2=80 28
    echo.color=70
    echo.clear

    rem Write some text
    echo.cursor1=46 17
    echo.text=%program_name% %%program_name_pun%%
    echo.down
    echo.text=1  Check debug збірку
    echo.text=2  Перевірити min-size build
    echo.text=3  Check релізний build
    echo.down
    echo.text=0  Exit
  )>"layout.conf"
exit /b



:prepareLayout_info
  (
    echo.console_width=120
    echo.console_height=40
    echo.console_margin=1

    echo.cursor1=0 39
    rem Wrong movements are omitted
    echo.left=300
    echo.right=6
    echo.text=Layout rendered with %1 operations^^^! That's a lot^^^!
  )>"layout.conf"
exit /b



:prepareLayout_input
  (
    echo.console_width=120
    echo.console_height=40
    echo.console_margin=1

    echo.cursor1=46 26
    echo.color=70
    echo.text=^> 
    echo.up
    echo.right=2
    echo.caret
  )>"layout.conf"
exit /b