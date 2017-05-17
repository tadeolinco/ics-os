# ics-os

## Features

### 1. Autocomplete

Press tab to autocomplete the current input.

The input will not be autocompleted if there is no input or the last character is whitespace.

The first input will always autocomplete from all possible commands in the OS. The rest of the inputs will be autocompleted from all the files in the working directory. The chosen command or file to be autocompleted from is chosen alphabetically.

### 2. cal

#### How to run:
Prints the calendar of the current month of the current year:
```shell
cal 
```

Prints the calendar of the specified month of the specified year:
```shell
// cal <month> <year>
cal 5 2017
```

Prints the calendar of the all the months of the specified year:
```shell
// cal <year>
cal 2017
```

Prints all possible options for the command
```
cal -h
```

### 3. date

