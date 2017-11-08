#include <MyPoolManage.h>

PoolManage::PoolManage() {
	NumOfThread = 10;
	Pool = new ThreadPool(NumOfThread);
}

PoolManage::PoolManage(int num) {
	NumOfThread = num;
	Pool = new ThreadPool(NumOfThread);
}

PoolManage::~PoolManage() {
	if (NULL != Pool)
		delete Pool;
}

void PoolManage::SetParallelNum(int num) {
	NumOfThread = num;
}

void PoolManage::Run(BaseJob* job) {
	Pool->Run(job);
}

void PoolManage::TerminateAll(void) {
	delete Pool;
	Pool = nullptr;
}