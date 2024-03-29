#include <SDL2/SDL.h>
// You do not need to change this file.
// The display module provides graphics for the sketch program.

// A display structure needs to be created by calling newDisplay, and then
// needs to be passed to each sketching function.
struct display {
    SDL_Window *window;
    SDL_Renderer *renderer;
};
typedef struct display display;

// Create a display object representing a plain white window of a given size.
display *newDisplay(char *title, int width, int height);

// Draw a line from (x0,y0) to (x1,y1) which is black by default.
void line(display *d, int x0, int y0, int x1, int y1);

// Change the drawing colour.
void colour(display *d, int rgba);

// Pause for the given number of milliseconds.
void pause(display *d, int ms);

// Clear the display to white.
void clear(display *d);

// Wait for a key press.
char key(display *d);

// Hold the display for a few seconds, then shut down.
void end(display *d);

void fail();
int I(int n);
void *P(void *p);
void show(display *d);
