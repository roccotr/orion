#ifndef CORETEVN
#define CORETEVN
#include <pthread.h>

namespace orion {

class CoreTevn {

private:
	bool bVsignaled;
	pthread_mutex_t cVhandle;
	pthread_cond_t cVcondition;
	
public:
  const static int iCwrsignaled;
  const static int iCwrnotsignaled;
	
  CoreTevn (void* = NULL, bool = true, bool = false, char* = NULL);
  ~CoreTevn ();

	void ResetEvent ();
	int WaitFor (int iMsec);
	void SetEvent ();
};

};

#endif