#ifndef CORESCPL
#define CORESCPL
#include "CoreVcl_.hpp"
#include "CoreSmph.hpp"

#define iCpipe 8192

namespace orion {

class CoreScpl
{
private:

	const static AnsiString sCmagic;
	const static int iCpacketheadersize;
	const static int iCcreatepipetimeout;
	const static int iCacceptattempt;
	const static int iCaccepttimems;

	typedef struct { 
		HANDLE cVinstance;	
		OVERLAPPED cVoverlap;
		BOOL bVpendingio; 
		DWORD iVstate;  
		bool bVoverlapped;
	} PIPELOC;

	AnsiString sVpipename;
	AnsiString sVabsolutepipename;
	int iVdefaultbuffersize;
	int iVlisteningpipes;
	int iVdefaultbuffersizepacket;
	int iVwritewatermark;
	int iVreadwatermarkprocessed;
	int iVreadwatermarkdatafilled;
	int iVreadwatermarkfilled;
	int iVreadwatermarkdataprocessed;
	bool bVlistening;
	bool bVconnection;
	bool bVmalloc;
	char* bVwritebuffer;
	char* bVreadbuffer;
	char* bVreadbuffernet;
	HANDLE* cVglobalevent;
	PIPELOC* cVglobalpipe;

  enum iCopcodes
  { iCopcodeconnect = 10,
		iCopcodepart,
		iCopcodedata,
		iCopcodeerror,

		iCopcodedims
	};

	__fastcall void crtit (int iPort);
	__fastcall AnsiString open_ (PIPELOC* cPipe, int iPort);
	__fastcall void delte ();
	__fastcall int lsndr (	PIPELOC* cPipe);
	__fastcall int lsndc (	PIPELOC* cPipe,
													const char* bBuffer,
													int iBuffer,
													bool bFlush = false);
	__fastcall bool lsndp (	PIPELOC* cPipe,
													const char* bBuffer,
													int iBuffer,
													int iOpcode);
	__fastcall bool lsend (	PIPELOC* cPipe,
													const char* bBuffer,
													int iBuffer,
													int iOpcode);
	__fastcall int lrcvr (	PIPELOC* cPipe);
	__fastcall bool lrcvc (	PIPELOC* cPipe, 
													char* bBuffer, 
													int iBuffer);
	__fastcall bool lrcvp (	PIPELOC* cPipe, 
													int* iOpcode);
	__fastcall bool lrecv (	PIPELOC* cPipe,
													char* bBuffer, 
													int iBuffer, 
													int* iOpcode);
	__fastcall bool cntcl(  HANDLE hPipe,
                          LPOVERLAPPED cOverlapped);
	__fastcall void recon ( bool bDisconnect,
                          PIPELOC* cPipe);

public:

	const static int iCconnectattempt;

  __fastcall CoreScpl (int iDefaultbuffersize = iCpipe);
  __fastcall ~CoreScpl ();

	__fastcall void init_ (AnsiString sPipename);
	__fastcall bool cnnas (PIPELOC* cPipe);
	__fastcall bool cnnct (int iPort);
	__fastcall bool listn (int iPort, int iNumports);
	__fastcall AnsiString accpt ();
	__fastcall int send_ (	const char* bBuffer, int iLength);
	__fastcall int recv_ (	char* bBuffer, int iLength);
	__fastcall bool iscnn ();

};

};

#endif