#ifndef _MAP_H_
#define _MAP_H_

#include "../../shared/map_size.h"
#include "../../shared/field_types.h"

#include <list>

#define DEFAULT_MAP_PATH "resources/default_map.dat"

namespace GameObjects
{
  class Character;
  class Bomb;

  struct Coord
  {
    Coord(int x, int y) : x(x), y(y) {};
    int x;
    int y;
  };

  class Map
  {
    public:
      Map(const char *map_file);
      virtual ~Map();

      std::list<Character*>* get_characters();
      void add_character(Character* character);
      Character* find_character(char object_id);
      void remove_character(char object_id);
      int get_alive_characters_count();

      void add_bomb(Bomb* bomb);
      Bomb* find_bomb(char object_id);
      void remove_bomb(char object_id);

      short *get_surroundings(int x, int y);
      short get_element_type_on_coord(int x, int y);
      char get_field_type(int x, int y);
      Coord find_free_space();

      void destruct_field(int x, int y);


      char* dump_to_binary_string();

      void init_from_file(const char *map_file);

    private:
      char field[MAP_WIDTH][MAP_HEIGHT];
      std::list<Character*> *characters;
      std::list<Bomb*> *bombs;
  };
}

#endif
