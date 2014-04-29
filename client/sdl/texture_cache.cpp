#include "texture_cache.h"
#include <string>

SDL_Texture *TextureCache::destructible_texture = NULL;
SDL_Texture *TextureCache::indestructible_texture = NULL;
SDL_Texture *TextureCache::grass_texture = NULL;

SDL_Texture *TextureCache::mc_front_texture = NULL;
SDL_Texture *TextureCache::mc_back_texture = NULL;
SDL_Texture *TextureCache::mc_left_texture = NULL;
SDL_Texture *TextureCache::mc_right_texture = NULL;
SDL_Texture *TextureCache::ec_front_texture = NULL;
SDL_Texture *TextureCache::ec_back_texture = NULL;
SDL_Texture *TextureCache::ec_left_texture = NULL;
SDL_Texture *TextureCache::ec_right_texture = NULL;
SDL_Texture *TextureCache::killed_character_texture = NULL;

SDL_Texture *TextureCache::big_bomb_texture = NULL;
SDL_Texture *TextureCache::medium_bomb_texture = NULL;
SDL_Texture *TextureCache::small_bomb_texture = NULL;

SDL_Texture *TextureCache::explosion_center_texture = NULL;
SDL_Texture *TextureCache::explosion_left_texture = NULL;
SDL_Texture *TextureCache::explosion_left_end_texture = NULL;
SDL_Texture *TextureCache::explosion_right_texture = NULL;
SDL_Texture *TextureCache::explosion_right_end_texture = NULL;
SDL_Texture *TextureCache::explosion_top_texture = NULL;
SDL_Texture *TextureCache::explosion_top_end_texture = NULL;
SDL_Texture *TextureCache::explosion_bottom_texture = NULL;
SDL_Texture *TextureCache::explosion_bottom_end_texture = NULL;

void TextureCache::init(SDLWrapper *sdl_wrapper)
{
  init_map(sdl_wrapper);
  init_character(sdl_wrapper);
  init_bomb(sdl_wrapper);
  init_explosion(sdl_wrapper);
}

void TextureCache::init_map(SDLWrapper *sdl_wrapper)
{
  std::string texture_path = BASE_TEXTURE_PATH;
  texture_path.append(DESTRUCTIBLE_TEXTURE_PATH);
  destructible_texture = sdl_wrapper->load_texture(texture_path.c_str());

  texture_path = BASE_TEXTURE_PATH;
  texture_path.append(INDESTRUCTIBLE_TEXTURE_PATH);
  indestructible_texture = sdl_wrapper->load_texture(texture_path.c_str());

  texture_path = BASE_TEXTURE_PATH;
  texture_path.append(GRASS_TEXTURE_PATH);
  grass_texture = sdl_wrapper->load_texture(texture_path.c_str());
}

void TextureCache::init_character(SDLWrapper *sdl_wrapper)
{
  std::string texture_path = BASE_TEXTURE_PATH;
  texture_path.append(MC_FRONT_TEXTURE_PATH);
  mc_front_texture = sdl_wrapper->load_texture(texture_path.c_str());

  texture_path = BASE_TEXTURE_PATH;
  texture_path.append(MC_BACK_TEXTURE_PATH);
  mc_back_texture = sdl_wrapper->load_texture(texture_path.c_str());

  texture_path = BASE_TEXTURE_PATH;
  texture_path.append(MC_LEFT_TEXTURE_PATH);
  mc_left_texture = sdl_wrapper->load_texture(texture_path.c_str());

  texture_path = BASE_TEXTURE_PATH;
  texture_path.append(MC_RIGHT_TEXTURE_PATH);
  mc_right_texture = sdl_wrapper->load_texture(texture_path.c_str());

  texture_path = BASE_TEXTURE_PATH;
  texture_path.append(EC_FRONT_TEXTURE_PATH);
  ec_front_texture = sdl_wrapper->load_texture(texture_path.c_str());

  texture_path = BASE_TEXTURE_PATH;
  texture_path.append(EC_BACK_TEXTURE_PATH);
  ec_back_texture = sdl_wrapper->load_texture(texture_path.c_str());

  texture_path = BASE_TEXTURE_PATH;
  texture_path.append(EC_LEFT_TEXTURE_PATH);
  ec_left_texture = sdl_wrapper->load_texture(texture_path.c_str());

  texture_path = BASE_TEXTURE_PATH;
  texture_path.append(EC_RIGHT_TEXTURE_PATH);
  ec_right_texture = sdl_wrapper->load_texture(texture_path.c_str());

  texture_path = BASE_TEXTURE_PATH;
  texture_path.append(KILLED_CHARACTER_TEXTURE);
  killed_character_texture = sdl_wrapper->load_texture(texture_path.c_str());
}

void TextureCache::init_bomb(SDLWrapper *sdl_wrapper)
{
  std::string texture_path = BASE_TEXTURE_PATH;
  texture_path.append(SMALL_BOMB_TEXTURE_PATH);
  small_bomb_texture = sdl_wrapper->load_texture(texture_path.c_str());

  texture_path = BASE_TEXTURE_PATH;
  texture_path.append(MEDIUM_BOMB_TEXTURE_PATH);
  medium_bomb_texture = sdl_wrapper->load_texture(texture_path.c_str());

  texture_path = BASE_TEXTURE_PATH;
  texture_path.append(BIG_BOMB_TEXTURE_PATH);
  big_bomb_texture = sdl_wrapper->load_texture(texture_path.c_str());
}

void TextureCache::init_explosion(SDLWrapper *sdl_wrapper)
{
  std::string texture_path = BASE_TEXTURE_PATH;
  texture_path.append(EXPLOSION_CENTER_TEXTURE_PATH);
  explosion_center_texture = sdl_wrapper->load_texture(texture_path.c_str());

  texture_path = BASE_TEXTURE_PATH;
  texture_path.append(EXPLOSION_LEFT_TEXTURE_PATH);
  explosion_left_texture = sdl_wrapper->load_texture(texture_path.c_str());

  texture_path = BASE_TEXTURE_PATH;
  texture_path.append(EXPLOSION_LEFT_END_TEXTURE_PATH);
  explosion_left_end_texture = sdl_wrapper->load_texture(texture_path.c_str());

  texture_path = BASE_TEXTURE_PATH;
  texture_path.append(EXPLOSION_RIGHT_TEXTURE_PATH);
  explosion_right_texture = sdl_wrapper->load_texture(texture_path.c_str());

  texture_path = BASE_TEXTURE_PATH;
  texture_path.append(EXPLOSION_RIGHT_END_TEXTURE_PATH);
  explosion_right_end_texture = sdl_wrapper->load_texture(texture_path.c_str());

  texture_path = BASE_TEXTURE_PATH;
  texture_path.append(EXPLOSION_TOP_TEXTURE_PATH);
  explosion_top_texture = sdl_wrapper->load_texture(texture_path.c_str());

  texture_path = BASE_TEXTURE_PATH;
  texture_path.append(EXPLOSION_TOP_END_TEXTURE_PATH);
  explosion_top_end_texture = sdl_wrapper->load_texture(texture_path.c_str());

  texture_path = BASE_TEXTURE_PATH;
  texture_path.append(EXPLOSION_BOTTOM_TEXTURE_PATH);
  explosion_bottom_texture = sdl_wrapper->load_texture(texture_path.c_str());

  texture_path = BASE_TEXTURE_PATH;
  texture_path.append(EXPLOSION_BOTTOM_END_TEXTURE_PATH);
  explosion_bottom_end_texture = sdl_wrapper->load_texture(texture_path.c_str());
}
