#include <iostream>
#include "DetailJob.h"
#include "MyThreadPool.h"
using namespace std;

int main(int argc, char** argv) {
	ThreadPool* tp = new ThreadPool(3);
	int i = 0;
	for (; i<5; i++) {
		DetailJob* job = new DetailJob(i+1);
		tp->Run(job);
	}
	sleep(1);
	delete tp;
	tp = nullptr;
	return 0;
}
