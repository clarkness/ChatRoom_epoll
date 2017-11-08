#ifndef DETAILJOB_H_
#define DETAILJOB_H_

#include<BaseJob.h>
class DetailJob: virtual public BaseJob {
public:
	DetailJob(int num = -1):BaseJob(num){}
	~DetailJob();

	void Run()
	{
//		cout << pthread_self() << endl;
		cout << "This is Job : " << GetJobID() << ", done!" << endl;
	}

};

#endif /* DETAILJOB_H_ */
