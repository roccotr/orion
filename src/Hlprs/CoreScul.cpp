#include "CoreScul.hpp"
#include <stdio.h>

#define iCfootprint 10

using namespace orion;

	const AnsiString CoreScul::sCmagic = "SCUL";
	const int CoreScul::iCpacketheadersize = CoreScul::sCmagic.Length () + 3 * sizeof (int);
	const int CoreScul::iCacktimeout = 30;
	const int CoreScul::iCackattempts = 5;
	const int CoreScul::iCrcvtimeout = 2000;

  __fastcall CoreScul::CoreScul (int iDefaultbuffersize) {
		iVdefaultbuffersize = iDefaultbuffersize;
		iVdefaultbuffersizepacket = iVdefaultbuffersize + iCpacketheadersize;
		iVpacketcounter = 0;
		bVreadbuffer = bVreadbufferdata = bVwritebuffer = NULL;
		cVacktimeout.tv_sec = cVrcvtimeout.tv_sec = 0;
		cVacktimeout.tv_usec = iCacktimeout * 1000;
		cVrcvtimeout.tv_usec = iCrcvtimeout * 1000;
	}

  __fastcall CoreScul::~CoreScul () {

		delte (); 		
  }

	/*CReaTe ITem*/
	void __fastcall CoreScul::crtit (	SOCKET cSocket,
																		struct sockaddr* cAddress, 
																		int iAddresslength) {

		bVactive = false;
		cVsocket = cSocket; 
		cVremoteaddress = cAddress;
		iVremoteaddresslength = iAddresslength;
		iVwritewatermark = 0;
		iVreadwatermarkfilled = iVreadwatermarkprocessed = 
		iVreadwatermarkdatafilled = iVreadwatermarkdataprocessed = 0;
		bVreadbuffer = (char*) factorymalloc (iVdefaultbuffersizepacket);
		bVreadbufferdata = (char*) factorymalloc (iVdefaultbuffersize);
		bVwritebuffer = (char*) factorymalloc (iVdefaultbuffersizepacket);
		FD_ZERO (&cVsetreceive);
		FD_SET(cVsocket, &cVsetreceive);
	}

	/*DELeTE*/
	void __fastcall CoreScul::delte () {

		if (bVreadbuffer) {
			factoryfree (bVreadbuffer);
			bVreadbuffer = NULL;
		}
		if (bVreadbufferdata) {
			factoryfree (bVreadbufferdata);
			bVreadbufferdata = NULL;
		}
		if (bVwritebuffer) {
			factoryfree (bVwritebuffer);
			bVwritebuffer = NULL;
		}
		bVactive = false;
	}

	/*GeT ACKnowledge*/
	bool __fastcall CoreScul::gtack (int iAcknowledge) {
		bool bVreturn;
		int iVopcode;
		int iVack;

		bVreturn = false;
		iVack = lrcvp (&cVacktimeout, &iVopcode, true);
		bVreturn = (iVopcode == iCopcodeack && iVack == iAcknowledge);
		return bVreturn;
	}

	/*Low level SeND Raw*/
	int __fastcall CoreScul::lsndr () {
		int iV;
		int iVcurrent;

		iVcurrent = 0;
		if (iVwritewatermark > 0) do {						
			iV = sendto(	cVsocket,  
										&bVwritebuffer [iVcurrent], 
										iVwritewatermark - iVcurrent,
										0,
										cVremoteaddress, 
										iVremoteaddresslength);
			iVcurrent += iV;
		} while (iVcurrent < iVwritewatermark && iV >= 0);
		iVwritewatermark = 0;				
		if (iV >= 0) return iVcurrent;
		else return iV;
	}

	/*Low level SeND Chunk*/
	int __fastcall CoreScul::lsndc (	const char* bBuffer,
																		int iBuffer,
																		bool bFlush) {

		if (iBuffer > 0) {
			memcpy (&bVwritebuffer [iVwritewatermark], bBuffer, iBuffer);
			iVwritewatermark += iBuffer;
		}
		if (bFlush) lsndr ();
		return iBuffer;
	}

	/*Low level SeND Packet*/
	bool __fastcall CoreScul::lsndp (	const char* bBuffer,
																		int iBuffer,
																		int iOpcode,
																		int iAck) {
		int iVpacketsize;
		int iVattempts;
		bool bVexit;

		iVpacketsize = iCpacketheadersize + iBuffer;
		bVexit = true;
		iVattempts = 0;
		do {
			lsndc (sCmagic.c_str (), sCmagic.Length ());
			lsndc ((const char*) &iVpacketsize, sizeof (int));
			if (iAck >= 0) lsndc ((const char*) &iAck, sizeof (int));
			else lsndc ((const char*) &iVpacketcounter, sizeof (int));
			lsndc ((const char*) &iOpcode, sizeof (int));						
			lsndc (bBuffer, iBuffer, true); 		
			if (iAck < 0) {
				bVexit = gtack (iVpacketcounter);
				if (bVexit) iVpacketcounter++;
				else iVattempts++;
			}
		} while (!bVexit && iVattempts < iCackattempts);
		return bVexit;
	}

	/*Low level SEND*/
	bool __fastcall CoreScul::lsend (	const char* bBuffer,
																		int iBuffer,
																		int iOpcode,
																		bool bDontack) {
		int iVelapsed;
		int iVtempelapsed;
		int iVbytes;
		int iVopcode;
		int iVdontack;
		bool bVsuccess;

		iVelapsed = iBuffer;
		iVdontack = bDontack ? 0 : -1;
		do {
			iVbytes = iVelapsed > iVdefaultbuffersize - iCpacketheadersize ?
								iVdefaultbuffersize - iCpacketheadersize : iVelapsed;
			iVtempelapsed = iVelapsed;
			iVelapsed -= iVbytes;
			if (iVelapsed == 0) iVopcode = iOpcode;
			else iVopcode = iCopcodepart;		
			if (iVbytes > 0) {				
				bVsuccess = lsndp (&bBuffer [iBuffer - iVtempelapsed], iVbytes, iVopcode, iVdontack);
			} else bVsuccess = lsndp (NULL, 0, iVopcode, iVdontack);	
		} while (bVsuccess && iVelapsed > 0);
		return bVsuccess;
	}

	/*CoNNeCT*/
	int __fastcall CoreScul::cnnct (	SOCKET cSocket, struct sockaddr* cAddress,
																		int iAddresslength) {

		crtit (cSocket, cAddress, iAddresslength);
		if (lsend (NULL, 0, iCopcodeconnect)) return 0;
		else return -1;
	}

	/*Low level ReCeiVe Raw*/
	int __fastcall CoreScul::lrcvr (struct timeval* cTimeout) {
		int iVreceived;
		
		iVreadwatermarkfilled = iVreadwatermarkprocessed = 0;
		if (select (int (cVsocket + 1), &cVsetreceive, NULL, NULL, cTimeout) > 0) {
			iVreceived = recvfrom (		
											cVsocket,  
											bVreadbuffer, 
											iVdefaultbuffersize,
											0,
											cVremoteaddress, 
											&iVremoteaddresslength);
			iVreadwatermarkfilled = iVreceived;
		} else {
			iVreceived = -1;
		}
		return iVreceived;
	}

	/*Low level ReCeiVe Chunk*/
	bool __fastcall CoreScul::lrcvc (	struct timeval* cTimeout,
																		char* bBuffer, int iBuffer) {
		int iVelapsed;
		int iVbytes;
		bool bVok;
		
		if (iBuffer == 0) return true;
		iVelapsed = iBuffer;
		do {
			if (iVreadwatermarkfilled == iVreadwatermarkprocessed) 
				bVok = (lrcvr (cTimeout) > 0);
			else bVok = true;
			iVbytes = iVelapsed > (iVreadwatermarkfilled - iVreadwatermarkprocessed) ?
								(iVreadwatermarkfilled - iVreadwatermarkprocessed) : iVelapsed;
			if (iVbytes > 0) 
				memcpy (&bBuffer [iBuffer - iVelapsed], 
								&bVreadbuffer [iVreadwatermarkprocessed], 
								iVbytes);
			iVreadwatermarkprocessed += iVbytes;
			iVelapsed -= iVbytes;
		} while (bVok && iVbytes >= 0 && iVelapsed > 0);
		return (iVelapsed == 0);
	}

	/*Low level ReCeiVe Packet*/
	int __fastcall CoreScul::lrcvp (	struct timeval* cTimeout,
																		int* iOpcode, bool bDontack) {
		int iVpacketsize;
		int iVpacketcounter;
		bool bVok;
		char bVmagic [4];

		bVok = true;
		iVpacketcounter = -1;
		if (bVok) bVok &= lrcvc (cTimeout, bVmagic, sCmagic.Length ());
		if (bVok) bVok &= lrcvc (cTimeout, (char*) &iVpacketsize, sizeof (int));
		if (bVok) bVok &= lrcvc (cTimeout, (char*) &iVpacketcounter, sizeof (int));
		if (bVok) bVok &= lrcvc (cTimeout, (char*) iOpcode, sizeof (int));
		if (bVok) {
			bVok &= lrcvc (cTimeout, bVreadbufferdata, iVpacketsize - iCpacketheadersize);
			iVreadwatermarkdatafilled = iVpacketsize - iCpacketheadersize;
			iVreadwatermarkdataprocessed = 0;
		}	
		if (bVok && !bDontack) {
			lsndp (NULL, 0, iCopcodeack, iVpacketcounter);
		}
		if (!bVok) *iOpcode = iCopcodeerror;
		return iVpacketcounter;
	}

	/*Low level RECeiVe*/
	bool __fastcall CoreScul::lrecv (char* bBuffer, int iBuffer, int* iOpcode, bool bDontack) {
		int iVelapsed;
		int iVbytes;
		bool bVok;

		iVelapsed = iBuffer;
		do {
			if (iVreadwatermarkdatafilled == iVreadwatermarkdataprocessed) 
				bVok = (lrcvp (&cVrcvtimeout, iOpcode, bDontack) > 0);
			else bVok = true;
			iVbytes = iVelapsed > (iVreadwatermarkdatafilled - iVreadwatermarkdataprocessed) ?
								(iVreadwatermarkdatafilled - iVreadwatermarkdataprocessed) : iVelapsed;
			if (iVbytes > 0) 
				memcpy (&bBuffer [iBuffer - iVelapsed], 
								&bVreadbufferdata [iVreadwatermarkdataprocessed], 
								iVbytes);
			iVreadwatermarkdataprocessed += iVbytes;
			iVelapsed -= iVbytes;
		} while (bVok && iVbytes >= 0 && iVelapsed > 0);
		return (*iOpcode != iCopcodeerror);
	}

	/*CoNNeCT*/
	SOCKET __fastcall CoreScul::accpt (	SOCKET cSocket, struct sockaddr* cAddress,
																		  SOCKLENT* iAddresslength) {
		int iVopcode;

		crtit (cSocket, cAddress, *iAddresslength);
		if (lrecv (NULL, 0, &iVopcode))	return 0;
		else return -1;
	}

	/*SEND*/
	int __fastcall CoreScul::send_ (	SOCKET cSocket, const char* bBuffer, int iLength) {
		
		if (lsend (bBuffer, iLength, iCopcodedata, false)) return iLength;
		else return -1;
	}

	/*RECV*/
	int __fastcall CoreScul::recv_ (	SOCKET cSocket, char* bBuffer, int iLength) {
		int iVopcode;		

		if (lrecv (bBuffer, iLength, &iVopcode, false)) return iLength;
		else return -1;
	}