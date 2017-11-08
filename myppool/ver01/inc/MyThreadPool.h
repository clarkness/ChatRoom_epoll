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
		pthread_mutex_t mutex;
	public:
		Plock() { pthread_mutex_init(&mutex, nullptr); }
		virtual ~Plock() { pthread_mutex_destroy(&mutex); }
		void lock(){
			int s = pthread_mutex_lock(&mutex);
			assert(s == 0);
		}
		void unlock(){
			int s = pthread_mutex_unlock(&mutex);
			assert(s == 0);
		}
	};
public:
	ThreadPool(int NUM = 5);
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

};

#endif /* MYTHREADPOOL_H_ */
