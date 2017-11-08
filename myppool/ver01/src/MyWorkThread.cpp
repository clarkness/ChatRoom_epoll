#include <MyWorkThread.h>
#include <MyThreadPool.h>

WorkThread::WorkThread(void)
{
	ppool = nullptr;
	pjob = nullptr;
	//return *this;	//???
}

WorkThread::~WorkThread(void)
{
	if (pjob != nullptr)
		delete pjob;
	pjob = nullptr;
	ppool = nullptr;
}

void WorkThread::Run(void)
{
	for (;; usleep(10))
	{
		while (pjob == nullptr)
			usleep(20);
		int cnt = *(int *)pthread_getspecific(pid_key);
		cout << "thread " << cnt << " process job " << pjob->GetJobID() << endl;
		pjob->Run();
		pjob->SetThread(nullptr);
		pjob = nullptr;
		usleep(20);
		ppool->MoveIdle(this);
	}
}
