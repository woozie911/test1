#include "./head_files/tcp_server.h"

int main()
{
    server serv(80,"172.18.0.2");
    serv.run();
    return 0;
}