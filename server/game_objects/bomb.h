#ifndef _BOMB_H_
#define _BOMB_H_

#include "game_object.h"
#include "map.h"
#include "../../shared/bomb_explode_time.h"

namespace GameObjects
{
  class Bomb : public GameObject
  {
    public:
      Bomb(char object_id, Map* map, Character* character, int initial_x, int initial_y);
      virtual ~Bomb();

      void explode();

      int get_x(){ return x; };
      int get_y(){ return y; };

      void remove(){ removed = true; };
      bool is_removed(){ return removed; };

    private:
      Map* map;
      Character* character;

      int x;
      int y;
      bool removed = false;
  };
}

#endif
