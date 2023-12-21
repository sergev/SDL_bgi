# Speedtest

The following is a rough indicator of program speed using `fern`,
compiled to native code, Python, and Wasm.

msys2:~$ time ./fern.exe

real    0m6.796s
user    0m0.000s
sys     0m0.000s

msys2:~$ time ./fern.py

real    4m32.474s
user    0m0.000s
sys     0m0.015s

msys2:~$ start fern.html
timed by hand: 25 seconds

In general, native code is approx. 3 times faster than Wasm,
and much, much faster than Python.
