# COG - A simple build system

**UNDER CONSTRUCTION**: This library is in work in progress. Expect destructive changes.

cog is a simple program that reads `cog.ini` from the root directory.

# Example
The following `cog.ini`
``` ini
[general]
language=c++
compailer=g++
flags=-Wall
output_file=main
output_folder=out
folders=./src
```
produces the following command.
``` sh
g++ -Wall  -o out/main ./src/args.cpp ./src/cmd.cpp ./src/main.cpp ./src/cog.cpp
```

