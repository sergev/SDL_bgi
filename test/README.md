# Test Programs

These programs have been adapted from the test programs available in
the Borland C 3.1 Library Reference. They can be compiled with `SDL_bgi`
and also with Turbo C / Borland C++ running in DOSBox.


## Using TC in DOSBox

Assuming that Turbo C / Borland C is installed in `C:\TC`,
you can compile the programs using these commands:

````
rem  Run the following command only once
path=c:\tc\bin;%path%
rem  cd to the directory where 'program.c' is, then:
tcc program.c graphics.lib
````
