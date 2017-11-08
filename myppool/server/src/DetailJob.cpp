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
		//���������������ͺ��¼����ͽ��д���
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
	//���������׽���Ϊ������ģʽ���׽�������Ϊ������ģʽ���ڵ���Windows Sockets API����ʱ�����ú������������ء�
	//���������£���Щ�������ö�����á�ʧ�ܡ���������WSAEWOULDBLOCK������롣˵������Ĳ����ڵ����ڼ���û��ʱ����ɡ�
	//ͨ����Ӧ�ó�����Ҫ�ظ����øú�����ֱ����óɹ����ش��롣
	fcntl(clifd, F_SETFL, fcntl(clifd, F_GETFD, 0) | O_NONBLOCK);	//???
	if (clifd == -1) {
		perror("accept error: ");
		exit(1);
	}
	else {
		printf("connect a new client!\n");
		cfd.insert(clifd);
		add_event(epollfd, clifd, EPOLLIN);	//���һ���ͻ����������¼�
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
		printf("client %d: %s", fd, buf);//�����Լ������������û��Ҫ����������Ѿ����ļ�������		
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
	ev.data.fd = fd;	//����û����ݣ�һ�㷵�ؿ�����ָ��
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

void DetailJob::modify_event(int epollfd, int fd, int state) {
	struct epoll_event ev;
	ev.events = state | EPOLLET;
	ev.data.fd = fd;	//����û����ݣ�һ�㷵�ؿ�����ָ��
	epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}

void DetailJob::delete_event(int epollfd, int fd, int state) {
	struct epoll_event ev;
	ev.events = state | EPOLLET;
	ev.data.fd = fd;	//����û����ݣ�һ�㷵�ؿ�����ָ��
	epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
}
