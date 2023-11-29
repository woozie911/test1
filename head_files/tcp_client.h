#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <iostream>
#include <thread>
#include <vector>

class client
{
    private:
        int server_port;
        std::string server_ip;
        int sock;   //与服务器建立连接的套接字描述符

    public:
        client(int port,std::string ip);
        ~client();
        void run();
        static void SendMsg(int conn);
        // static void RecvMsg(int conn);

};
#endif