#include "./head_files/tcp_client.h"

int main()
{
    client clit(80,"172.17.0.2");
    clit.run();
    return 0;
}