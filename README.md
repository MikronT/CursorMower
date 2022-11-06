# CursorMower

*Render CLI Faster*

A C++ based tool that intends to provide fast and convenient command line interface building experience

Version: **0.5.4**

- *Developed for Windows 10*
- *Not tested on any other versions yet*

---

## Usage

Detailed explanation of the program usage with examples

### Layout file syntax

This tool uses layout files to render the screen. The syntax is very simple

Notes

- Coordinates are starting from (1,1) at the top left corner and go to the window dimensions
- All the parameters are optional (exceptions below)

#### Set window dimensions

Should be placed at the beginning of the layout file

```ini
console_width={columns}
console_height={lines}
```

Example

```ini
console_width=120
console_height=40
```

Notes

- Current command line window dimensions are used as default dimensions
- Each of them can be the one only for the whole layout file (otherwise previous parameter is omitted)
- You can change only one of them if you need

#### Set window margins

```ini
console_margin={lines}
```

Example

The window width and height will be automatically extended by the margin but all the coordinates will work in the coordinate system of the old window: (1;1) will point to (3;3) automatically

```ini
console_width=120
console_height=40
console_margin=1
```

Notes

- Margins are applied to all the coordinates automatically
- Margins from left and right sides are applied automatically by multiplying the value specified by 2 (because 1 line = 2 columns) to make an accurate box
- There are no default margins
- Can be the one only for the whole layout file (otherwise previous parameter is omitted)
- Margins are applied via extending the window size

#### Move the cursor

```ini
cursor1={x} {y}
cursor2={x} {y}
```

`cursor1` is the main cursor. All the inline elements are printed at these coordinates

`cursor2` is required to draw block elements

Examples below

You can also move every cursor relatively not to calculate the coordinates by hand

```ini
cursor1_up={lines}
cursor1_down={lines}
cursor1_left={columns}
cursor1_right={columns}

cursor2_up={lines}
cursor2_down={lines}
cursor2_left={columns}
cursor2_right={columns}
```

There is a shorter syntax if you want

```ini
; Move cursor1
up={lines}
down={lines}
left={columns}
right={columns}

; Move cursor2
up2={lines}
down2={lines}
left2={columns}
right2={columns}
```

To move a cursor by 1 step you can omit specifying values

```ini
; Move cursor1
up
down
left
right

; Move cursor2
up2
down2
left2
right2
```

Example

```ini
; These short commands
up
up
left

; Are equivalent to the following long ones
cursor1_up=2
cursor1_left=1
```

#### Clear screen

The following parameter clears the whole screen

```ini
clear=screen
```

Can be used without parameters to clear a specific area

```ini
cursor1=40 15
cursor2=60 27
clear
```

#### Set colors

Uses the same syntax as the default `color` command does to change some color

```ini
color={0-f}{0-f}
```

Here's #CommandPrompt default color reference

| Code | Color  | Code | Color        |
|:----:| ------ |:----:| ------------ |
|  0   | Black  |  8   | Gray         |
|  1   | Blue   |  9   | Light Blue   |
|  2   | Green  |  A   | Light Green  |
|  3   | Aqua   |  B   | Light Aqua   |
|  4   | Red    |  C   | Light Red    |
|  5   | Purple |  D   | Light Purple |
|  6   | Yellow |  E   | Light Yellow |
|  7   | White  |  F   | Bright White |

The only difference is that you change color attributes only for the text you print instead of the whole window

```ini
color=0b
text=Hello

cursor1_right=1
color
text=is written with a different color
```

You can also combine this command with `clear` to draw empty rectangular areas

```ini
cursor1=40 15
cursor2=80 27
color=70
clear
```

To extend coloring variety of #CommandPrompt you can remap any of 16 predefined colors with the following command. The general syntax is below

```ini
console_color={0-f}   {000000-ffffff}
console_color={color} {000000-ffffff}
```

The first parameter is a built-in color code and the second is the color you want to remap to. Here's an example to better understand it

```ini
; Remap black to red
console_color=0 ff0000

; Remap white to gray
console_color=f aaaaaa
```

And more advanced example (remap all the colors): set [Aurora](https://github.com/mbadolato/iTerm2-Color-Schemes#aurora) theme

```ini
console_color=0 23262e
console_color=1 0321d7
console_color=2 8fd46d
console_color=3 03d6b8
console_color=4 f0266f
console_color=5 ee5d43
console_color=6 ffe66d
console_color=7 ffca28
console_color=8 292e38
console_color=9 03d6b8
console_color=a 8fd46d
console_color=b 03d6b8
console_color=c f92672
console_color=d ee5d43
console_color=e ffe66d
console_color=f c74ded
```

Or the same example using color names instead

```ini
console_color=black         23262e
console_color=blue          0321d7
console_color=green         8fd46d
console_color=cyan          03d6b8
console_color=red           f0266f
console_color=purple        ee5d43
console_color=yellow        ffe66d
console_color=white         ffca28
console_color=bright-black  292e38
console_color=bright-blue   03d6b8
console_color=bright-green  8fd46d
console_color=bright-cyan   03d6b8
console_color=bright-red    f92672
console_color=bright-purple ee5d43
console_color=bright-yellow ffe66d
console_color=bright-white  c74ded
```

#### Print the text

Lets you set what should be printed

```ini
cursor1={x} {y}
text={any text with spaces, Unicode characters, etc.}
```

If you want to write text line by line, you can use the following syntax

```ini
cursor1=5 5
text=Line 1
text=This line is below
down
text=A blank line was printed above
```

`down` lets you skip 1 line not to move the cursor by hand (a shorter form of `cursor1_down=1`)

### Run

To process the layout file written execute the following

```batch
cursorMower "file"
```

And you will see the result

### Example

You can write the file by yourself but if you want more access to the layout (to make it more dynamic), you can generate the one on the go. All the Batch features are available inside the brackets

```batch
set program_name=CursorMower

(
    echo.console_width=120
    echo.console_height=40
    echo.console_margin=1

    echo.color=0b
    echo.cursor1=49 17
    echo.text=%program_name%
    echo.cursor1=54 19
    echo.text=1  Check debug build
    echo.text=2  Check release build
    echo.down
    echo.text=0  Exit

    rem Move the cursor for user input
    echo.cursor1=52 24
    echo.text=^> 
)>"layout.tmp"

cursorMower "layout.tmp"

set /p input=
```

---

## Error levels

| Error level | Explanation                                                                 |
|:-----------:| --------------------------------------------------------------------------- |
|      0      | Everything is OK                                                            |
|      1      | Not enough/too many arguments (no file specified)                           |
|      2      | Error reading a file (file not found or not accessible)                     |
|      3      | Illegal line syntax (check docs or use `/help`)                             |
|      4      | Out of screen buffer bounds (text or coords exceed window frame dimensions) |
|      5      | Help message is shown                                                       |