#ifndef DETAILJOB_H_
#define DETAILJOB_H_

#include<BaseJob.h>

extern unordered_set<int> cfd;	//�������пͻ��˵��׽��֣����ڹ㲥��Ϣ

class DetailJob : virtual public BaseJob {
public:
	DetailJob(int num = -1) :BaseJob(num) {}
	DetailJob(int efd, struct epoll_event eve, int serfd, char s[]) :BaseJob(++JobId), epollfd(efd), event(eve), listenfd(serfd), buf(s) {}
	~DetailJob();
private:
	static void handle_events(int epollfd, struct epoll_event event, int listenfd, char *buf); //�¼�������
	static void handle_accept(int epollfd, int listenfd);	//������յ�������
	//void send_msg(int epollfd, int fd, char* buf);	//�㲥��Ϣ
	static void do_read(int epollfd, int fd, char* buf);	//������
	static void do_write(int epollfd, int fd, char* buf);	//д����
	static void add_event(int epollfd, int fd, int state);	//����¼�
	static void modify_event(int epollfd, int fd, int state);	//�޸��¼�
	static void delete_event(int epollfd, int fd, int state);	//ɾ���¼�
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