#ifndef DETAILJOB_H_
#define DETAILJOB_H_

#include<BaseJob.h>

extern unordered_set<int> cfd;	//保存所有客户端的套接字，用于广播消息

class DetailJob : virtual public BaseJob {
public:
	DetailJob(int num = -1) :BaseJob(num) {}
	DetailJob(int efd, struct epoll_event eve, int serfd, char s[]) :BaseJob(++JobId), epollfd(efd), event(eve), listenfd(serfd), buf(s) {}
	~DetailJob();
private:
	static void handle_events(int epollfd, struct epoll_event event, int listenfd, char *buf); //事件处理函数
	static void handle_accept(int epollfd, int listenfd);	//处理接收到的连接
	//void send_msg(int epollfd, int fd, char* buf);	//广播消息
	static void do_read(int epollfd, int fd, char* buf);	//读处理
	static void do_write(int epollfd, int fd, char* buf);	//写处理
	static void add_event(int epollfd, int fd, int state);	//添加事件
	static void modify_event(int epollfd, int fd, int state);	//修改事件
	static void delete_event(int epollfd, int fd, int state);	//删除事件
public:
	void Run();
	const static int MAXSIZE = 1024;
private:
	int epollfd;
	struct epoll_event event;
	int listenfd;
	char *buf;
	static int JobId;
};

#endif /* DETAILJOB_H_ */