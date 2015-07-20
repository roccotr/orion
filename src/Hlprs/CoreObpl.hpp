#ifndef COREOBPL
#define COREOBPL
#include "boost/shared_ptr.hpp"
#include "CoreVcl_.hpp"
#include "CoreList.hpp"
#include "CoreSmph.hpp"

namespace orion {

using boost::shared_ptr;

template<typename T>
class CoreObpl {

public:
	typedef boost::shared_ptr<T> PTR_T;

private:
	static const double dCtimestampfactor;

	typedef struct {
		AnsiString sVindex;

		PTR_T cVobject;
		TDateTime dVstarttime;
	} OBJECTLOC;

	double dVexpirationtime;
	CoreList<OBJECTLOC> cVunlockedpool;
	CoreList<OBJECTLOC> cVlockedpool;
	CoreSmph cVmutex;

	AnsiString __fastcall key__ (typename CoreObpl::PTR_T cObject); 

protected:
	void __fastcall close ();

public:
  __fastcall CoreObpl(int iExpirationtimems);
  virtual __fastcall ~CoreObpl ();

	virtual PTR_T __fastcall set__ () = 0;
	virtual void __fastcall unset (PTR_T cObject) = 0;
	virtual bool __fastcall valid(PTR_T cObject) = 0;

	PTR_T __fastcall chout (); 
	void __fastcall chin_ (PTR_T cObject);

};

};

#endif