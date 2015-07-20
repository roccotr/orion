#include "CoreKutl.hpp"
#include "CoreVcl_.hpp"
#include <sys/time.h>

#define UnsignedWideToUInt64(x) (*((u64*)(&x)))
typedef int     clock_res_t;

/*CoNvert MP Duration*/
void CoreKutl::cnmpd (Duration cDuration, struct timespec* cTimespec) {
		struct timeval cVtval;
		struct timespec cVnow;
		struct timezone cVjunk;
		/*SOMMO*/
		//UInt64 cVdurationns;
		u64 cVdurationns;

		cVdurationns = (cDuration*1000000L);
		//cVdurationns = UnsignedWideToUInt64 (DurationToNanoseconds(cDuration));
		gettimeofday(&cVtval, &cVjunk);
		TIMEVAL_TO_TIMESPEC( &cVtval, &cVnow );
		cTimespec->tv_sec = (unsigned int) (cVnow.tv_sec +  (cVdurationns / 1000000000));
		cTimespec->tv_nsec = (clock_res_t) (cVnow.tv_nsec + (cVdurationns % 1000000000));
		if (cTimespec->tv_nsec > 1000000000 ) {
			cTimespec->tv_sec += 1;
			cTimespec->tv_nsec -= 1000000000;
		}
  }
