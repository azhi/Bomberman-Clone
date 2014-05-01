#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "game_object.h"
#include "map.h"
#include "../../shared/character_move_directions.h"

#define CHARACTER_MOVE_ANIMATE_TIME 180
#define CHARACTER_MOVE_ANIMATE_TIMER_COUNT 5

namespace GameObjects
{
  class Character : public GameObject
  {
    public:
      Character(SDLWrapper *sdl_wrapper, Map* map, char object_id,
                int initial_x, int initial_y, char last_move,
                bool killed, bool is_main_character);
      virtual ~Character();

      virtual void draw();
      bool can_move(char move_direction);
      void set_move(int x, int y, char last_move);

      int get_last_move(){ return last_move; };
      int get_x(){ return field_x; };
      int get_y(){ return field_y; };

      bool is_killed(){ return killed; };
      void kill(){ killed = true; };
      void respawn(){ killed = false; };

      void set_mark(){ marked = true; };
      void unset_mark(){ marked = false; };
      bool is_marked(){ return marked; };

      void set_screen_coord(int x, int y){ screen_x = x; screen_y = y; };

      void start_animate_move();

      static Uint32 animate_timer_callback(Uint32 interval, void* params);
      int timer_callbacks_triggered;

    private:
      Map* map;
      bool marked = false;
      bool is_main_character;

      int field_x;
      int field_y;
      int screen_x;
      int screen_y;
      char last_move;
      bool killed;
  };
}

#endif
