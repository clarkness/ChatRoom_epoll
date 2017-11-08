#include "DetailJob.h"

//DetailJob::DetailJob() {
//	// TODO Auto-generated constructor stub
//}
int DetailJob::JobId = 0;
DetailJob::~DetailJob() {
	// TODO Auto-generated destructor stub
}

void DetailJob::Run() {
	handle_events(epollfd, event, listenfd, buf);
}

void DetailJob::handle_events(int epollfd, struct epoll_event event, int listenfd, char *buf) {
	int fd;
	fd = event.data.fd;
		//根据描述符的类型和事件类型进行处理
	if (fd == listenfd && (event.events & EPOLLIN))
		handle_accept(epollfd, fd);
	else if (event.events & EPOLLIN)
		do_read(epollfd, fd, buf);
	else
		do_write(epollfd, fd, buf);
}


void DetailJob::handle_accept(int epollfd, int listenfd) {
	int clifd;
	struct sockaddr_in cliaddr;
	socklen_t cliaddrlen;
	clifd = accept(listenfd, (struct sockaddr*) &cliaddr, &cliaddrlen);
	//设置连接套接字为非阻塞模式，套接字设置为非阻塞模式后，在调用Windows Sockets API函数时，调用函数会立即返回。
	//大多数情况下，这些函数调用都会调用“失败”，并返回WSAEWOULDBLOCK错误代码。说明请求的操作在调用期间内没有时间完成。
	//通常，应用程序需要重复调用该函数，直到获得成功返回代码。
	fcntl(clifd, F_SETFL, fcntl(clifd, F_GETFD, 0) | O_NONBLOCK);	//???
	if (clifd == -1) {
		perror("accept error: ");
		exit(1);
	}
	else {
		printf("connect a new client!\n");
		cfd.insert(clifd);
		add_event(epollfd, clifd, EPOLLIN);	//添加一个客户描述符和事件
	}
}

void DetailJob::do_read(int epollfd, int fd, char* buf) {
	int nread;
	memset(buf, 0, MAXSIZE);
	nread = read(fd, buf, MAXSIZE);
	if (nread == -1) {
		perror("read error: ");
		exit(1);
	}
	else if (nread == 0) {
		fprintf(stderr, "client %d close.\n", fd);
		cfd.erase(fd);
		close(fd);
		delete_event(epollfd, fd, EPOLLIN);
	}
	else {
		printf("client %d: %s", fd, buf);//多试试几种输入输出，没必要，输入输出已经成文件被监听		
	}
}

void DetailJob::do_write(int epollfd, int fd, char* buf) {
	int nwrite;
	nwrite = write(fd, buf, strlen(buf));
	if (nwrite == -1) {
		perror("write error: ");
		exit(1);
	}
	else if (nwrite == 0) {
		fprintf(stderr, "client close.\n");
		close(fd);
		delete_event(epollfd, fd, EPOLLOUT);
	}
	else {
			modify_event(epollfd, fd, EPOLLIN);			
	}
		
}

void DetailJob::add_event(int epollfd, int fd, int state) {
	struct epoll_event ev;
	ev.events = state | EPOLLET;
	ev.data.fd = fd;	//这个用户数据，一般返回空类型指针
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

void DetailJob::modify_event(int epollfd, int fd, int state) {
	struct epoll_event ev;
	ev.events = state | EPOLLET;
	ev.data.fd = fd;	//这个用户数据，一般返回空类型指针
	epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}

void DetailJob::delete_event(int epollfd, int fd, int state) {
	struct epoll_event ev;
	ev.events = state | EPOLLET;
	ev.data.fd = fd;	//这个用户数据，一般返回空类型指针
	epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
}
