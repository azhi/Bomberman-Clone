#include "game_object.h"

namespace GameObjects
{
  GameObject::GameObject(SDLWrapper *sdl_wrapper, char object_id) : sdl_wrapper(sdl_wrapper), object_id(object_id)
  {
  }

  GameObject::~GameObject()
  {
  }

  char GameObject::get_object_id()
  {
    return object_id;
  }
}
