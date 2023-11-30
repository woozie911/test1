#include "./head_files/tcp_client.h"

client::client(int port,std::string ip)
{
    this->server_ip=ip;
    this->server_port=port;
}
client::~client()
{
    close(this->sock);
}

void client::run()
{
    this->sock=socket(AF_INET,SOCK_STREAM,0);
    // 定义servaddr
    struct sockaddr_in servaddr;

    // 清零
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(80);
    servaddr.sin_addr.s_addr=inet_addr("172.18.0.2");

    char server_ip[INET_ADDRSTRLEN];
    if (connect(this->sock,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
    {
        perror("connect error");
        exit(1);
    }
    std::cout<<"连接服务器成功"<<std::endl;
    std::cout<<"客户端IP："<<inet_ntop(AF_INET,&(servaddr.sin_addr),server_ip,INET_ADDRSTRLEN)<<"端口："<<ntohs(servaddr.sin_port)<<std::endl;

    std::thread send_t(this->SendMsg,this->sock);
    send_t.join();
    std::cout<<"发送完毕"<<std::endl;
    return;
}

void client::SendMsg(int conn)
{
    while (1)
    {
        std::string send_str;
        std::cin>>send_str;
        int len=send(conn,send_str.c_str(),send_str.length(),0);
        //对端关闭或者recv出错
        if (len<=0)
        {
            perror("send error");
            exit(1);
        }
    }
}