#ifndef _SDL_WRAPPER_H_
#define _SDL_WRAPPER_H_

#include "SDL2/SDL.h"

#define TRANSPARENT_COLOR_R 192
#define TRANSPARENT_COLOR_G 192
#define TRANSPARENT_COLOR_B 192

class SDLWrapper
{
  public:
    SDLWrapper(const char *window_title);
    virtual ~SDLWrapper();

    void clear();
    void update();

    SDL_Texture *load_texture(const char *file_path);
    void draw_texture(SDL_Texture *texture, int x, int y);
    void free_texture(SDL_Texture *texture);

  private:
    void init(const char *window_title);
    void report_error(const char *function);

    SDL_Window *window;
    SDL_Renderer *renderer;
};

#endif
