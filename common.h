#ifndef COMMMON_H
#define COMMMON_H
#include <SDL3/SDL.h>

#define CELL_DIMENSION 20
#define SDL_WINDOW_WIDTH  (32 * (CELL_DIMENSION))
#define SDL_WINDOW_HEIGHT  (24 * (CELL_DIMENSION))
#define SNAKE_WINDOW_WIDTH  (32 * (CELL_DIMENSION))
#define SNAKE_WINDOW_HEIGHT (22 * (CELL_DIMENSION))

#define SNAKE_SDL_WINDOW_PAD_TOP(HEIGHT) ((SDL_WINDOW_HEIGHT - SNAKE_WINDOW_HEIGHT) + (HEIGHT))

#define BLACK_COLOR (Color){0, 0, 0} 
#define WHITE_COLOR (Color){0xFF, 0xFF, 0xFF} 

typedef struct {
  Uint8 r;
  Uint8 g;
  Uint8 b;
} Color;

#endif

