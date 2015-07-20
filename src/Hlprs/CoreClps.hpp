#ifndef CORECLPS
#define CORECLPS
#include "CoreChls.hpp"

namespace orion {

template<typename T>
struct CoreClps {

	typedef CoreChls<T> CHAINEDLISTLOC;

	

public:
	
	CHAINEDLISTLOC* cVpointer;
	int iVposition;

	CoreClps ();
	CoreClps (int iPosition, CHAINEDLISTLOC* cPointer);
	CoreClps (const CoreClps& cCopy);

	void rpair (bool bUpdirection);

	CoreClps& operator= (const CoreClps& cCopy);
	void operator+= (int iPosition);
	CoreClps operator++ (int iDummy);
	CoreClps operator-- (int iDummy);
	CoreClps operator+ (const int iV);
	CoreClps operator- (const int iV);
	bool operator< (const CoreClps& cSource);
	bool operator<= (const CoreClps& cSource);
	bool operator>= (const CoreClps& cSource);
	bool operator> (int iPosition);
	bool operator>= (int iPosition);

};

};


#endif