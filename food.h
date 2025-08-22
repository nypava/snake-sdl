#ifndef FOOD_H
#define FOOD_H
#include <SDL3/SDL.h>
#include "common.h"

#define FOODS_AVAILABLE 4

typedef struct {
  Color color;
  SDL_FRect cell;
} Food;

typedef struct {
  Food foods[FOODS_AVAILABLE];
} FoodContext;

FoodContext *initialize_food_context();
void replace_food(FoodContext *food_context, int index);
void render_food(SDL_Renderer *renderer, const FoodContext *food_context);
#endif
