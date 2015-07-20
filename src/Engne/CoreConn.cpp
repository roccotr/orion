#include "CoreConn.hpp"

using namespace orion;

  __fastcall CoreConn::CoreConn() {
		bVconnection = false;
	}

  __fastcall CoreConn::~CoreConn () {
		close ();
	}

  /*OPEN*/
	bool __fastcall CoreConn::open_ (AnsiString sAddress, int iPort) {

		if (!bVconnection && sAddress != NULL && sAddress.Length () > 0) {
			cVsocket = shared_ptr<TSocket> (new TSocket (sAddress.to_string (), iPort));
			cVtransport = shared_ptr<TTransport> (new TFramedTransport(cVsocket));
			cVprotocol = shared_ptr<TProtocol> (new TBinaryProtocol(cVtransport));
			cVclient = new ThrfOrn_Client (cVprotocol);
			bVconnection = true;
			try {
				cVtransport->open ();
			} catch (...) {
				bVconnection = false;
			}			
		}
		return bVconnection;
	}


	/*CLOSE*/
	bool __fastcall CoreConn::close () {
		if (bVconnection) {
			cVtransport->close ();
			bVconnection = false;
			return true;
		}
		return false;
	}

	/*GET*/
	ThrfOrn_Client* __fastcall CoreConn::get__ () {
		return cVclient;
	}

	/*VALID*/
	bool __fastcall CoreConn::valid () {

		return bVconnection;
	}