# controls
wasd    move
e       zoom in
q       zoom out
r       reset zoom and position
f       save a screenshot file to current directory
c       cycle colour
x       exit

# Fractal
I want to make a fractal viewing program.

## Goals:
    6- view mandelbrot fractal
    5- zoom
    5- move
    4- export as images
    3- menu
    2- changing colours
    7- julia set

Review:
After reading into the mandelbrot set I had a basic idea of what to do. The wikipedia description was excellent.

I started with a window borrowed from sketch. I thought I would make a structure just incase I needed to carry addition
information. Unlike sketch where I had a display as well as a state structure that had to be passed together.

After learning how to draw a dot on the screen. From there it was a double loop for every pixel in the window.
The cordinates for the loop had to be translated from screen cords (starting from 0,0) to one that reflected the cartisian
plane the set was about to be rendered upon. A youtuber had a similar problem so I created a <squish> function which
translated the number along with it's min and max while retaining preportion.

Programming the iterations involved some algebra on paper before implementing it. I had a problem
where I set the next iterations x prematurly. This x was then used in the calculation of the y where it should
still been the current x. This was solved after a lot of debugging and paper calculations.

The colours that are generated are based on the iterations ran and the max iterations. It can be easily
swapped in code or by press c.

I really wished for a menu system as well as some juicy julia on mandelbrot dual window action.
Next time if possible I would like to use my GPU for the bulk of the work as the program is quite slow
(unless it already was and my computer is just slow), in doing so understand more of SDL.
