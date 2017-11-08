#ifndef MYTHREADPOOL_H_
#define MYTHREADPOOL_H_

#include<MyWorkThread.h>
#include<BaseJob.h>

using namespace std;

class ThreadPool {
	friend WorkThread;
private:
	class Plock {
	private:
		pthread_mutex_t	mutex;
		pthread_cond_t	cond;
	public:
		Plock() { 
			pthread_mutex_init(&mutex, nullptr);
			pthread_cond_init(&cond, nullptr);
		}
		virtual ~Plock() { 
			pthread_mutex_destroy(&mutex);
			pthread_cond_destroy(&cond);
		}
		void lock(){
			int s = pthread_mutex_lock(&mutex);
			assert(s == 0);
		}
		void unlock(){
			int s = pthread_mutex_unlock(&mutex);
			assert(s == 0);
		}
		void cond_wait() { pthread_cond_wait(&cond, &mutex); }
		void cond_signal() { pthread_cond_signal(&cond); }
		void cond_broadcast() { pthread_cond_broadcast(&cond); }
	};
public:
	ThreadPool(int NUM = 10);
	virtual ~ThreadPool();

	void Run(BaseJob* job);

	void CreateIdleThread(int num);
	void RemoveIdleThread(int num);
	int GetAllNum() {return BusyList.size() + IdleList.size();}

	WorkThread* GetThread();
	void AppendIdle(WorkThread* Thread);
	void MoveIdle(WorkThread* Thread);
	void AppendBusy(WorkThread* Thread);
	void MoveBusy(WorkThread* Thread);
	
private:
	unsigned int InitNum;
	unsigned int MaxNum;
	unsigned int AvailHigh;
	unsigned int AvailLow;
	vector<WorkThread*> IdleList;
	vector<WorkThread*> BusyList;
	
	Plock IdleLock;
	Plock BusyLock;
	Plock ThreadLock;	//�߳���������
};

#endif /* MYTHREADPOOL_H_ */
