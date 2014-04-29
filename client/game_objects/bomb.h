#ifndef _BOMB_H_
#define _BOMB_H_

#include "game_object.h"
#include "map.h"
#include "../../shared/bomb_explode_time.h"

#define BOMB_ANIMATE_TIME BOMB_EXPLODE_TIME
#define BOMB_ANIMATE_TIMER_COUNT 4

#define BOMB_SMALL 0
#define BOMB_MEDIUM 1
#define BOMB_BIG 2
#define BOMB_EXPLODED 3

namespace GameObjects
{
  class Bomb : public GameObject
  {
    public:
      Bomb(SDLWrapper *sdl_wrapper, Map* map, char object_id, int initial_x, int initial_y);
      virtual ~Bomb();

      virtual void draw();
      void explode();

      int get_x(){ return field_x; };
      int get_y(){ return field_y; };
      void next_state(){ state++; };
      void free();

      static Uint32 animate_timer_callback(Uint32 interval, void* params);
      int timer_callbacks_triggered;

    private:
      Map* map;
      bool is_exploded;

      int field_x;
      int field_y;
      int screen_x;
      int screen_y;
      int state;
  };
}

#endif
