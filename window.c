#include "window.h"
#include <X11/Xlib.h>
#include <stdlib.h>

static Display* display;
static Window window;
static XEvent event;
static int screen, width, height;

// stores pixel data
static vec3_t** pixels;

// converts vec3 value to rgb color
static unsigned long vec3_to_rgb(vec3_t v)
{
    // gama correct and scale the color
    v = vec3_scale(vec3(sqrt(v.r < 1 ? v.r : 1),
                        sqrt(v.g < 1 ? v.g : 1),
                        sqrt(v.b < 1 ? v.b : 1)), 255);
    return ((uint8_t)v.r << 16) |
           ((uint8_t)v.g <<  8) |
           ((uint8_t)v.b <<  0);
}

void win_update()
{
    XNextEvent(display, &event);
    if (event.type == Expose)
        for (size_t y = 0; y < height; y++)
            for (size_t x = 0; x < width; x++) {
                XSetForeground(display, DefaultGC(display, screen), vec3_to_rgb(pixels[y][x]));
                XDrawPoint(display, window, DefaultGC(display, screen), x, y);
            }
}

void win_putpixel(int x, int y, vec3_t clr) { pixels[y][x] = clr; }

void win_init(int w, int h)
{
    width = w;
    height = h;

    // allocate memory for pixel data
    pixels = (vec3_t**)malloc(sizeof(vec3_t*) * height);
    for (size_t y = 0; y < height; y++)
        pixels[y] = (vec3_t*)malloc(sizeof(vec3_t) * width);

    // open the display
    display = XOpenDisplay(NULL);
    if (!display)
        exit(-1);

    // get default screen and create window
    screen = DefaultScreen(display);
    window = XCreateSimpleWindow(display, RootWindow(display, screen),
                                 100, 100, w, h, 1,
                                 BlackPixel(display, screen),
                                 WhitePixel(display, screen));
    XStoreName(display, window, "Raytracing Demo");

    // select the inputs we want to recieve
    XSelectInput(display, window, KeyPressMask | ExposureMask);

    // show the window on screen
    XMapWindow(display, window);
}
