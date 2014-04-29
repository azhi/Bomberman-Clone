#ifndef _MAIN_CONTROLLER_H_
#define _MAIN_CONTROLLER_H_

#include "../sdl/sdl_wrapper.h"
#include "../game_objects/map.h"
#include "client.h"

class MainController
{
  public:
    MainController();
    virtual ~MainController();

    void main_cycle();

  private:
    void process_sdl_event(SDL_Event &event);

    SDLWrapper *sdl_wrapper;

    GameObjects::Map *map;
    Client *client;

    bool quit = false;
};

#endif
