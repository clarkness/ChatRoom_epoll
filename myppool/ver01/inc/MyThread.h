#ifndef MYTHREAD_H_
#define MYTHREAD_H_

#include<iostream>
#include<pthread.h>
#include<cassert>
#include<algorithm>
#include<string>
#include<vector>
#include<unistd.h>
#include<cstdlib>


using namespace std;

class MyThread
{
public:
	MyThread() = default;
	virtual ~MyThread(void) = default;
	virtual void Run(void) = 0;

	void create(MyThread* thread);
	pthread_t getPID(void) { return tid; }
	int GetID(){return pid_key;}
private:
	pthread_t tid;
	pthread_t* ptid = &tid;
protected:
	static pthread_once_t once;
	static pthread_key_t pid_key;
	static int pthread_cnt;

	static void CreateKey()
	{
		int s = pthread_key_create(&pid_key, NULL);
		assert(s == 0);
	}
	static void *routine(void* arg);


};

#endif /* MYTHREAD_H_ */
