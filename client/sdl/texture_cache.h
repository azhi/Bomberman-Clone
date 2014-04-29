#ifndef _TEXTURE_CACHE_H_
#define _TEXTURE_CACHE_H_

#include "sdl_wrapper.h"

#define BASE_TEXTURE_PATH "resources/sprites/"

#define DESTRUCTIBLE_TEXTURE_PATH "terrain/destructible.bmp"
#define INDESTRUCTIBLE_TEXTURE_PATH "terrain/indestructible.bmp"
#define GRASS_TEXTURE_PATH "terrain/grass.bmp"

#define MC_FRONT_TEXTURE_PATH "characters/blue/front.bmp"
#define MC_BACK_TEXTURE_PATH "characters/blue/back.bmp"
#define MC_LEFT_TEXTURE_PATH "characters/blue/left.bmp"
#define MC_RIGHT_TEXTURE_PATH "characters/blue/right.bmp"

#define EC_FRONT_TEXTURE_PATH "characters/red/front.bmp"
#define EC_BACK_TEXTURE_PATH "characters/red/back.bmp"
#define EC_LEFT_TEXTURE_PATH "characters/red/left.bmp"
#define EC_RIGHT_TEXTURE_PATH "characters/red/right.bmp"

#define KILLED_CHARACTER_TEXTURE "characters/killed.bmp"

#define SMALL_BOMB_TEXTURE_PATH "bombs/small.bmp"
#define MEDIUM_BOMB_TEXTURE_PATH "bombs/medium.bmp"
#define BIG_BOMB_TEXTURE_PATH "bombs/big.bmp"

#define EXPLOSION_CENTER_TEXTURE_PATH "explosions/expl_center.bmp"
#define EXPLOSION_LEFT_TEXTURE_PATH "explosions/expl_left.bmp"
#define EXPLOSION_LEFT_END_TEXTURE_PATH "explosions/expl_left_end.bmp"
#define EXPLOSION_RIGHT_TEXTURE_PATH "explosions/expl_right.bmp"
#define EXPLOSION_RIGHT_END_TEXTURE_PATH "explosions/expl_right_end.bmp"
#define EXPLOSION_TOP_TEXTURE_PATH "explosions/expl_top.bmp"
#define EXPLOSION_TOP_END_TEXTURE_PATH "explosions/expl_top_end.bmp"
#define EXPLOSION_BOTTOM_TEXTURE_PATH "explosions/expl_bottom.bmp"
#define EXPLOSION_BOTTOM_END_TEXTURE_PATH "explosions/expl_bottom_end.bmp"

class TextureCache
{
  public:
    static void init(SDLWrapper *sdl_wrapper);

    static SDL_Texture *destructible_texture;
    static SDL_Texture *indestructible_texture;
    static SDL_Texture *grass_texture;

    static SDL_Texture *mc_front_texture;
    static SDL_Texture *mc_back_texture;
    static SDL_Texture *mc_left_texture;
    static SDL_Texture *mc_right_texture;
    static SDL_Texture *ec_front_texture;
    static SDL_Texture *ec_back_texture;
    static SDL_Texture *ec_left_texture;
    static SDL_Texture *ec_right_texture;
    static SDL_Texture *killed_character_texture;

    static SDL_Texture *big_bomb_texture;
    static SDL_Texture *medium_bomb_texture;
    static SDL_Texture *small_bomb_texture;

    static SDL_Texture *explosion_center_texture;
    static SDL_Texture *explosion_left_texture;
    static SDL_Texture *explosion_left_end_texture;
    static SDL_Texture *explosion_right_texture;
    static SDL_Texture *explosion_right_end_texture;
    static SDL_Texture *explosion_bottom_texture;
    static SDL_Texture *explosion_bottom_end_texture;
    static SDL_Texture *explosion_top_texture;
    static SDL_Texture *explosion_top_end_texture;

  private:
    static void init_map(SDLWrapper *sdl_wrapper);
    static void init_character(SDLWrapper *sdl_wrapper);
    static void init_bomb(SDLWrapper *sdl_wrapper);
    static void init_explosion(SDLWrapper *sdl_wrapper);
};

#endif
