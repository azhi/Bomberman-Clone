#include <cmath>
#include <ctgmath>
#include "../sdl/texture_cache.h"
#include "bomb.h"

namespace GameObjects
{
  Bomb::Bomb(SDLWrapper *sdl_wrapper, Map* map, char object_id, int initial_x, int initial_y) :
    GameObject(sdl_wrapper, object_id), map(map), field_x(initial_x), field_y(initial_y), state(0)
  {
    screen_x = TILE_SIZE * initial_x;
    screen_y = TILE_SIZE * initial_y;
    timer_callbacks_triggered = 0;
    SDL_AddTimer(BOMB_ANIMATE_TIME / BOMB_ANIMATE_TIMER_COUNT, animate_timer_callback, this);
  }

  Bomb::~Bomb()
  {
  }

  void Bomb::draw()
  {
    SDL_Texture *texture = NULL;
    switch(state)
    {
      case BOMB_SMALL:
        texture = TextureCache::small_bomb_texture;
        break;
      case BOMB_MEDIUM:
        texture = TextureCache::medium_bomb_texture;
        break;
      case BOMB_BIG:
        texture = TextureCache::big_bomb_texture;
        break;
      case BOMB_EXPLODED:
        int x = screen_x, y = screen_y;
        sdl_wrapper->draw_texture(TextureCache::explosion_center_texture, x, y);
        if (map->get_field_type(field_x - 1, field_y) == GRASS_FIELD)
        {
          sdl_wrapper->draw_texture(TextureCache::explosion_left_texture, x - TILE_SIZE, y);
          if (map->get_field_type(field_x - 2, field_y) == GRASS_FIELD)
            sdl_wrapper->draw_texture(TextureCache::explosion_left_end_texture, x - 2 * TILE_SIZE, y);
        }
        if (map->get_field_type(field_x + 1, field_y) == GRASS_FIELD)
        {
          sdl_wrapper->draw_texture(TextureCache::explosion_right_texture, x + TILE_SIZE, y);
          if (map->get_field_type(field_x + 2, field_y) == GRASS_FIELD)
            sdl_wrapper->draw_texture(TextureCache::explosion_right_end_texture, x + 2 * TILE_SIZE, y);
        }
        if (map->get_field_type(field_x, field_y - 1) == GRASS_FIELD)
        {
          sdl_wrapper->draw_texture(TextureCache::explosion_top_texture, x, y - TILE_SIZE);
          if (map->get_field_type(field_x, field_y - 2) == GRASS_FIELD)
            sdl_wrapper->draw_texture(TextureCache::explosion_top_end_texture, x, y - 2 * TILE_SIZE);
        }
        if (map->get_field_type(field_x, field_y + 1) == GRASS_FIELD)
        {
          sdl_wrapper->draw_texture(TextureCache::explosion_bottom_texture, x, y + TILE_SIZE);
          if (map->get_field_type(field_x, field_y + 2) == GRASS_FIELD)
            sdl_wrapper->draw_texture(TextureCache::explosion_bottom_end_texture, x, y + 2 * TILE_SIZE);
        }
        break;
    }
    sdl_wrapper->draw_texture(texture, screen_x, screen_y);
  }

  void Bomb::explode()
  {
    state = BOMB_EXPLODED;
  }

  void Bomb::free()
  {
    map->remove_bomb(get_object_id());
    // delete this;
  }

  Uint32 Bomb::animate_timer_callback(Uint32 interval, void* params)
  {
    Bomb* object = (Bomb*) params;

    object->timer_callbacks_triggered++;
    if (object->timer_callbacks_triggered == BOMB_ANIMATE_TIMER_COUNT - 1)
      object->explode();
    else if (object->timer_callbacks_triggered == BOMB_ANIMATE_TIMER_COUNT)
    {
      object->free();
      return 0;
    }
    else
      object->next_state();

    return interval;
  }
}
