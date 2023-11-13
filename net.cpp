#include"net.h"

Net* Net::getInstance()
{
    // 若为空则创建
    if (instance == nullptr)
    instance = new Net();
    return instance;
}


Net::Net()
{
    //创建监听socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);//相当于网络通讯的电话号
    if(sockfd < 0)
    {
        perror("socket create error");
    }

    //绑定本地ip和端口
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;//使用ipv4
    addr.sin_addr.s_addr = htonl(INADDR_ANY);//绑定本地能绑定的所有ip地址
    addr.sin_port = htons(9999); //设置端口号

    int ret = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));//绑定电话和电话号码
    if(sockfd < 0)
    {
        perror("bind error");
    }

    //监听客户端
    ret = listen(sockfd, 1024);//把监听端设置为监听状态
    if(ret < 0)
    {
        printf("listen error\n");
    }

    //创建epoll实例
    epld = epoll_create1(0);
    if(epld < 0)
    {
        perror("epoll create error");
    }

    //将监听的socket加入epoll
    struct epoll_event ev;
    ev.events = EPOLLIN;//设为读取状态
    ev.data.fd = sockfd;

    ret = epoll_ctl(epld, EPOLL_CTL_ADD, sockfd, &ev);//把监听端信息放入epoll池中
    if(ret < 0)
    {
        printf("epoll_ctl error\n");
    }
};

int Net::get_epoll()
{
    return epld;
}

int Net::get_sock()
{
    return sockfd;
}

int Net::send_msg(std::string name, std::string msg)
{
    int sock = name_to_sock(name);
    if(sock < 0)
    {
        printf("name not found\n");
        return -1;
    }

    int ret = send(sock, msg.c_str(), msg.size(), 0);
    if(ret < 0)
    {
        perror("send error");
    }
    return ret;
}

int Net::send_all_msg(std::string msg)
{
    int ret = 0;
    for(auto& c : clients)
    {
        ret += send(c.first, msg.c_str(), msg.size(), 0);
        printf("send:%s", msg.c_str());
    }
    return ret;
}

void Net::close_net()
{
    //关闭实例
    close(epld);
    close(sockfd);
}

Net::~Net()
{
    close_net();
}

int Net::name_to_sock(std::string name)
{
    for(auto& c : clients)
    {
        if(c.second.name == name) return c.first;
    }
    return -1;
}

std::string Net::sock_to_name(int fd)
{
    return clients[fd].name;
}
