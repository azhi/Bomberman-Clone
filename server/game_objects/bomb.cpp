#include "bomb.h"

#include "character.h"

namespace GameObjects
{
  Bomb::Bomb(char object_id, Map* map, Character* character, int initial_x, int initial_y) :
    GameObject(object_id, BOMB), map(map), character(character), x(initial_x), y(initial_y)
  {
    character->inc_current_bombs_count();
  }

  Bomb::~Bomb()
  {
    character->dec_current_bombs_count();
  }

  ExplodeResults Bomb::explode()
  {
    char left_aoe = 0;
    if (map->get_field_type(x - 1, y) != DESTRUCTIBLE_FIELD &&
        map->get_field_type(x - 1, y) != INDESTRUCTIBLE_FIELD)
    {
      left_aoe = 1;
      if (map->get_field_type(x - 2, y) != DESTRUCTIBLE_FIELD &&
          map->get_field_type(x - 2, y) != INDESTRUCTIBLE_FIELD)
        left_aoe = 2;
    }

    char right_aoe = 0;
    if (map->get_field_type(x + 1, y) != DESTRUCTIBLE_FIELD &&
        map->get_field_type(x + 1, y) != INDESTRUCTIBLE_FIELD)
    {
      right_aoe = 1;
      if (map->get_field_type(x + 2, y) != DESTRUCTIBLE_FIELD &&
          map->get_field_type(x + 2, y) != INDESTRUCTIBLE_FIELD)
        right_aoe = 2;
    }

    char top_aoe = 0;
    if (map->get_field_type(x, y - 1) != DESTRUCTIBLE_FIELD &&
        map->get_field_type(x, y - 1) != INDESTRUCTIBLE_FIELD)
    {
      top_aoe = 1;
      if (map->get_field_type(x, y - 2) != DESTRUCTIBLE_FIELD &&
          map->get_field_type(x, y - 2) != INDESTRUCTIBLE_FIELD)
        top_aoe = 2;
    }

    char bottom_aoe = 0;
    if (map->get_field_type(x, y + 1) != DESTRUCTIBLE_FIELD &&
        map->get_field_type(x, y + 1) != INDESTRUCTIBLE_FIELD)
    {
      bottom_aoe = 1;
      if (map->get_field_type(x, y + 2) != DESTRUCTIBLE_FIELD &&
          map->get_field_type(x, y + 2) != INDESTRUCTIBLE_FIELD)
        bottom_aoe = 2;
    }

    char *field_xys = new char[8];
    char current_xys_pos = 0;
    if (left_aoe != 2 && map->get_field_type(x - left_aoe - 1, y) == DESTRUCTIBLE_FIELD)
    {
      map->destruct_field(x - left_aoe - 1, y);
      field_xys[current_xys_pos++] = x - left_aoe - 1;
      field_xys[current_xys_pos++] = y;
    }
    if (right_aoe != 2 && map->get_field_type(x + right_aoe + 1, y) == DESTRUCTIBLE_FIELD)
    {
      map->destruct_field(x + right_aoe + 1, y);
      field_xys[current_xys_pos++] = x + right_aoe + 1;
      field_xys[current_xys_pos++] = y;
    }
    if (top_aoe != 2 && map->get_field_type(x, y - top_aoe - 1) == DESTRUCTIBLE_FIELD)
    {
      map->destruct_field(x, y - top_aoe - 1);
      field_xys[current_xys_pos++] = x;
      field_xys[current_xys_pos++] = y - top_aoe - 1;
    }
    if (bottom_aoe != 2 && map->get_field_type(x, y + bottom_aoe + 1) == DESTRUCTIBLE_FIELD)
    {
      map->destruct_field(x, y + bottom_aoe + 1);
      field_xys[current_xys_pos++] = x;
      field_xys[current_xys_pos++] = y + bottom_aoe + 1;
    }

    char *player_ids = new char[map->get_characters()->size()];
    char current_player_ids_pos = 0;
    std::list<Character*> *characters = map->get_characters();
    for(std::list<Character*>::iterator character = characters->begin();
        character != characters->end();
        character++)
      if ( (*character)->get_y() == y )
      {
        if ( (x - (*character)->get_x()) > 0 && (x - (*character)->get_x()) <= left_aoe ||
             ((*character)->get_x() - x) >= 0 && ((*character)->get_x() - x) <= right_aoe )
        {
          (*character)->kill();
          player_ids[current_player_ids_pos++] = (*character)->get_object_id();
        }
      }
      else if ( (*character)->get_x() == x )
      {
        if ( (y - (*character)->get_y()) > 0 && (y - (*character)->get_y()) <= top_aoe ||
             ((*character)->get_y() - y) >= 0 && ((*character)->get_y() - y) <= bottom_aoe )
        {
          (*character)->kill();
          player_ids[current_player_ids_pos++] = (*character)->get_object_id();
        }
      }
    ExplodeResults results = {field_xys, current_xys_pos, player_ids, current_player_ids_pos};
    return results;
  }
}
