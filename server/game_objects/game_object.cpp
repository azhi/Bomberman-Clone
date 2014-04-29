#include "game_object.h"

namespace GameObjects
{
  GameObject::GameObject(char object_id, char object_type) : object_id(object_id), object_type(object_type)
  {
  }

  GameObject::~GameObject()
  {
  }

  char GameObject::get_object_id()
  {
    return object_id;
  }

  char GameObject::get_object_type()
  {
    return object_type;
  }
}
