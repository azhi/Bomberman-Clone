#include <iostream>

#include "sdl_wrapper.h"
#include "../game_objects/map.h"

SDLWrapper::SDLWrapper(const char *window_title)
{
  init(window_title);
}

SDLWrapper::~SDLWrapper()
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void SDLWrapper::clear()
{
  SDL_RenderClear(renderer);
}

void SDLWrapper::update()
{
  SDL_RenderPresent(renderer);
}

SDL_Texture* SDLWrapper::load_texture(const char *file_path)
{
  SDL_Surface *bmp = SDL_LoadBMP(file_path);
  SDL_SetColorKey(bmp, SDL_TRUE,
                  SDL_MapRGB(bmp->format,
                             TRANSPARENT_COLOR_R,
                             TRANSPARENT_COLOR_G,
                             TRANSPARENT_COLOR_B));
  if (bmp == nullptr) {
    report_error("SDL_LoadBMP");
    return nullptr;
  }

  SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, bmp);
  SDL_FreeSurface(bmp);
  if (tex == nullptr) {
    report_error("SDL_CreateTextureFromSurface");
    return nullptr;
  }
  return tex;
}

void SDLWrapper::draw_texture(SDL_Texture *texture, int x, int y)
{
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;

  SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
  SDL_RenderCopy(renderer, texture, NULL, &dst);
}

void SDLWrapper::free_texture(SDL_Texture *texture)
{
  SDL_DestroyTexture(texture);
}

void SDLWrapper::init(const char *window_title)
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    report_error("SDL_Init");
    exit(1);
  }

  window = SDL_CreateWindow(window_title, 0, 0, MAP_WIDTH * 32, MAP_HEIGHT * 32,
    SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    report_error("SDL_CreateWindow");
    exit(1);
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == nullptr) {
    report_error("SDL_CreateRenderer");
    exit(1);
  }

  clear();
}

void SDLWrapper::report_error(const char *function_name)
{
  std::cerr << function_name << " Error: " << SDL_GetError() << std::endl;
}
