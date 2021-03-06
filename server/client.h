#ifndef __CLIENT_H_
#define __CLIENT_H_

#include <netinet/in.h>
#include "utils/write_thread.h"

#define MAX_SENTS_WITHOUT_SYNC 30

class GameLogic;

class Client
{
  public:
    Client(GameLogic *game_logic, Utils::WriteThread *write_thread, sockaddr_in *sock_addr, socklen_t sock_addr_len);
    ~Client();

    void set_character_object_id(char character_object_id){ this->character_object_id = character_object_id; };
    char get_character_object_id(){ return character_object_id; };
    sockaddr_in* get_sock_addr(){ return sock_addr; };

    void async_write(char *msg, int msg_len, bool force_full_state = false);
    void process_first_send(char *msg, int msg_len);

  private:
    void do_async_write(char *msg, int msg_len);
    void send_full_state();

    GameLogic* game_logic;
    Utils::WriteThread *write_thread;
    sockaddr_in *sock_addr;
    socklen_t sock_addr_len;
    char character_object_id;

    bool sent_create_character = false;
    int sents_without_sync = 0;
};

#endif
