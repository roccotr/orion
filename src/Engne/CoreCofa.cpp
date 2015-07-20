#include "CoreConn.hpp"
#include "CoreCofa.hpp"
#include "TmplObpl.cpp"
#include "TmplList.cpp"


using namespace orion;

  __fastcall CoreCofa::CoreCofa(int iExpirationtime)  : 
			CoreObpl<CoreConn> (iExpirationtime) {
	}

  __fastcall CoreCofa::~CoreCofa () {
		close ();
	}

	/*INIT*/
	void __fastcall CoreCofa::init_ (AnsiString sAddress, int iPort) {

		sVaddress = sAddress;
		iVport = iPort;
	}

	/*CHECK*/
	bool __fastcall CoreCofa::check (AnsiString sAddress, int iPort) {

		return sVaddress == sAddress && iVport == iPort;
	}

  /*SET*/
	PTR_CONNECTION __fastcall CoreCofa::set__ () {
		
		PTR_CONNECTION cVconnectionloc = shared_ptr<CoreConn> (new CoreConn ());
		try {
			cVconnectionloc->open_ (sVaddress, iVport);
		} catch (...) {}
		return cVconnectionloc;
	}

	/*UNSET*/
	void __fastcall CoreCofa::unset (PTR_CONNECTION cObject) {

		cObject.reset ();
	}
	
	/*VALID*/
	bool __fastcall CoreCofa::valid (PTR_CONNECTION cObject) {

		return cObject->valid ();
	}

	/*BEGIN*/
	PTR_CONNECTION __fastcall CoreCofa::begin () {

		return chout ();
	}
	
	/*END*/
	void __fastcall CoreCofa::end__ (PTR_CONNECTION cConnection) {

		chin_ (cConnection);
	}

	AnsiString __fastcall CoreCofa::debug () {

		return (AnsiString) "orion:" + sVaddress + (AnsiString) ":" + (AnsiString) iVport;
	}