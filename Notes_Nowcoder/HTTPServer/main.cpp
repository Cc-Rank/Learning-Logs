#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <signal.h>

#include "locker.h"
#include "ThreadPool.h"
#include "http_conn.h"

#define MAX_FD 65536           //最大的文件描述符个数
#define MAX_EVENT_NUMBER 10000 //监听的最大事件数

// 添加信号捕捉
void addsig(int sig, void(handler)(int))
{
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = handler;
    sigfillset(&sa.sa_mask);
    sigaction(sig, &sa, NULL);
}

// 添加文件描述符到 epoll 中
extern void addfd(int epollfd, int fd, bool one_shot);
// 从 epoll 中删除文件描述符
extern void removefd(int epollfd, int fd);

int main(int argc, char* argv[])
{
    if( argc <= 1 ) 
    {
        printf( "usage: %s port_number\n", basename(argv[0]));
        return 1;
    }

    // 获取端口号
    int port = atoi(argv[1]);
    addsig(SIGPIPE, SIG_IGN);

    // 创建线程池
    ThreadPool<http_conn> * pool = NULL;
    try {
        pool = new ThreadPool<http_conn>;
    } catch (...) {
        return 1;
    }

    // 创建一个数组用于保存所有的客户端信息
    http_conn *users = new http_conn[MAX_FD];
    
    int listenfd = socket(PF_INET, SOCK_STREAM, 0);

    // 设置端口复用
    int reuse = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    // 绑定
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    bind(listenfd, (struct sockaddr*)&address, sizeof(address));

    // 监听
    listen(listenfd, 5);

    // 创建 epoll 对象，事件数组，添加
    epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(5);

    // 将监听的文件描述符添加到 epoll
    addfd(epollfd, listenfd, false);
    http_conn::m_epollfd = epollfd;

    while (true)
    {
        int num = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
        if ((num < 0) && (errno != EINTR))
        {
            printf("epoll failure\n");
            break;
        }

        // 循环遍历事件数组
        for (int i = 0; i < num; ++i)
        {
            int sockfd = events[i].data.fd;
            if (sockfd == listenfd)
            {
                struct sockaddr_in client_address;
                socklen_t client_addrlen = sizeof(client_address);
                int connfd = accept(listenfd, (struct sockaddr *)&client_address, &client_addrlen);
                if ( connfd < 0 ) 
                {
                    printf( "errno is: %d\n", errno );
                    continue;
                }
                
                if (http_conn::m_user_count >= MAX_FD)
                {   // 目前连接数满了
                    // 给客户端写一个信息：服务器内部正忙。
                    close(connfd);
                    continue;
                }
                // 将新的客户数据初始化，放到数组中
                users[connfd].init(connfd, client_address);
            }
            else if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
            {   // 对方异常断开或者错误等事件
                users[sockfd].close_conn();
            }
            else if (events[i].events & EPOLLIN)
            {
                if (users[sockfd].read())   // 一次性把所有数据都读完
                    pool->append(users + sockfd);
                else
                    users[sockfd].close_conn();
            }
            else if (events[i].events & EPOLLOUT)
            {
                if (!users[sockfd].write())   // 一次性写完所有数据
                    users[sockfd].close_conn();
            }
        }
    }

    close(epollfd);
    close(listenfd);
    delete [] users;
    delete pool;

    return 0;
}