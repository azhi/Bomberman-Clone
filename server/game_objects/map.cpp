#include "map.h"
#include "character.h"
#include "bomb.h"
#include "../../shared/messages/server.h"

#include <stdio.h>
#include <stdlib.h>

namespace GameObjects
{
  Map::Map(const char *map_file)
  {
    characters = new std::list<Character*>;
    bombs = new std::list<Bomb*>;
    init_from_file(map_file);
  }

  Map::~Map()
  {
  }

  void Map::add_character(Character* character)
  {
    characters->push_back(character);
  }

  Character* Map::find_character(char object_id)
  {
    for(std::list<Character*>::iterator character = characters->begin();
        character != characters->end();
        character++)
      if ((*character)->get_object_id() == object_id)
        return *character;
    return NULL;
  }

  void Map::remove_character(char object_id)
  {
    for(std::list<Character*>::iterator character = characters->begin();
        character != characters->end();
        character++)
      if ((*character)->get_object_id() == object_id)
      {
        characters->erase(character);
        delete *character;
        break;
      }
  }

  int Map::get_alive_characters_count()
  {
    int res = 0;
    for(std::list<Character*>::iterator character = characters->begin();
        character != characters->end();
        character++)
      if (!(*character)->is_killed())
        res++;
    return res;
  }

  std::list<Character*>* Map::get_characters(){
    return characters;
  }

  void Map::add_bomb(Bomb* bomb)
  {
    bombs->push_back(bomb);
  }

  Bomb* Map::find_bomb(char object_id)
  {
    for(std::list<Bomb*>::iterator bomb = bombs->begin();
        bomb != bombs->end();
        bomb++)
      if ((*bomb)->get_object_id() == object_id)
        return *bomb;
    return NULL;
  }

  void Map::remove_bomb(char object_id)
  {
    for(std::list<Bomb*>::iterator bomb = bombs->begin();
        bomb != bombs->end();
        bomb++)
      if ((*bomb)->get_object_id() == object_id)
      {
        bombs->erase(bomb);
        delete *bomb;
        break;
      }
  }

  char Map::get_field_type(int x, int y)
  {
    return field[x][y];
  }

  Coord Map::find_free_space()
  {
    if (get_element_type_on_coord(1, 1) == GRASS_FIELD)
      return Coord(1, 1);
    if (get_element_type_on_coord(MAP_WIDTH - 2, MAP_HEIGHT - 2) == GRASS_FIELD)
      return Coord(MAP_WIDTH - 2, MAP_HEIGHT - 2);
    if (get_element_type_on_coord(1, MAP_HEIGHT - 2) == GRASS_FIELD)
      return Coord(1, MAP_HEIGHT - 2);
    if (get_element_type_on_coord(MAP_WIDTH - 2, 1) == GRASS_FIELD)
      return Coord(MAP_WIDTH - 2, 1);
    return Coord(1, 1);
  }

  // 4 element array - [down, up, left, right]
  short* Map::get_surroundings(int x, int y)
  {
    short* res = new short[4];
    res[0] = get_element_type_on_coord(x, y+1);
    res[1] = get_element_type_on_coord(x, y-1);
    res[2] = get_element_type_on_coord(x-1, y);
    res[3] = get_element_type_on_coord(x+1, y);
    return res;
  }

  void Map::destruct_field(int x, int y)
  {
    field[x][y] = GRASS_FIELD;
  }

  char* Map::dump_to_binary_string()
  {
    char characters_size = characters->size();
    char bombs_size = bombs->size();
    char* res = new char[(MAP_HEIGHT * MAP_WIDTH / 4 + 1) + characters_size * 4 + bombs_size * 3 + 3];
    int current_pos = 0;
    res[current_pos++] = FULL_STATE_DUMP_CMD;
    char _4fields = '\0';
    char _2bit_groups_count = 0;
    for(int i = 0; i < MAP_HEIGHT; i++)
      for(int j = 0; j < MAP_WIDTH; j++)
      {
        _4fields |= field[j][i];
        _2bit_groups_count++;
        if ( _2bit_groups_count == 4 )
        {
          res[current_pos++] = _4fields;
          _4fields = '\0';
          _2bit_groups_count = 0;
        }
        else
          _4fields <<= 2;
      }
    if (_2bit_groups_count != 0)
    {
      for(int i = _2bit_groups_count; i < 3; i++)
        _4fields <<= 2;
      res[current_pos++] = _4fields;
    }

    res[current_pos++] = characters_size;
    for(std::list<Character*>::iterator character = characters->begin();
        character != characters->end();
        character++)
    {
      res[current_pos++] = (*character)->get_object_id();
      res[current_pos++] = (*character)->get_x();
      res[current_pos++] = (*character)->get_y();
      res[current_pos] = (*character)->get_last_move();
      if ((*character)->is_killed())
        res[current_pos] |= 0x08;
      current_pos++;
    }

    res[current_pos++] = bombs_size;
    for(std::list<Bomb*>::iterator bomb = bombs->begin();
        bomb != bombs->end();
        bomb++)
    {
      res[current_pos++] = (*bomb)->get_object_id();
      res[current_pos++] = (*bomb)->get_x();
      res[current_pos++] = (*bomb)->get_y();
    }

    res[current_pos++] = '\0';
    return res;
  }

  void Map::init_from_file(const char *map_file)
  {
    FILE *file = fopen(map_file, "r");
    for(int i = 0; i < MAP_HEIGHT; i++)
    {
      char str[MAP_WIDTH + 2];
      fgets(str, MAP_WIDTH + 2, file);
      for(int j = 0; j < MAP_WIDTH; j++)
      {
        char chr = str[j];
        chr -= 48;
        chr += 1;
        if (chr == GRASS_FIELD &&
            (i != 1 || j != 1) &&
            (i != 1 || j != 2) &&
            (i != 2 || j != 1) &&
            (i != MAP_HEIGHT - 2 || j != 1) &&
            (i != MAP_HEIGHT - 2 || j != 2) &&
            (i != MAP_HEIGHT - 3 || j != 1) &&
            (i != 1 || j != MAP_WIDTH - 2) &&
            (i != 1 || j != MAP_WIDTH - 3) &&
            (i != 2 || j != MAP_WIDTH - 2) &&
            (i != MAP_HEIGHT - 2 || j != MAP_WIDTH - 2) &&
            (i != MAP_HEIGHT - 2 || j != MAP_WIDTH - 3) &&
            (i != MAP_HEIGHT - 3 || j != MAP_WIDTH - 2))
        {
          if (rand() % 10 < 7)
            chr = DESTRUCTIBLE_FIELD;
        }
        field[j][i] = chr;
      }
    }
  }

  short Map::get_element_type_on_coord(int x, int y)
  {
    for(std::list<Bomb*>::iterator bomb = bombs->begin();
        bomb != bombs->end();
        bomb++)
      if ((*bomb)->get_x() == x && (*bomb)->get_y() == y)
        return BOMB;
    for(std::list<Character*>::iterator character = characters->begin();
        character != characters->end();
        character++)
      if ((*character)->get_x() == x && (*character)->get_y() == y)
        return CHARACTER;
    return field[x][y];
  }
}
