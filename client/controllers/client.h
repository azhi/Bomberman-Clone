#ifndef __CLIENT_H_
#define __CLIENT_H_

#define BUF_SIZE 10240

#include "../game_objects/map.h"
#include <netinet/in.h>

class Client
{
  public:
    Client(long ip, int port, GameObjects::Map* map, SDLWrapper *sdl_wrapper);
    ~Client();

    void write(char* cmd);
    void read_and_process();

    void do_register();

    void send_register();
    void send_character_move(char move_direction);
    void send_place_bomb(bool previous_location);

  private:
    void init_socket();
    void process_single_msg();

    int wait_for_socket(timeval* tv);

    void process_add_character(bool main_character);

    int socket_fd;
    fd_set rds;
    long ip;
    int port;
    char* buf;

    GameObjects::Map *map;
    SDLWrapper *sdl_wrapper;
};

#endif
