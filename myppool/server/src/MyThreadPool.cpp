#include <MyThreadPool.h>

ThreadPool::ThreadPool(int Num)
{
	assert(Num >= 2 && Num <= 20);
	InitNum = Num;
	MaxNum = 20;
	AvailLow = 3;
	AvailHigh = 15;
	BusyList.clear();
	IdleList.clear();
	for (unsigned int i = 0; i < InitNum; ++i)
	{
		WorkThread* thread = new WorkThread();
		thread->create(thread);
		thread->SetPool(this);
		AppendIdle(thread);
	}
}
ThreadPool::~ThreadPool()
{
	for(WorkThread* p : IdleList)
		delete p;
	for(WorkThread* p : BusyList)
		delete p;
	IdleList.clear();
	BusyList.clear();
}

void ThreadPool::CreateIdleThread(int num)
{
	for(int i = 0; i < num; ++i)
	{
		WorkThread* thread = new WorkThread;
		thread->SetPool(this);
		AppendIdle(thread);
		thread->create(thread);
	}
	return;
}

void ThreadPool::RemoveIdleThread(int num) {
	for(int i = 0; i < num; ++i) {
		delete IdleList.back();
		IdleList.pop_back();
	}
	return;
}


WorkThread* ThreadPool::GetThread()
{
	while (IdleList.empty())
	{
		usleep(10);
	}
	IdleLock.lock();
	WorkThread* p = IdleList.front();
	IdleList.erase(IdleList.begin());
	IdleLock.unlock();
	return p;
}

void ThreadPool::AppendIdle(WorkThread* Thread)
{
	IdleLock.lock();	//how resource protection???
	IdleList.push_back(Thread);
	IdleLock.unlock();
}
void ThreadPool::AppendBusy(WorkThread* Thread)
{
	BusyLock.lock();
//	int s = *(int*)pthread_getspecific(Thread->GetID());
	cout << "Move Thread To Busy List" << endl;
	BusyList.push_back(Thread);
	BusyLock.unlock();
}

void ThreadPool::MoveIdle(WorkThread* Thread)
{
	AppendIdle(Thread);
	IdleLock.lock();
	BusyLock.lock();
	int s = *(int*)pthread_getspecific(Thread->GetID());
	cout << "Move Thread " << s <<" To Idle List" << endl;
	vector<WorkThread*>::iterator p = find(BusyList.begin(), BusyList.end(), Thread);
	if (p != BusyList.end())
		BusyList.erase(p);
	BusyLock.unlock();
	IdleLock.unlock();
}

void ThreadPool::MoveBusy(WorkThread* Thread)
{
	//WorkThread* Thread = GetThread();
	AppendBusy(Thread);
	IdleLock.lock();
	BusyLock.lock();
	int s = *(int*)pthread_getspecific(Thread->GetID());
	cout << "Move Thread " << s <<"To Idle List" << endl;
	vector<WorkThread*>::iterator p = find(IdleList.begin(), IdleList.end(), Thread);
	if (p != IdleList.end())
		IdleList.erase(p);
	BusyLock.unlock();
	IdleLock.lock();
}

void ThreadPool::Run(BaseJob* job)
{
	//  assert(job == nullptr);
	//	if(Busylist.size() >= MaxNum)
	if (IdleList.size() < AvailLow)
	{
		if (GetAllNum() + InitNum - IdleList.size() < MaxNum)
			CreateIdleThread(InitNum - IdleList.size());
		else
			CreateIdleThread(MaxNum - GetAllNum());
	}
	else if (IdleList.size() > AvailHigh)
		RemoveIdleThread(IdleList.size() - InitNum);
	WorkThread* Thread = GetThread();
	AppendBusy(Thread);
	job->SetThread(Thread);
	Thread->SetJob(job);
	ThreadLock.cond_broadcast();
	cout << GetAllNum() << endl;
}

