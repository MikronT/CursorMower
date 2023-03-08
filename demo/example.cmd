@echo off
rem Default Batch file header with required options
chcp 65001>nul
setlocal EnableExtensions EnableDelayedExpansion

rem Go to the project directory
pushd "%~dp0"
if not exist "cache" md "cache"

set programName=CursorMower Example
set cursorMower=cursorMower.exe
set layout=cache\layout

if not exist "!cursorMower!" (
    echo.^(i^) CursorMower executable wasn't found
    echo.    Ensure the binary is near the sample
    echo.
    echo.    Current dir: %cd%
    echo.    Module path: !cursorMower!
    pause
    exit /b
)


rem Set window title
title %programName%
rem Clear screen just to be sure
cls



:main
rem Dynamic variable workaround
set workaround_date=%date%
set workaround_time=%time%

rem Recursive variable expansion is supported
set programName_recursive=%%programName%%

rem Write cache files only once
if not exist "%layout%_main" (
    rem Set window dims and inner margin
    echo.console_width=120
    echo.console_height=40

    rem Clear screen if you need
    echo.color=0f
    echo.clear=screen

    rem Draw a frame around
    echo.cursor1=8 5
    echo.cursor2=73 11
    echo.color=80
    echo.clear
    rem Fill a box inside
    echo.right=2
    echo.down
    echo.left2=2
    echo.up2
    echo.color=f0
    echo.clear

    rem Write some text
    echo.right=2
    echo.down
    rem Double percent to write variable names for dynamic expansion
    echo.color=1f
    echo.text=Current date/time: %%workaround_date%% %%workaround_time%%
    echo.color=e0
    echo.text=Dynamic recursive variable expansion: %%programName_recursive%%

    rem Draw a frame around
    echo.cursor1=34 13
    echo.cursor2=86 28
    echo.color=80
    echo.clear
    rem Fill a box inside
    echo.right=2
    echo.down
    echo.left2=2
    echo.up2
    echo.color=f0
    echo.clear

    rem Create your interface
    echo.cursor1=40 16
    echo.color=f5
    rem Static variable which value is written directly to the file
    echo.text=%programName%
    echo.down
    echo.color=f1
    echo.text=1  Do nothing
    echo.text=2  Spawn popup
    echo.text=3  Clear cache
    echo.down
    echo.text=0  Exit

    rem Prepare user input
    echo.down
    rem Don't forget to escape certain characters
    echo.color=f5
    echo.text=$^> 
    rem Set the caret position
    echo.up
    echo.right=3
    echo.caret
)>"%layout%_main"

rem Get user input
%cursorMower% "%layout%_main"

set /p input=
       if "!input!" == "2" ( call :main_popup
) else if "!input!" == "3" ( del /q "%layout%*"
) else if "!input!" == "0" exit /b 0
goto :main



:main_popup
if not exist "%layout%_main_popup" (
    rem Draw a frame around
    echo.cursor1=58 19
    echo.cursor2=92 32
    echo.color=80
    echo.clear
    rem Fill a box inside
    echo.right=2
    echo.down
    echo.left2=2
    echo.up2
    echo.color=f0
    echo.clear

    echo.right=4
    echo.down=2
    echo.color=fc
    echo.text=Warning^!
    echo.down
    echo.color=f1
    echo.text=It's a popup^!
    echo.down
    echo.text=0  Close

    rem Prepare user input
    echo.down
    rem Don't forget to escape certain characters
    echo.color=fc
    echo.text=$^> 
    rem Set the caret position
    echo.up
    echo.right=3
    echo.caret
)>"%layout%_main_popup"

%cursorMower% "%layout%_main_popup"

set /p input=
if "!input!" == "0" exit /b 0
goto :main_popup