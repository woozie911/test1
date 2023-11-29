#include "./head_files/tcp_client.h"

int main()
{
    client clit(80,"127.0.0.1");
    clit.run();
    return 0;
}