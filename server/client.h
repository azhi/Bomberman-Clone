#ifndef __CLIENT_H_
#define __CLIENT_H_

#include <netinet/in.h>

class Client
{
  public:
    Client(int socket_fd, sockaddr_in *sock_addr, socklen_t sock_addr_len);
    ~Client();

    void set_character_object_id(char character_object_id){ this->character_object_id = character_object_id; };
    char get_character_object_id(){ return character_object_id; };
    sockaddr_in* get_sock_addr(){ return sock_addr; };

    void write(char* msg, size_t msg_len);

  private:
    int socket_fd;
    sockaddr_in *sock_addr;
    socklen_t sock_addr_len;
    char character_object_id;
};

#endif
