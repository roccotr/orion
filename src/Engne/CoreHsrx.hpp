#ifndef COREHSRX
#define COREHSRX
#include <glog/logging.h>
#include <protocol/TBinaryProtocol.h>
#include "CorePool.hpp"
#include "CoreStix.hpp"
#include "CoreSoix.hpp"

namespace orion {

#define iCunchained -1

class CoreHsrx {

public:
  enum iCsstabletypes { 
		iCsstable = 0,
		iCmemorysstable = 1,
		iCpartialmemtable = 2
	};

private:
	static const int iCcollisionfactor;
	const static AnsiString sCindexmagic;

	typedef struct CoreIxhd {
		char bCmagic [4];
		int iVversion;

		CoreIxhd () {}

		CoreIxhd (int iVersion, AnsiString sMagic) {
			iVversion = iVersion;
			strncpy (bCmagic, sMagic.c_str (), sMagic.Length ());
		}
	} INDEXHEADER;

	PTR_POOL cVpool;
	PTR_SORTEDINDEX cVsortedradix;

private:
	static AnsiString __fastcall getcp (AnsiString sKey1, AnsiString sKey2);
	static unsigned int __fastcall hash_ (AnsiString sKey);


	bool __fastcall tocnt (PrtoIndx* cVproto, iCsstabletypes iSstabletype, AnsiString sRedotimestamp);
	bool __fastcall tocnt (PTR_FILESYSTEM cFilesystem, iCsstabletypes iSstabletype, AnsiString sRedotimestamp);
	bool __fastcall fmcnt (PrtoIndx* cVproto, iCsstabletypes& iSstabletype, AnsiString& sRedotimestamp);
	bool __fastcall fmcnt (PTR_FILESYSTEM cFilesystem, iCsstabletypes& iSstabletype, AnsiString& sRedotimestamp);
	bool __fastcall torio (PTR_FILESYSTEM cFilesystem, iCsstabletypes iSstabletype, AnsiString sRedotimestamp);
	bool __fastcall fmrio (PTR_FILESYSTEM cFilesystem, iCsstabletypes& iSstabletype, AnsiString& sRedotimestamp);

public:
	static const int iCshorterindexes;

	typedef struct {
		int64vcl iVindexed;
	} REMAPPINGINDEXLOC;

	__fastcall CoreHsrx (PTR_POOL cPool);
  __fastcall ~CoreHsrx ();
	bool __fastcall build (PTR_CUSTOMSORTEDINDEX cSortedindex, bool bKeepindexed = false, bool bCreatecommonprefix = false);
	void __fastcall clear ();
	int64vcl __fastcall query (	AnsiString sKey);
	bool __fastcall rmpng (boost::shared_ptr<CoreList<CoreHsrx::REMAPPINGINDEXLOC> >  cIndexremapping);
		
	bool __fastcall tocnt (AnsiString sFilename, iCsstabletypes iSstabletype, AnsiString sRedotimestamp, int64vcl& iSize, int iFormat = 1);
	bool __fastcall fmcnt (AnsiString sFilename, iCsstabletypes& iSstabletype, AnsiString& sRedotimestamp, int64vcl& iSize);


	PTR_SORTEDINDEX __fastcall getsx ();
	unsigned int __fastcall getsc ();
	unsigned int __fastcall getsz ();
	AnsiString __fastcall debug ();

};

typedef boost::shared_ptr<CoreHsrx> PTR_HASHRADIX;
typedef boost::shared_ptr<CoreList<CoreHsrx::REMAPPINGINDEXLOC> > PTR_REMAPPINGLIST;

};

#endif