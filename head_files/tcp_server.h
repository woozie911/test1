#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <thread>
#include <sys/epoll.h>
#include <mutex>

#define MAXLIINE 10
class server
{
    private:
        int server_port;
        int server_sockfd;
        std::string server_ip;

    public:
        server(int port,std::string ip);
        ~server();

        void run();
        static void HandleClient(int conn);  //子线程工作的静态函数
        static void Handler(std::string str,int number,int &current_thread);

};

#endif