#ifndef COREMKTR
#define COREMKTR
#include <glog/logging.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"
#include "CoreList.hpp"
#include "CoreAlog.hpp"
#include "CoreAutl.hpp"

namespace orion {

class CoreMktr {

	typedef struct CoreNdlc {
		PrtoLkey cVkey;
		PrtoLval cVvalue;

		CoreNdlc () {
			
			CoreAutl::zrkey (&cVkey);
		}
	} NODEDATALOC;


	typedef struct CoreNhlc {

		bool bVleaf;
		AnsiString sVhash;
		void* cVfather;
		int iVmerkeltreedataposition;
		CoreList<CoreNhlc*> cVchildrenpool;

		/*INITIALIZE*/
		CoreNhlc () {
			bVleaf = false;
			cVfather = NULL;
			sVhash = "";
			iVmerkeltreedataposition = -1;
		}

	} NODEHASHLOC;

	typedef struct CoreLhlc {

		CoreList<NODEHASHLOC> cVlevel;
	} LEVELHASHLOC;

	bool bVmaxsizecheck;
	bool bVcheckonlydestination;
	int iVmaxdim;
	int iVmaxsize;
	int iVleafdim;
	AnsiString sVsourcenode;
	AnsiString sVdestinationnode;
	AnsiString sVkeystart;
	AnsiString sVkeyend;
	CoreList<NODEDATALOC> cVmerkeltreedata;
	PrtoMktr cVmerkeltreehash;
	PTR_REDOLOG cVredolog;
	PTR_PARTITIONER cVpartitioner;
	PTR_ENDPOINT cVendpoint;
	PTR_REVERSE cVreverse;
	CoreTbsv* cVtabletserver;
	CorePart::PARTITIONLOC* cVpartitionloc;
	PrtoLmtb cVmutable;

	void __fastcall bmkin (NODEHASHLOC* cNode, PrtoMktr* cPrtonode, int iLevel = 0);
	bool __fastcall bmkin (int iLeafdim);

public:
  __fastcall CoreMktr(	PTR_REDOLOG cRedolog, 
												PTR_PARTITIONER cPartitioner, PTR_ENDPOINT cEndpoint,
												AnsiString sSourcenode, int iLeafdim = 10);
  __fastcall ~CoreMktr ();

	bool __fastcall rbld_ ();
	void __fastcall build (	PrtoLmtb* cMutable, CorePart::PARTITIONLOC* cPartitionloc);
	bool __fastcall build (	bool bCheckonlydestination, AnsiString sDestinationnode, 
													PrtoLmtb* cMutable, CorePart::PARTITIONLOC* cPartitionloc,
													AnsiString sKeystart, AnsiString sKeyend, int iMaxdim, int iMaxsize = -1);
	int __fastcall getdm ();
	AnsiString __fastcall getlm ();
	PrtoMktr* __fastcall getmh ();
	bool __fastcall pllmh (PrtoMktr* cInput, PrtoMktr* cOutput, PrtoMktr* cCurrent = NULL, bool bFix = false);
	int __fastcall apply (PrtoMktr* cInput);

	bool __fastcall pushs (PrtoMktr* cOutput);
	bool __fastcall pushd (PrtoMktr* cInput, PrtoMktr* cOutput);
	bool __fastcall pulls (PrtoMktr* cInput, PrtoMktr* cOutput);
	bool __fastcall pulld (PrtoMktr* cInput, PrtoMktr* cOutput);

	bool __fastcall purge (PrtoLmtb* cMutable, CorePart::PARTITIONLOC* cPartitionloc); 

	static AnsiString __fastcall debug (PrtoMktr* cInput, int iLevel = 0);

};

};

#endif