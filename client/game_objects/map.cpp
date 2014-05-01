#include "map.h"
#include "character.h"
#include "bomb.h"
#include "../sdl/texture_cache.h"
#include "../../shared/debug.h"

#include <iostream>
#include <stdlib.h>
#include <vector>

namespace GameObjects
{
  Map::Map(SDLWrapper *sdl_wrapper) : GameObject(sdl_wrapper, 1)
  {
    characters = new std::list<Character*>;
    bombs = new std::list<Bomb*>;
  }

  Map::~Map()
  {
  }

  void Map::draw()
  {
    for(int i = 0; i < MAP_WIDTH; i++)
      for(int j = 0; j < MAP_HEIGHT; j++)
      {
        switch(field[i][j])
        {
          case DESTRUCTIBLE_FIELD:
            sdl_wrapper->draw_texture(TextureCache::destructible_texture, i * TILE_SIZE, j * TILE_SIZE);
            break;
          case INDESTRUCTIBLE_FIELD:
            sdl_wrapper->draw_texture(TextureCache::indestructible_texture, i * TILE_SIZE, j * TILE_SIZE);
            break;
          case GRASS_FIELD:
            sdl_wrapper->draw_texture(TextureCache::grass_texture, i * TILE_SIZE, j * TILE_SIZE);
            break;
        }
      }
    for(std::list<Character*>::iterator character = characters->begin();
        character != characters->end();
        character++)
      (*character)->draw();
    for(std::list<Bomb*>::iterator bomb = bombs->begin();
        bomb != bombs->end();
        bomb++)
      (*bomb)->draw();
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

  char Map::get_field_type(int x, int y)
  {
    return field[x][y];
  }

  short Map::get_element_type_on_coord(int x, int y)
  {
    for(std::list<Character*>::iterator character = characters->begin();
        character != characters->end();
        character++)
      if ((*character)->get_x() == x && (*character)->get_y() == y)
        return CHARACTER;
    for(std::list<Bomb*>::iterator bomb = bombs->begin();
        bomb != bombs->end();
        bomb++)
      if ((*bomb)->get_x() == x && (*bomb)->get_y() == y)
        return BOMB;
    return field[x][y];
  }

  void Map::restore_from_binary_dump(char* dump)
  {
    int field_bytes_size = MAP_WIDTH * MAP_HEIGHT / 4;
    if ((MAP_WIDTH * MAP_HEIGHT) % 4 != 0)
      field_bytes_size++;
    int current_x = 0, current_y = 0;
    for(int i = 1; i < field_bytes_size + 1; i++)
    {
      char byte = dump[i];
      for(int j = 3; j > -1; j--)
      {
        char field_cell = (byte >> (2 * j)) & 0x03;
        if (field_cell != 0)
        {
          field[current_x][current_y] = field_cell;
          current_x++;
          if (current_x == MAP_WIDTH)
          {
            current_x = 0;
            current_y++;
          }
        }
      }
    }

    int offset = field_bytes_size + 1;
    char characters_count = dump[offset++];
    for(int i = 0; i < characters_count; i++)
    {
      unsigned char object_id = dump[offset + 4 * i];
      unsigned char x = dump[offset + 4 * i + 1];
      unsigned char y = dump[offset + 4 * i + 2];
      unsigned char last_move_and_killed = dump[offset + 4 * i + 3];
      unsigned char last_move = last_move_and_killed & 0x07;
      bool killed = (last_move_and_killed & 0x08) == 0x08;

      D(std::cerr << "CHARACTER " << (int) object_id << " " << (int) x << " " << (int) y << " " << killed << std::endl);
      Character* character = find_character(object_id);
      if (character == NULL)
      {
        D(std::cerr << "ADDING CHARACTER " << (int) object_id << std::endl);
        character = new Character(sdl_wrapper, this, object_id, x, y, CHARACTER_MOVE_DOWN, killed, false);
        add_character(character);
      }
      else
      {
        D(std::cerr << "MOVING CHARACTER " << (int) object_id << std::endl);
        character->set_move(x, y, last_move);
        if (killed && !character->is_killed())
          character->kill();
        if (!killed && character->is_killed())
          character->respawn();
      }
    }

    offset += 4 * characters_count;
    char bombs_count = dump[offset++];
    for(int i = 0; i < bombs_count; i++)
    {
      char object_id = dump[offset + 3 * i];
      char x = dump[offset + 3 * i + 1];
      char y = dump[offset + 3 * i + 2];

      Bomb* bomb = find_bomb(object_id);
      if (bomb == NULL)
      {
        bomb = new Bomb(sdl_wrapper, this, object_id, x, y);
        add_bomb(bomb);
      }
    }
  }
}
