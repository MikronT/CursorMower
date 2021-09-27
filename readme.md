# CursorMower

*Render Batch Faster & Smarter*

This C++ based tool intends to provide fast and convenient command line interface building experience

**Version:** 3.0

*Developed for Windows 10*
*Not tested on other versions yet*

---

## Usage

### Layout file syntax

This tool uses layout files to render the screen
The syntax is very simple

#### Set window dimensions

*This parameter can be only one*

```text
dims`{columns}`{lines}
```

Notes

- For the default dimensions current command line window dimensions are used

#### Set window margins

*This parameter can be only one*

```text
margins`{left&right margin}`{top&bottom margin}
```

Notes

- The default margins are 0
- Margins are applied via changing the window size

#### Move the cursor

```text
goto`{x}`{y}
```

Notes

- Coordinates are starting from (0,0) at the top left corner
- Margins are applied to all the coordinates automatically

#### Print the text

Lets you set what should be printed

```text
text`{any text with spaces}
```

### Run

To process the layout file written execute the following

```batch
cursor "file"
```

And you will see the result

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