#include <cmath>

#include "character.h"
#include "../sdl/texture_cache.h"

namespace GameObjects
{
  Character::Character(SDLWrapper *sdl_wrapper, Map* map, char object_id,
                       int initial_x, int initial_y, char last_move,
                       bool killed, bool is_main_character) :
    GameObject(sdl_wrapper, object_id), map(map), field_x(initial_x), field_y(initial_y),
    last_move(last_move), killed(killed), is_main_character(is_main_character),
    timer_callbacks_triggered(0)
  {
    screen_x = TILE_SIZE * initial_x;
    screen_y = TILE_SIZE * initial_y;
  }

  Character::~Character()
  {
  }

  bool Character::can_move(char move_direction)
  {
    if (killed)
      return false;
    if (timer_callbacks_triggered != 0 && timer_callbacks_triggered <= CHARACTER_MOVE_ANIMATE_TIMER_COUNT / 2)
      return false;
    bool res = false;
    short* surroundings = map->get_surroundings(field_x, field_y);
    if (surroundings[move_direction - 1] == GRASS_FIELD ||
        surroundings[move_direction - 1] == CHARACTER)
    {
      res = true;
    }

    last_move = move_direction;
    delete[] surroundings;
    return res;
  }

  void Character::set_move(int x, int y, char last_move)
  {
    int old_x = field_x, old_y = field_y;
    field_x = x;
    field_y = y;
    this->last_move = last_move;

    if ((old_x != x || old_y != y) &&
        (abs(field_x - x) < 2 && abs(field_y - y) < 2))
    {
      start_animate_move();
    }
  }

  void Character::draw()
  {
    SDL_Texture *texture = NULL;
    if (killed)
      texture = TextureCache::killed_character_texture;
    else if (is_main_character)
    {
      switch(last_move)
      {
        case CHARACTER_MOVE_DOWN:
          texture = TextureCache::mc_front_texture;
          break;
        case CHARACTER_MOVE_UP:
          texture = TextureCache::mc_back_texture;
          break;
        case CHARACTER_MOVE_LEFT:
          texture = TextureCache::mc_left_texture;
          break;
        case CHARACTER_MOVE_RIGHT:
          texture = TextureCache::mc_right_texture;
          break;
      }
    }
    else
    {
      switch(last_move)
      {
        case CHARACTER_MOVE_DOWN:
          texture = TextureCache::ec_front_texture;
          break;
        case CHARACTER_MOVE_UP:
          texture = TextureCache::ec_back_texture;
          break;
        case CHARACTER_MOVE_LEFT:
          texture = TextureCache::ec_left_texture;
          break;
        case CHARACTER_MOVE_RIGHT:
          texture = TextureCache::ec_right_texture;
          break;
      }
    }
    sdl_wrapper->draw_texture(texture, screen_x, screen_y);
  }

  void Character::start_animate_move()
  {
    if (timer_callbacks_triggered == 0)
      SDL_AddTimer(CHARACTER_MOVE_ANIMATE_TIME / CHARACTER_MOVE_ANIMATE_TIMER_COUNT, animate_timer_callback, this);
  }

  Uint32 Character::animate_timer_callback(Uint32 interval, void* params)
  {
    Character* object = (Character*) params;

    object->timer_callbacks_triggered++;

    float multiplier = (CHARACTER_MOVE_ANIMATE_TIMER_COUNT - object->timer_callbacks_triggered) / (float) CHARACTER_MOVE_ANIMATE_TIMER_COUNT;
    int screen_x = object->get_x() * TILE_SIZE;
    int screen_y = object->get_y() * TILE_SIZE;
    switch(object->get_last_move())
    {
      case CHARACTER_MOVE_DOWN:
        screen_y -= floor(multiplier * TILE_SIZE + 0.5);
        break;
      case CHARACTER_MOVE_UP:
        screen_y += floor(multiplier * TILE_SIZE + 0.5);
        break;
      case CHARACTER_MOVE_LEFT:
        screen_x += floor(multiplier * TILE_SIZE + 0.5);
        break;
      case CHARACTER_MOVE_RIGHT:
        screen_x -= floor(multiplier * TILE_SIZE + 0.5);
        break;
    }
    object->set_screen_coord(screen_x, screen_y);

    if (object->timer_callbacks_triggered == CHARACTER_MOVE_ANIMATE_TIMER_COUNT)
    {
      object->timer_callbacks_triggered = 0;
      return 0;
    }
    else
      return interval;
  }
}
