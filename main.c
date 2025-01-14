#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

#define WIDTH 1600
#define HEIGHT 900
#define COLOR_WHITE 0xFFFFFFFF
#define COLOR_BLACK 0x00000000
#define RAYS_NUMBER 360
#define COLOR_GRAY 0x80808080
#define COLOR_RAY 0xffd43b
#define RAY_THICKNESS 5

struct Circle {
  double x;
  double y;
  double radius;
};

struct Ray {
  double x_start,y_start;
  double angle;
};

void FillCircle(SDL_Surface* surface, struct Circle circle,Uint32 color) {
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

//OBJECT çoğaltılabilir
void FillRays(SDL_Surface* surface, struct Ray rays[RAYS_NUMBER],Uint32 color_ray,struct Circle object) {

  double radius_squared = pow(object.radius,2);
  for (int i=0;i<RAYS_NUMBER;i++) {
    struct Ray ray = rays[i];

    int end_of_screen = 0;
    int object_hit = 0;

    double step=1;
    double x_draw = ray.x_start;
    double y_draw = ray.y_start;
    while (!end_of_screen && !object_hit) {
       x_draw += step*cos(ray.angle);
       y_draw += step*sin(ray.angle);

      SDL_RECT ray_point = (SDL_Rect){x_draw,y_draw,RAY_THICKNESS,RAY_THICKNESS};
      SDL_FillRect(surface, &ray_point,color_ray);

      if (x_draw<0 || x_draw>WIDTH || y_draw<0 || y_draw>HEIGHT) {
        end_of_screen = 1;

        // if the ray hit an object
        double distance_squared = pow(x_draw-object.x,2) + pow(y_draw-object.y,2);
        if (distance_squared < radius_squared) {
          break;
        }
      }
    }
  }
}

void generate_rays(struct Circle circle,struct Ray rays[RAYS_NUMBER]) {

  for (int i=0;i<RAYS_NUMBER;i++) {
    double angle = 2*M_PI*i/RAYS_NUMBER;
    struct Ray ray = {circle.x,circle.y,
      angle};
    rays[i] = ray;
    printf("angle: %f\n",angle);
  }
}

int main(){

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("c-ray",
  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT,0);

  SDL_SUrface *surface = SDL_GetWindowSurface(window);
  SDL_Rect rect = (SDL_Rect){200,200,200,200};
  SDL_FillRect(surface, &rect, COLOR_WHITE);


  struct Circle circle = {200,200,40};
  struct Circle shadow_circle = {550,300,140};
  SDL_Rect erase_rect = {0,0,WIDTH,HEIGHT};

  struct Ray rays[RAYS_NUMBER];
  generate_rays(circle,rays);

  double obstacle_speed_y = 1;
  int simulation_running = 1;
  SDL_Event event;
  while (simulation_running && SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        simulation_running = 0;
      }
      if (event.type == SDL_MOUSEMOTION && event.motion.state !=0) {
        circle.x = event.motion.x;
        circle.y = event.motion.y;
        generate_rays(circle,rays);
    }
    SDL_FillRect(surface,&erase_rect,COLOR_BLACK);
    FillCircle(surface,circle,COLOR_RAY);

    FillRays(surface,rays,COLOR_RAY,shadow_circle);
    FillCircle(surface,shadow_circle,COLOR_WHITE);



    shadow_circle.y += obstacle_speed_y;
    if (shadow_circle.y-shadow_circle.radius<0 || shadow_circle.y+shadow_circle.radius>HEIGHT) {
      obstacle_speed_y *= -1;
    }
    SDL_UpdateWindowSurface(window);

    SDL_Delay(10);
  }





}