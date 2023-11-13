#include<iostream>
#include<string>
#include<sys/epoll.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<map>
#include<unordered_map>
#include<stdio.h>
#include"ch.h"
#include"round.h"
#include"net.h"

//最大连接数
const int MAX_CONN = 1024;
//玩家数量
int player_num = 2;
//玩家计数
int player_cnt = 0;
//建立回合处理
std::unordered_map<int, ch> Round::players;
//建立网络模块
Net* net = Net::getInstance();
Net* Net::instance;


//删除角色数据
void delete_player(int fd)
{
    //客户端断开连接
    std::string name = net->sock_to_name(fd);
    printf("client %s exit\n", name.c_str());
    Round::delete_player(fd);
    close(fd);
    epoll_ctl(net->get_sock(), EPOLL_CTL_DEL, fd, 0);
    net->clients.erase(fd);
    player_cnt--;
    return;
}

int main()
{
    restart:
    while(1)
    {
        bool game_start = false;//游戏是否开始
        struct epoll_event evs[MAX_CONN];
        //游戏开始之前玩家加入阶段
        while(!game_start)
        {
            int n = epoll_wait(net->get_epoll(), evs, MAX_CONN, -1);//n为有多少个客户端连接 若没有则阻塞
            if(n < 0)
            {
                printf("epoll_wait error\n");
                break;
            }
            //等待玩家加入
            for(int i = 0; i < n; i ++)
            {
                int fd = evs[i].data.fd;//fd为检测到的socket
                //如果是监听的fd收到消息，那么则是有客户端进行连接了
                if(fd == net->get_sock())
                {
                    struct sockaddr_in client_addr;
                    socklen_t client_addr_len = sizeof(client_addr);
                    int client_sockfd = accept(net->get_sock(), (struct sockaddr*)&client_addr, &client_addr_len);//接收客户端到client_addr
                    if(client_sockfd < 0)
                    {
                        printf("连接出错\n");
                        continue;
                    }
                    //将客户端的socket加入epoll
                    struct epoll_event ev_client;
                    ev_client.events = EPOLLIN;//检测客户端有没有消息过来
                    ev_client.data.fd = client_sockfd;

                    int ret = epoll_ctl(net->get_epoll(), EPOLL_CTL_ADD, client_sockfd, &ev_client);//把客户端信息放入epoll池中
                    if(ret < 0)
                    {
                        printf("client epoll_ctl error\n");
                        break;
                    }
                    printf("%d is connecting\n", client_addr.sin_addr.s_addr);

                    //保存客户端信息
                    Client client;
                    client.sockfd = client_sockfd;
                    client.name = "";
                    net->clients[client_sockfd] = client;
                }
                else
                {
                    char buffer[1024];
                    int n = read(fd, buffer, 1024);//获取消息
                    printf("read %d bytes\n", n);
                    if(n < 0)
                    {
                        printf("user exit\n");
                    }
                    else if(n == 0)
                    {
                        delete_player(fd);
                        goto restart;
                    }
                    else
                    {
                        std::string msg(buffer, n);
                        if(net->clients[fd].name == "")
                        {
                            //如果该客户端name为空，说明该消息是这个客户端的用户名
                            net->clients[fd].name = msg;
                            //初始化角色信息
                            ch player;
                            player.init(msg, (double)player_cnt * 5.0, 0.0);
                            player_cnt = Round::add_player(fd, player);//把玩家加入回合系统中
                            printf("We have %d player name %s\n", player_cnt, msg.c_str());
                            if(player_cnt == player_num)
                            {
                                printf("游戏开始\n");
                                net->send_all_msg("S");
                                net->send_all_msg("游戏开始\n");
                                game_start = true;
                            }
                        }
                    }
                }
            }
        }
        int done_num = 0;//回合结束的玩家数量
        //建立回合处理
        Round round;
        //进入游戏回合阶段
        while(game_start)
        {
            int n = epoll_wait(net->get_epoll(), evs, MAX_CONN, -1);//n为有多少个客户端连接 若没有则阻塞
            if(n < 0)
            {
                printf("epoll_wait error\n");
                break;
            }
            for(int i = 0; i < n; i ++)
            {
                int fd = evs[i].data.fd;//fd为检测到的socket
                char buffer[1024];
                int n = read(fd, buffer, 1024);//获取消息
                buffer[n] = '\0';
                printf("read:%s\n", buffer);
                if(n < 0)
                {
                    printf("user exit\n");
                }
                else if(n == 0)
                {
                    //客户端断开连接
                    delete_player(fd);
                    net->send_all_msg(std::string("有玩家退出,重新开始游戏\n"));
                    goto restart;
                }
                else
                {
                    std::string msg(buffer, n);
                    round.handle_msg(msg);
                    done_num ++;
                    if(done_num == player_num)
                    {
                        std::string result = round.handle_round();
                        net->send_all_msg("S");
                        net->send_all_msg(result);
                        done_num = 0;
                        
                    }
                }
            }
        }
    }
    
    //关闭实例
    net->close_net();
}