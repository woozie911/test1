#include "./head_files/tcp_server.h"
#include<pthread.h>
#include<condition_variable>


std::string msg_str;
std::mutex mtx;
std::condition_variable cv;
bool is_comlete=false;
int current_thread=1;

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
        std::cout<<str<<std::endl;
        if (str=="work")
        {
            std::thread t1(server::Handler,"Cpp is",1);
            std::thread t2(server::Handler,"the best",2);
            std::thread t3(server::Handler,"language",3);
            t1.join();
            t2.join();
            t3.join();
        }
        std::cout<<msg_str<<std::endl;
        //current_thread=1;
    }
    close(conn);
}

void server::Handler(std::string str,int number)
{
    std::unique_lock<std::mutex> lock(mtx);
    int i=current_thread;
    cv.wait(lock,[i,number]{return number==i;});

    //线程执行的任务
    msg_str += str;
    msg_str += " ";

    ++current_thread;

    cv.notify_all();
}