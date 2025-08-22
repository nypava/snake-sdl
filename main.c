#include <stdlib.h>
#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string.h>
#include <stdio.h>

#include "snake.h"
#include "food.h"
#include "obstacle.h"
#include "common.h"

#define TEXT_COLOR (SDL_Color){0, 0, 0}
#define TOTAL_CELLS ((WINDOW_WIDTH/CELL_DIMENSION) * (WINDOW_HEIGHT/CELL_DIMENSION))
#define SPEED_UP_RATE 2
#define BACKGROUND_COLOR WHITE_COLOR
#define DIRECTION_INFO_LENGTH 10
#define SCORE_LENGTH 32

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SnakeContext *snake_context = NULL;
FoodContext *food_context = NULL;
ObstacleContext *obstacle_context = NULL;
TTF_Font *font = NULL;

static int STEP_RATE = 13;
static int SPEED_UP = 0;
static int previous_level_length;
static int last_score = -1;
static char last_direction_info[DIRECTION_INFO_LENGTH];
static SDL_Texture *score_texture = NULL;
static SDL_Texture *direction_info_texture = NULL;
static bool is_direction_reversed;

static void render_score(SDL_Renderer *renderer, TTF_Font *font, const SnakeContext *snake_context){
  int score = snake_context->length - 2;
  
  if (score != last_score){
    last_score = score;
    if (score_texture) SDL_DestroyTexture(score_texture);

    char score_text[SCORE_LENGTH];
    sprintf(score_text, "%d", snake_context->length - 2);

    SDL_Surface *surface = TTF_RenderText_Blended(font, score_text, 0, TEXT_COLOR);
    score_texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_DestroySurface(surface);
  }

  SDL_FRect score_rect; 
  score_rect.x = 10; 
  score_rect.y = 0;

  SDL_GetTextureSize(score_texture, &score_rect.w, &score_rect.h);
  SDL_RenderTexture(renderer, score_texture, NULL, &score_rect);
}

static void render_border(SDL_Renderer *renderer){
  SDL_RenderLine(renderer, 0, SNAKE_SDL_WINDOW_PAD_TOP(0), SNAKE_WINDOW_WIDTH, SNAKE_SDL_WINDOW_PAD_TOP(0) );
}

static void render_direction_info(SDL_Renderer *renderer, bool is_direction_reversed){
  char direction_info[DIRECTION_INFO_LENGTH];

  if (strcmp(direction_info, last_direction_info) != 1){
    if (direction_info_texture) SDL_DestroyTexture(direction_info_texture);

    strncpy(direction_info, is_direction_reversed ? "Reverse" : "Normal", sizeof(direction_info) - 1 );
    direction_info[sizeof(direction_info)-1] = '\0';

    SDL_Surface *surface = TTF_RenderText_Blended(font, direction_info, 0, TEXT_COLOR);
    direction_info_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
  }

  SDL_FRect direction_info_rect; 

  SDL_GetTextureSize(direction_info_texture, &direction_info_rect.w, &direction_info_rect.h);

  direction_info_rect.x = SDL_WINDOW_WIDTH - direction_info_rect.w;
  direction_info_rect.y = 0; 

  SDL_RenderTexture(renderer, direction_info_texture, NULL, &direction_info_rect);
}

static void level_up(int length){
  length -= 2;
  
  if ((length % 10 == 0) && (previous_level_length != length) && (length != 0)){
    SPEED_UP += SPEED_UP_RATE;

    previous_level_length = length;
    is_direction_reversed = is_direction_reversed ? false: true;
  }
};


static void initialize_game(SnakeContext **snake_context, ObstacleContext **obstacle_context, FoodContext **food_context){
  if (*snake_context){
    free_snake_context(*snake_context);
    SDL_free(*food_context);
    SDL_free(*obstacle_context);
  }

  SPEED_UP = 0;
  is_direction_reversed = true;

  strncpy(last_direction_info, "NORMAL", sizeof(last_direction_info) - 1);
  last_direction_info[sizeof(last_direction_info) - 1] = '\0';

  *snake_context = initialize_snake_context();
  *food_context = initialize_food_context();
  *obstacle_context = initialize_obstacle_context();
}

