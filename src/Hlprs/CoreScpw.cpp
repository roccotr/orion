#include "CoreScpl.hpp"
#include <stdio.h>

using namespace orion;

#define CONNECTING_STATE 0  
#define READING_STATE 1 
#define MAXSTRINGCONNECTION 64

	const AnsiString CoreScpl::sCmagic = "SCPL";
	const int CoreScpl::iCpacketheadersize = CoreScpl::sCmagic.Length () + 2 * sizeof (int);
	const int CoreScpl::iCcreatepipetimeout = 10000;
	const int CoreScpl::iCacceptattempt = 100;
	const int CoreScpl::iCconnectattempt = 10;  
	const int CoreScpl::iCaccepttimems = 10;

  __fastcall CoreScpl::CoreScpl (int iDefaultbuffersize) {

		iVdefaultbuffersize = iDefaultbuffersize;
		iVdefaultbuffersizepacket = iVdefaultbuffersize + iCpacketheadersize;
		bVreadbuffer = bVwritebuffer = bVreadbuffernet = NULL;
		bVlistening = false;
		bVconnection = false;
		bVmalloc = false;
	}

  __fastcall CoreScpl::~CoreScpl () {
		int iV;
		
		if (bVlistening) {
			bVlistening = false;
			for (iV = 0; iV < iVlisteningpipes; iV++) {
				if (cVglobalevent [iV] != NULL) {
					CloseHandle (cVglobalevent [iV]);					
				}
			}
			delete [] cVglobalevent;
			cVglobalevent = NULL;			
			for (iV = 0; iV < iVlisteningpipes; iV++) {
				if (cVglobalpipe [iV].cVinstance != NULL && 
						cVglobalpipe [iV].cVinstance != INVALID_HANDLE_VALUE) {
					DisconnectNamedPipe(cVglobalpipe [iV].cVinstance );
					CloseHandle (cVglobalpipe [iV].cVinstance);
					
				}
			}	
			delete [] cVglobalpipe;
			cVglobalpipe = NULL;
		}
		if (bVconnection) {
			if (	cVglobalpipe->cVinstance != NULL && 
						cVglobalpipe->cVinstance != INVALID_HANDLE_VALUE)
				CloseHandle (cVglobalpipe->cVinstance);
			delete cVglobalpipe;
			cVglobalpipe = NULL;
		}
		delte ();
	}

	/*INIT*/
	__fastcall void CoreScpl::init_ (AnsiString sPipename) {

		sVpipename = sPipename;
		crtit (-1);
		cVglobalpipe = new PIPELOC;
		ZeroMemory (cVglobalpipe, sizeof (PIPELOC));
		if (cnnas (cVglobalpipe)) {
			bVconnection = true;
		} else {
			delete cVglobalpipe;
		}
	}

	/*CReaTe ITem*/
	void __fastcall CoreScpl::crtit (int iPort) {

		iVwritewatermark = 0;
		iVreadwatermarkfilled = iVreadwatermarkprocessed = 
		iVreadwatermarkdatafilled = iVreadwatermarkdataprocessed = 0;
		if (iPort >= 0) sVpipename = (AnsiString) sCpipename + (AnsiString) iPort;
		sVabsolutepipename =	(AnsiString) "\\\\.\\pipe\\" + sVpipename;
		delte ();
		bVreadbuffernet = (char*) factorymalloc (iVdefaultbuffersize);
		bVreadbuffer = (char*) factorymalloc (iVdefaultbuffersizepacket);
		bVwritebuffer = (char*) factorymalloc (iVdefaultbuffersizepacket);
		bVmalloc = true;
	}	

	/*OPEN*/
	AnsiString __fastcall CoreScpl::open_ (PIPELOC* cPipe, int iPort) {
		int iVport;
		AnsiString sVreturn;
		AnsiString sVtempabsolutepipename;

		sVreturn = NULL;
		iVport = iPort;
		do {
			sVreturn = (AnsiString) sCpipename + (AnsiString) iVport;
			sVtempabsolutepipename = (AnsiString) "\\\\.\\pipe\\" + sVreturn;
			cPipe->cVinstance =
						   CreateNamedPipe (	sVtempabsolutepipename.c_str (),
																	PIPE_ACCESS_DUPLEX, 
																	PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 
																	1, 
																	iVdefaultbuffersizepacket,
																	iVdefaultbuffersizepacket, 
																	iCcreatepipetimeout, 
																	NULL); 
			iVport++;
		} while (cPipe->cVinstance == INVALID_HANDLE_VALUE);		
		cPipe->bVoverlapped = false;
		return sVreturn;
	}

	/*DELeTE*/
	void __fastcall CoreScpl::delte () {
		
		if (bVmalloc) {
			factoryfree (bVreadbuffer);
			factoryfree (bVwritebuffer);
			factoryfree (bVreadbuffernet);
		}
		bVreadbuffer = NULL;		
		bVwritebuffer = NULL;
		bVreadbuffernet = NULL;
		bVmalloc = false;
	}

	/*Low level SeND Raw*/
	int __fastcall CoreScpl::lsndr (PIPELOC* cPipe) {
		DWORD iV;
		int iVcurrent;
		LPOVERLAPPED cVoverlap;

		iVcurrent = 0;
		if (!cPipe->bVoverlapped) cVoverlap = NULL;
		else cVoverlap = &cPipe->cVoverlap;
		if (iVwritewatermark > 0) do {						
			if (!WriteFile(	cPipe->cVinstance, 
											&bVwritebuffer [iVcurrent], 
											iVwritewatermark - iVcurrent, 
											&iV, cVoverlap)) {
				iV = -1;
				break;
			}
			iVcurrent += iV;
		} while (iVcurrent < iVwritewatermark && iV >= 0);
		iVwritewatermark = 0;		
		if (iV >= 0) return iVcurrent;
		else return iV;
	}

	/*Low level SeND Chunk*/
	int __fastcall CoreScpl::lsndc (	PIPELOC* cPipe,
																		const char* bBuffer,
																		int iBuffer,
																		bool bFlush) {
		int iVreturn;

		iVreturn = 0;
		if (iBuffer > 0) {
			memcpy (&bVwritebuffer [iVwritewatermark], bBuffer, iBuffer);
			iVwritewatermark += iBuffer;
		}
		if (bFlush) iVreturn = lsndr (cPipe);
		return iVreturn;
	}

	/*Low level SeND Packet*/
	bool __fastcall CoreScpl::lsndp (	PIPELOC* cPipe,
																		const char* bBuffer,
																		int iBuffer,
																		int iOpcode) {
		int iVpacketsize;

		iVpacketsize = iCpacketheadersize + iBuffer;
		lsndc (cPipe, sCmagic.c_str (), sCmagic.Length ());
		lsndc (cPipe, (const char*) &iVpacketsize, sizeof (int));
		lsndc (cPipe, (const char*) &iOpcode, sizeof (int));								
		return (lsndc (cPipe, bBuffer, iBuffer, true) > 0); 
	}

	/*Low level SEND*/
	bool __fastcall CoreScpl::lsend (	PIPELOC* cPipe,
																		const char* bBuffer,
																		int iBuffer,
																		int iOpcode) {
		int iVelapsed;
		int iVtempelapsed;
		int iVbytes;
		int iVopcode;
		bool bVsuccess;

		iVelapsed = iBuffer;
		do {
			iVbytes = iVelapsed > iVdefaultbuffersize - iCpacketheadersize ?
								iVdefaultbuffersize - iCpacketheadersize : iVelapsed;
			iVtempelapsed = iVelapsed;
			iVelapsed -= iVbytes;
			if (iVelapsed == 0) iVopcode = iOpcode;
			else iVopcode = iCopcodepart;		
			if (iVbytes > 0) {				
				bVsuccess = lsndp (cPipe, &bBuffer [iBuffer - iVtempelapsed], iVbytes, iVopcode);
			} else bVsuccess = lsndp (cPipe, NULL, 0, iVopcode);	
		} while (bVsuccess && iVelapsed > 0);
		return bVsuccess;
	}

	/*Low level ReCeiVe Raw*/
	int __fastcall CoreScpl::lrcvr (PIPELOC* cPipe) {
		DWORD iVreceived;
		BOOL bVsuccess;
		LPOVERLAPPED cVoverlap;
		
		iVreadwatermarkfilled = iVreadwatermarkprocessed = 0;
		if (!cPipe->bVoverlapped) cVoverlap = NULL;
		else cVoverlap = &cPipe->cVoverlap;
		bVsuccess =		ReadFile (cPipe->cVinstance, 
									bVreadbuffer, iVdefaultbuffersizepacket, 
									&iVreceived, cVoverlap);		
		if (bVsuccess) {
			iVreadwatermarkfilled = iVreceived;
			return int (iVreceived);
		} else return 0;		
	}

	/*Low level ReCeiVe Chunk*/
	bool __fastcall CoreScpl::lrcvc (	PIPELOC* cPipe, char* bBuffer, int iBuffer) {
		int iVelapsed;
		int iVbytes;
		bool bVok;
		
		if (iBuffer == 0) return true;
		iVelapsed = iBuffer;
		do {
			if (iVreadwatermarkfilled == iVreadwatermarkprocessed) 
				bVok = (lrcvr (cPipe) > 0);
			else bVok = true;
			iVbytes = iVelapsed > (iVreadwatermarkfilled - iVreadwatermarkprocessed) ?
								(iVreadwatermarkfilled - iVreadwatermarkprocessed) : iVelapsed;
			if (iVbytes > 0) {
				memcpy (&bBuffer [iBuffer - iVelapsed], 
								&bVreadbuffer [iVreadwatermarkprocessed], 
								iVbytes);
			}
			iVreadwatermarkprocessed += iVbytes;
			iVelapsed -= iVbytes;
		} while (bVok && iVbytes >= 0 && iVelapsed > 0);
		return (iVelapsed == 0);
	}

	/*Low level ReCeiVe Packet*/
	bool __fastcall CoreScpl::lrcvp (	PIPELOC* cPipe, int* iOpcode) {
		int iVpacketsize;
		bool bVok;
		char bVmagic [4];

		bVok = true;
		if (bVok) bVok &= lrcvc (cPipe, bVmagic, sCmagic.Length ());
		if (bVok) bVok &= lrcvc (cPipe, (char*) &iVpacketsize, sizeof (int));
		if (bVok) bVok &= lrcvc (cPipe, (char*) iOpcode, sizeof (int));
		if (bVok) {
			bVok &= lrcvc (cPipe, bVreadbuffernet, iVpacketsize - iCpacketheadersize);
			iVreadwatermarkdatafilled = iVpacketsize - iCpacketheadersize;
			iVreadwatermarkdataprocessed = 0;
		}
		if (!bVok) *iOpcode = iCopcodeerror;
		return bVok;
	}

	/*Low level RECeiVe*/
	bool __fastcall CoreScpl::lrecv (	PIPELOC* cPipe,
																		char* bBuffer,
																		int iBuffer, int* iOpcode) {
		int iVelapsed;
		int iVbytes;
		bool bVok;

		iVelapsed = iBuffer;
		*iOpcode = iCopcodedata;
		do {
			if (iVreadwatermarkdatafilled == iVreadwatermarkdataprocessed) 
				bVok = lrcvp (cPipe, iOpcode);
			else bVok = true;
			iVbytes = iVelapsed > (iVreadwatermarkdatafilled - iVreadwatermarkdataprocessed) ?
								(iVreadwatermarkdatafilled - iVreadwatermarkdataprocessed) : iVelapsed;
			if (iVbytes > 0) {
				memcpy (&bBuffer [iBuffer - iVelapsed], 
								&bVreadbuffernet [iVreadwatermarkdataprocessed], 
								iVbytes);
			}
			iVreadwatermarkdataprocessed += iVbytes;
			iVelapsed -= iVbytes;
		} while (bVok && iVbytes >= 0 && iVelapsed > 0);
		return (*iOpcode != iCopcodeerror);
	}

	/*CoNnect To CLient*/
	bool __fastcall CoreScpl::cntcl(HANDLE hPipe, LPOVERLAPPED cOverlapped) { 
	 bool bVreturn; 
  
	 bVreturn = false;
	 if (ConnectNamedPipe(hPipe, cOverlapped)) return false;
		switch (GetLastError()) { 
      case ERROR_IO_PENDING: 
         bVreturn = true; 
         break; 
      case ERROR_PIPE_CONNECTED: 
         if (SetEvent(cOverlapped->hEvent)) break;  
      default: return false;
		}  
		return bVreturn; 
	}

	/*RECONnect*/
	void __fastcall CoreScpl::recon (bool bDisconnect, PIPELOC* cPipe) { 
		if (bDisconnect) DisconnectNamedPipe(cPipe->cVinstance);
		cPipe->bVpendingio = cntcl (cPipe->cVinstance, &cPipe->cVoverlap);
		cPipe->iVstate = cPipe->bVpendingio ? CONNECTING_STATE : READING_STATE;
	}

	/*CoNNect AnsiString*/
	bool __fastcall CoreScpl::cnnas (PIPELOC* cPipe) {
		DWORD iVmode;
		bool bVreturn;

		cPipe->bVoverlapped = false;
		bVreturn = false;
		while (true) {
			cPipe->cVinstance = CreateFile(		sVabsolutepipename.c_str (), 
																				GENERIC_READ | GENERIC_WRITE, 
																				0, NULL, OPEN_EXISTING, 0, NULL);  
			if (cPipe->cVinstance != INVALID_HANDLE_VALUE) {
				bVreturn = true;
				break;
			} 
			if (!WaitNamedPipe(	sVabsolutepipename.c_str (), 
				CoreScpl::iCcreatepipetimeout)) {
				break;
			}
		}
		if (bVreturn) {
			iVmode = PIPE_READMODE_BYTE;
			bVreturn = 
				(SetNamedPipeHandleState(cPipe->cVinstance , &iVmode, NULL, NULL) == TRUE);

		}
		return bVreturn;
	}

	/*CoNNeCT*/
	bool __fastcall CoreScpl::cnnct (int iPort) {
		bool bVsent;
		PIPELOC cVconnectingpipe;
		PIPELOC cVconnectedpipe;
		AnsiString sVnewpipe;
		
		if (bVconnection) return false;
		bVsent = false;
		ZeroMemory(&cVconnectingpipe, sizeof (PIPELOC));
		ZeroMemory(&cVconnectedpipe, sizeof (PIPELOC));
		sVnewpipe = open_ (&cVconnectedpipe, 0);
		if (sVnewpipe != NULL) {
			crtit (iPort);
			if (cnnas (&cVconnectingpipe)) {				
				bVsent = lsend (&cVconnectingpipe, sVnewpipe.c_str (), 
													MAXSTRINGCONNECTION, iCopcodeconnect);
				if (bVsent) 
					bVconnection = ConnectNamedPipe (cVconnectedpipe.cVinstance, NULL) != 0;												
				CloseHandle (cVconnectingpipe.cVinstance);
			}
			if (bVconnection) {
				cVglobalpipe = new PIPELOC;
				memcpy (cVglobalpipe, &cVconnectedpipe, sizeof (PIPELOC));
				ZeroMemory(&cVconnectedpipe, sizeof (PIPELOC));
			} else CloseHandle (cVconnectedpipe.cVinstance);	
		}
		return bVconnection;
	}

	/*LISTeN*/
	bool __fastcall CoreScpl::listn (int iPort, int iNumports) {
		int iV;
		bool bVerror;

		bVlistening = true;
		bVerror = false;
		crtit (iPort);
		iVlisteningpipes = iNumports;
		cVglobalevent = new HANDLE [iVlisteningpipes];
		cVglobalpipe = new PIPELOC [iVlisteningpipes];
		ZeroMemory(cVglobalevent, sizeof (HANDLE) * iVlisteningpipes);
		ZeroMemory(cVglobalpipe, sizeof (PIPELOC) * iVlisteningpipes);
		for (iV = 0; iV < iVlisteningpipes; iV++) {
			cVglobalpipe [iV].cVinstance = NULL;
			cVglobalevent [iV] = NULL;
		}

		cVglobalpipe [0].cVinstance = CreateFile(			sVabsolutepipename.c_str (), 
																				GENERIC_READ | GENERIC_WRITE, 
																				0, NULL, OPEN_EXISTING, 0, NULL);  
		bVerror = !(cVglobalpipe [0].cVinstance == INVALID_HANDLE_VALUE);
		CloseHandle (cVglobalpipe [0].cVinstance);
		cVglobalpipe [0].cVinstance = NULL;

		for (iV = 0; iV < iVlisteningpipes; iV++) {
			cVglobalpipe [iV].bVoverlapped = true;
			cVglobalpipe [iV].cVinstance =
				CreateNamedPipe (	sVabsolutepipename.c_str (),
													PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, 
													PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 
													PIPE_UNLIMITED_INSTANCES, 
													iVdefaultbuffersizepacket,
													iVdefaultbuffersizepacket, 
													iCcreatepipetimeout, 
													NULL); 
			if (cVglobalpipe [iV].cVinstance == INVALID_HANDLE_VALUE) {
				bVerror = true;
				break;
			}
			cVglobalevent [iV] = CreateEvent(NULL, TRUE, TRUE, NULL);   
			if (cVglobalevent [iV] == NULL) {
				bVerror = true;
				break;
			}
			cVglobalpipe [iV].cVoverlap.hEvent = cVglobalevent [iV]; 
			cVglobalpipe [iV].cVoverlap.Offset = 0;
			cVglobalpipe [iV].cVoverlap.OffsetHigh = 0;
			recon (false, &cVglobalpipe [iV]);
		}
		return !bVerror;
	}

	/*ACCePT*/
	AnsiString __fastcall CoreScpl::accpt () {
		DWORD iVwait;
		DWORD iVret;
		BOOL bVsuccess;
		bool bVlowlevelfunc;
		int iVopcode;
		int iV;
		int iVattempts;
		char bVnewpipename [MAX_PATH];
		
		iVattempts = 0;
		if (bVlistening) {
			while (true) {
				iVwait = WaitForMultipleObjects (iVlisteningpipes, cVglobalevent, FALSE, iCaccepttimems); 					 
				iVattempts++;
				if (iVattempts == iCacceptattempt) break;
				iV = iVwait - WAIT_OBJECT_0;
				if (iV >= 0 && iV < iVlisteningpipes) {		
					if (cVglobalpipe [iV].bVpendingio) { 
						bVsuccess = GetOverlappedResult( 
							cVglobalpipe [iV].cVinstance, &cVglobalpipe [iV].cVoverlap, 
							&iVret, FALSE);
						if (cVglobalpipe [iV].iVstate == CONNECTING_STATE) {
							if (bVsuccess) {
								cVglobalpipe [iV].iVstate = READING_STATE;
								continue;
							}			
						}
						if(cVglobalpipe [iV].iVstate == READING_STATE) {							
							Sleep (iCaccepttimems);
							bVlowlevelfunc = (lrcvr (&cVglobalpipe [iV]) > 0); 					
							if (bVlowlevelfunc) {
								lrecv (	&cVglobalpipe [iV], bVnewpipename, 
												MAXSTRINGCONNECTION, &iVopcode);
								memset (bVreadbuffer, 0, iVdefaultbuffersizepacket);
								memset (bVreadbuffernet, 0, iVdefaultbuffersize);
								cVglobalpipe [iV].iVstate = CONNECTING_STATE;
								return (AnsiString) bVnewpipename;
							} else {
								switch (GetLastError ()) {
									case ERROR_IO_PENDING:
										cVglobalpipe [iV].bVpendingio = TRUE;
									case ERROR_BROKEN_PIPE:
										recon (true, &cVglobalpipe [iV]);		
								}
								continue;
							}
						}																													
					}
				}
			}
		}
		return NULL;
	}

	/*SEND*/
	int __fastcall CoreScpl::send_ (	const char* bBuffer, int iLength) {

		if (lsend (cVglobalpipe, bBuffer, iLength, iCopcodedata)) return iLength;
		else return -1;
	}

	/*RECV*/
	int __fastcall CoreScpl::recv_ (	char* bBuffer, int iLength) {
		int iVopcode;		

		if (lrecv (cVglobalpipe, bBuffer, iLength, &iVopcode)) return iLength;
		else return -1;
	}

	bool __fastcall CoreScpl::iscnn () {
		return bVconnection;
	}