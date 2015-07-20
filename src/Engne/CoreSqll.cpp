#include "CoreSqll.hpp"

using namespace orion;

	const char CoreSqll::bCcharcarriagereturn = '\n';

  __fastcall CoreSqll::CoreSqll (PTR_POOL cPool, CorePlan* cPlanner) {
		
		cVpool = cPool;
		bVopen = false;
		cVopenfsys = new CoreFsys (cVpool);
		cVplanner = cPlanner;
		iVreadrows = 0;
		iVreadbytes = 0;
		iVchartoparse = 0;
		bVbuffer = NULL;
	}
  
	__fastcall CoreSqll::~CoreSqll () {
		
		end__ ();
		delete cVopenfsys;
	}

  /*OPEN*/
	bool __fastcall CoreSqll::begin (	AnsiString sFilename, PrtoSctl* cSqlcontrol) {
		PrtoScol* cVcolumn;
		PrtoPvlc* cVtargetcolumn;
		PrtoL2cl* cVcreatetablecolumn;
		PrtoL2ct cVcreatetable;

		cVopenstatement.Clear ();
		cVsqlcontrol = cSqlcontrol;
		bVseparator = (char) cVsqlcontrol->svterminatedby ().c_str () [0];
		iVreadbuffer = 0;
		iVcolumns = cVsqlcontrol->cvcolumns_size ();
		if (cVsqlcontrol->has_ivlimitrows ()) iVlimitrows = cVsqlcontrol->ivlimitrows ();
		else iVlimitrows = -1;
		cVopenresultrow.Clear ();
		for (int iV = 0; iV < iVcolumns; iV++) {	
			cVcolumn = cVsqlcontrol->mutable_cvcolumns (iV);	
			if (cVcolumn->bviskey ()) iVopenkeyposition = iV;
			cVcreatetablecolumn = cVcreatetable.mutable_cvcolumns ()->Add ();
			switch (cVcolumn->ivtype ()) {
				case STRINGLIST: cVcreatetablecolumn->set_ivtype (LSTRNGTYPE); break;
				case INTEGER: cVcreatetablecolumn->set_ivtype (INTEGRTYPE); break;
				case STRING:
				default: cVcreatetablecolumn->set_ivtype (STRINGTYPE);
			}
			cVcreatetablecolumn->set_svcolumn (cVcolumn->svcolumn ());
			cVcreatetablecolumn = cVopenstatement.mutable_cvcolumns ()->Add ();
			cVcreatetablecolumn->set_svcolumn (cVcolumn->svcolumn ());
			iVreadbuffer += cVcolumn->ivlength ();	
			cVtargetcolumn = cVopenresultrow.add_cvvaluecells ();
			cVtargetcolumn->set_svcolumn (cVcolumn->svcolumn ());
		}				
		if (cVopenfsys->begin (iVreadbuffer, sFilename, true)) {
			bVopen = true;
			bVbuffer = (char*) cVpool->mallc (CorePool::iCchar, 2 * iVreadbuffer, "SQLL");
		}
		cVopenresultmutable.set_svnamespace (cSqlcontrol->svnamespace ());
		cVopenresultmutable.set_svtable (cSqlcontrol->svtable ());
		cVcreatetable.mutable_cvmutable ()->CopyFrom (cVopenresultmutable);
		bVopen &= cVplanner->crttb (&cVcreatetable);		
		cVopenstatement.mutable_cvmutable ()->CopyFrom (cVopenresultmutable);
		cVopenstatement.mutable_cvkey ()->set_ivstate (UPSERT);
		cVopenstatement.mutable_cvcolumns ()->CopyFrom (*cVcreatetable.mutable_cvcolumns ());
		return bVopen;
	}

  /*OPEN*/
	bool __fastcall CoreSqll::begin (	AnsiString sFilename, PrtoSctl* cSqlcontrol, 
																		PrtoLmtb** cResultmutable, PrtoPval** cResultrow) {

		begin (sFilename, cSqlcontrol);
		*cResultrow = &cVopenresultrow;
		*cResultmutable = &cVopenresultmutable;		
		return bVopen;
	}

	/*END*/
	void __fastcall CoreSqll::end__ () {

		if (bVopen) {
			bVopen = false;
			if (bVbuffer != NULL) cVpool->free_ (bVbuffer);
			bVbuffer = NULL;
			cVopenfsys->end__ ();
		}
	}


	/*READ Row*/
	bool __fastcall CoreSqll::readr () {
		AnsiString sVreadstring;
		void* vVdata;
		int iVread;
		int iVelement;
		int iVprevious;

		if (iVreadrows == iVlimitrows) return false;
		if (iVchartoparse < iVreadbuffer) {			
			iVread = iVreadbuffer;
			if (!cVopenfsys->rchnk (&iVread)) return false;
			vVdata = cVopenfsys->baddr ();
			iVreadbytes += iVread;			
			memcpy (&bVbuffer [iVchartoparse], vVdata, iVread);
			iVchartoparse += iVread;
			if (iVread == 0) return false;
		}
		iVelement = 0;
		iVprevious = 0;
		for (int iV = 0; iV < iVchartoparse; iV++) {		
			if (bVbuffer [iV] == bVseparator || bVbuffer [iV] == bCcharcarriagereturn) {
				if (iV > 0) sVreadstring = AnsiString (&bVbuffer [iVprevious], iV + 1 - iVprevious);
				else sVreadstring = "";
				iVprevious = iV + 1;
				cVopenresultrow.mutable_cvvaluecells (iVelement)->set_svvalue (sVreadstring.to_string ());
				iVelement++;
			}
			if (bVbuffer [iV] == bCcharcarriagereturn && (iV + 1 < iVchartoparse)) {
				iVprevious = 0;
				iVchartoparse -= iV + 1;
				memmove (bVbuffer, &bVbuffer [iV + 1], iVchartoparse);
				iVreadrows++;
				return true;
			}
		}
		return false;
	}

	/*GET Read Rows*/
	int __fastcall CoreSqll::getrr () {

		return iVreadrows;
	}

	/*RUN*/
	bool __fastcall CoreSqll::run__ (bool bDebug) {
		PrtoPvlc* cVvaluecell;
		int iVopenstatementcolumn;
		int iVpreviouslength;
		AnsiString sVtemp;
		AnsiString sVlstringvalue;
		char* bVtemp;
		char bVlistseparator;
		CorePlan::PTR_TABLELOC cVtableloc;
		
		bVlistseparator = (char) cVsqlcontrol->svlistseparator ().c_str () [0];
		while (readr ()) {
			cVopenstatement.mutable_cvkey ()->set_svmain (
				cVopenresultrow.mutable_cvvaluecells (iVopenkeyposition)->svvalue ());
			cVopenstatement.mutable_cvkey ()->set_ivtimestamp (CoreButl::tmstl ());
			iVopenstatementcolumn = 0;
			for (int iV = 0; iV < cVopenresultrow.mutable_cvvaluecells ()->size (); iV++) {	
				cVvaluecell = cVopenresultrow.mutable_cvvaluecells (iV);
				if (bDebug) {
					printf ("[%s] ", cVopenstatement.mutable_cvcolumns (iV)->mutable_svcolumn ()->c_str ());
					printf ("%s\n", cVvaluecell->mutable_svvalue ()->c_str ());
				}
				switch (cVopenstatement.mutable_cvcolumns (iV)->ivtype ()) {
					case INTEGRTYPE: 			
						cVopenstatement.mutable_cvcolumns (iVopenstatementcolumn)->
							mutable_cvvalue ()->set_ivvalue (((AnsiString) cVvaluecell->svvalue ()).ToInt ());	
					case LSTRNGTYPE:
						cVopenstatement.mutable_cvcolumns (iVopenstatementcolumn)->mutable_cvvalue ()->Clear ();
						sVtemp = cVvaluecell->svvalue ();
						bVtemp = sVtemp.c_str ();
						iVpreviouslength = 0;
						for (int iV = 0; iV < sVtemp.Length (); iV++) {
							if (bVtemp [iV] == bVlistseparator || iV == sVtemp.Length () - 1) {
								sVlstringvalue = AnsiString (bVtemp, iV + 1).SubString (iVpreviouslength + 1, iV - iVpreviouslength);
								cVopenstatement.mutable_cvcolumns (iVopenstatementcolumn)->mutable_cvvalue ()->add_svlistvalue (
									sVlstringvalue.to_string ());
								iVpreviouslength = iV + 1;
							}
						}
						break;
					case STRINGTYPE: 
					default: 
						cVopenstatement.mutable_cvcolumns (iVopenstatementcolumn)->
							mutable_cvvalue ()->set_svvalue (cVvaluecell->svvalue ());						
						break;
				}
				iVopenstatementcolumn++;
			}
			cVplanner->statm (&cVopenstatement, true, false, false, &cVtableloc);
		}
		return true;
	}