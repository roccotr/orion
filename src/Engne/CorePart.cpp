#include "CorePart.hpp"
#include "CoreBint.hpp"
#include "CoreButl.hpp"
#include "TmplList.cpp"
#include "md5.h"

using namespace orion;

	const AnsiString CorePart::sCchar0 = AnsiString (char (0), true);
	const AnsiString CorePart::sCignored = "[IGNORED]";

  __fastcall CorePart::CorePart() {
	}

  __fastcall CorePart::~CorePart () {
	}

	/*INIT*/
	void __fastcall CorePart::init_ (PTR_ENDPOINT cEndpoint) {

		cVendpoint = cEndpoint;
	}

	/*GET ToKen*/
	AnsiString __fastcall CorePart::gettk (AnsiString sKey) {

		return md5 (sKey.to_string ());
	}

	
	/*DEScribe OWnership*/
	int __fastcall CorePart::desow (	iCpartitiontype iPartitiontype, AnsiString sKey,
																		bool bIndex, CorePart::PARTITIONLOC* cPartitionloc,
																		bool bScan) {
		AnsiString sVtoken;
		CoreBint cVkey; 
		int iVcount;
		CoreList<CUSTOMPARTITIONLOC>* cVcustompartitionpool;

		switch (iPartitiontype) {
			case iCrandompartition:
				if (bScan) return -1;
				sVtoken = gettk (sKey.to_string ());
				break;
			case iCbyteorderpartition:
				cVkey.ans2b (sKey);
				sVtoken = cVkey.write ();		
				break;
			case iCcustompartition:
				cVcustompartitionpool = 
					bIndex ? &cPartitionloc->cVpartitionindex.cVpartitionpool : &cPartitionloc->cVpartition.cVpartitionpool;
				iVcount = cVcustompartitionpool->count ();
				sVtoken = cVcustompartitionpool->get__ ((cVcustompartitionpool->bnsrc (sKey) + iVcount - 1) % iVcount)->sVtoken;
				break;
			default:
				sVtoken = "";
		}		
		return cVendpoint->tk2po (sVtoken);
	}

	/*NEXT*/
	bool __fastcall CorePart::next_ (	int iFirstseed, 
																		iCpartitiontype iPartitiontype, 
																		CoreTble::iCquerytype iQuerytype,
																		int& iCursor) {

		iCursor = cVendpoint->next_ (iCursor);
		switch (iPartitiontype) {
			case iCrandompartition:
				switch (iQuerytype) {
					case CoreTble::iCexactquery: 
						return false;
					case CoreTble::iCrangequery:
					case CoreTble::iCtokenquery:					
						return !(iCursor == iFirstseed);
				}
			case iCbyteorderpartition:
				switch (iQuerytype) {
					case CoreTble::iCexactquery: 
						return false;
					case CoreTble::iCrangequery:
						return iCursor != 0;
					case CoreTble::iCtokenquery:
						return !(iCursor == iFirstseed);
				}
			default: return false;
		}
	}

	/*SET Partition Data*/
	void __fastcall CorePart::setpd (	CorePtcl* cCustompartition, 
																		AnsiString sNodeid, AnsiString sToken, AnsiString sPartition, 
																		int iReplicationfactor) {
		CUSTOMPARTITIONLOC* cVcustompartitionloc;
		bool bVchangenext;
		int iVlocal;
		int iVreplicationfactor;
		int iVcount;
		CoreList<CUSTOMPARTITIONLOC>* cVpartitionpool;

		cVpartitionpool = &cCustompartition->cVpartitionpool;
		iVreplicationfactor = iReplicationfactor > 0 ? iReplicationfactor : cVendpoint->getrf ();
		bVchangenext = true;
		cCustompartition->sVupperbound = NULL;
		cCustompartition->sVlowerbound = NULL;
		if (sPartition != NULL) {
			if (sPartition.UpperCase ().Trim () != sCignored)  {
				for (int iV = cVpartitionpool->count () - 1; iV >= 0; iV--) 
					if (cVpartitionpool->get__ (iV)->sVnodeid == sNodeid) cVpartitionpool->unset (iV);		
				cVcustompartitionloc = cVpartitionpool->bnset (sPartition);
				cVcustompartitionloc->sVnodeid = sNodeid;
				cVcustompartitionloc->sVtoken = sToken;
				iVcount = cVpartitionpool->count ();
				for (int iV = 0; iV < iVcount; iV++) {	
					cVcustompartitionloc = cVpartitionpool->get__ (iV);
					if (bVchangenext) cCustompartition->sVupperbound = cVcustompartitionloc->sVindex;
					bVchangenext = cVcustompartitionloc->sVnodeid == cVendpoint->getni ();
					if (bVchangenext) iVlocal = iV;
				}
				if (iVreplicationfactor >= iVcount) cCustompartition->sVlowerbound = cCustompartition->sVupperbound;
				else cCustompartition->sVlowerbound  = 
					cVpartitionpool->get__ ((iVcount + iVlocal + 1 - iVreplicationfactor) % iVcount)->sVindex;

			}
		}
	}

	/*SET Partition Data*/
	bool __fastcall CorePart::setpd (	CorePart::PARTITIONLOC* cPartitiondata, AnsiString sNodeid, AnsiString sToken,
																		AnsiString sPartition, AnsiString sPartitionindex) {
		AnsiString sVnodeid;
		AnsiString sVtoken; 

		sVnodeid = sNodeid != NULL ? sNodeid : cVendpoint->getni ();
		sVtoken = sNodeid != NULL ? sToken : cVendpoint->gettk ();
		if (cPartitiondata->cVpartition.iVpartitiontype == iCcustompartition) 
			setpd (&cPartitiondata->cVpartition, sVnodeid, sVtoken, sPartition, cPartitiondata->iVcustomreplicationfactor);
		if (cPartitiondata->cVpartitionindex.iVpartitiontype == iCcustompartition) 
				setpd (&cPartitiondata->cVpartitionindex, sVnodeid, sVtoken, sPartitionindex, cPartitiondata->iVcustomreplicationfactor);
		return true;
	}

	/*GET Partition Type*/
	CorePart::iCpartitiontype __fastcall CorePart::getpt (CorePart::PARTITIONLOC* cPartitiondata, bool bIndex) {
		iCpartitiontype iVreturn;
		iCpartitiontype iVpartitiontype;

		iVpartitiontype = bIndex ? cPartitiondata->cVpartitionindex.iVpartitiontype : cPartitiondata->cVpartition.iVpartitiontype;
		iVreturn = iVpartitiontype;

		
		/*
		if (	iVpartitiontype == CorePart::iCcustompartition && 
					(bIndex ? cPartitiondata->cVpartitionindex.cVpartitionpool.count () : cPartitiondata->cVpartition.cVpartitionpool.count ()) <= 1)
			iVreturn = CorePart::iCnopartition;
		else iVreturn = iVpartitiontype; */
		return iVreturn;
	}

	/*SPLIT*/
	AnsiString CorePart::split (AnsiString sLeft, AnsiString sRight, int iNum, int iDen, AnsiString& sHex) {
		int iVlength = Max (sLeft.Length (), sRight.Length ());
		int iV;
		AnsiString sVreturn;
		CoreBint cVleft;
		CoreBint cVright;
		CoreBint cVresult;

		cVleft.ans2b (CoreButl::rpad_ (sLeft, CorePart::sCchar0, iVlength));
		cVright.ans2b (CoreButl::rpad_ (sRight, CorePart::sCchar0, iVlength));
		cVresult = ((cVleft * (iDen - iNum)) / iDen + (cVright * iNum) / iDen);
		sHex = cVresult.write ();
		sVreturn = cVresult.bin2a ();					
		iV = 0;
		while (sVreturn.c_str () [iV] == 0 && iV < sVreturn.Length ()) iV++;
		return CoreButl::sbstr (sVreturn, iV + 1);
	}

	/*GET HeX*/
	AnsiString CorePart::gethx (AnsiString sBound) {
		CoreBint cVbound;

		cVbound.ans2b (sBound);
		return cVbound.write ();
	}

	/*Partition Pool To Protocol Buffers*/
	void __fastcall CorePart::pptpb ( CoreList<CorePart::CUSTOMPARTITIONLOC>* cPartitionpool, 
																		google::protobuf::RepeatedPtrField<PrtoPtel>* cProtocolbuffer) {
		CUSTOMPARTITIONLOC* cVcustompartitionloc;
		PrtoPtel* cVpartitionelement;

		for (int iV = 0; iV < cPartitionpool->count (); iV++) {
				cVcustompartitionloc = cPartitionpool->get__ (iV);
				cVpartitionelement = cProtocolbuffer->Add ();
				cVpartitionelement->set_svnodeid (cVcustompartitionloc->sVnodeid.to_string ());
				cVpartitionelement->set_svbound (cVcustompartitionloc->sVindex.to_string ());
			}
	}

	/*Protocol Buffers To Partition Pool*/
	void __fastcall CorePart::pbtpp (	google::protobuf::RepeatedPtrField<PrtoPtel>* cProtocolbuffer,
																		CorePtcl* cCustompartition, int iReplicationfactor) {
		PrtoPtel* cVpartitionelement;

		cCustompartition->sVupperbound = NULL;
		cCustompartition->sVlowerbound = NULL;
		for (int iV = 0; iV < cProtocolbuffer->size (); iV++) {
			cVpartitionelement = cProtocolbuffer->Mutable (iV);			
			setpd (cCustompartition, cVpartitionelement->svnodeid (), cVendpoint->rngtk (cVpartitionelement->svnodeid ()), 
				cVpartitionelement->svbound (), iReplicationfactor);
		}
	}

	/*REBuild Partition Pool*/
	AnsiString __fastcall CorePart::rebpp ( CoreList<CorePart::CUSTOMPARTITIONLOC>* cPartitionpool) {
		AnsiString sVreturn;
		CUSTOMPARTITIONLOC* cVcustompartitionloc;
		bool bVchangenext;
		CoreList<AnsiString> cVdeletelist;

		bVchangenext = true;
		sVreturn = NULL;
		for (int iV = 0; iV < cPartitionpool->count (); iV++) {
			cVcustompartitionloc = cPartitionpool->get__ (iV);
			if (cVendpoint->rngtk (cVcustompartitionloc->sVnodeid) == NULL) *cVdeletelist.set__ () = cVcustompartitionloc->sVindex;
		}
		for (int iV = 0; iV < cVdeletelist.count (); iV++) cPartitionpool->bnuns (*cVdeletelist.get__ (iV));
		for (int iV = 0; iV < cPartitionpool->count (); iV++) {
			cVcustompartitionloc = cPartitionpool->get__ (iV);
			if (bVchangenext) sVreturn = cVcustompartitionloc->sVindex;
			bVchangenext = cVcustompartitionloc->sVnodeid == cVendpoint->getni ();
		}
		return sVreturn;
	}

