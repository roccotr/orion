#ifndef CORESCKT
#define CORESCKT
#include "CoreSckl.hpp"
#include "CoreScul.hpp"
#include "CoreScpl.hpp"

#ifndef MACOS
	struct sockaddr_un {
		short sun_family;
		char sun_path [108];
	};
	#define iCtranslatetypepipe iCsocketpipe
	#define UNLINK(X)
	#define AF_LOCAL 0
#else
	#define iCtranslatetypepipe iCsocketunix
	#define UNLINK(X) unlink (X)
#endif

namespace orion {

class CoreSckt : public CoreSckl {

private:
	const static AnsiString sCsocketunixname;

	char* bVbuffer;
	CoreScul cVudplayer;
	CoreScpl cVpipelayer;
	struct sockaddr_in cVlocaladdress;
	struct sockaddr_un cVlocaladdressun;
	SOCKLENT iVlocaladdresslength;
	int iVbuffer;
	int iVbufferdim;
	int iVbufferallocateddim;
	int iVtype;
	int iVnumports;
	bool bVreadyfortransmission;

public:

	const static AnsiString sCpipeaddess;

  enum iCsockettype {
    iCsockettcp = 0,
    iCsocketudp,
		iCsocketpipe,
		iCsocketunix
  };

  __fastcall CoreSckt (int iType = iCsockettcp);
  __fastcall CoreSckt (void* cSocket, int iType);
  __fastcall CoreSckt (struct sockaddr* cUdpremote);
  __fastcall CoreSckt (AnsiString sPipename);
	__fastcall ~CoreSckt ();

	bool __fastcall open_ (int iPort);
	void __fastcall clse_ ();

	int __fastcall getbf (int iType);
	void __fastcall setbf (int iType, int iSize);

	bool __fastcall bind_ ();
	bool __fastcall listn (int iNumports);
	CoreSckt* __fastcall accpt ();
	bool __fastcall cnnct (int iPort, AnsiString sHostip = NULL);

	bool __fastcall lflsh ();
	void __fastcall lsetb (int iDim);
	void __fastcall lsend (char* bBuffer, int iSize);
	bool __fastcall lrecv (char* bBuffer, int iSize);

	void __fastcall send_ (char* bBuffer, int iSize, bool bSplit = false);
	int __fastcall recve ();
	int __fastcall recve (char* bBuffer, int iSize = 0);
	void __fastcall msend (AnsiString sParam = "");
	bool __fastcall mrcve (AnsiString& sParam);
	bool __fastcall rdftr ();
	static int __fastcall trsla (AnsiString sHostip);

	void __fastcall prsta ();

};

};

#endif




