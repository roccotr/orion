#ifndef CORESQLL
#define CORESQLL
#include <glog/logging.h>
#include <protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"
#include "CorePool.hpp"
#include "CorePlan.hpp"
#include "CoreFsys.hpp"
#include "PrtoSqll.pb.h"
#include "PrtoAlog.pb.h"
#include "PrtoPlan.pb.h"

namespace orion {

class CoreSqll
{

	const static char bCcharcarriagereturn;

	bool bVopen;
	int iVreadrows;
	int iVreadbytes;
	int iVchartoparse;
	int iVreadbuffer;
	int iVcolumns;
	int iVlimitrows;
	int iVopenkeyposition;
	PTR_POOL cVpool;
	CoreFsys* cVopenfsys;
	CorePlan* cVplanner;
	PrtoSctl* cVsqlcontrol;
	char* bVbuffer;
	char bVseparator;
	PrtoPval cVopenresultrow;
	PrtoLmtb cVopenresultmutable;
	PrtoL2st cVopenstatement;

public:
  __fastcall CoreSqll (PTR_POOL cPool, CorePlan* cPlanner);
  __fastcall ~CoreSqll ();

	bool __fastcall begin (	AnsiString sFilename, PrtoSctl* cSqlcontrol);
	bool __fastcall begin (	AnsiString sFilename, PrtoSctl* cSqlcontrol, 
													PrtoLmtb** cResultmutable, PrtoPval** cResultrow);
	void __fastcall end__ ();
	bool __fastcall readr ();
	int __fastcall getrr ();
	bool __fastcall run__ (bool bDebug = false);
};

};

#endif