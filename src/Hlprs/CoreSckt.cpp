#include "CoreSckt.hpp"

#define iCbuffertcp 1024
#define iCbufferpipe iCpipe
	//using namespace std;

using namespace orion;

	const AnsiString CoreSckt::sCsocketunixname = (AnsiString) "/tmp/" + (AnsiString) sCpipename;
	const AnsiString CoreSckt::sCpipeaddess = "IPC";

  __fastcall CoreSckt::CoreSckt (int iType) : CoreSckl () {
		iVbuffer = iVbufferdim = iVbufferallocateddim = 0;
		iVtype = iType;
		switch (iVtype) {
			case iCsockettcp:
			case iCsocketunix:
			case iCsocketudp:
				lsetb (iCbuffertcp);
				break;
			case iCsocketpipe:
				lsetb (iCbufferpipe);
				break;
		}
		bVreadyfortransmission = bVsocket;
	}

	/*only tcp*/
	__fastcall CoreSckt::CoreSckt (void* cSocket, int iType) : CoreSckl (cSocket) {
		
		iVbuffer = iVbufferdim = iVbufferallocateddim = 0;
		iVtype = iType;
		switch (iVtype) {
			case iCsockettcp:
			case iCsocketunix:
			case iCsocketudp:
				lsetb (iCbuffertcp);
				break;
			case iCsocketpipe:
				lsetb (iCbufferpipe);
				break;
		}
		bVreadyfortransmission = bVsocket;
	}

	/*only udp*/
	__fastcall CoreSckt::CoreSckt (struct sockaddr* cUdpremote) : CoreSckl () {
		bool bVok;
		struct sockaddr_in cVremoteaddress;				

		iVbuffer = iVbufferdim = iVbufferallocateddim = 0;
		iVtype = iCsocketudp;
		lsetb (iCbuffertcp);
		open_ (0);
		bVok = bind_ ();
		memcpy (&cVremoteaddress, cUdpremote, sizeof (struct sockaddr));	
		if (bVok) bVok = (cVudplayer.cnnct
											(*(SOCKET*) cVsocket,
											(struct sockaddr*) &cVremoteaddress, 
											sizeof(struct sockaddr_in)) != -1);
		bVreadyfortransmission = bVok;
	}

	/*only pipes*/
	__fastcall CoreSckt::CoreSckt (AnsiString sPipename) : CoreSckl ()  {

		iVbuffer = iVbufferdim = iVbufferallocateddim = 0;
		iVtype = iCsocketpipe;
		cVpipelayer.init_ (sPipename);
		lsetb (iCbufferpipe);
		bVreadyfortransmission = cVpipelayer.iscnn ();
	}

  __fastcall CoreSckt::~CoreSckt () {
		cls ();
		factoryfree (bVbuffer);
	}


	/*OPEN socket*/
	bool __fastcall CoreSckt::open_ (int iPort) {
		AnsiString sVsocketunixname;

		cls ();
		iVport = iPort;
		switch (iVtype) {
			case iCsockettcp:
				*(SOCKET*)cVsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				break;
			case iCsocketunix:
				*(SOCKET*)cVsocket = socket(PF_UNIX, SOCK_STREAM, 0);
				break;
			case iCsocketudp:
				*(SOCKET*)cVsocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
				break;
		}
		switch (iVtype) {
			case iCsockettcp:
			case iCsocketudp:
				if (*(SOCKET*) cVsocket != -1) {
					memset((void*) &cVlocaladdress, 0, sizeof(struct sockaddr_in));
					cVlocaladdress.sin_family = AF_INET;			
					cVlocaladdress.sin_port = htons (iVport);
					cVlocaladdress.sin_addr.s_addr = htonl (INADDR_ANY);	
					iVlocaladdresslength = sizeof(struct sockaddr_in);					
					bVreadyfortransmission = bVsocket = true;	
				};
				return bVsocket;
			case iCsocketunix:		
				if (*(SOCKET*) cVsocket != -1) {
					memset((void*) &cVlocaladdressun, 0, sizeof(struct sockaddr_un));
					cVlocaladdressun.sun_family = AF_LOCAL;
					sVsocketunixname = sCsocketunixname + (AnsiString) iVport;
					memcpy (cVlocaladdressun.sun_path, sVsocketunixname.c_str (), 
									sVsocketunixname.Length ());
					iVlocaladdresslength =  int (strlen (cVlocaladdressun.sun_path) + 
																	sizeof (cVlocaladdressun.sun_family) + 1);							
					UNLINK(cVlocaladdressun.sun_path);
					bVreadyfortransmission = bVsocket = true;	
				};
				return bVsocket;
			default:
				return true;
		}
	}

	/*CLOSE socket*/
	void __fastcall CoreSckt::clse_ () {
		if (bVsocket) {
			closesocket(*(SOCKET*) cVsocket);
		}
		bVreadyfortransmission = bVsocket = false;
	}

	/*GET BuFfer*/
	int __fastcall CoreSckt::getbf (int iType) {
		int iVreturn;
    SOCKLENT iVlength = sizeof(iVreturn);

		iVreturn = 0;
		if (bVsocket) 
			getsockopt(	*(SOCKET*) cVsocket,SOL_SOCKET, iType, 
									(char *) &iVreturn, &iVlength);
		return iVreturn;
	}

	/*SET BuFfer*/
	void __fastcall CoreSckt::setbf (int iType, int iSize) {

		if (bVsocket) 
			setsockopt(	*(SOCKET*) cVsocket, SOL_SOCKET, iType,
									(char *) &iSize, sizeof(iSize)); 
	}

	/*BIND socket*/
	bool __fastcall CoreSckt::bind_ () {
		bool bVreturn;
		
		if (bVsocket) {
			switch (iVtype) {
				case iCsocketunix:
					bVreturn = (bind(*(SOCKET*) cVsocket,
													 (sockaddr*) &cVlocaladdressun,
													 iVlocaladdresslength) != -1);
					break;
				case iCsockettcp:
				case iCsocketudp:
					bVreturn = (bind(*(SOCKET*) cVsocket,
													 (sockaddr*) &cVlocaladdress,
													 iVlocaladdresslength) != -1);		
					
					break;
			}
		} else bVreturn = true;
		return bVreturn;
	}

	/*LISTeN*/
	bool __fastcall CoreSckt::listn (int iNumports) {
		bool bVreturn;

		iVnumports = iNumports;
		bVreturn = true;
		if (bVsocket && (iVtype == iCsockettcp || iVtype == iCsocketunix))
			listen(*(SOCKET*) cVsocket, iNumports);
		else if (iVtype == iCsocketpipe) {
			bVreturn = cVpipelayer.listn (iVport, iVnumports);
		}
		return bVreturn;
	}

	/*ACCePT*/
	CoreSckt* __fastcall CoreSckt::accpt () {
		AnsiString sVpipename;
		CoreSckt* cVnewsocketclass;
		SOCKET cVnewsocket;
		struct sockaddr_in cVremoteaddress;		
		struct sockaddr_un cVremoteaddressun;				
		SOCKLENT iVremoteaddresslength;

		cVnewsocketclass = NULL;
		if (bVsocket) {
			switch (iVtype) {
				case iCsockettcp:
					iVremoteaddresslength = sizeof (struct sockaddr_in);
					cVnewsocket =
					accept(	*(SOCKET*) cVsocket, 
								 (struct sockaddr *) &cVremoteaddress, 
								 &iVremoteaddresslength);
					if (cVnewsocket != -1) 
						cVnewsocketclass = new CoreSckt (&cVnewsocket, iVtype);
					break;
				case iCsocketunix:
					iVremoteaddresslength = sizeof (struct sockaddr_un);
					cVnewsocket =
					accept(	*(SOCKET*) cVsocket, 
								 (struct sockaddr *) &cVremoteaddressun, 
								 &iVremoteaddresslength);
					if (cVnewsocket != -1) 
						cVnewsocketclass = new CoreSckt (&cVnewsocket, iVtype);
					break;
				case iCsocketudp:
					iVremoteaddresslength = sizeof (struct sockaddr_in);
					if (cVudplayer.accpt(	*(SOCKET*) cVsocket, 
																(struct sockaddr *) &cVremoteaddress, 
																&iVremoteaddresslength) == 0) {
						cVnewsocketclass = new CoreSckt ((struct sockaddr *) &cVremoteaddress);
					}
					break;
			}
		} else if (iVtype == iCsocketpipe) {
			sVpipename = cVpipelayer.accpt ();
			if (sVpipename != NULL) cVnewsocketclass = new CoreSckt (sVpipename);
		}
		if (cVnewsocketclass != NULL) {
			if (!cVnewsocketclass->rdftr ()) {
				delete cVnewsocketclass;
				cVnewsocketclass = NULL;
			}
		}
		return cVnewsocketclass;
	}

	/*CoNNeCT*/
	bool __fastcall CoreSckt::cnnct (int iPort, AnsiString sHostip) {
		AnsiString sVsocketunixname;
		bool bVreturn;
    int iVattempt;
		struct sockaddr_in cVremoteaddress;		
		struct sockaddr_un cVremoteaddressun;		
		SOCKLENT iVremoteaddresslength;
	
		bVreturn = false;
		if (bVsocket) {	
			if (iVtype == iCsocketunix) {
				memset((void*) &cVremoteaddressun, 0, sizeof(struct sockaddr_un));
				cVremoteaddressun.sun_family = AF_LOCAL;
				sVsocketunixname = sCsocketunixname + (AnsiString) iPort;
				memcpy (cVremoteaddressun.sun_path, sVsocketunixname.c_str (), 
								sVsocketunixname.Length ());
				iVremoteaddresslength = int (strlen (cVremoteaddressun.sun_path) + 
																sizeof (cVremoteaddressun.sun_family) + 1);
				bVreturn = (connect(*(SOCKET*) cVsocket,(struct sockaddr*) 
										&cVremoteaddressun, iVremoteaddresslength) != -1);					
			} else {
				iVremoteaddresslength = sizeof(struct sockaddr_in);
				memset ((void*) &cVremoteaddress, 0, iVremoteaddresslength);
				cVremoteaddress.sin_family = AF_INET;			
				cVremoteaddress.sin_port = htons(iPort);
				if (iVtype == iCsockettcp) {
					cVremoteaddress.sin_addr.s_addr = inet_addr(sHostip.c_str ());
					bVreturn = (connect(*(SOCKET*) cVsocket,(struct sockaddr*) 
											&cVremoteaddress, iVremoteaddresslength) != -1);
				} else {	
					iVremoteaddresslength = sizeof(struct sockaddr_in);
					bVreturn = bind_ ();
					cVremoteaddress.sin_addr.s_addr = inet_addr(sHostip.c_str ());
					if (bVreturn) bVreturn = (cVudplayer.cnnct
										(*(SOCKET*) cVsocket,
										(struct sockaddr*) &cVremoteaddress, 
										iVremoteaddresslength) != -1); 
					if (bVreturn) bVreturn = 
															(cVudplayer.accpt(	*(SOCKET*) cVsocket, 
															(struct sockaddr *) &cVremoteaddress, 
															&iVremoteaddresslength) == 0);
				}
			}
		} else if (iVtype == iCsocketpipe) {
			iVattempt = 0;
      do {
			  bVreturn = bVreadyfortransmission = cVpipelayer.cnnct (iPort);
        iVattempt++;
        if (iVattempt == CoreScpl::iCconnectattempt) break;
      } while (!bVreturn);
		}
		return bVreturn;
	}

	/*Low level FLuSH*/
	bool __fastcall CoreSckt::lflsh () {

#ifndef iColdalg
		int iVsentbytes;
		int iVsent;
		int iVsends;

		iVsent = 0;
		iVsends = 0;
		if (iVbuffer > 0) {			
			iVsentbytes = 0;
			do {
				switch (iVtype) {
					case iCsockettcp:
					case iCsocketunix:
						iVsent = 
							send (*(SOCKET*) cVsocket, &bVbuffer [iVsentbytes], iVbuffer - iVsentbytes, 0);
						break;
					case iCsocketudp:
						iVsent = 
							cVudplayer.send_ (	*(SOCKET*) cVsocket, 
																	&bVbuffer [iVsentbytes], 
																	iVbuffer - iVsentbytes);
						break;
					case iCsocketpipe:
						iVsent =
							cVpipelayer.send_ (&bVbuffer [iVsentbytes], 
																	iVbuffer - iVsentbytes);
						break;
				}
				iVsentbytes += iVsent;
				iVsendnum++;
				iVsends++;
			} while (iVsentbytes < iVbuffer && iVsent > 0);		
			iVsendbytes += iVsentbytes;
			iVbuffer = 0;
		}
#endif
		return (iVsent >= 0);
	}

	/*Low level SET internal Buffer*/
	void __fastcall CoreSckt::lsetb (int iDim) {
		
		if (iDim != iVbufferdim) {
			if (iDim > iVbufferallocateddim) {
				if (iVbufferallocateddim > 0) {
					lflsh ();
					factoryfree (bVbuffer);
				}
				iVbufferallocateddim = iDim;
				bVbuffer = (char*) factorymalloc (iVbufferallocateddim);			
			} else {
				if (iDim < iVbuffer) lflsh ();
			}
			iVbufferdim = iDim;
		}
	}

	/*Low level SEND*/
	void __fastcall CoreSckt::lsend (char* bBuffer, int iSize) {

		int iVelapsed;
		int iVframe;
		int iVposition;

		iVelapsed = iSize;
		iVposition = 0;
		while (iVelapsed > 0) {
			if (iVbuffer == iVbufferdim) lflsh ();
			iVframe = Min (iVelapsed, iVbufferdim - iVbuffer);
			memcpy (&bVbuffer [iVbuffer], &bBuffer [iVposition], iVframe);
			iVbuffer += iVframe;
			iVposition += iVframe;
			iVelapsed -= iVframe;
		}
	}

	/*Low level ReCeiVE*/
	bool __fastcall CoreSckt::lrecv (char* bBuffer, int iSize) {
		int iVpointer;
		int iVreceived;

		iVpointer = 0;
		iVreceived = 0;
		do {
			switch (iVtype) {
				case iCsockettcp:
				case iCsocketunix:
					iVreceived = recv (*(SOCKET*) cVsocket, &bBuffer [iVpointer], iSize - iVpointer, 0);
					break;
				case iCsocketudp:
					iVreceived = cVudplayer.recv_ (*(SOCKET*) cVsocket, 
																					&bBuffer [iVpointer], iSize - iVpointer);
					break;
				case iCsocketpipe:
					iVreceived = cVpipelayer.recv_ (&bBuffer [iVpointer], iSize - iVpointer);
					break;
			}
			iVpointer += iVreceived;
			iVrecvnum++;
		} while (iVpointer < iSize && iVreceived > 0);
		iVrecvbytes += iSize;
		return (iVreceived >= 0);
	}
	
	/*SEND*/
	void __fastcall CoreSckt::send_ (char* bBuffer, int iSize, bool bSplit) {

		if (bVreadyfortransmission) {
			if (!bSplit) lsend((char*) &iSize, sizeof (iSize));
			if (iSize > 0) lsend(bBuffer, iSize);
		}
	}

	/*RECeiVE*/
	int __fastcall CoreSckt::recve () {
		int iVreturn;
		bool bVsuccess;

		iVreturn = 0;
		bVsuccess = true;
		if (bVreadyfortransmission) 
			bVsuccess = lrecv ((char*) &iVreturn, sizeof (iVreturn));
		if (bVsuccess) return iVreturn;
		else return -1;
	}

	/*RECeiVE*/
	int __fastcall CoreSckt::recve (char* bBuffer, int iSize) {
		int iVsize;
		bool bVsuccess;

		bVsuccess = true;
		if (bVreadyfortransmission) {			
			if (iSize <= 0) {
				bVsuccess = lrecv ((char*) &iVsize, sizeof (iVsize));
			} else iVsize = iSize;
			if (iVsize > 0 && bVsuccess)
				bVsuccess = lrecv (bBuffer, iVsize);
		}
		if (bVsuccess) return iVsize;
		else return -1;
	}

	/*Magic SEND*/
	void __fastcall CoreSckt::msend (AnsiString sParam) {

		send_ (sCmagic.c_str (), sCmagic.Length () + 1, true);	
		send_ (sParam.c_str (), sParam.Length () + 1);	
		lflsh ();
	}
	
	/*Magic ReCeiVE*/
	bool __fastcall CoreSckt::mrcve (AnsiString& sParam) { 
		bool bVreturn;
		int iVdim;
		char bVmagic [iCmaxdir];
		char bVparam [iCmaxdir];

		recve (bVmagic, sCmagic.Length () + 1);
		bVreturn = (sCmagic == bVmagic);
		iVdim = recve ();
		recve (bVparam, iVdim);
		sParam = bVparam;
		return bVreturn;
	}

	/*TRaSLate Address*/
	int __fastcall CoreSckt::trsla (AnsiString sHostip) {

		if (sHostip.Trim () == sCpipeaddess) return iCtranslatetypepipe;
		else return iCsockettcp;
	}

	/*ReaDy For TRasmission*/
	bool __fastcall CoreSckt::rdftr () {
		return bVreadyfortransmission;
	}

	/*PRint STAts*/
	void __fastcall CoreSckt::prsta () {
		printf (" sendnum = %i\n recvnum = %i\n sendbytes = %i\n recvbytes = %i\n", 
			iVsendnum, iVrecvnum, iVsendbytes, iVrecvbytes);
		iVsendnum = iVrecvnum = iVsendbytes = iVrecvbytes = 0;
	}

