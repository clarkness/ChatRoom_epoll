#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <iostream>

using namespace std;

#define MAXSIZE     1024
#define IPADDRESS   "127.0.0.1"
#define SERV_PORT   8787
#define FDSIZE        1024
#define EPOLLEVENTS 20

static void handle_connection(int sockfd);
static void 
handle_events(int epollfd, struct epoll_event *events, int num, int sockfd, char *buf);
static void do_read(int epollfd, int fd, int sockfd, char *buf);
static void do_read(int epollfd, int fd, int sockfd, char *buf);
static void do_write(int epollfd, int fd, int sockfd, char *buf);
static void add_event(int epollfd, int fd, int state);
static void delete_event(int epollfd, int fd, int state);
static void modify_event(int epollfd, int fd, int state);


int main(int argc, char *argv[])
{
	for (int i = 0; i < 600; ++i) {//创建10个进程，模拟并发
		int pid = fork();
		if (pid == 0) {
			break;
		}
		usleep(800);
	}
	int sockfd;
	struct sockaddr_in  servaddr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	//servaddr.sin_addr.s_addr = inet_addr(IPADDRESS);
	//cout << STDIN_FILENO << ' ' << STDOUT_FILENO  << endl;
	inet_pton(AF_INET, IPADDRESS, &servaddr.sin_addr);
	connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	//处理连接
	//handle_connection(sockfd);
	char buf[1024] = "client send you a message\n";
	int nwrite = send(sockfd, buf, strlen(buf), 0);
	if (nwrite == -1)
	{
		perror("write error:");
		close(sockfd);
	}
	usleep(4096);
	close(sockfd);
		
	return 0;
}


static void handle_connection(int sockfd)
{
	int epollfd;
	struct epoll_event events[EPOLLEVENTS];
	char buf[MAXSIZE];
	int ret;
	epollfd = epoll_create(FDSIZE);
	add_event(epollfd, STDIN_FILENO, EPOLLIN);
	add_event(epollfd, sockfd, EPOLLIN);
	for (; ; )
	{
		ret = epoll_wait(epollfd, events, EPOLLEVENTS, 500);
		handle_events(epollfd, events, ret, sockfd, buf);
		if (strcmp(buf, "exit\n") == 0)
			break;
	}
	close(epollfd);
}

static void
handle_events(int epollfd, struct epoll_event *events, int num, int sockfd, char *buf)
{
	int fd;
	int i;
	for (i = 0; i < num; i++)
	{
		fd = events[i].data.fd;
		if (events[i].events & EPOLLIN)
			do_read(epollfd, fd, sockfd, buf);
		else if (events[i].events & EPOLLOUT)
			do_write(epollfd, fd, sockfd, buf);
	}
}

static void do_read(int epollfd, int fd, int sockfd, char *buf)
{
	int nread;
	nread = read(fd, buf, MAXSIZE);	//recv()函数不能读取标准输入文件描述符
	if (nread == -1)
	{
		perror("read error:");
		close(fd);
	}
	else if (nread == 0)
	{
		fprintf(stderr, "server close.\n");
		close(fd);
	}
	else
	{
		if (fd != sockfd) {
			if(strcmp(buf, "exit\n") != 0)
				modify_event(epollfd, sockfd, EPOLLOUT);
		}
		else
		{
			delete_event(epollfd, sockfd, EPOLLIN);
			add_event(epollfd, STDOUT_FILENO, EPOLLOUT);
		}
	}
}

static void do_write(int epollfd, int fd, int sockfd, char *buf)
{
	int nwrite;
	nwrite = send(fd, buf, strlen(buf), 0);
	if (nwrite == -1)
	{
		perror("write error:");
		close(fd);
	}
	else
	{
		if (fd == STDOUT_FILENO) {
			delete_event(epollfd, fd, EPOLLOUT);
			add_event(epollfd, STDIN_FILENO, EPOLLIN);
		}
		else
			modify_event(epollfd, fd, EPOLLIN);
	}
	memset(buf, 0, MAXSIZE);
}

static void add_event(int epollfd, int fd, int state)
{
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = fd;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

static void delete_event(int epollfd, int fd, int state)
{
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = fd;
	epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
}

static void modify_event(int epollfd, int fd, int state)
{
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = fd;
	epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}
