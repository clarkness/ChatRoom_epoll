#ifndef BASEJOB_H_
#define BASEJOB_H_

#include <iostream>
#include <pthread.h>
#include <cassert>
#include <algorithm>
#include <string>
#include <vector>
#include <MyThread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
//#include <iostream>
#include <unordered_set>
#include <thread>	//C++里面的线程
#include <sys/epoll.h>
#include <errno.h>

using namespace std;

class BaseJob {
public:
	BaseJob(int ID = -1);
	virtual ~BaseJob();

	virtual void Run(){};

	int GetJobID (){ return JobID; }

	void SetThread(MyThread* thread){pthread = thread;}
private:
	MyThread* pthread;
	int JobID;
};
#endif /* BASEJOB_H_ */
