#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

#define WIDTH 900
#define HEIGHT 600
#define COLOR_WHITE 0xFFFFFFFF

struct Circle {
  double x;
  double y;
  double radius;
};

void FillCircle(SDL_Surface* surface, struct Circle circle, Uint32 color) {
  double radius_squared = pow(circle.radius,2);
  for (double x=circle.x-circle.radius; x<=circle.x+circle.radius;x++) {

    for (double y=circle.y-circle.radius; y<=circle.y+circle.radius;y++){
      double distance_squared = pow(x-circle.x,2) + pow(y-circle.y,2);
      if (distance_squared < radius_squared) {
        SDL_RECT pixel = (SDL_Rect){x,y,1,1};
        SDL_FillRect(surface, &pixel, color);
      }
    }
  }
}

int main(){

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("c-ray",
  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT,0);

  SDL_SUrface *surface = SDL_GetWindowSurface(window);
  SDL_Rect rect = (SDL_Rect){200,200,200,200};
  SDL_FillRect(surface, &rect, COLOR_WHITE);

  struct Circle circle = {200,200,80};
  int simulation_running = 1;
  SDL_Event quit;
  while (simulation_running) {
    while (SDL_PollEvent(&quit)) {
      if (quit.type == SDL_QUIT) {
        simulation_running = 0;
      }
    }
    FillCircle(surface,circle,COLOR_WHITE);

    SDL_UpdateWindowSurface(window);
    SDL_Delay(10);
  }





}