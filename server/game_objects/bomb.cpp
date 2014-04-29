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

  void Bomb::explode()
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

    if (map->get_field_type(x - left_aoe - 1, y) == DESTRUCTIBLE_FIELD)
      map->destruct_field(x - left_aoe - 1, y);
    if (map->get_field_type(x + right_aoe + 1, y) == DESTRUCTIBLE_FIELD)
      map->destruct_field(x + right_aoe + 1, y);
    if (map->get_field_type(x, y - top_aoe - 1) == DESTRUCTIBLE_FIELD)
      map->destruct_field(x, y - top_aoe - 1);
    if (map->get_field_type(x, y + bottom_aoe + 1) == DESTRUCTIBLE_FIELD)
      map->destruct_field(x, y + bottom_aoe + 1);

    std::list<Character*> *characters = map->get_characters();
    for(std::list<Character*>::iterator character = characters->begin();
        character != characters->end();
        character++)
      if ( (*character)->get_y() == y )
      {
        if ( (x - (*character)->get_x()) > 0 && (x - (*character)->get_x()) <= left_aoe ||
             ((*character)->get_x() - x) >= 0 && ((*character)->get_x() - x) <= right_aoe )
          (*character)->kill();
      }
      else if ( (*character)->get_x() == x )
      {
        if ( (y - (*character)->get_y()) > 0 && (y - (*character)->get_y()) <= top_aoe ||
             ((*character)->get_y() - y) >= 0 && ((*character)->get_y() - y) <= bottom_aoe )
          (*character)->kill();
      }
  }
}
