@echo off
chcp 65001>nul

setlocal EnableExtensions EnableDelayedExpansion

pushd "%~dp0"



set program_name=CursorMower
set program_version=Beta v4.2



set arg_targetDir=%1
if "!arg_targetDir!" == "" set arg_targetDir=Release

set module_cursor=!arg_targetDir!\cursorMower.exe



if not exist "%module_cursor%" (
  echo.^(i^) CursorMower module not found
  echo.    Compile the project at first
  exit /b
)







:loop
  call :prepareLayout_main

  set counter=13
  for /f "delims=" %%i in ('type "layout.conf" 2^>nul') do set /a counter+=1

  %module_cursor% "layout.conf"

  call :prepareLayout_info !counter!
  %module_cursor% "layout.conf"

  call :prepareLayout_input
  %module_cursor% "layout.conf"
  set /p input=


         if "!input!" == "0" ( exit /b
  ) else if "!input!" == "1" ( set arg_targetDir=Debug
  ) else if "!input!" == "2" ( set arg_targetDir=MinSizeRelease
  ) else if "!input!" == "3"   set arg_targetDir=Release


  set module_cursor=!arg_targetDir!\CursorMower.exe
goto :loop







:prepareLayout_main
  (
    echo.console_width=120
    echo.console_height=40
    echo.console_margin=1
    echo.console_color=0 23262e
    echo.console_color=1 0321d7
    echo.console_color=2 8fd46d
    echo.console_color=3 03d6b8
    echo.console_color=4 f0266f
    echo.console_color=5 ee5d43
    echo.console_color=6 ffe66d
    echo.console_color=7 ffca28
    echo.console_color=8 292e38
    echo.console_color=9 03d6b8
    echo.console_color=a 8fd46d
    echo.console_color=b 03d6b8
    echo.console_color=c f92672
    echo.console_color=d ee5d43
    echo.console_color=e ffe66d
    echo.console_color=f c74ded
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


    rem Write some text
    echo.cursor1=40 15
    echo.cursor2=80 28
    echo.color=70
    echo.clear

    echo.cursor1=40 15
    echo.text=!arg_targetDir!

    echo.cursor1=49 17
    echo.text=%program_name% %program_version%
    echo.down
    echo.text=1  Check debug build
    echo.text=2  Check min-size build
    echo.text=3  Check release build
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

    echo.cursor1=49 26
    echo.color=70
    echo.text=^> 
  )>"layout.conf"
exit /b