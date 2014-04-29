#ifndef __SERVER_H_
#define __SERVER_H_

#define BUF_SIZE 10240

#include "game_logic.h"
#include "client.h"

#include <list>
#include <netinet/in.h>

class Server
{
  public:
    Server(int port);
    ~Server();

    void listen();

    void send_full_state();
    void send_create_character(char object_id, char x, char y, char last_move);
    void send_destruct_cell(char x, char y);

    void send_to_all_clients(char* msg, size_t msg_len);

    void add_client(Client* client);
    void remove_client(char object_id);
    Client* find_client_by_sockaddr(sockaddr_in *sockaddr);
    Client* find_client_by_character_id(char character_object_id);

  private:
    void init_socket(int port);
    void process_single_msg(sockaddr_in *sock_addr, socklen_t address_len);

    int socket_fd;
    char* buf;

    GameLogic *game_logic;

    std::list<Client*> *clients;
};

#endif
