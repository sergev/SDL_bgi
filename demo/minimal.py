#  run with: python3 minimal.py

from sdl_bgi import *

gd, gm = DETECT, 0
initgraph (gd, gm, "")
setbkcolor (BLACK)
cleardevice ()
outtextxy (0, 0, "Drawing 1000 lines...")
for i in range (1000):
    setcolor (1 + random (15))
    line (random(getmaxx()), random(getmaxy()),
          random (getmaxx()), random(getmaxy()))
getch ()
closegraph ()
