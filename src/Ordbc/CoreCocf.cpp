#include "CoreCoco.hpp"
#include "CoreCocf.hpp"
#include "CoreButl.hpp"
#include "TmplObpl.cpp"
#include "TmplList.cpp"

  __fastcall CoreCocf::CoreCocf(int iExpirationtime)  : 
			CoreObpl<CoreCoco> (iExpirationtime) {
	}

  __fastcall CoreCocf::~CoreCocf () {
		close ();
	}

	/*INIT*/
	void __fastcall CoreCocf::init_ (PTR_POOL cPool, AnsiString sAddress, int iPort, bCprotocoltypes cProtocol) {

		cVpool = cPool;
		sVaddress = sAddress;
		iVport = iPort;
		bVprotocoltype = cProtocol;
	}

  /*SET*/
	PTR_ORIONCONNECTION __fastcall CoreCocf::set__ () {
		
		PTR_ORIONCONNECTION cVconnectionloc = boost::shared_ptr<CoreCoco> (new CoreCoco ());
		try {
			cVconnectionloc->open_ (cVpool, sVaddress, iVport, CoreButl::chr2s (bVprotocoltype));
		} catch (...) {}
		return cVconnectionloc;
	}


	/*UNSET*/
	void __fastcall CoreCocf::unset (PTR_ORIONCONNECTION cObject) {

		cObject.reset ();
	}

	
	/*VALID*/
	bool __fastcall CoreCocf::valid (PTR_ORIONCONNECTION cObject) {

		return cObject->valid ();
	}


	/*BEGIN*/
	PTR_ORIONCONNECTION __fastcall CoreCocf::begin () {

		return chout ();
	}
	
	/*END*/
	void __fastcall CoreCocf::end__ (PTR_ORIONCONNECTION cConnection) {

		chin_ (cConnection);
	}

	AnsiString __fastcall CoreCocf::debug () {

		return (AnsiString) "orion:" + sVaddress + (AnsiString) ":" + (AnsiString) iVport;
	}