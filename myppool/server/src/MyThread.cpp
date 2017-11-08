#include <MyThread.h>

pthread_once_t MyThread::once = PTHREAD_ONCE_INIT;
pthread_key_t MyThread::pid_key;
int MyThread::pthread_cnt = 0;

void* MyThread::routine(void* arg)
{
	int s = pthread_once(&once, CreateKey);
	assert(s == 0);
	int Pthread_Var = ++pthread_cnt;
	s = pthread_setspecific(pid_key, &Pthread_Var);
	assert(s == 0);
	MyThread *p = (MyThread*)arg;
	p->Run();
	return nullptr;
}

void MyThread::create(MyThread* thread)
{

	int s = pthread_create(ptid, NULL, &routine, thread);
	assert(s == 0);
	return;
}
