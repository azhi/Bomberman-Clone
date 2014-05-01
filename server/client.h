#ifndef __CLIENT_H_
#define __CLIENT_H_

#include <netinet/in.h>
#include "utils/write_thread.h"

class Client
{
  public:
    Client(sockaddr_in *sock_addr, socklen_t sock_addr_len);
    ~Client();

    void set_character_object_id(char character_object_id){ this->character_object_id = character_object_id; };
    char get_character_object_id(){ return character_object_id; };
    sockaddr_in* get_sock_addr(){ return sock_addr; };

    void async_write(char *msg, int msg_len, Utils::WriteThread *write_thread);

  private:
    sockaddr_in *sock_addr;
    socklen_t sock_addr_len;
    char character_object_id;
};

#endif
