#ifndef COREXUTL
#define COREXUTL

#include <glog/logging.h>

namespace orion {


class CoreXutl {
public:
	const static double dVtdatetimeconst;
	
	double static ttime ();
	double static tgtme ();		
	void static sleep (int iMsec);
	void static delfl (char* bName);
	unsigned long int static getti ();
  void static lgexc (  char* bFilename, char* bContent,
                       bool bExclusive = false);
	int static getpn ();
	unsigned int static getpi ();

};

};

#endif