static bool is_food_eaten(const SnakeContext *snake_context, int *eaten_food_index){
  for (int i = 0; i < FOODS_AVAILABLE; i++){
    if (is_snake_collide(snake_context, food_context->foods[i].cell.x, food_context->foods[i].cell.y)){
      *eaten_food_index = i;
      return true;
    }
  }
  return false;
}

static bool is_game_over(const SnakeContext *snake_context){
  for (int i = 1; i < snake_context->length; i++){
    if (is_snake_collide(snake_context, snake_context->snake[i].cell.x, snake_context->snake[i].cell.y)){
      return true;
    }
  }

  for (int i = 0; i < OBSTACLES_AVAILABLE; i++){
    if (is_snake_collide(snake_context, obstacle_context->obstacles[i].cell.x, obstacle_context->obstacles[i].cell.y)){
      return true;
    }
  }
  return false;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv){
  if (!SDL_Init(SDL_INIT_VIDEO))
    SDL_Log("can't init %s", SDL_GetError());

  if (!SDL_CreateWindowAndRenderer("snake game", SDL_WINDOW_WIDTH, SDL_WINDOW_HEIGHT, 0, &window, &renderer))
    SDL_Log("can't create window and renderer %s\n", SDL_GetError());
  
  if (!TTF_Init())
    SDL_Log("error initializing ttf: %s\n", SDL_GetError());

  SDL_Color text_color = { 0, 0, 255 };

  font = TTF_OpenFont("asset/fonts/Symtext.ttf", 24);
  if (!font){
    SDL_Log("can't open the font: %s", SDL_GetError());
  }

  initialize_game(&snake_context, &obstacle_context, &food_context);
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event){
  SnakeDirection snake_direction = snake_context->direction;
  SDL_Keycode key = event->key.key;

  if (event->type == SDL_EVENT_QUIT){
    return SDL_APP_SUCCESS;
  } else if (event->type == SDL_EVENT_KEY_DOWN && snake_context->is_direction_used){
    if (key == SDLK_RIGHT || key == SDLK_D){
      if (snake_direction != DIR_LEFT && snake_direction != DIR_RIGHT) 
        snake_context->direction = is_direction_reversed ? DIR_LEFT : DIR_RIGHT;
    }
    else if(key == SDLK_LEFT || key == SDLK_A){
      if (snake_direction != DIR_RIGHT && snake_direction != DIR_LEFT) 
        snake_context->direction = is_direction_reversed ? DIR_RIGHT : DIR_LEFT;
    }
    else if (key == SDLK_DOWN || key == SDLK_S){
      if (snake_direction != DIR_UP && snake_direction != DIR_DOWN)
        snake_context->direction = is_direction_reversed ? DIR_UP : DIR_DOWN;
    }
    else if (key == SDLK_UP || key == SDLK_W){
      if (snake_direction != DIR_DOWN && snake_direction != DIR_UP)
        snake_context->direction = is_direction_reversed ? DIR_DOWN : DIR_UP; 
    }
    snake_context->is_direction_used = false;
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate){
  const Uint32 now = SDL_GetTicks();
  static Uint32 temp;
  int eaten_food_index = 0;

  SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  if ((now - temp) >= (1000/(STEP_RATE + SPEED_UP))){
    move_snake(snake_context);
    snake_context->is_direction_used = true;

    if (is_food_eaten(snake_context, &eaten_food_index)){
      increase_snake_length(snake_context);
      replace_food(food_context, eaten_food_index);
    }
    if (is_game_over(snake_context)){
      initialize_game(&snake_context, &obstacle_context, &food_context);
    }
     
    level_up(snake_context->length);
    temp = now;
  }

  render_food(renderer, food_context);
  render_snake(renderer, snake_context);
  render_obstacle(renderer, obstacle_context);
  render_score(renderer, font, snake_context);
  render_border(renderer);
  render_direction_info(renderer, is_direction_reversed);
  SDL_RenderPresent(renderer);
  SDL_Delay(1); 

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result){
  free_snake_context(snake_context);
  SDL_free(food_context);
  SDL_free(obstacle_context);
}
