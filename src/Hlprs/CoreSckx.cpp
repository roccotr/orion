#include "CoreSckl.hpp"
#include <netdb.h>

using namespace orion;

const AnsiString CoreSckl::sCmagic = "SCKL";

  __fastcall CoreSckl::CoreSckl () {

		cVsocket = new SOCKET;
		endhostent();
		sethostent (1);			
		bVwsa = true;
		bVsocket = false;
		iVsendnum = iVrecvnum = iVsendbytes = iVrecvbytes = 0;
  }

	__fastcall CoreSckl::CoreSckl (void* cSocket) {

		bVwsa = false;
		bVsocket = true;
		cVsocket = new SOCKET;
		cVwsadata = NULL;
		*(SOCKET*) cVsocket = *(SOCKET*) cSocket;
		iVsendnum = iVrecvnum = iVsendbytes = iVrecvbytes = 0;
	}

  __fastcall CoreSckl::~CoreSckl () {
	
		cls ();
		if (bVwsa) endhostent ();
		delete (SOCKET*) cVsocket;
		cVsocket = NULL;
  }

	/*SET NO tcp*/
	void __fastcall CoreSckl::setno () {}

	/*GET Host Ip address*/
	__fastcall AnsiString CoreSckl::gethi (AnsiString sHostname) {
		AnsiString sVreturn;
		struct hostent *cVhostptr;
		struct in_addr *cVinaddress;
		
		sVreturn = "";
		if (sHostname == NULL) cVhostptr = gethostent ();
		else cVhostptr = gethostbyname (sHostname.c_str ());
		cVinaddress = (struct in_addr*) *cVhostptr->h_addr_list;
		sVreturn = inet_ntoa (*cVinaddress);
		return sVreturn;
	}
