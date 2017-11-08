#include <iostream>
#include "DetailJob.h"
#include "MyThreadPool.h"
using namespace std;

#define IPADDRESS	"127.0.0.1"
#define PORT		8787
#define MAXSIZE		1024
#define LISTENQ		5
#define FDSIZE		1000	//����epollʱָ�������׽��ֵ�����
#define EPOLLEVENTS	100		//�ں˷������¼������׽��ֵ����������С��FDSIZE

unordered_set<int> cfd;	//�������пͻ��˵��׽��֣����ڹ㲥��Ϣ

static int socket_bind(const char* ip, int port);
static void do_epoll(int listenfd);
static void send_msg(int epollfd, int fd, char* buf);
static void add_event(int epollfd, int fd, int state);
static void modify_event(int epollfd, int fd, int state);
static void delete_event(int epollfd, int fd, int state);

int main(int argc, char* argv[]) {
	int listenfd;
	listenfd = socket_bind(IPADDRESS, PORT);
	listen(listenfd, LISTENQ);
	do_epoll(listenfd);

	return 0;
}

static int socket_bind(const char* ip, int port) {
	int listenfd;
	struct sockaddr_in servaddr;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd == -1) {
		perror("socket error: ");
		exit(1);
	}
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	/*servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	cout << servaddr.sin_addr.s_addr << endl;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);	//���õ�ַΪ0
	cout << servaddr.sin_addr.s_addr << endl;*/
	inet_pton(AF_INET, ip, &servaddr.sin_addr);
	servaddr.sin_port = htons(PORT);
	if (bind(listenfd, (sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
		perror("bind error: ");
		exit(1);
	}
	return listenfd;
}

static void do_epoll(int listenfd) {
	int epollfd;
	struct epoll_event events[EPOLLEVENTS];
	int ret;
	ThreadPool* tp = new ThreadPool();
	char buf[MAXSIZE];
	memset(buf, 0, MAXSIZE);
	epollfd = epoll_create(FDSIZE);	//����һ��epoll������
	add_event(epollfd, listenfd, EPOLLIN);	//��Ӽ����������¼�
	add_event(epollfd, STDIN_FILENO, EPOLLIN);
	while (1) {
		ret = epoll_wait(epollfd, events, EPOLLEVENTS, 500);	//��ȡ�Ѿ�׼���õ��������¼�, ����׼���õ��¼�����������Ϊ������ģʽ
		//handle_events(epollfd, events, ret, listenfd, buf);
		int i = 0;
		for (; i < ret; ++i) {
			if (events[i].data.fd == STDIN_FILENO) {	//����������˵���Ϣ����ѭ���������
				send_msg(epollfd, events[i].data.fd, buf);
				if (strcmp(buf, "exit\n") == 0) {
					break;
				}
				continue;
			}
			DetailJob* job = new DetailJob(epollfd, events[i], listenfd, buf);
			tp->Run(job);
			//delete job;
		}
		if (i < ret)
			break;
	}
	close(epollfd);
	sleep(1);
	delete tp;
	tp = nullptr;
}

static void send_msg(int epollfd, int fd, char* buf) {
	int nread;
	memset(buf, 0, MAXSIZE);
	nread = read(fd, buf, MAXSIZE);
	if (nread == -1) {
		perror("Input error: ");
		exit(1);
	}
	else if (nread == 0) {
		perror("Input closed.\n");
		exit(1);
	}
	else {
		if (strcmp(buf, "exit\n") != 0)
			for (auto &clifd : cfd) {
				//delete_event(epollfd, STDIN_FILENO, EPOLLIN);
				modify_event(epollfd, clifd, EPOLLOUT);
				/*do_write(epollfd, clifd, buf);
				modify_event(epollfd, clifd, EPOLLIN);*/
			}
	}
}

static void add_event(int epollfd, int fd, int state) {
	struct epoll_event ev;
	ev.events = state | EPOLLET;
	ev.data.fd = fd;	//����û����ݣ�һ�㷵�ؿ�����ָ��
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

static void modify_event(int epollfd, int fd, int state) {
	struct epoll_event ev;
	ev.events = state | EPOLLET;
	ev.data.fd = fd;	//����û����ݣ�һ�㷵�ؿ�����ָ��
	epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}

static void delete_event(int epollfd, int fd, int state) {
	struct epoll_event ev;
	ev.events = state | EPOLLET;
	ev.data.fd = fd;	//����û����ݣ�һ�㷵�ؿ�����ָ��
	epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
}

