#ifndef BASEJOB_H_
#define BASEJOB_H_

#include <iostream>
#include <pthread.h>
#include <cassert>
#include <algorithm>
#include <string>
#include <vector>
#include <MyThread.h>

using namespace std;

class BaseJob {
public:
	BaseJob(int ID);
	virtual ~BaseJob();

	virtual void Run(){};

	int GetJobID (){ return JobID; }

	void SetThread(MyThread* thread){pthread = thread;}
private:
	MyThread* pthread;
	int JobID;
};
#endif /* BASEJOB_H_ */
