#ifndef CORESCUL
#define CORESCUL
#include "CoreVcl_.hpp"

namespace orion {

class CoreScul
{
private:

	const static AnsiString sCmagic;
	const static int iCpacketheadersize;
	const static int iCacktimeout;
	const static int iCrcvtimeout;
	const static int iCackattempts;

	SOCKET cVsocket;
	struct sockaddr* cVremoteaddress; 
	SOCKLENT iVremoteaddresslength;
	char* bVwritebuffer;
	char* bVreadbuffer;
	char* bVreadbufferdata;
	int iVreadwatermarkfilled;
	int iVreadwatermarkprocessed;
	int iVreadwatermarkdatafilled;
	int iVreadwatermarkdataprocessed;
	int iVpacketcounter;
	int iVwritewatermark;
	int iVdefaultbuffersize;
	int iVdefaultbuffersizepacket;
	bool bVactive;
	struct timeval cVacktimeout; 
	struct timeval cVrcvtimeout; 
	fd_set cVsetreceive;

  enum iCopcodes
  { iCopcodeconnect = 0,
		iCopcodepart,
		iCopcodeack,
		iCopcodedata,
		iCopcodeerror,

		iCopcodedims
	};

	__fastcall void crtit (	SOCKET cSocket,
													struct sockaddr* cRemoteaddress, 
													int iRemoteaddresslength);
	__fastcall void delte ();
	__fastcall bool gtack (int iAcknowledge);
	__fastcall int lsndr ();
	__fastcall int lsndc (	const char* bBuffer,
													int iBuffer,
													bool bFlush = false);
	__fastcall bool lsndp (	const char* bBuffer,
													int iBuffer,
													int iOpcode,
													int iAck = -1);
	__fastcall bool lsend (	const char* bBuffer,
													int iBuffer,
													int iOpcode,
													bool bDontack = false);
	__fastcall int lrcvr (	struct timeval* cTimeout);
	__fastcall bool lrcvc (	struct timeval* cTimeout,
													char* bBuffer, 
													int iBuffer);
	__fastcall int lrcvp (	struct timeval* cTimeout,
													int* iOpcode, 
													bool bDontack = false);
	__fastcall bool lrecv (	char* bBuffer, 
													int iBuffer, 
													int* iOpcode,
													bool bDontack = false);

public:
  __fastcall CoreScul (int iDefaultbuffersize = 8192);
  __fastcall ~CoreScul ();
	__fastcall int cnnct (	SOCKET cSocket, struct sockaddr* cAddress, 
													int iAddresslength);
	__fastcall SOCKET accpt (	SOCKET cSocket, struct sockaddr* cAddress, 
														SOCKLENT* iAddresslength);
	__fastcall int send_ (	SOCKET cSocket, const char* bBuffer, int iLength);
	__fastcall int recv_ (	SOCKET cSocket, char* bBuffer, int iLength);

};

};

#endif