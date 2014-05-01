#ifndef __SERVER_H_
#define __SERVER_H_

#define BUF_SIZE 10240

#include "game_logic.h"
#include "client.h"
#include "utils/listen_thread.h"

#include <list>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <netinet/in.h>

struct ProcessJobParams
{
  char *buf;
  int buf_len;
  sockaddr_in *sock_addr;
  socklen_t address_len;
};

class Server
{
  public:
    Server(int port);
    ~Server();

    void process();

    void send_full_state();
    void send_create_character(char object_id, char x, char y, char last_move);
    void send_destruct_cell(char x, char y);

    void send_to_all_clients(char* msg, size_t msg_len);

    void add_client(Client* client);
    void remove_client(char object_id);
    Client* find_client_by_sockaddr(sockaddr_in *sockaddr);
    Client* find_client_by_character_id(char character_object_id);

  private:
    void process_single_msg(char* buf, size_t buf_len, sockaddr_in *sock_addr, socklen_t address_len);

    GameLogic *game_logic;
    Utils::ListenThread *listen_thread;
    Utils::WriteThread *write_thread;

    std::list<Client*> *clients;

    std::queue<ProcessJobParams> *process_queue;
    std::mutex *process_queue_mutex;
    std::condition_variable *cv_process_queue;
};

#endif
