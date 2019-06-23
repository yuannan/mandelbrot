#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include "display.h"

struct screen{
    display *d;
    int wpx;
    int hpx;
    long double cx;
    long double cy;
    long double scale;
    int iterations;
    long double movementfactor;
    int colourset;
    int maxcoloursets;
};
typedef struct screen screen;

void freeScreen(screen *s){
    end(s->d);
    free(s->d);
    free(s);
}

screen *newScreen(char *title, int width, int height){
    screen *s = malloc(sizeof(screen));
    s->d = newDisplay(title, width, height);
    s->wpx = width;
    s->hpx = height;
    s->cx = 0;
    s->cy = 0;
    s->scale = 1;
    s->iterations = 100;
    s->movementfactor = 4;
    s->colourset = 0;
    s->maxcoloursets = 5;
    
    return s;
}

void complexprint(long double a, long double b){
    printf("%Lf + %Lfi\n", a, b);
}

long double squish(long double in,
              long double inmin, long double inmax,
              long double outmin, long double outmax){
    
    long double floor = in - inmin;
    long double inrange = inmax - inmin;
    long double outrange = outmax - outmin;
    long double out = ((floor * outrange) / inrange) + outmin;

    return out;
}

long int getIterations(long double x, long double y, int maxi){
    long double xi = x;
    long double yi = y;
    
    int i;
    for(i = 0; i < maxi && (fabsl(xi) + fabsl(yi) < 5); i++){
        long double z2x = (xi*xi - yi*yi);
        long double z2y = (2*xi*yi);
        xi = z2x + x;
        yi = z2y + y;
    }
    /* uncomment for more accurate look
    if(i > (maxi-10) || i < 10){
        i = 0;
    }
    */

    return i;
}

int packRGBA(int r, int g, int b, int a){
    int colour = 0;
    colour = colour | (r & 0xFF);
    colour = (colour << 8) | (g & 0xFF);
    colour = (colour << 8) | (b & 0xFF);
    colour = (colour << 8) | (a & 0xFF);

    return colour;
}

int generatePackedRGBA(screen *s, int p){
    int r;
    int g;
    int b;
    int a;
    switch(s->colourset){
        case 0:
            r = squish(p, 0, s->iterations, 100, 255);
            g = squish(p, 0, s->iterations, 0, 127);
            b = squish(sqrt(p), 0, 14, 0, 255);
            a = 255;
            break;
        case 1:
            r = squish(p*p, 0, s->iterations*s->iterations, 5, 70);
            g = squish(p, 0, s->iterations, 0, 255);
            b = squish(p, 0, s->iterations, 0, 150);
            a = 255;
            break;
        case 2:
            r = squish(p, 0, s->iterations, 0, 70);
            g = squish(p*p, 0, s->iterations*s->iterations, 10, 70);
            b = squish(p*p, 0, s->iterations*s->iterations, 70, 255);
            a = 255;
            break;
        case 3:
            r = squish(p, 0, s->iterations, 0, 255);
            g = squish(p*p, 0, s->iterations*s->iterations, 50, 100);
            b = squish(p*p, 0, s->iterations*s->iterations, 50, 255);
            a = 255;
            break;
        case 4:
            r = squish(p*p, 0, s->iterations*s->iterations, 50, 100);
            g = squish(p, 0, s->iterations, 0, 70);
            b = squish(p*p, 0, s->iterations*s->iterations, 50, 255);
            a = 255;
            break;
        default:
            r = 5;
            g = 55;
            b = 255;
            a = 255;
            break;
    }

    int packedColour = packRGBA(r, g, b, a);
    return packedColour;
}

void mandelbrotDrawPixel(screen *s, int xp, int yp){
    long double xcord = squish(xp, 0, s->wpx, s->cx - s->scale, s->cx + s->scale);
    long double ycord = squish(yp, 0, s->hpx, s->cy - s->scale, s->cy + s->scale);
    long int i = getIterations(xcord, ycord, s->iterations);
    int mancolour = generatePackedRGBA(s, i);
    colour(s->d, mancolour);
    SDL_RenderDrawPoint(s->d->renderer, xp, yp);
}

void generateAndFillPixels(screen *s){
    for(int xp = 0; xp < s->wpx; xp++){
        for(int yp = 0; yp < s->hpx; yp++){
            mandelbrotDrawPixel(s, xp, yp);
        }
    }
    show(s->d);
}

void saveCurrentImage(screen *s){
        char *filepath = malloc(100);
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        sprintf(filepath, "fractal %d-%d-%d %d:%d:%d.bmp",
                tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        SDL_SaveBMP(P(SDL_GetWindowSurface(s->d->window)), filepath);
        free(filepath);
}

// start drawing the mandelbrot set
void mandelbrot(){
    int width = 1000;
    int height = 1000;
    screen *ms = newScreen("Mandelbrot", width, height);
    bool running = true;
    while(running){
        generateAndFillPixels(ms);
        char k = key(ms->d);
        switch(k){
            case 'x':
                running = false;
                break;
            // movement controls
            case 'w':
                ms->cy -= (ms->scale / ms->movementfactor);
                break;
            case 'a':
                ms->cx -= (ms->scale / ms->movementfactor);
                break;
            case 's':
                ms->cy += (ms->scale / ms->movementfactor);
                break;
            case 'd':
                ms->cx += (ms->scale / ms->movementfactor);
                break;
            // zoom
            case 'q':
                ms->scale *= 1.5;
                ms->iterations /= 1.05;
                break;
            case 'e':
                ms->scale /= 1.5;
                ms->iterations *= 1.05;
                break;
            case 'r':
                ms->cx = 0;
                ms->cy = 0;
                ms->scale = 1;
                break;
            // saves images to file
            case 'f':
                saveCurrentImage(ms);
                break;
            // change colours
            case 'c':
                ms->colourset = (ms->colourset + 1) % ms->maxcoloursets;
                break;
            default:
                printf("key: %c\n", k);
                break;
        }
    }

    freeScreen(ms);
}

#ifdef test

void testGetIterations(){
    assert(getIterations(3, 3, 200) == 0);
    assert(getIterations(0, 0, 356) == 356);
}

void testNewScreen(){
    screen *ts = newScreen("test screen", 400,400);

    freeScreen(ts);
}

void testPrintf(){
    printf("testing printf\n");
}

void testDraw(){
    display *d = newDisplay("testing draw display", 500, 500);
    line(d, 0, 0, 500, 500);

    end(d);
}

void testSquish(){
    assert(squish(0, 0, 1, 0, 1) == 0);
    assert(squish(1, 0, 2, 0, 10) == 5);
    assert(squish(100, 0, 400, -200, 200) == -100);
    assert(squish(50, 0, 400, -2, 2) == -1.5);
}

void testPackRGBA(){
    assert(packRGBA(0, 0, 0, 0) == 0);
    assert(packRGBA(1, 1, 1, 1) == 0x01010101);
}

void testColours(){
    testPackRGBA();
}

void testAll(){
    testPrintf();
    testNewScreen();
    testDraw();
    testSquish();
    testGetIterations();
    testColours();
}

int main(int n, char *args[n]){
    testAll();
    return 0;
}
#endif

#ifndef test
int main(int n, char *args[n]){
    printf("Lets get infinite!\n");
    mandelbrot();

    return 0;
}
#endif
