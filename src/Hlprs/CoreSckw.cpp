#include "CoreSckl.hpp"

using namespace orion;

	const AnsiString CoreSckl::sCmagic = "SCKL";

  __fastcall CoreSckl::CoreSckl () {

		cVsocket = new SOCKET;
		cVwsadata = new WSADATA;
		WSAStartup (MAKEWORD(2, 2), (WSADATA*) cVwsadata);
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
		if (bVwsa) WSACleanup();
		if (cVwsadata != NULL) delete (WSADATA*) cVwsadata;
		cVwsadata = NULL;
		delete (SOCKET*) cVsocket;
		cVsocket = NULL;
  }

	/*SET NO tcp*/
	void __fastcall CoreSckl::setno () {
		bool bVhack = true;

		if (bVsocket)
			setsockopt(	*(SOCKET*) cVsocket, IPPROTO_TCP, TCP_NODELAY, 
									(char *)&bVhack, sizeof(bool));
	}

	/*GET Host Ip address*/
	AnsiString __fastcall CoreSckl::gethi (AnsiString sHostname) {
		AnsiString sVreturn;
		char bVhostname [iCmaxdir];
		struct hostent *cVhostptr;
		struct in_addr *cVinaddress;

		sVreturn = "";
		if (sHostname == NULL) {
			gethostname(bVhostname, iCmaxdir);
			cVhostptr = gethostbyname (bVhostname);
		} else {
			cVhostptr = gethostbyname (sHostname.c_str ());
		}
		cVinaddress = (struct in_addr*) *cVhostptr->h_addr_list;
		sVreturn = inet_ntoa (*cVinaddress);
		return sVreturn;
	}
