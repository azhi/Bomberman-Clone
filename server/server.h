#ifndef __SERVER_H_
#define __SERVER_H_

#define BUF_SIZE 10240

#include "game_logic.h"
#include "client.h"
#include "utils/dispatcher_thread.h"

#include <list>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <netinet/in.h>

struct ProcessJobParams
{
  char *buf;
  int buf_len;
  Client *client;
};

class Server
{
  public:
    Server(int port);
    ~Server();

    GameLogic *get_game_logic(){ return game_logic; };

    void process();

    void send_full_state();
    void send_create_character(char object_id, char x, char y, char last_move);
    void send_delete_character(char object_id);
    void send_move_character(char object_id, char x, char y, char last_move);
    void send_kill_character(char *object_ids, int object_count);
    void send_place_bomb(char object_id, char x, char y);
    void send_destruct_cell(char *xys, int xys_length);

    void send_to_all_clients(char* msg, size_t msg_len, bool force_full_state = false);

    void add_client(Client* client);
    void remove_client(char object_id);

    std::queue<ProcessJobParams> *process_queue;
    std::mutex *process_queue_mutex;
    std::condition_variable *cv_process_queue;

  private:
    void process_single_msg(char* buf, size_t buf_len, Client *client);

    GameLogic *game_logic;
    Utils::DispatcherThread *dispatcher_thread;

    std::list<Client*> *clients;
};

#endif
