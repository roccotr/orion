#ifndef CORESOIX
#define CORESOIX
#include <glog/logging.h>
#include <protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"
#include "CoreStib.hpp"
#include "CoreOo2w.hpp"
#include "CoreFsys.hpp"
#include "PrtoSstb.pb.h"


namespace orion{

	using namespace com::tomting::orion;


//#undef iCoo2sortedindex
 
#ifdef iCoo2sortedindex
	#define iCmemtableengine "O2"
	#define PTR_CUSTOMSORTEDINDEX PTR_SORTEDOO2INDEX
	#define CUSTOMSORTEDINDEXPOS CoreSoix::OO2POSITION
	#define BASE_CUSTOMSORTEDINDEX CoreSoix
  #define CUSTOMSORTEDINDEXPOSNULL CoreSoix::OO2POSITION (-1, NULL)
	#define BASE_CUSTOMMEMTABLE CoreMeot
#else
	#define iCmemtableengine "List"
	#define PTR_CUSTOMSORTEDINDEX PTR_SORTEDINDEX
  #define CUSTOMSORTEDINDEXPOS int
	#define BASE_CUSTOMSORTEDINDEX CoreStix
  #define CUSTOMSORTEDINDEXPOSNULL -1
	#define BASE_CUSTOMMEMTABLE CoreMetb
#endif

#define OO2POSITIONNULL OO2POSITION (-1, NULL)

class CoreSoix  : CoreStib {
public: 

#define iCsoixleafdim 4

	typedef CoreOo2w<SORTEDINDEXLOC, iCsoixleafdim>::OO2POS OO2POSITION;

private:
	CoreOo2w<SORTEDINDEXLOC, iCsoixleafdim> cVlist;
	void deall (SORTEDINDEXLOC* cSortedindexloc);
	void alloc (SORTEDINDEXLOC* cSortedindexloc, DATALOC* cDataloc);

	PTR_POOL cVpool;
	unsigned int iVcurrentsize;

public:

  __fastcall CoreSoix (PTR_POOL cPool); 
  __fastcall ~CoreSoix ();
	bool __fastcall add__ (AnsiString sKey, int64vcl iIndexed = 0, short int iMainindexlength = -1);
	bool __fastcall addop (AnsiString sKey, int64vcl iIndexed, short int iMainindexlength, 
												bool& bRecycle, unsigned int& iRecycle);
	bool __fastcall addo2 (	AnsiString sKey, short int iMainindexlength, DATALOC* cDataloc);
	bool __fastcall addns (AnsiString sKey, int64vcl iIndexed = 0, short int iMainindexlength = -1);
	OO2POSITION __fastcall bnsrc (AnsiString sKey);
	AnsiString __fastcall get__ (OO2POSITION iPosition, bool bOnlymain = false);
	bool __fastcall delop (AnsiString sKey, short int iMainindexlength, unsigned int& iRecycle);
	bool __fastcall delo2 (AnsiString& sKey, short int iMainindexlength, bool& bSearchallkey);	
	void __fastcall deldp ();
	SORTEDINDEXLOC* __fastcall getil (OO2POSITION iPosition);
	int __fastcall count ();
	unsigned int __fastcall getsz ();
	unsigned int __fastcall getsd ();
	void __fastcall clear ();

	int64vcl __fastcall getix (OO2POSITION iPosition);

	void __fastcall tocnt (PrtoIndx* cVproto);
	void __fastcall fmcnt (PrtoIndx* cVproto);

	OO2POSITION first ();
	OO2POSITION last_ ();

	AnsiString __fastcall debug ();
};

typedef boost::shared_ptr<CoreSoix> PTR_SORTEDOO2INDEX;

};

#endif