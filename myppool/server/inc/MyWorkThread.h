#ifndef MYWORKTHREAD_H_
#define MYWORKTHREAD_H_

#include<MyThread.h>
#include <DetailJob.h>

class ThreadPool;

class WorkThread: public MyThread{
public:
	WorkThread(void);
	virtual ~WorkThread(void);

	void Run(void);

	void SetPool(ThreadPool* threadpool) { ppool = threadpool; }
	ThreadPool* getpool(void) { return ppool; }

	void SetJob(BaseJob* job) {
		pjob = job;
		pjob->SetThread(this);
	}
	BaseJob* getjob(void) { return pjob; }
	
private:
	ThreadPool *ppool;
	BaseJob *pjob;

};

#endif /* MYWORKTHREAD_H_ */
