#include "./head_files/tcp_server.h"
#include<pthread.h>
#include<condition_variable>


std::string msg_str;
std::mutex m;
bool is_comlete=false;
int flag=1;

server::server(int port,std::string ip)
{
    this->server_port=port;
    this->server_ip=ip;
}
server::~server()
{
    close(this->server_sockfd);
}
void server::run()
{
    //定义serer_sockfd
    this->server_sockfd=socket(AF_INET,SOCK_STREAM,0);

    //设置端口复用
    int enable=-1;
    setsockopt(this->server_sockfd,SOL_SOCKET,SO_REUSEADDR,&enable,sizeof(int));

    //定义sockaddr_in
    struct sockaddr_in serv_addr;
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(this->server_port);
    // serv_addr.sin_addr.s_addr=INADDR_ANY;
    inet_pton(AF_INET,server_ip.c_str(),&serv_addr.sin_addr.s_addr);

    //bind绑定地址结构块和套接字描述符
    if (bind(this->server_sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
    {
        perror("bind error");
        exit(1);
    }

    //listen
    listen(this->server_sockfd,128);

    //客户端套接字
    struct sockaddr_in client_addr;
    socklen_t client_addr_len=sizeof(client_addr);


    int connfd=accept(this->server_sockfd,(struct sockaddr*)&client_addr,&client_addr_len);
    char client_ip[INET_ADDRSTRLEN];
    if(connfd<0)
    {
        perror("accept error");
        exit(1);
    }
    std::cout<<"文件描述符为"<<connfd<<"的客户端连接成功"<<std::endl;
    std::cout<<"客户端IP："<<inet_ntop(AF_INET,&(client_addr.sin_addr),client_ip,INET_ADDRSTRLEN)<<"端口："<<ntohs(client_addr.sin_port)<<std::endl;

    //处理客户端的请求
    HandleClient(connfd);

}

void server::HandleClient(int conn)
{

    while (1) {
        if (is_comlete)
        {
            break;
        }
        
        char recv_str[1024];
        memset(recv_str, 0, sizeof(recv_str));
        int n = recv(conn, recv_str, sizeof(recv_str), 0);
        if (n < 0) {
            // 处理接收错误或连接关闭的情况
            perror("recv error");
            break;
        } else if (n == 0) {
            // 连接关闭
            std::cout << "客户端断开连接" << std::endl;
            break;
        }

        std::string str(recv_str);
        for (int i = 0; i < 3; i++)
        {
            /* code */
        }
        
        std::thread t(server::Handler,str);
        t.detach();

    }
    close(conn);
}

void server::Handler(std::string str)
{
    m.lock();
    if (str == "complete") {
        is_comlete = true;
        std::cout << msg_str << std::endl;
    } else {
        msg_str += str;
        msg_str += ";";
    }
    m.unlock();
}