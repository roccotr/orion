#include "CorePars.hpp"
#include "CoreButl.hpp"
#include "CorePlan.hpp"
#include "TmplList.cpp" 

using namespace orion;
 
	const AnsiString CorePars::sCkeywordselect = "SELECT";
	const AnsiString CorePars::sCkeywordlocsel = "LOCAL";
	const AnsiString CorePars::sCkeywordinsert = "INSERT";
	const AnsiString CorePars::sCkeywordupdate = "UPDATE";
	const AnsiString CorePars::sCkeyworddelete = "DELETE";
	const AnsiString CorePars::sCkeywordfrom = "FROM";
	const AnsiString CorePars::sCkeywordwhere = "WHERE";
	const AnsiString CorePars::sCkeywordorderby = "ORDER BY";
	const AnsiString CorePars::sCkeywordlimit = "LIMIT";
	const AnsiString CorePars::sCkeywordoffset = "OFFSET";
	const AnsiString CorePars::sCkeywordinto = "INTO";
	const AnsiString CorePars::sCkeywordbracketopen = "(";
	const AnsiString CorePars::sCkeywordbracketclose = ")";
	const AnsiString CorePars::sCkeywordvalues = "VALUES";
	const AnsiString CorePars::sCkeywordset = "SET";

	const AnsiString CorePars::sCquerydelimiter = ",";
	const AnsiString CorePars::sCqueryanddelimiter = " AND ";
	const AnsiString CorePars::sCquerystringdelimiter = "'";
	const AnsiString CorePars::sCquerycondvardelimiter = ":";
	const AnsiString CorePars::sCquerybooleanfalsevalue = "false";
	const AnsiString CorePars::sCqueryorderdesctype = "DESC";

	const AnsiString CorePars::sCminansistring = "";
	const AnsiString CorePars::sCmaxansistring = (char) 255;

  __fastcall CorePars::CorePars() {

		cVselectparsetemplatelocs.set__ ()->sVdelimiter = sCkeywordselect;
		cVselectparsetemplatelocs.set__ ()->sVdelimiter = sCkeywordfrom;
		cVselectparsetemplatelocs.set__ ()->sVdelimiter = sCkeywordwhere;
		cVselectparsetemplatelocs.set__ ()->sVdelimiter = sCkeywordorderby;
		cVselectparsetemplatelocs.set__ ()->sVdelimiter = sCkeywordlimit;
		cVselectparsetemplatelocs.set__ ()->sVdelimiter = sCkeywordoffset;
		preps (&cVselectparsetemplatelocs);
		cVinsertparsetemplatelocs.set__ ()->sVdelimiter = sCkeywordinsert;
		cVinsertparsetemplatelocs.set__ ()->sVdelimiter = sCkeywordinto;
		cVinsertparsetemplatelocs.set__ ()->sVdelimiter = sCkeywordbracketopen;
		cVinsertparsetemplatelocs.set__ ()->sVdelimiter = sCkeywordbracketclose;
		preps (&cVinsertparsetemplatelocs);
		cVinsertvaluesparsetemplatelocs.set__ ()->sVdelimiter = sCkeywordvalues;
		cVinsertvaluesparsetemplatelocs.set__ ()->sVdelimiter = sCkeywordbracketopen;
		cVinsertvaluesparsetemplatelocs.set__ ()->sVdelimiter = sCkeywordbracketclose;
		preps (&cVinsertvaluesparsetemplatelocs);
		cVupdateparsetemplatelocs.set__ ()->sVdelimiter = sCkeywordupdate;
		cVupdateparsetemplatelocs.set__ ()->sVdelimiter = sCkeywordset;
		cVupdateparsetemplatelocs.set__ ()->sVdelimiter = sCkeywordwhere;
		cVupdateparsetemplatelocs.set__ ()->sVdelimiter = sCkeywordlimit;
		preps (&cVupdateparsetemplatelocs);
		cVdeleteparsetemplatelocs.set__ ()->sVdelimiter = sCkeyworddelete;
		cVdeleteparsetemplatelocs.set__ ()->sVdelimiter = sCkeywordfrom;
		cVdeleteparsetemplatelocs.set__ ()->sVdelimiter = sCkeywordwhere;
		cVdeleteparsetemplatelocs.set__ ()->sVdelimiter = sCkeywordlimit;
		preps (&cVdeleteparsetemplatelocs);
		cVbooloptemplatelocs.set__ ()->set__ ("<=", LECOND);
		cVbooloptemplatelocs.set__ ()->set__ (">=", GECOND);
		cVbooloptemplatelocs.set__ ()->set__ ("=", ECOND);
		cVbooloptemplatelocs.set__ ()->set__ ("<", LCOND);
		cVbooloptemplatelocs.set__ ()->set__ (">", GCOND);
	}

  __fastcall CorePars::~CorePars () {

	}

	/*PREpare ParSer*/
	void __fastcall CorePars::preps (CoreList<PARSETEMPLATELOC>* cSelectparsetemplatelocs) {
		PARSETEMPLATELOC* cVparsetemplateloc;
		AnsiString sVleft;
		AnsiString sVright;

		for (int iV = 0; iV < cSelectparsetemplatelocs->count (); iV++) {
			cVparsetemplateloc = cSelectparsetemplatelocs->get__ (iV);
			cVparsetemplateloc->sVname = cVparsetemplateloc->sVdelimiter = 
				cVparsetemplateloc->sVdelimiter.UpperCase ();
			cVparsetemplateloc->sVdelimiterend = CoreButl::right (cVparsetemplateloc->sVdelimiter, " ");
			cVparsetemplateloc->sVdelimiter = CoreButl::left_ (cVparsetemplateloc->sVdelimiter, " ");
		}
	}

	/*DELimiter ParSer*/
	bool __fastcall CorePars::delps (	AnsiString sQuery, 
																		CorePars::PARSESESSION* cParsesession, 
																		CorePars::iCparsetype iParsetype) {
		int iV;
		int iVposition;
		AnsiString sVtemp;
		AnsiString sVquery;
		PARSETEMPLATELOC* cVparsetempateloc;
		PARSELOC* cVparseloc;
		PARSELOC* cVpreviousparseloc;

		cParsesession->cVparselocs.clear ();
		switch (iParsetype) {
			case iCselect: cParsesession->cVparsetemplatelocs = &cVselectparsetemplatelocs; break;
			case iCinsert: cParsesession->cVparsetemplatelocs = &cVinsertparsetemplatelocs; break;
			case iCinsertvalues: cParsesession->cVparsetemplatelocs = &cVinsertvaluesparsetemplatelocs; break;
			case iCupdate: cParsesession->cVparsetemplatelocs = &cVupdateparsetemplatelocs; break;
			case iCdelete: cParsesession->cVparsetemplatelocs = &cVdeleteparsetemplatelocs; break;
		}
		sVquery = sQuery;
		cVpreviousparseloc = NULL;
		for (iV = 0; iV < cParsesession->cVparsetemplatelocs->count (); iV++) {
			cVparsetempateloc = cParsesession->cVparsetemplatelocs->get__ (iV);
			cVparseloc = cParsesession->cVparselocs.set__ ();
			cVparseloc->sVparsed = NULL;						
			iVposition = sVquery.UpperCase ().Pos (cVparsetempateloc->sVdelimiter);
			if (iVposition == 0) continue;
			if (cVpreviousparseloc != NULL) cVpreviousparseloc->sVparsed =
				cVpreviousparseloc->sVparsed.SubString (1, iVposition - 1);
			iVposition += cVparsetempateloc->sVdelimiter.Length ();
			if (cVparsetempateloc->sVdelimiterend != "") {
				iVposition = sVquery.UpperCase ().Pos (cVparsetempateloc->sVdelimiterend);			
				if (iVposition == 0) continue;
				iVposition += cVparsetempateloc->sVdelimiterend.Length ();
			}
			cVparseloc->iVposition = iVposition;
			sVquery = sVquery.SubString (iVposition, sVquery.Length () - iVposition + 1);
			cVparseloc->sVparsed = sVquery;
			cVpreviousparseloc = cVparseloc;
		}
		return true;
	}
	
	/*GET ParSed*/
	AnsiString __fastcall CorePars::getps (
																		CorePars::PARSESESSION* cParsesession, 
																		AnsiString sToken) {
			 
		for (int iV = 0; iV < cParsesession->cVparselocs.count (); iV++) {
			if (cParsesession->cVparsetemplatelocs->get__ (iV)->sVname == sToken) 
				return cParsesession->cVparselocs.get__ (iV)->sVparsed.Trim ();							
		}
		return "";
	}

	/*SELect ParSed*/
	bool __fastcall CorePars::selps (	CorePars::PARSESESSION* cParsesession, PrtoL2qr* cQuery, PTR_TABLELOCVOID cTableloc) {
		AnsiString sVselection;
		AnsiString sVtoken;
		AnsiString sVnettoken;
		CorePlan::PTR_TABLELOC cVtableloc;

		cVtableloc = boost::static_pointer_cast<CorePlan::TABLELOC> (cTableloc);		
		sVselection = getps (cParsesession, sCkeywordselect);
		if (sVselection == NULL) return true;
		cQuery->mutable_cvselect()->Clear ();
		while (sVselection.Length () > 0) {
			sVtoken = CoreButl::left2 (sVselection, sCquerydelimiter);
			sVnettoken = sVtoken.UpperCase ().Trim ();
			if (cVtableloc->cVcolumnpool.bnget (sVnettoken) != NULL)
				cQuery->add_cvselect ()->set_svcolumn	(sVnettoken.to_string ());						
		}
		return true;
	}

	/*INSert ParSed*/
	bool __fastcall CorePars::insps (	CorePars::PARSESESSION* cParsesession, PrtoL2st* cStatement) {
		AnsiString sVinsert;
		AnsiString sVtoken;
		
		sVinsert = getps (cParsesession, sCkeywordbracketopen);
		if (sVinsert == NULL) return true;
		cStatement->mutable_cvcolumns()->Clear();
		while (sVinsert.Length () > 0) {
			sVtoken = CoreButl::left2 (sVinsert, sCquerydelimiter);
			cStatement->add_cvcolumns ()->set_svcolumn (sVtoken.UpperCase ().Trim ().to_string ());	
		}
		return true;
	}

	/*VALues ParSed*/
	bool __fastcall CorePars::valps (	CorePars::PARSESESSION* cParsesession, PrtoL2st* cStatement, PTR_TABLELOCVOID cTableloc) {
		AnsiString sVvalues;
		AnsiString sVtoken;
		bool bVisstring;
		int iVtemp;
		int iVelement;
		iCcolumntype iVcolumntype;
		CorePlan::COLUMNLOC* cVcolumnloc;
		PrtoL2cl* cVcolumn;
		CorePlan::PTR_TABLELOC cVtableloc;

		cVtableloc = boost::static_pointer_cast<CorePlan::TABLELOC> (cTableloc);
		sVvalues = getps (cParsesession, sCkeywordbracketopen);
		if (sVvalues == NULL) return true;
		iVelement = 0;
		while (sVvalues.Length () > 0) {
			sVtoken = CoreButl::left2 (sVvalues, sCquerydelimiter);
			iVtemp = sVtoken.Pos (sCquerystringdelimiter);
			bVisstring = false;
			if (iVtemp > 0) {
				sVtoken = sVtoken.SubString (iVtemp + 1, sVtoken.Length () - iVtemp);
				iVtemp = sVtoken.Pos (sCquerystringdelimiter);
				sVtoken = sVtoken.SubString (1, iVtemp - 1);
				bVisstring = true;
			}
			cVcolumn = cStatement->mutable_cvcolumns (iVelement);
			cVcolumnloc = cVtableloc->cVcolumnpool.bnget (cVcolumn->svcolumn ().c_str ());
			if (cVcolumnloc == NULL) return false;			
			iVcolumntype = (iCcolumntype) cVcolumnloc->iVtype;		
			cVcolumn->mutable_cvvalue ()->set_ivtype (iVcolumntype);	
			ccops (iVcolumntype, cVcolumn->mutable_cvvalue (), sVtoken, bVisstring);
			if (iVelement == 0) cStatement->mutable_cvkey ()->set_svmain (sVtoken.to_string ());
			iVelement++;
		}
		return true;
	}

	/*CONdition ParSe*/
	bool __fastcall CorePars::conps (				AnsiString sQuerycondition,
																					iCconditiontype& iConditiontype,
																					AnsiString& sOpleft, AnsiString& sOpright, bool& bIsoprightastring) {
		int iVposition;
		int iVtemp;
		BOOLOPTEMPLATELOC* cVbooloptemplateloc;
		
		for (int iV = 0; iV < cVbooloptemplatelocs.count (); iV++) {
			cVbooloptemplateloc = cVbooloptemplatelocs.get__ (iV);
			iVposition = sQuerycondition.Pos (cVbooloptemplateloc->sVdelimiter);
			if (iVposition > 0) {
				iConditiontype = cVbooloptemplateloc->iVconditiontype;
				sOpleft = sQuerycondition.SubString ( 1, iVposition - 1).UpperCase ().Trim ();
				iVtemp = iVposition + cVbooloptemplateloc->sVdelimiter.Length ();
				
				sOpright = sQuerycondition.SubString (iVtemp, sQuerycondition.Length () - iVtemp + 1).Trim ();
				iVtemp = sOpright.Pos (sCquerystringdelimiter);
				bIsoprightastring = false;
				if (iVtemp > 0) {
					sOpright = sOpright.SubString (iVtemp + 1, sOpright.Length () - iVtemp);
					iVtemp = sOpright.Pos (sCquerystringdelimiter);
					sOpright = sOpright.SubString (1, iVtemp - 1);
					bIsoprightastring = true;
				}
				return true;
			}
		}
		return false;
	}

	/*Condition COnstant ParSe*/
	bool __fastcall CorePars::ccops (iCcolumntype iColumntype, PrtoL2cv* cColumnvalue, AnsiString sOpright, bool bIsoprightastring) {

		switch (iColumntype) {
			case STRINGTYPE: 
				if (bIsoprightastring) cColumnvalue->set_svvalue (sOpright.to_string ()); break;
			case LSTRNGTYPE:
				if (bIsoprightastring) cColumnvalue->add_svlistvalue (sOpright.to_string ()); break;
			case INTEGRTYPE:
				cColumnvalue->set_ivvalue (sOpright.ToInt ()); break;
			case LINTGRTYPE:
				cColumnvalue->add_ivlistvalue (sOpright.ToInt ()); break;
			case DOUBLETYPE:
				cColumnvalue->set_dvvalue (sOpright.ToDouble ()); break;
			case LDOUBLTYPE:
				cColumnvalue->add_dvlistvalue (sOpright.ToDouble ()); break;
			case BOOLN_TYPE:
				cColumnvalue->set_bvvalue (sOpright != sCquerybooleanfalsevalue); break;
		}
		return true;
	}

	/*FRoM ParSed*/
	bool  __fastcall CorePars::frmps (	AnsiString sNamespace, CorePars::PARSESESSION* cParsesession, PrtoL2qr* cQuery) {
		PrtoLmtb* cVmutable;

		cVmutable = cQuery->mutable_cvmutable ();
		cVmutable->Clear ();
		cVmutable->set_svnamespace (sNamespace.to_string ());
		cVmutable->set_svtable (getps (cParsesession, sCkeywordfrom).UpperCase ().Trim ().to_string ());
		return true;
	}

	/*Single TaBle ParSed*/
	bool  __fastcall CorePars::stbps (	AnsiString sNamespace, int64vcl iTimestamp, 
																			CorePars::PARSESESSION* cParsesession, PrtoL2st* cStatement,
																			AnsiString sKeyword) {
		PrtoLmtb* cVmutable;

		cVmutable = cStatement->mutable_cvmutable ();
		cVmutable->Clear ();
		cVmutable->set_svnamespace (sNamespace.to_string ());
		cVmutable->set_svtable (getps (cParsesession, sKeyword).UpperCase ().Trim ().to_string ());
		cStatement->set_ivtimestamp (iTimestamp);
		return true;
	}

	/*WHeRe ParSed*/
	bool __fastcall CorePars::whrps (	CorePars::PARSESESSION* cParsesession, 
																		PrtoL2qr* cQuery, PTR_TABLELOCVOID cTableloc,
																		google::protobuf::RepeatedPtrField<PrtoL2cv>* cInputbindings) {
		AnsiString sVwhere;
		AnsiString sVtoken;
		AnsiString sVopleft;
		AnsiString sVopright;
		bool bVisoprightastring;
		int iVcurrentbinding;
		PrtoL2wh* cVwhere;
		PrtoL2cl* cVcondition;
		PrtoL2cv* cVcolumnvalue;
		iCconditiontype iVconditiontype;
		iCcolumntype iVcolumntype;
		CorePlan::COLUMNLOC* cVcolumnloc;
		CorePlan::PTR_TABLELOC cVtableloc;

		cVtableloc = boost::static_pointer_cast<CorePlan::TABLELOC> (cTableloc);
		iVcurrentbinding = 0;
		sVwhere = getps (cParsesession, sCkeywordwhere);
		if (sVwhere == NULL) return true;
		cVwhere = cQuery->mutable_cvwhere ();
		cVwhere->Clear ();
		while (sVwhere.Length () > 0) {
			sVtoken = CoreButl::left3 (sVwhere, sCqueryanddelimiter);
			if (!conps (sVtoken, iVconditiontype, sVopleft, sVopright, bVisoprightastring)) return false;
			cVcondition = cVwhere->add_cvcondition ();

			cVcondition->set_ivconditiontype (iVconditiontype);
			cVcondition->set_svcolumn (sVopleft.to_string ());				
			cVcolumnvalue = cVcondition->mutable_cvvalue ();
			if (!bVisoprightastring && sVopright.Pos (sCquerycondvardelimiter) > 0) {		
				if (iVcurrentbinding >= cInputbindings->size ()) return false;
				cVcolumnvalue->CopyFrom (cInputbindings->Get (iVcurrentbinding++));		
			} else {
				cVcolumnloc = cVtableloc->cVcolumnpool.bnget (sVopleft);
				if (cVcolumnloc == NULL) return false;			
				iVcolumntype = (iCcolumntype) cVcolumnloc->iVtype;		
				cVcolumnvalue->set_ivtype (iVcolumntype);	
				ccops (iVcolumntype, cVcolumnvalue, sVopright, bVisoprightastring);
			}						
		}		
		return true;
	}

	/*SET ParSed*/
	bool __fastcall CorePars::setps (CorePars::PARSESESSION* cParsesession, PrtoL2st* cStatement, PTR_TABLELOCVOID cTableloc,
												 google::protobuf::RepeatedPtrField<PrtoL2cv>* cInputbindings) {
		AnsiString sVset;
		AnsiString sVtoken;
		AnsiString sVopleft;
		AnsiString sVopright;
		CorePlan::PTR_TABLELOC cVtableloc;
		iCconditiontype iVconditiontype;
		int iVcurrentbinding;
		bool bVisoprightastring;
		PrtoL2cl* cVsetval;
		PrtoL2cv* cVcolumnvalue;
		CorePlan::COLUMNLOC* cVcolumnloc;
		iCcolumntype iVcolumntype;

		iVcurrentbinding = 0;
		cVtableloc = boost::static_pointer_cast<CorePlan::TABLELOC> (cTableloc);
		sVset = getps (cParsesession, sCkeywordset);
		if (sVset == NULL) return true;
		while (sVset.Length () > 0) {
			sVtoken = CoreButl::left2 (sVset, sCquerydelimiter);
			if (!conps (sVtoken, iVconditiontype, sVopleft, sVopright, bVisoprightastring)) return false;
			cVsetval = cStatement->add_cvcolumns ();
			cVsetval->set_svcolumn (sVopleft.to_string ());
			cVcolumnvalue = cVsetval->mutable_cvvalue ();
			if (!bVisoprightastring && sVopright.Pos (sCquerycondvardelimiter) > 0) {		
				cVcolumnvalue->CopyFrom (cInputbindings->Get (iVcurrentbinding++));		
			} else {
				cVcolumnloc = cVtableloc->cVcolumnpool.bnget (sVopleft);
				if (cVcolumnloc == NULL) return false;			
				iVcolumntype = (iCcolumntype) cVcolumnloc->iVtype;		
				cVcolumnvalue->set_ivtype (iVcolumntype);	
				ccops (iVcolumntype, cVcolumnvalue, sVopright, bVisoprightastring);
			}						
		}	
		return true;
	}

	/*ORDer by ParSed*/
	bool __fastcall CorePars::ordps (	CorePars::PARSESESSION* cParsesession, PrtoL2qr* cQuery) {
		AnsiString sVorderby;
		AnsiString sVtoken;
		AnsiString sVcolumn;
		PrtoL2or* cVorderby;

		sVorderby = getps (cParsesession, sCkeywordorderby);
		if (sVorderby == NULL) return true;
		while (sVorderby.Length () > 0) {
			sVtoken = CoreButl::left2 (sVorderby, sCquerydelimiter).UpperCase ().Trim ();	
			sVcolumn = CoreButl::left2 (sVtoken, " ");		
			cVorderby = cQuery->mutable_cvwhere ()->add_cvorderby ();
			cVorderby->set_svcolumn (sVcolumn.to_string());
			cVorderby->set_ivordertype (sVtoken != sCqueryorderdesctype ? ASC : DESC);
		}		
		return true;
	}

	/*LIMit ParSed*/
	bool __fastcall CorePars::limps (	CorePars::PARSESESSION* cParsesession, PrtoL2qr* cQuery) {
		AnsiString sVlimit;
		int iVvalue;

		sVlimit = getps (cParsesession, sCkeywordlimit).Trim ();
		if (sVlimit.Length () > 0) {
			iVvalue = sVlimit.ToInt ();
			if (iVvalue > 0) cQuery->set_ivcount (iVvalue);
		}
		return true;
	}


	/*Build FaKE Condition*/
	bool __fastcall CorePars::bfkec (PrtoL2or* cOrderby, PrtoL2cl* cColumn, void* cColumnloc) {
		bool bVasc;
		PrtoL2cv* cVvalue;

		cColumn->set_svcolumn (cOrderby->svcolumn ());
		if (cColumnloc == NULL) return false;
		bVasc = cOrderby->ivordertype () == ASC;
		cColumn->set_ivconditiontype (bVasc ? GCOND : LCOND);
		cVvalue = cColumn->mutable_cvvalue ();
		cVvalue->set_ivtype ((iCcolumntype) ((CorePlan::COLUMNLOC*) cColumnloc)->iVtype);
		switch (cVvalue->ivtype ()) {
			case STRINGTYPE:
			case LSTRNGTYPE:
				cVvalue->set_svvalue (bVasc ? sCminansistring.to_string () : sCmaxansistring.to_string ());		
				return true;
		}
		return false;
	}
