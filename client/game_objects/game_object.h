#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include "../sdl/sdl_wrapper.h"

#define TILE_SIZE 32

namespace GameObjects
{
  class GameObject
  {
    public:
      GameObject(SDLWrapper *sdl_wrapper, char object_id);
      virtual ~GameObject();

      virtual void draw() = 0;

      char get_object_id();

    protected:
      SDLWrapper *sdl_wrapper;
      char object_id;
  };
}

#endif
