#include <SDL3/SDL.h>
#include "common.h"
#include "obstacle.h"

static void generate_obstacle(Obstacle *obstacle){

  Sint32 x_axis = SDL_rand(SNAKE_WINDOW_WIDTH);
  x_axis -= (x_axis % CELL_DIMENSION);

  Sint32 y_axis = SDL_rand(SNAKE_WINDOW_HEIGHT);
  y_axis -= (y_axis % CELL_DIMENSION);
  y_axis = SNAKE_SDL_WINDOW_PAD_TOP(y_axis);

  obstacle->color = BLACK_COLOR;

  SDL_FRect cell = {.x = x_axis, .y = y_axis, .w = CELL_DIMENSION, .h = CELL_DIMENSION};
  obstacle->cell = cell;
}

ObstacleContext *initialize_obstacle_context(void){
  ObstacleContext *tmp = SDL_calloc(1, sizeof(ObstacleContext));
  ObstacleContext *obstacle_context = NULL;
  
  if (!tmp){
    SDL_Log("can't allocate obstacle_context %s", SDL_GetError());
    return 0; 
  }

  obstacle_context = tmp;

  for (int i = 0; i < OBSTACLES_AVAILABLE; i++){
    generate_obstacle(&obstacle_context->obstacles[i]);
  }

  return obstacle_context;
}

void render_obstacle(SDL_Renderer *renderer, const ObstacleContext *obstacle_context){
   for (int i = 0; i < OBSTACLES_AVAILABLE; i++){
    Color color = obstacle_context->obstacles[i].color;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &obstacle_context->obstacles[i].cell);
  }
}
