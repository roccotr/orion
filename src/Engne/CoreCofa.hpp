#ifndef CORECOFA
#define CORECOFA
#include "CoreConn.hpp"
#include "CoreObpl.hpp"

namespace orion {

class CoreCofa  : public CoreObpl<CoreConn> {

	AnsiString sVaddress;
	int iVport;

public:
  __fastcall CoreCofa (int iExpirationtime = 0);
  __fastcall ~CoreCofa ();

	void __fastcall init_ (AnsiString sAddress, int iPort);
	bool __fastcall check (AnsiString sAddress, int iPort);

	PTR_CONNECTION __fastcall set__ ();
	void __fastcall unset (PTR_CONNECTION cObject);
	bool __fastcall valid (PTR_CONNECTION cObject);

	PTR_CONNECTION __fastcall begin ();
	void __fastcall end__ (PTR_CONNECTION cConnection);

	AnsiString __fastcall debug ();
};

typedef boost::shared_ptr<CoreCofa> PTR_CONNECTIONFACTORY;

};



#endif