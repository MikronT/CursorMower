# CursorMower

*Render Batch Faster & Smarter*

This C++ based tool intends to provide fast and convenient command line interface building experience

**Version:** Alpha 4.0

*Developed for Windows 10*
*Not tested on other versions yet*

---

## Usage

### Layout file syntax

This tool uses layout files to render the screen
The syntax is very simple

Notes

- Coordinates are starting from (1,1) at the top left corner and go to the window dimensions
- All the parameters are optional (exceptions below)

#### Set window dimensions

Should be placed at the beginning of the layout file

```batch
screen_width={columns}
screen_height={lines}
```

Example

```batch
screen_width=120
screen_height=40
```

Notes

- Current command line window dimensions are used as default dimensions
- Each of them can be the one only for the whole layout file (otherwise previous parameter is omitted)
- You can change only one of them if you need

#### Set window margins

```batch
screen_margin={lines}
```

Example

The window width and height will be automatically extended by the margin but all the coordinates will work in the coordinate system of the old window: (1;1) will point to (3;3) automatically

```batch
screen_width=120
screen_height=40
screen_margin=1
```

Notes

- Margins are applied to all the coordinates automatically
- Margins from left and right sides are applied automatically by multiplying the value specified by 2 (because 1 line = 2 columns) to make an accurate box
- There are no default margins
- Can be the one only for the whole layout file (otherwise previous parameter is omitted)
- Margins are applied via extending the window size

#### Move the cursor

Cursor `to` is the main cursor. All the inline elements are printed at these coordinates

Cursor `from` is required to draw block elements

```batch
cursor_from={x} {y}
cursor_to={x} {y}
```

Examples below

You can also move the cursor relatively not to calculate the coordinates every time

```batch
cursor_up={lines}
cursor_down={lines}
cursor_left={columns}
cursor_right={columns}
```

#### Clear screen

```batch
clear
```

Is used to clear a specific area

```batch
cursor_from=40 15
cursor_to=60 27
clear
```

The following parameter clears the whole screen

```batch
clear_screen
```

#### Print the text

Lets you set what should be printed

```batch
cursor_to={x} {y}
text={any text with spaces, Unicode characters, etc.}
```

If you want to write text line by line, you can use the following syntax

```batch
cursor_to=5 5
text=Line 1
text=This line is below
skip
text=An empty line was printed above
```

`skip` parameter lets you skip 1 line not to move the cursor by hand (a shorter form of `cursor_down=1`)

### Run

To process the layout file written execute the following

```batch
cursor "file"
```

And you will see the result

### Example

You can write the file by yourself but if you want more access to the layout (to make it more dynamic), you can generate the one on the go. All the Batch features are available inside the brackets

```batch
set program_name=CursorMower
set program_version=Alpha v4.0

(
  echo.screen_width=120
  echo.screen_height=40
  echo.screen_margin=1

  echo.cursor_to=49 17
  echo.text=%program_name% %program_version%

  echo.cursor_to=54 19
  echo.text=1  Check debug build
  echo.text=2  Check release build
  echo.skip
  echo.text=0  Exit

  rem Move the cursor for user input
  echo.cursor_to=52 24
)>"layout.tmp"

cursor "layout.tmp"

set /p input="> "
```

---

## Returns

| Error level | Explanation                                                                |
|:-----------:| -------------------------------------------------------------------------- |
|      0      | All is OK                                                                  |
|      1      | Not enough/too many arguments (no file specified)                          |
|      2      | Error reading a file (file not found or is inaccessible)                   |
|      3      | Illegal line syntax (the syntax is wrong, check `cursor /help`)            |
|      4      | Out of screen buffer bounds (text or coordinates exceed window dimensions) |
|      5      | Help message is shown                                                      |