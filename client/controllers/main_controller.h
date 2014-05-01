#ifndef _MAIN_CONTROLLER_H_
#define _MAIN_CONTROLLER_H_

#include "../sdl/sdl_wrapper.h"
#include "../game_objects/map.h"
#include "client.h"

class MainController
{
  public:
    MainController(char* ip_str, bool is_bot);
    virtual ~MainController();

    void main_cycle();

  private:
    void process_sdl_event(SDL_Event &event);
    static Uint32 push_random_sdl_event(Uint32 interval, void* params);
    long parse_str_ip(char* ip_str);

    SDLWrapper *sdl_wrapper;

    GameObjects::Map *map;
    Client *client;

    bool quit = false;
    bool is_bot = false;
    long server_ip = 0;
};

#endif
