#include "character.h"
#include <cstdio>

namespace GameObjects
{
  Character::Character(char object_id, Map* map, int initial_x, int initial_y) :
    GameObject(object_id, CHARACTER), map(map), x(initial_x), y(initial_y),
    prev_x(initial_x), prev_y(initial_y), last_move(CHARACTER_MOVE_DOWN),
    killed(false), current_bombs_count(0)
  {
  }

  Character::~Character()
  {
  }

  char Character::get_last_move()
  {
    return last_move;
  }

  bool Character::is_killed()
  {
    return killed;
  }

  void Character::kill()
  {
    killed = true;
  }

  void Character::respawn()
  {
    killed = false;
  }

  void Character::move(short move_direction)
  {
    short* surroundings = map->get_surroundings(x, y);
    if (surroundings[move_direction - 1] == GRASS_FIELD ||
        surroundings[move_direction - 1] == CHARACTER)
    {
      int new_x = x, new_y = y;
      switch(move_direction)
      {
        case CHARACTER_MOVE_DOWN:
          new_y += 1;
          break;
        case CHARACTER_MOVE_UP:
          new_y -= 1;
          break;
        case CHARACTER_MOVE_LEFT:
          new_x -= 1;
          break;
        case CHARACTER_MOVE_RIGHT:
          new_x += 1;
          break;
      }

      prev_x = x; prev_y = y;
      x = new_x; y = new_y;
    }
    last_move = move_direction;

    delete[] surroundings;
  }
}
