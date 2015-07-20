#ifndef CORESSTB
#define CORESSTB
#include <glog/logging.h>
#include "CoreMetb.hpp"
#include "CoreMeot.hpp"
#include "CoreHsrx.hpp"
#include "CoreFsys.hpp"
#include "CoreList.hpp"
#include "CoreSmph.hpp"
#include "PrtoAlog.pb.h"

namespace orion {

class CoreSstb : public CoreTble {

protected:
  using CoreTble::get__;

	PTR_HASHRADIX cVhsrx;

	PTR_FILESYSTEM __fastcall getfr ();
	bool pcfds (PrtoData* cProtodata, bool &bParsed, int64vcl iPosition);

public:

	typedef boost::shared_ptr<CoreSstb> PTR;

	const static unsigned int iVmaxcompactionsize;
	const static AnsiString sCversiondelimiter;

	AnsiString __fastcall getfn ();

private:
	AnsiString sVcachegetkeyinput;
	AnsiString sVcachegetkeyoutput;
	AnsiString sVpublicpath;
	AnsiString sVpublicfilename;
	PTR_POOL cVpool;
	PTR_FILESYSTEM cVfsysrandomread;
	PTR_FILESYSTEM cVserializer;
	CoreSmph cVmutex;
	CoreSmph cVmutexpcfds;
	bool bVindexvalid;
	unsigned int iVsizedbf;
	unsigned int iVsizeidx;
	int64vcl iVcachegetkey;

	void __fastcall setrr ();

	static AnsiString __fastcall getfd (AnsiString sPath, AnsiString sFilename);
	static AnsiString __fastcall getfi (AnsiString sPath, AnsiString sFilename);
	bool __fastcall savei (	PTR_CUSTOMSORTEDINDEX cStix, PTR_REMAPPINGLIST cIndexremapping,
													bool bKeepindexed, CoreHsrx::iCsstabletypes iSstabletype, AnsiString sRedotimestamp);
	bool __fastcall loadi (CoreHsrx::iCsstabletypes& iSstabletype, AnsiString& sRedotimestamp);


public:
	const static AnsiString sCindxext;
	const static AnsiString sCdataext;

	__fastcall CoreSstb ();
	__fastcall CoreSstb (PTR_POOL cPool);
  __fastcall ~CoreSstb ();
	void __fastcall init (PTR_POOL cPool);


	void __fastcall setmd (AnsiString sPath, AnsiString sFilename);
	bool __fastcall saved (BASE_CUSTOMMEMTABLE::PTR cMemtable, CoreHsrx::iCsstabletypes iSstabletype, AnsiString sRedotimestamp);
	bool __fastcall loadd (CoreHsrx::iCsstabletypes& iSstabletype, AnsiString& sRedotimestamp);
	bool __fastcall tomem (BASE_CUSTOMMEMTABLE::PTR cMemtable);
	bool __fastcall majcp (CoreSstb::PTR cS1, CoreSstb::PTR cS2, AnsiString sRedotimestamp);
	bool __fastcall shrnk (CoreSstb::PTR cSource, AnsiString sRedotimestamp, bool bSoft, int64vcl iGracetimestamp);
	bool __fastcall get__ (AnsiString& sKey, PrtoLval* cValue);
	AnsiString __fastcall getid ();
	bool __fastcall delfs ();
	void __fastcall locks ();
	void __fastcall relse ();
	unsigned int __fastcall getsc ();
	unsigned int __fastcall getrs ();
	unsigned int __fastcall getma ();
	unsigned int __fastcall getsz ();

	static bool __fastcall chkix (AnsiString sFilename, AnsiString& sRedotimestamp, CoreHsrx::iCsstabletypes& iSstabletype);
	static bool __fastcall chkdk (AnsiString sPath, AnsiString sFilename);
	static bool __fastcall make_ (AnsiString sPath, AnsiString sFilenameold, AnsiString sFilenamenew);
	static bool __fastcall debug (AnsiString sPath, AnsiString sFilename);
	static bool __fastcall dbgix (PTR_POOL cPool, AnsiString sPath, AnsiString sFilename);
	static AnsiString __fastcall debug (PrtoData* cData);

};

};

#endif