#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include "../../shared/field_types.h"

namespace GameObjects
{
  class GameObject
  {
    public:
      GameObject(char object_id, char object_type);
      virtual ~GameObject();

      char get_object_id();
      char get_object_type();

    protected:
      char object_id;
      char object_type;
  };
}

#endif
