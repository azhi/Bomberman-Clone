#include "game_logic.h"

#include "server.h"
#include "../shared/character_move_directions.h"
#include "../shared/debug.h"
#include "game_objects/character.h"
#include "game_objects/bomb.h"

#include <chrono>
#include <future>
#include <iostream>

GameLogic::GameLogic(Server* server) :
  server(server), current_bomb_object_id(0), current_player_object_id(0)
{
  init_scene();
}

GameLogic::~GameLogic()
{
}

void GameLogic::register_new_character(Client* client)
{
  GameObjects::Coord free_space = map->find_free_space();
  GameObjects::Character *character = new GameObjects::Character(next_player_object_id(), map, free_space.x, free_space.y);
  map->add_character(character);
  client->set_character_object_id(character->get_object_id());
  server->send_create_character(character->get_object_id(), character->get_x(),
                                character->get_y(), character->get_last_move());
  D(std::cerr << "NEW CHAR JOINING: " << (int) character->get_object_id() << std::endl);
}

void GameLogic::move_character(char object_id, char direction)
{
  GameObjects::Character *character = map->find_character(object_id);
  if (character && !character->is_killed())
  {
    character->move(direction);
    server->send_move_character(object_id, character->get_x(), character->get_y(), character->get_last_move());
  }
}

void GameLogic::place_bomb(char object_id, char previous_location)
{
  GameObjects::Character *character = map->find_character(object_id);
  if (character && !character->is_killed())
  {
    char x = previous_location == 1 ? character->get_prev_x() : character->get_x();
    char y = previous_location == 1 ? character->get_prev_y() : character->get_y();
    char element_type = map->get_element_type_on_coord(x, y);
    if ((element_type == GRASS_FIELD || element_type == CHARACTER) &&
        character->get_current_bombs_count() < MAX_BOMBS_COUNT)
    {
      GameObjects::Bomb *bomb = new GameObjects::Bomb(next_bomb_object_id(), map, character, x, y);
      BombExplodeCallbackParams* params = new BombExplodeCallbackParams(this, bomb);
      std::thread(bomb_explode_callback, params).detach();
      map->add_bomb(bomb);
      server->send_place_bomb(bomb->get_object_id(), x, y);
    }
  }
}

void GameLogic::unregister_character(Client* client)
{
  unsigned char char_id = client->get_character_object_id();
  map->remove_character(char_id);
  D(std::cerr << "CHAR LEAVING: " << (int) char_id << std::endl);
  server->send_delete_character(char_id);
}

void GameLogic::bomb_explode_callback(void *params)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(BOMB_EXPLODE_TIME));

  BombExplodeCallbackParams *casted_params = (BombExplodeCallbackParams*) params;
  GameObjects::Bomb *bomb = casted_params->bomb;
  GameLogic *game_logic = casted_params->game_logic;
  delete casted_params;

  if (!bomb->is_removed())
  {
    GameObjects::ExplodeResults results = bomb->explode();
    game_logic->get_map()->remove_bomb(bomb->get_object_id());
    if (!game_logic->is_restarting())
    {
      if (results.field_xys_count > 0)
        game_logic->get_server()->send_destruct_cell(results.field_xys, results.field_xys_count);
      if (results.player_ids_count > 0)
        game_logic->get_server()->send_kill_character(results.player_ids, results.player_ids_count);
      delete[] results.field_xys;
      delete[] results.player_ids;

      if (game_logic->get_map()->get_alive_characters_count() < 2)
        game_logic->restart();
    }
  }
  else
    delete bomb;
}

char* GameLogic::get_current_full_state()
{
  return map->dump_to_binary_string();
}

void GameLogic::restart()
{
  restarting = true;
  std::this_thread::sleep_for(std::chrono::milliseconds(RESTART_DELAY));
  map->init_from_file(DEFAULT_MAP_PATH);
  for(auto bomb : *map->get_bombs())
    bomb->remove();
  map->get_bombs()->clear();
  for(std::list<GameObjects::Character*>::iterator character = map->get_characters()->begin();
      character != map->get_characters()->end();
      character++)
  {
    GameObjects::Coord free_space = map->find_free_space();
    (*character)->set_x(free_space.x);
    (*character)->set_y(free_space.y);
    (*character)->respawn();
  }
  restarting = false;
  server->send_full_state();
}

unsigned char GameLogic::next_bomb_object_id()
{
  current_bomb_object_id = (current_bomb_object_id + 1) % 255;
  return current_bomb_object_id;
}

unsigned char GameLogic::next_player_object_id()
{
  current_player_object_id = (current_player_object_id + 1) % 255;
  return current_player_object_id;
}

void GameLogic::init_scene()
{
  map = new GameObjects::Map(DEFAULT_MAP_PATH);
}
