#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "game_object.h"
#include "map.h"
#include "../../shared/character_move_directions.h"

#define MAX_BOMBS_COUNT 2

namespace GameObjects
{
  class Character : public GameObject
  {
    public:
      Character(char object_id, Map* map, int initial_x, int initial_y);
      virtual ~Character();

      void move(short move_direction);
      char get_last_move();

      bool is_killed();
      void kill();
      void respawn();

      int get_current_bombs_count(){ return current_bombs_count; };
      void inc_current_bombs_count(){ current_bombs_count++; };
      void dec_current_bombs_count(){ current_bombs_count--; };

      int set_x(int x){ this->x = x; };
      int set_y(int y){ this->y = y; };
      int get_x(){ return x; };
      int get_y(){ return y; };
      int get_prev_x(){ return prev_x; };
      int get_prev_y(){ return prev_y; };

    private:
      Map* map;

      int prev_x;
      int prev_y;
      int x;
      int y;
      int last_move;
      bool killed;
      int current_bombs_count;
  };
}

#endif
