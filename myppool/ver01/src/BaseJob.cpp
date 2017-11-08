#include "BaseJob.h"

BaseJob::BaseJob(int ID) {
	// TODO Auto-generated constructor stub
	pthread = nullptr;
	JobID = ID;
}

BaseJob::~BaseJob() {
	// TODO Auto-generated destructor stub
	pthread = nullptr;
}
