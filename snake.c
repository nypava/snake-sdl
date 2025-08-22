#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include "snake.h"
#include "common.h"

void free_snake_context(SnakeContext *snake_context) {
  if (snake_context) {
    SDL_free(snake_context->snake);
    SDL_free(snake_context);
  }
}

SnakeContext *initialize_snake_context(void){
  SnakeContext *snake_context = (SnakeContext *)SDL_malloc(sizeof(SnakeContext));
  if (!(snake_context)){
    SDL_Log("fail to malloc snake context %s", SDL_GetError());
    return 0;
  }
  snake_context->snake = NULL;
  snake_context->direction = DIR_RIGHT; 
  snake_context->is_direction_used = false;
  snake_context->length = 0;
  increase_snake_length(snake_context); 
  increase_snake_length(snake_context); 
  return snake_context;
}

void move_snake(SnakeContext *snake_context){
  int len = snake_context->length;
  SnakeDirection direction = snake_context->direction;

  for (int i = len - 1; i > 0; i--){
    snake_context->snake[i].cell.x = snake_context->snake[i-1].cell.x;
    snake_context->snake[i].cell.y = snake_context->snake[i-1].cell.y;
  }

  SDL_FRect *head = &snake_context->snake[0].cell;
 
  switch (direction){
    case DIR_UP:
      if (head->y <= SNAKE_SDL_WINDOW_PAD_TOP(0)) head->y = SNAKE_SDL_WINDOW_PAD_TOP(SNAKE_WINDOW_HEIGHT - CELL_DIMENSION);
      else head->y -= CELL_DIMENSION;
      break;
    case DIR_DOWN:
      if (head->y >= SNAKE_SDL_WINDOW_PAD_TOP(SNAKE_WINDOW_HEIGHT - CELL_DIMENSION)) head->y = SNAKE_SDL_WINDOW_PAD_TOP(0);
      else head->y += CELL_DIMENSION;
      break;
    case DIR_RIGHT:
      if (head->x >= SNAKE_WINDOW_WIDTH - CELL_DIMENSION) head->x = 0;
      else head->x += CELL_DIMENSION;
      break;
    case DIR_LEFT:
      if (head->x <= 0) head->x = SNAKE_WINDOW_WIDTH - CELL_DIMENSION;
      else head->x -= CELL_DIMENSION;
      break;
  }
}

void increase_snake_length(SnakeContext *snake_context){
  SnakePart *tmp = SDL_realloc(snake_context->snake, sizeof(SnakePart) * (snake_context->length + 1));

  if (!tmp){
    SDL_Log("reallocation failed: %s", SDL_GetError());
    return;
  }

  const int len = ++snake_context->length;
  snake_context->snake = tmp;

  Color *new_color = &snake_context->snake[len-1].color;
  SDL_FRect *new_cell = &snake_context->snake[len-1].cell;
  new_cell->w = new_cell->h = CELL_DIMENSION;

  *new_color = BLACK_COLOR;
  if (len == 1){
    new_cell->x = SNAKE_SDL_WINDOW_PAD_TOP((float)SNAKE_WINDOW_WIDTH / 2);
    new_cell->y = SNAKE_SDL_WINDOW_PAD_TOP((float)SNAKE_WINDOW_HEIGHT / 2);
    return;
  }

  SDL_FRect tail = snake_context->snake[len-2].cell;

  *new_cell = tail;

  if (len > 2) {
    SDL_FRect before_tail = snake_context->snake[len-3].cell;

    if (tail.x == before_tail.x) {
      if (tail.y > before_tail.y) new_cell->y += CELL_DIMENSION;
      else new_cell->y -= CELL_DIMENSION;
    } else {
      if (tail.x > before_tail.x) new_cell->x += CELL_DIMENSION;
      else new_cell->x -= CELL_DIMENSION;
    }
  } else {
    switch (snake_context->direction){
      case DIR_UP:    new_cell->y += CELL_DIMENSION; break;
      case DIR_DOWN:  new_cell->y -= CELL_DIMENSION; break;
      case DIR_RIGHT: new_cell->x -= CELL_DIMENSION; break;
      case DIR_LEFT:  new_cell->x += CELL_DIMENSION; break;
    }
  }
}

void render_snake(SDL_Renderer *renderer, const SnakeContext *snake_context){
  Color color;

  color = snake_context->snake[0].color;
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
  SDL_RenderRect(renderer, &snake_context->snake[0].cell);

  for (int i = 1; i < (int)snake_context->length; i++){
    color = snake_context->snake[i].color;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &snake_context->snake[i].cell);
  }
}

bool is_snake_collide(const SnakeContext *snake_context, float x_pos, float y_pos){
  if (snake_context->length == 0) return false;
  const SDL_FRect head = snake_context->snake[0].cell;
  return (head.x == x_pos && head.y == y_pos);
}
