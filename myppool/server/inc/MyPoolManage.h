#ifndef MyThreadPool_H_
#define MyThreadPool_H_
#include <MyThreadPool.h>

class PoolManage
{
private:
	ThreadPool  *Pool;
	int     NumOfThread;

protected:

public:
	PoolManage();
	PoolManage(int num);
	virtual ~PoolManage();

	void   SetParallelNum(int num);
	void  Run(BaseJob* job);
	void  TerminateAll(void);
};
#endif