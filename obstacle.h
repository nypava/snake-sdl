#ifndef Obstacle_H
#define Obstacle_H
#include <SDL3/SDL.h>
#include "common.h"

#define OBSTACLES_AVAILABLE 5

#define CELL_DIMENSION 20

typedef struct {
  Color color;
  SDL_FRect cell;
} Obstacle;

typedef struct {
  Obstacle obstacles[OBSTACLES_AVAILABLE];
} ObstacleContext;

ObstacleContext *initialize_obstacle_context();
void render_obstacle(SDL_Renderer *renderer, const ObstacleContext *obstacle_context);
#endif
