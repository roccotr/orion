#ifndef COREPARS
#define COREPARS
#include <glog/logging.h>
#include <protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"
#include "CoreList.hpp"
#include "PrtoAlog.pb.h"  

namespace orion {

typedef boost::shared_ptr<void> PTR_TABLELOCVOID;
using namespace com::tomting::orion; 

class CorePars {

public:
	typedef struct CorePslc {
		AnsiString sVparsed;
		int iVposition;

	} PARSELOC;

	typedef struct CorePspt {
		AnsiString sVname;
		AnsiString sVdelimiter;
		AnsiString sVdelimiterend;

		/*INITIALIZE*/
		CorePspt() {
			
			sVdelimiterend = NULL;
		}
	} PARSETEMPLATELOC;

	typedef struct CorePsss {
		CoreList <PARSETEMPLATELOC>* cVparsetemplatelocs;
		CoreList <PARSELOC> cVparselocs;	
	} PARSESESSION;

private:

	typedef struct CoreBtlc {
		AnsiString sVdelimiter;
		iCconditiontype iVconditiontype;

		void set__ (AnsiString sDelimiter, iCconditiontype iConditiontype) {
			sVdelimiter = sDelimiter;
			iVconditiontype = iConditiontype;
		}
	} BOOLOPTEMPLATELOC;


	const static AnsiString sCkeywordorderby;
	const static AnsiString sCkeywordoffset;
	const static AnsiString sCkeywordbracketopen;
	const static AnsiString sCquerydelimiter;
	const static AnsiString sCqueryanddelimiter;
	const static AnsiString sCquerystringdelimiter;
	const static AnsiString sCquerycondvardelimiter;
	const static AnsiString sCquerybooleanfalsevalue;
	const static AnsiString sCqueryorderdesctype;

	CoreList<PARSETEMPLATELOC> cVselectparsetemplatelocs;	
	CoreList<PARSETEMPLATELOC> cVinsertparsetemplatelocs;	
	CoreList<PARSETEMPLATELOC> cVinsertvaluesparsetemplatelocs;	
	CoreList<PARSETEMPLATELOC> cVupdateparsetemplatelocs;	
	CoreList<PARSETEMPLATELOC> cVdeleteparsetemplatelocs;	
	CoreList<BOOLOPTEMPLATELOC> cVbooloptemplatelocs;

public:
	const static AnsiString sCkeywordselect;
	const static AnsiString sCkeywordlocsel;
	const static AnsiString sCkeywordinsert;
	const static AnsiString sCkeywordupdate;
	const static AnsiString sCkeyworddelete;
	const static AnsiString sCkeywordbracketclose;
	const static AnsiString sCkeywordinto;
	const static AnsiString sCkeywordvalues;
	const static AnsiString sCkeywordset;
	const static AnsiString sCkeywordwhere;
	const static AnsiString sCkeywordfrom;
	const static AnsiString sCkeywordlimit;

	const static AnsiString sCminansistring;
	const static AnsiString sCmaxansistring;

	enum iCparsetype {
		iCselect = 0,
		iCinsert = 1,
		iCinsertvalues = 2,
		iCupdate = 3,
		iCdelete = 4
	};

  __fastcall CorePars();
  __fastcall ~CorePars ();

	void __fastcall preps (CoreList<PARSETEMPLATELOC>* cSelectparsetemplatelocs);
	bool __fastcall delps (	AnsiString sQuery,  
													CorePars::PARSESESSION* cParsesession, 
													CorePars::iCparsetype iParsetype);
	bool __fastcall selps (CorePars::PARSESESSION* cParsesession, PrtoL2qr* cQuery, PTR_TABLELOCVOID cTableloc);
	bool __fastcall insps (	CorePars::PARSESESSION* cParsesession, PrtoL2st* cStatement);
	bool __fastcall ccops (iCcolumntype iColumntype, PrtoL2cv* cColumnvalue, 
												 AnsiString sOpright, bool bIsoprightastring);
	bool __fastcall conps (AnsiString sQuerycondition,
												 iCconditiontype& iConditiontype, AnsiString& sOpleft, 
												 AnsiString& sOpright, bool& bIsoprightastring);
	bool __fastcall frmps (AnsiString sNamespace, CorePars::PARSESESSION* cParsesession, PrtoL2qr* cQuery);
	bool __fastcall stbps (	AnsiString sNamespace, int64vcl iTimestamp,
													CorePars::PARSESESSION* cParsesession, PrtoL2st* cStatement,
													AnsiString sKeyword);
	bool __fastcall valps (CorePars::PARSESESSION* cParsesession, PrtoL2st* cStatement, PTR_TABLELOCVOID cTableloc);
	bool __fastcall whrps (CorePars::PARSESESSION* cParsesession, PrtoL2qr* cQuery, PTR_TABLELOCVOID cTableloc,
												 google::protobuf::RepeatedPtrField<PrtoL2cv>* cInputbindings);
	bool __fastcall setps (CorePars::PARSESESSION* cParsesession, PrtoL2st* cStatement, PTR_TABLELOCVOID cTableloc,
												 google::protobuf::RepeatedPtrField<PrtoL2cv>* cInputbindings);
	bool __fastcall ordps (CorePars::PARSESESSION* cParsesession, PrtoL2qr* cQuery);
	bool __fastcall limps (CorePars::PARSESESSION* cParsesession, PrtoL2qr* cQuery);

	static AnsiString __fastcall getps (CorePars::PARSESESSION* cParsesession, AnsiString sToken);
	static bool __fastcall bfkec (PrtoL2or* cOrderby, PrtoL2cl* cColumn, void* cTableloc);
};


typedef boost::shared_ptr<CorePars> PTR_PARSER;

};

#endif
