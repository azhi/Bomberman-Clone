#ifndef _MAP_H_
#define _MAP_H_

#include "game_object.h"
#include "../../shared/map_size.h"
#include "../../shared/field_types.h"

#include <list>

namespace GameObjects
{
  class Character;
  class Bomb;

  class Map : public GameObject
  {
    public:
      Map(SDLWrapper *sdl_wrapper);
      virtual ~Map();

      virtual void draw();

      void add_character(Character* character);
      Character* find_character(char object_id);
      void remove_character(char object_id);

      void add_bomb(Bomb* bomb);
      Bomb* find_bomb(char object_id);
      void remove_bomb(char object_id);

      void set_main_character(Character* character){ main_character = character; };
      Character* get_main_character(){ return main_character; };

      short *get_surroundings(int x, int y);
      char get_field_type(int x, int y);
      void destruct_field(int x, int y);

      void restore_from_binary_dump(char* dump);

    private:
      short get_element_type_on_coord(int x, int y);

      short field[MAP_WIDTH][MAP_HEIGHT];
      Character *main_character;
      std::list<Character*> *characters;
      std::list<Bomb*> *bombs;
  };
}

#endif
