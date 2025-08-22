#include <SDL3/SDL.h>
#include "food.h"
#include "common.h"

static void generate_food(Food *food){
  Sint32 x_axis = SDL_rand(SNAKE_WINDOW_WIDTH);
  x_axis -= (x_axis % CELL_DIMENSION);

  Sint32 y_axis = SDL_rand(SNAKE_WINDOW_HEIGHT);
  y_axis -= (y_axis % CELL_DIMENSION);
  y_axis = SNAKE_SDL_WINDOW_PAD_TOP(y_axis);

  food->color = BLACK_COLOR;
  
  SDL_FRect cell = {.x = x_axis, .y = y_axis, .w = CELL_DIMENSION, .h = CELL_DIMENSION};
  food->cell = cell;
}

FoodContext *initialize_food_context(void){
  FoodContext *tmp = SDL_calloc(1, sizeof(FoodContext));
  FoodContext *food_context = NULL;
  
  if (!tmp){
    SDL_Log("can't allocate food_context %s %s", __func__, SDL_GetError());
    return 0; 
  }

  food_context = tmp;

  for (int i = 0; i < FOODS_AVAILABLE; i++){
    generate_food(&food_context->foods[i]);
  }

  return food_context;
}

void replace_food(FoodContext *food_context, int index){
  generate_food(&food_context->foods[index]);
}

void render_food(SDL_Renderer *renderer, const FoodContext *food_context){
   for (int i = 0; i < FOODS_AVAILABLE; i++){
    Color color = food_context->foods[i].color;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    SDL_RenderRect(renderer, &food_context->foods[i].cell);
  }
}
