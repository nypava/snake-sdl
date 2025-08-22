#ifndef SNAKE_H
#include <SDL3/SDL.h>
#include "common.h"

#define SNAKE_H

typedef enum { DIR_UP, DIR_DOWN, DIR_RIGHT, DIR_LEFT } SnakeDirection;

typedef struct {
  Color color;
  Color border_color;
  SDL_FRect cell;
} SnakePart;

typedef struct {
  SnakeDirection direction;
  bool is_direction_used;
  unsigned int length;
  SnakePart *snake;
} SnakeContext;

SnakeContext *initialize_snake_context(void);
void reset_snake_context(SnakeContext *snake_context);
void move_snake(SnakeContext *snake_context);
void increase_snake_length(SnakeContext *snake_context);
void render_snake(SDL_Renderer *renderer, const SnakeContext *snake_context);
bool is_snake_collide(const SnakeContext *snake_context, float x_pos, float y_pos);
void free_snake_context(SnakeContext *snake_context);
#endif
