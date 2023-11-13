#ifndef NET_H
#define NET_H
#include<iostream>
#include<string>
#include<sys/epoll.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<map>
#include<stdio.h>

//保存客户端信息
struct Client
{
    int sockfd;//sock
    std::string name;//名字
};

class Net
{
public:
    //创建单例
    static Net* getInstance();
    //获取epoll实例
    int get_epoll();
    //获取监听socket
    int get_sock();
    //保存客户端信息
    std::map<int, struct Client> clients;
    //向指定用户发送信息
    int send_msg(std::string name, std::string msg);

    //向所有人发送消息
    int send_all_msg(std::string msg);
    //名字对应的fd
    int name_to_sock(std::string name);
    //fd对应的名字
    std::string sock_to_name(int fd);
    //关闭网络模块
    void close_net();
    ~Net();

private:
    Net();
    //单例
    static Net* instance;
    
    

    //最大连接数
    const int MAX_CONN = 1024;
    
    //创建监听socket
    int sockfd;
    //创建epoll实例
    int epld;

};



#endif