#include "main_controller.h"
#include "../game_objects/character.h"
#include "../game_objects/bomb.h"
#include "../sdl/texture_cache.h"
#include "../../shared/debug.h"

#include <string>
#include <iostream>
#include <cstdlib>

MainController::MainController(char* ip_str, bool is_bot) : is_bot(is_bot)
{
  sdl_wrapper = new SDLWrapper("Bomberman");
  TextureCache::init(sdl_wrapper);
  server_ip = parse_str_ip(ip_str);
  if (is_bot)
    SDL_AddTimer(CHARACTER_MOVE_ANIMATE_TIME + 50, push_random_sdl_event, NULL);
}

MainController::~MainController()
{
  delete map;
  delete client;
  delete sdl_wrapper;
}

void MainController::main_cycle()
{
  map = new GameObjects::Map(sdl_wrapper);
  client = new Client(server_ip, 5555, map, sdl_wrapper);
  client->do_register();

  SDL_Event event;
  while(!quit) {
    sdl_wrapper->clear();
    map->draw();
    sdl_wrapper->update();

    client->read_and_process();
    while(SDL_PollEvent(&event))
    {
      SDL_PumpEvents();
      process_sdl_event(event);
    }
  }
}

void MainController::process_sdl_event(SDL_Event& event)
{
  switch(event.type)
  {
    case SDL_QUIT:
      quit = true;
      break;

    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE)
      {
        quit = true;
      }
      else if (event.key.keysym.sym == SDLK_LEFT)
      {
        if (map->get_main_character()->can_move(CHARACTER_MOVE_LEFT))
          client->send_character_move(CHARACTER_MOVE_LEFT);
      }
      else if (event.key.keysym.sym == SDLK_RIGHT)
      {
        if (map->get_main_character()->can_move(CHARACTER_MOVE_RIGHT))
          client->send_character_move(CHARACTER_MOVE_RIGHT);
      }
      else if (event.key.keysym.sym == SDLK_UP)
      {
        if (map->get_main_character()->can_move(CHARACTER_MOVE_UP))
          client->send_character_move(CHARACTER_MOVE_UP);
      }
      else if (event.key.keysym.sym == SDLK_DOWN)
      {
        map->get_main_character()->get_x();
        if (map->get_main_character()->can_move(CHARACTER_MOVE_DOWN))
          client->send_character_move(CHARACTER_MOVE_DOWN);
      }
      else if (event.key.keysym.sym == SDLK_LCTRL)
      {
        int timer = map->get_main_character()->timer_callbacks_triggered;
        bool previous_location = timer != 0 && timer < CHARACTER_MOVE_ANIMATE_TIMER_COUNT / 2;
        client->send_place_bomb(previous_location);
      }
      break;
    case SDL_USEREVENT:
      if (event.user.code == SDL_USER_REMOVE_BOMB)
      {
        unsigned char *bomb_id = (unsigned char*) event.user.data1;
        map->remove_bomb(*bomb_id);
        delete bomb_id;
      }
      break;
  }
}

Uint32 MainController::push_random_sdl_event(Uint32 interval, void* params)
{
  if (rand() % 10 < 9)
  {
    SDL_Event event;
    event.type = SDL_KEYDOWN;
    event.key.type = SDL_KEYDOWN;
    event.key.state = SDL_PRESSED;
    int rand1 = rand() % 4;
    if (rand1 == 0)
      event.key.keysym.sym = SDLK_DOWN;
    else if (rand1 == 1)
      event.key.keysym.sym = SDLK_UP;
    else if (rand1 == 2)
      event.key.keysym.sym = SDLK_LEFT;
    else if (rand1 == 3)
      event.key.keysym.sym = SDLK_RIGHT;
    SDL_PushEvent(&event);

    if (rand() % 100 < 5)
    {
      SDL_Event event;
      event.type = SDL_KEYDOWN;
      event.key.type = SDL_KEYDOWN;
      event.key.state = SDL_PRESSED;
      event.key.keysym.sym = SDLK_LCTRL;
      SDL_PushEvent(&event);
    }
  }
  return interval;
}

long MainController::parse_str_ip(char* ip_str)
{
  int current_ip_str_pos = 0;
  long res = 0;
  for(int i = 3; i > -1; i--)
  {
    int current_buf_pos = 0;
    char buf[4];
    while (current_ip_str_pos < strlen(ip_str) && ip_str[current_ip_str_pos] != '.')
      buf[current_buf_pos++] = ip_str[current_ip_str_pos++];
    buf[current_buf_pos] = '\0';
    current_ip_str_pos++;
    int byte = atoi(buf);
    res += byte << (i * 8);
  }
  D_VAR(res);
  return res;
}

