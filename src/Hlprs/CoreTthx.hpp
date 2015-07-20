#ifndef CORETTHR
#define CORETTHR
#include <pthread.h>

namespace orion {

class CoreTthr {
public:
	typedef struct {
		CoreTthr*	cVinstance;
		pthread_t cVpthread;
		pthread_key_t cVpthreadkey;
		pthread_mutex_t cVqueuehandle;
		pthread_cond_t cVqueuecondition;
		bool bVqueuesignaled;		
	} TTHREADLOC;
		
	enum {
		tpLowest = 0,
		tpLower,
		tpNormal,
		tpHigher,
		tpHighest
	};
	
	bool Terminated;
	int Priority;	

private:
	bool bVsuspend;
	bool bVcansetpriority;
	bool bVdeadmanwalking;
	int iVpriority [5];
	struct sched_param cVtparameters;
	int iVtpolicy;
	TTHREADLOC cVthreaddata;
	static void* ptsta (void* cHandle);
	static void ptend (void* cHandle);	
  virtual void start ();	
	
public:

  CoreTthr (bool bSuspend);
  ~CoreTthr ();
  virtual void Terminate ();
  virtual void Execute ();
	void Resume ();
	void WaitFor ();

	void exepr ();
};

};

#endif
