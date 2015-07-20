#ifndef CORECOCF
#define CORECOCF
#include "CoreCoco.hpp"
#include "CoreObpl.hpp"

class CoreCocf  : public CoreObpl<CoreCoco> {

public:
  enum bCprotocoltypes { 
		bCprotobuf = 0x10,
    bCthrift = 0xFF
	};

private:
	AnsiString sVaddress;
	int iVport;
	PTR_POOL cVpool;
	bCprotocoltypes bVprotocoltype;

public:
  __fastcall CoreCocf (int iExpirationtime = 30000);
  __fastcall ~CoreCocf ();

	void __fastcall init_ (PTR_POOL cPool, AnsiString sAddress, int iPort, bCprotocoltypes cProtocol);

	PTR_ORIONCONNECTION __fastcall set__ ();
	void __fastcall unset (PTR_ORIONCONNECTION cObject);
	bool __fastcall valid (PTR_ORIONCONNECTION cObject);

	PTR_ORIONCONNECTION __fastcall begin ();
	void __fastcall end__ (PTR_ORIONCONNECTION cConnection);
	
	AnsiString __fastcall debug ();
};

typedef boost::shared_ptr<CoreCocf> PTR_ORIONCONNECTIONFACTORY;

#endif