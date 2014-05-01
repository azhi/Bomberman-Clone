#ifndef __GAME_LOGIC_H_
#define __GAME_LOGIC_H_

#include "client.h"
#include "game_objects/map.h"
#include "game_objects/bomb.h"

#define RESTART_DELAY 2000

class Server;

class GameLogic
{
  public:
    GameLogic(Server *server);
    ~GameLogic();

    void register_new_character(Client* client);
    void move_character(char object_id, char direction);
    void place_bomb(char object_id, char previous_location);
    void unregister_character(Client* client);

    void restart();

    char* get_current_full_state();

    static void bomb_explode_callback(void *params);

    GameObjects::Map* get_map(){ return map; };
    Server* get_server(){ return server; };

  private:
    unsigned char next_bomb_object_id();
    unsigned char next_player_object_id();

    void init_scene();

    unsigned char current_bomb_object_id, current_player_object_id;
    GameObjects::Map *map;
    Server *server;
};

struct BombExplodeCallbackParams
{
  BombExplodeCallbackParams(GameLogic* game_logic, GameObjects::Bomb* bomb) : game_logic(game_logic), bomb(bomb) {};
  GameLogic *game_logic;
  GameObjects::Bomb* bomb;
};

#endif

