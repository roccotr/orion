#include "CoreTutl.hpp"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using boost::shared_ptr;
using namespace orion;

  /*CoPy KEY*/
	void CoreTutl::cpkey (const ThrfLkey* cSource, PrtoLkey* cDestination) {

		cDestination->set_svmain (cSource->sVmain);
		cDestination->set_svaccessgroup (cSource->sVaccessgroup);
		cDestination->set_svqualifier (cSource->sVqualifier);
		cDestination->set_ivstate ((iCstatetype) cSource->iVstate);
		cDestination->set_ivtimestamp (cSource->iVtimestamp);
	}

  /*CoPy KEY*/
	void CoreTutl::cpkey (const PrtoLkey* cSource, ThrfLkey* cDestination) {

		cDestination->sVmain = cSource->svmain ();
		cDestination->sVaccessgroup = cSource->svaccessgroup ();
		cDestination->sVqualifier = cSource->svqualifier ();
		cDestination->iVstate = (iEstatetype::type) cSource->ivstate ();
		cDestination->iVtimestamp = cSource->ivtimestamp ();
	}

	/*CoPy MuTaBle*/
	void CoreTutl::cpmut (const ThrfLmtb* cSource, PrtoLmtb* cDestination) {

		cDestination->set_svnamespace (cSource->sVnamespace);
		cDestination->set_svtable (cSource->sVtable);
	}

	/*CoPy MuTaBle*/
	void CoreTutl::cpmut (const PrtoLmtb* cSource, ThrfLmtb* cDestination) {

		cDestination->sVnamespace = cSource->svnamespace ();
		cDestination->sVtable = cSource->svtable ();
	}

	/*CoPy l2CV*/
	void CoreTutl::cp2cv (	iEcolumntype::type iColumntype, const ThrfL2cv* cSource, PrtoL2cv* cDestination) {
		unsigned int iVinner;

		switch (iColumntype) {
			case iEcolumntype::STRINGTYPE: cDestination->set_svvalue (cSource->sVvalue); break;
			case iEcolumntype::INTEGRTYPE: cDestination->set_ivvalue (cSource->iVvalue); break;
			case iEcolumntype::DOUBLETYPE: cDestination->set_dvvalue (cSource->dVvalue); break;
			case iEcolumntype::BOOLN_TYPE: cDestination->set_bvvalue (cSource->bVvalue); break;
			case iEcolumntype::LSTRNGTYPE: 
				for (iVinner = 0; iVinner < cSource->sVlistvalue.size (); iVinner++) 
					cDestination->add_svlistvalue (cSource->sVlistvalue [iVinner]);
				break;
			case iEcolumntype::LINTGRTYPE: 
				for (iVinner = 0; iVinner < cSource->iVlistvalue.size (); iVinner++) 
					cDestination->add_ivlistvalue (cSource->iVlistvalue [iVinner]);
				break;
			case iEcolumntype::LDOUBLTYPE: 
				for (iVinner = 0; iVinner < cSource->dVlistvalue.size (); iVinner++) 
					cDestination->add_dvlistvalue (cSource->dVlistvalue [iVinner]);
				break;
		}	
	}

	/*CoPy l2CV*/
	void CoreTutl::cp2cv (	iCcolumntype iColumntype, const PrtoL2cv* cSource, ThrfL2cv* cDestination) {
		int iVinner;

		switch (iColumntype) {
			case STRINGTYPE: cDestination->sVvalue = cSource->svvalue (); break;
			case INTEGRTYPE: cDestination->iVvalue = cSource->ivvalue (); break;
			case DOUBLETYPE: cDestination->dVvalue = cSource->dvvalue (); break;
			case BOOLN_TYPE: cDestination->bVvalue = cSource->bvvalue (); break;
			case LSTRNGTYPE: 
				for (iVinner = 0; iVinner < cSource->svlistvalue_size (); iVinner++) 
					cDestination->sVlistvalue.push_back (cSource->svlistvalue (iVinner));
				break;
			case LINTGRTYPE: 
				for (iVinner = 0; iVinner < cSource->ivlistvalue_size (); iVinner++) 
					cDestination->iVlistvalue.push_back (cSource->ivlistvalue (iVinner));
				break;
			case LDOUBLTYPE: 
				for (iVinner = 0; iVinner < cSource->dvlistvalue_size (); iVinner++) 
					cDestination->dVlistvalue.push_back (cSource->dvlistvalue (iVinner));
				break;
			}	
	}

	/*CoPy VALue*/
	void CoreTutl::cpval (const ThrfLval* cSource, PrtoLval* cDestination) {

		cDestination->set_svopaquevalue (cSource->sVopaquevalue);
		cDestination->set_ivtimestamp (cSource->iVtimestamp);
	}

	/*CoPy VALue*/
	void CoreTutl::cpval (const PrtoLval* cSource, ThrfLval* cDestination) {

		cDestination->sVopaquevalue = cSource->svopaquevalue ();
		cDestination->iVtimestamp = cSource->ivtimestamp ();
	}

	/*CoPy Query RESults*/
	void CoreTutl::cpres (const ThrfL1ks* cSource, PrtoQres* cDestination) {

		cpval (&cSource->cVvalue, cDestination->mutable_cvvalue ());
		cpkey (&cSource->cVkey, cDestination->mutable_cvkey ());
		cDestination->set_bvresult (cSource->bVresult);
	}

	/*CoPy STateMent*/
	void CoreTutl::cpstm (	const ThrfLstm* cSource, PrtoLstm* cDestination) {

		cDestination->set_ivopcode ((iCopcodetype) cSource->iVopcode);
		cpkey (&cSource->cVkey, cDestination->mutable_cvkey ());
		cpmut (&cSource->cVmutable, cDestination->mutable_cvmutable ());
		cpval (&cSource->cVvalue, cDestination->mutable_cvvalue ());
	}

	/*CoPy STateMent*/
	void CoreTutl::cpstm (	const PrtoLstm* cSource, ThrfLstm* cDestination) {

		cDestination->iVopcode = (iEopcodetype::type) cSource->ivopcode ();
		cpkey (&cSource->cvkey (), &cDestination->cVkey);
		cpmut (&cSource->cvmutable (), &cDestination->cVmutable);
		cpval (&cSource->cvvalue (), &cDestination->cVvalue);
	}

	/*CoPy QueRY*/
	void CoreTutl::cpqry (const ThrfLqry* cSource, PrtoLqry* cDestination) {
		cDestination->set_ivquery ((iCquerytype) cSource->iVquery);
		cDestination->set_bvdigest (cSource->bVdigest);
		cpkey (&cSource->cVkey, cDestination->mutable_cvkey ());
		cpmut (&cSource->cVmutable, cDestination->mutable_cvmutable ());
	}

	/*CoPy QueRY*/
	void CoreTutl::cpqry (const PrtoLqry* cSource, ThrfLqry* cDestination) {
		cDestination->iVquery = (iEquerytype::type) cSource->ivquery ();
		cDestination->bVdigest = cSource->bvdigest ();
		cpkey (&cSource->cvkey (), &cDestination->cVkey);
		cpmut (&cSource->cvmutable (), &cDestination->cVmutable);
	}

	/*CoPy QueRY*/
	void CoreTutl::cpqry (PrtoL2qr* cSource, ThrfL2qr* cDestination) {
		int iV;
		ThrfL2cl cVthriftvoidcolumn;
		ThrfL2cl* cVthriftcolumn;
		PrtoL2cl* cVprotocolumn;
		ThrfL2or cVthriftvoidorderby;
		ThrfL2or* cVthriftorderby;
		PrtoL2or* cVprotoorderby;

		cpmut (cSource->mutable_cvmutable (), &cDestination->cVmutable);
		cpkey (cSource->mutable_cvkey_start (), &cDestination->cVkey_start);
		cpkey (cSource->mutable_cvkey_end (), &cDestination->cVkey_end);
		cDestination->iVquery = (iEquerytype::type) cSource->ivquery ();
		cDestination->iVcount = cSource->ivcount ();
		cDestination->bVonlysecondary = cSource->bvonlysecondary ();
		cDestination->bVdisableteleport = cSource->bvdisableteleport ();
		for (iV = 0; iV < cSource->cvselect_size (); iV++) {
			cDestination->cVselect.push_back (cVthriftvoidcolumn);
			cVthriftcolumn = &cDestination->cVselect [iV];
			cVthriftcolumn->sVcolumn = cSource->cvselect (iV).svcolumn ();
		}
		for (iV = 0; iV < cSource->mutable_cvwhere ()->cvcondition_size (); iV++) {
			cDestination->cVwhere.cVcondition.push_back (cVthriftvoidcolumn);
			cVthriftcolumn = &cDestination->cVwhere.cVcondition [iV];
			cVprotocolumn = cSource->mutable_cvwhere ()->mutable_cvcondition (iV);
			cVthriftcolumn->sVcolumn = cVprotocolumn->svcolumn ();
			cVthriftcolumn->iVconditiontype = (iEconditiontype::type) cVprotocolumn->ivconditiontype ();
			cVthriftcolumn->cVvalue.iVtype = (iEcolumntype::type) cVprotocolumn->mutable_cvvalue ()->ivtype ();
			cp2cv (cVprotocolumn->cvvalue ().ivtype (), cVprotocolumn->mutable_cvvalue (), &cVthriftcolumn->cVvalue);
		}
		for (iV = 0; iV < cSource->mutable_cvwhere ()->cvorderby_size (); iV++) {
			cDestination->cVwhere.cVorderby.push_back (cVthriftvoidorderby);
			cVthriftorderby = &cDestination->cVwhere.cVorderby [iV];
			cVprotoorderby = cSource->mutable_cvwhere ()->mutable_cvorderby (iV);
			cVthriftorderby->sVcolumn = cVprotoorderby->svcolumn ();
			cVthriftorderby->iVordertype = (iEordertype::type) cVprotoorderby->ivordertype ();
		}
	}

	/*CoPy GOssip Element*/
	void CoreTutl::cpgoe (const ThrfGoel* cSource, PrtoGoel* cDestination) {

		cDestination->set_svtoken (cSource->sVtoken);
		cDestination->set_svnodeid (cSource->sVnodeid);
		cDestination->set_svdatacenterid (cSource->sVdatacenterid);
		cDestination->set_dvphiaccrual (cSource->dVphiaccrual);
		cDestination->set_svaddress (cSource->sVaddress);
		cDestination->set_ivport (cSource->iVport);
		cDestination->set_ivstate ((iCstategossipnode) cSource->iVstate);
		cDestination->set_ivtimestamp (cSource->iVtimestamp);
	}

	/*CoPy GOssip Element*/
	void CoreTutl::cpgoe (const PrtoGoel* cSource, ThrfGoel* cDestination) {

		cDestination->sVtoken = cSource->svtoken ();
		cDestination->sVnodeid = cSource->svnodeid ();
		cDestination->sVdatacenterid = cSource->svdatacenterid ();
		cDestination->dVphiaccrual = cSource->dvphiaccrual ();
		cDestination->sVaddress = cSource->svaddress ();
		cDestination->iVport = cSource->ivport ();
		cDestination->iVstate = (iEstategossipnode::type) cSource->ivstate ();
		cDestination->iVtimestamp = cSource->ivtimestamp ();
		cDestination->iVsize = cSource->ivsize ();
	}

	/*CoPy GOSsip*/
	void CoreTutl::cpgos (PrtoGoss* cSource, ThrfGoss* cDestination) {
		ThrfGoel cVthriftgossipvoidelement;
		ThrfGoel* cVthriftgossipelement;

		for (int iV = 0; iV < cSource->cvgossipelement_size (); iV++) {
			cDestination->cVgossipelement.push_back (cVthriftgossipvoidelement);			
			cVthriftgossipelement = &cDestination->cVgossipelement [iV];
			cpgoe (cSource->mutable_cvgossipelement (iV), cVthriftgossipelement);
		}
	}

	/*CoPy TaBlet Element*/
	void CoreTutl::cptbe (const PrtoTlel* cSource, ThrfTlel* cDestination) {

		cDestination->iVtablettype = (iEtablettype::type) cSource->ivtablettype ();
		cDestination->sVname = cSource->svname ();
		cDestination->bVvalid = cSource->bvvalid ();
		cDestination->bVneedcompaction = cSource->bvneedcompaction ();
		cDestination->bVvalidasmemorytable = cSource->bvvalidasmemorytable ();
		cDestination->iVcompactionlevel = cSource->ivcompactionlevel ();
		cDestination->sVmemorytable = cSource->svmemorytable ();
		cDestination->iVres = cSource->ivres ();
		cDestination->iVmapped = cSource->ivmapped ();
		cDestination->iVsize = cSource->ivsize ();
		cDestination->iVcount = cSource->ivcount ();
		cDestination->sVredotimestamp = cSource->svredotimestamp ();
		cDestination->iVsstabletype = cSource->ivsstabletype ();
	}

	/*CoPy TaBLet*/
	void CoreTutl::cptbl (PrtoTabl* cSource, ThrfTabl* cDestination) {
		ThrfPtel cVthriftpartitinvoidelement;
		ThrfPtel* cVthriftpartitionelement;
		ThrfTlel cVthrifttabletvoidelement;
		ThrfTlel* cVthrifttabletelement; 

		cDestination->sVxml = cSource->svxml ();
		cDestination->sVjoincustompartition = cSource->svjoincustompartition ();
		cDestination->sVjoincustompartitionindex = cSource->svjoincustompartitionindex ();
		cDestination->sVpartitiontype = cSource->svpartitiontype ();
		for (int iV = 0; iV < cSource->cvpartitionedelements_size (); iV++) {
			cDestination->cVpartitionedelements.push_back (cVthriftpartitinvoidelement);
			cVthriftpartitionelement = &cDestination->cVpartitionedelements [iV];
			cVthriftpartitionelement->sVnodeid = cSource->cvpartitionedelements (iV).svnodeid ();
			cVthriftpartitionelement->sVbound = cSource->cvpartitionedelements (iV).svbound ();
		}
		for (int iV = 0; iV < cSource->cvpartitionedindexelements_size (); iV++) {
			cDestination->cVpartitionedindexelements.push_back (cVthriftpartitinvoidelement);
			cVthriftpartitionelement = &cDestination->cVpartitionedindexelements [iV];
			cVthriftpartitionelement->sVnodeid = cSource->cvpartitionedindexelements (iV).svnodeid ();
			cVthriftpartitionelement->sVbound = cSource->cvpartitionedindexelements (iV).svbound ();
		}
		for (int iV = 0; iV < cSource->cvtabletelement_size (); iV++) {
			cDestination->cVtabletelement.push_back (cVthrifttabletvoidelement);
			cVthrifttabletelement = &cDestination->cVtabletelement [iV];
			cptbe (cSource->mutable_cvtabletelement (iV), cVthrifttabletelement);
		}
		for (int iV = 0; iV < cSource->cvindextabletelement_size (); iV++) {
			cDestination->cVindextabletelement.push_back (cVthrifttabletvoidelement);
			cVthrifttabletelement = &cDestination->cVindextabletelement [iV];
			cptbe (cSource->mutable_cvindextabletelement (iV), cVthrifttabletelement);
		}
	}

	/*CoPy STAts*/
	void CoreTutl::cpsta (const PrtoStel* cSource, ThrfStel* cDestination) {

		cDestination->iVcount = cSource->ivcount ();
		cDestination->iVmeandelay = cSource->ivmeandelay ();
		cDestination->iVworstdelay = cSource->ivworstdelay ();
		cDestination->iVsize = cSource->ivsize ();
		cDestination->iVelapsed = cSource->ivelapsed ();
	}

	/*CoPy AntiENtrophy*/
	void CoreTutl::cpaen (const ThrfAenp* cSource, PrtoAenp* cDestination) {

		cDestination->set_svopaquedata (cSource->sVopaquedata);
		cDestination->set_svnodeid (cSource->sVnodeid);
		cpmut (&cSource->cVmutable, cDestination->mutable_cvmutable ());
		cDestination->set_svkeystart (cSource->sVkeystart);
		cDestination->set_ivlimitsize (cSource->iVlimitsize);
	}

	/*CoPy AntiENtrophy*/
	void CoreTutl::cpaen (const PrtoAenp* cSource, ThrfAenp* cDestination) {

		cDestination->sVopaquedata = cSource->svopaquedata ();
	}

	/*CoPy TOP*/
	void CoreTutl::cptop (const PrtoTop_* cSource, ThrfTop_* cDestination) {
		ThrfTope cVthriftvoidelement;
		ThrfTope* cVthriftelement;

		cDestination->iVtimestamp = cSource->ivtimestamp ();
		for (int iV = 0; iV < cSource->cvtopelement_size (); iV++) {
			cDestination->cVtopelement.push_back (cVthriftvoidelement);
			cVthriftelement = &cDestination->cVtopelement[iV];
			cVthriftelement->sVtablet = cSource->cvtopelement (iV).svtablet ();
			cVthriftelement->iVreadtime = cSource->cvtopelement (iV).ivreadtime ();
			cVthriftelement->iVwritetime = cSource->cvtopelement (iV).ivwritetime ();
			cVthriftelement->iVreadcountl2 = cSource->cvtopelement (iV).ivreadcountl2 ();
			cVthriftelement->iVwritecountl2 = cSource->cvtopelement (iV).ivwritecountl2 ();
			cVthriftelement->iVreadcountl1 = cSource->cvtopelement (iV).ivreadcountl1 ();
			cVthriftelement->iVwritecountl1 = cSource->cvtopelement (iV).ivwritecountl1 ();
		}
	}

	/*WRiTe ReSult*/
	void CoreTutl::wrtrs (PrtoL2mr* cDmlresult, std::vector<ThrfL2ks>& _return) {
		PrtoL2ks* cVprotokeyslice;
		ThrfL2ks* cVthriftkeyslice;
		ThrfL2cl* cVthriftcolumn;
		PrtoL2cl* cVprotocolumn;
		ThrfL2ks cVthriftvoidkeyslice;		
		ThrfL2cl cVthriftvoidcolumn;

		for (int iV = 0; iV < cDmlresult->cvkeyslice_size (); iV++) {
			cVprotokeyslice = cDmlresult->mutable_cvkeyslice (iV);
			_return.push_back (cVthriftvoidkeyslice);
			cVthriftkeyslice = &_return [iV];		

			CoreTutl::cpkey (cVprotokeyslice->mutable_cvkey (), &cVthriftkeyslice->cVkey);
			for (int iVinner = 0; iVinner < cVprotokeyslice->cvcolumns_size (); iVinner++) {
				cVprotocolumn = cVprotokeyslice->mutable_cvcolumns (iVinner);
				cVthriftkeyslice->cVcolumns.push_back (cVthriftvoidcolumn);
				cVthriftcolumn = &cVthriftkeyslice->cVcolumns [iVinner];
				cVthriftcolumn->sVcolumn = cVprotocolumn->svcolumn ();
				cVthriftcolumn->iVtype = cVthriftcolumn->cVvalue.iVtype = (iEcolumntype::type) cVprotocolumn->ivtype ();
				cp2cv (cVprotocolumn->ivtype (), cVprotocolumn->mutable_cvvalue (), &cVthriftcolumn->cVvalue);
			}
		}
	}

	/*WRiTe ReSult*/
	void CoreTutl::wrtrs (const std::vector<ThrfL2ks>* cSource, PrtoL2mr* cDestination) {
		PrtoL2ks* cVprotokeyslice;
		PrtoL2cl* cVprotocolumn;
		const ThrfL2ks* cVthriftkeyslice;
		const ThrfL2cl* cVthriftcolumn;
		
		for (unsigned int iV = 0; iV < cSource->size (); iV++) {
			cVprotokeyslice = cDestination->add_cvkeyslice ();
			cVthriftkeyslice = &((*cSource) [iV]);
			cpkey (&cVthriftkeyslice->cVkey, cVprotokeyslice->mutable_cvkey ());
			for (unsigned int iVinner = 0; iVinner < cVthriftkeyslice->cVcolumns.size (); iVinner++) {
				cVprotocolumn = cVprotokeyslice->add_cvcolumns ();
				cVthriftcolumn = &cVthriftkeyslice->cVcolumns [iVinner];
				cVprotocolumn->set_svcolumn (cVthriftcolumn->sVcolumn);
				cVprotocolumn->set_ivtype ((iCcolumntype) cVthriftcolumn->iVtype);
				cp2cv (cVthriftcolumn->iVtype, &cVthriftcolumn->cVvalue, cVprotocolumn->mutable_cvvalue ());
			}
		}
	}

	/*SEND Local queries*/
	bool CoreTutl::sends (PTR_CONNECTIONFACTORY cConnectionfactory, std::vector<ThrfLstm>* cStatements) {
		PTR_CONNECTION cVconnection;
		bool bVreturn;

		cVconnection = cConnectionfactory->begin ();
		try {
			bVreturn = cVconnection->get__ ()->localstatement (*cStatements);			
			cConnectionfactory->end__ (cVconnection);
			return bVreturn;
		} catch (...) {
			cVconnection->close ();
			cConnectionfactory->end__ (cVconnection);
			return false;
		}
	}

	/*SEND Local queries*/
	bool CoreTutl::sendl (PTR_CONNECTIONFACTORY cConnectionfactory, std::vector<ThrfLqry>* cQueries, std::vector<ThrfL1ks>& cResults) {
		PTR_CONNECTION cVconnection;

		cVconnection = cConnectionfactory->begin ();
		try {
			cVconnection->get__ ()->localquery (cResults, *cQueries);			
			cConnectionfactory->end__ (cVconnection);
			return true;
		} catch (...) {
			cVconnection->close ();
			cConnectionfactory->end__ (cVconnection);
			return false;
		}
	}

	/*SEND Queries*/
	bool CoreTutl::sendq (PTR_CONNECTIONFACTORY cConnectionfactory, PrtoL2qr* cQuery, 
												bool bRecursiveteleportdisabled, PrtoL2mr* cResult) {
		PTR_CONNECTION cVconnection;
		ThrfL2qr cVthriftquery;
		ThrfL2qb cVthriftresult;
		
		cpqry ((PrtoL2qr*) cQuery, &cVthriftquery);
		if (bRecursiveteleportdisabled) cVthriftquery.bVdisableteleport = true;
		cVconnection = cConnectionfactory->begin ();
		try {
			cVconnection->get__ ()->query (cVthriftresult, cVthriftquery);		
			cConnectionfactory->end__ (cVconnection);
			if (cVthriftresult.bVreturn) wrtrs (&cVthriftresult.cKeyslices, cResult);
			return cVthriftresult.bVreturn;
		} catch (...) {
			cVconnection->close ();
			cConnectionfactory->end__ (cVconnection);
			return false;
		}
	}

	/*SEND Command*/
	bool CoreTutl::sendc (PTR_CONNECTIONFACTORY cConnectionfactory, const ThrfComm& cCommand, ThrfCort& cResult) {
		PTR_CONNECTION cVconnection;

		cVconnection = cConnectionfactory->begin ();
		try {
			cVconnection->get__ ()->command (cResult, cCommand);		
			cConnectionfactory->end__ (cVconnection);
			return true;
		} catch (...) {
			cVconnection->close ();
			cConnectionfactory->end__ (cVconnection);
			return false;
		}
	}

	/*SEND Command*/
	bool CoreTutl::sendc (std::string sAddress, int iPort, const ThrfComm& cCommand, ThrfCort& cResult) {
		
		PTR_CONNECTIONFACTORY cVconnectionfactory = boost::shared_ptr<CoreCofa> (new CoreCofa ());
		cVconnectionfactory->init_ (sAddress, iPort);
		return CoreTutl::sendc (cVconnectionfactory, cCommand, cResult);
	}

	/*STATeMent*/
	void CoreTutl::statm (const ThrfL2st& cSource, PrtoL2st& cDestination) {
		PrtoL2cl* cVprotocolumn;
		std::vector<ThrfL2cl> cVthriftcolumns;

		CoreTutl::cpmut (&cSource.cVmutable, cDestination.mutable_cvmutable ());
		CoreTutl::cpkey (&cSource.cVkey, cDestination.mutable_cvkey ());
		cVthriftcolumns = cSource.cVcolumns;
		for (unsigned int iV = 0; iV < cVthriftcolumns.size (); iV++) {
			cVprotocolumn = cDestination.mutable_cvcolumns ()->Add ();
			cVprotocolumn->set_svcolumn (cVthriftcolumns [iV].sVcolumn);
			cVprotocolumn->mutable_cvvalue ()->set_ivtype ((iCcolumntype) cVthriftcolumns [iV].cVvalue.iVtype);
			cVprotocolumn->set_ivtype (cVprotocolumn->mutable_cvvalue ()->ivtype ());
			CoreTutl::cp2cv (cVthriftcolumns [iV].cVvalue.iVtype, &cVthriftcolumns [iV].cVvalue, cVprotocolumn->mutable_cvvalue ());
		}
		cDestination.set_ivopcode ((iCopcodetype) cSource.iVopcode);
	}

	/*QUERY*/
	void CoreTutl::query (const ThrfL2qr& cSource, PrtoL2qr& cDestination) {
		unsigned int iV;
		PrtoL2cl* cVprotocolumn;
		const ThrfL2cl* cVthriftconstcolumn;
		const ThrfL2or* cVthriftconstorderby;
		PrtoL2or* cVprotoorderby;

		CoreTutl::cpmut (&cSource.cVmutable, cDestination.mutable_cvmutable ());
		cDestination.mutable_cvkey_start ()->set_svmain (cSource.cVkey_start.sVmain);
		cDestination.mutable_cvkey_end ()->set_svmain (cSource.cVkey_end.sVmain);
		cDestination.set_ivquery ((iCquerytype) cSource.iVquery);
		cDestination.set_ivcount (cSource.iVcount);
		cDestination.set_bvonlysecondary (cSource.bVonlysecondary);
		cDestination.set_bvdisableteleport (cSource.bVdisableteleport);
		for (iV = 0; iV < cSource.cVselect.size (); iV++) {
			cVprotocolumn = cDestination.mutable_cvselect ()->Add ();
			cVprotocolumn->set_svcolumn (cSource.cVselect [iV].sVcolumn);
		}
		for (iV = 0; iV < cSource.cVwhere.cVcondition.size (); iV++) {
			cVthriftconstcolumn = &cSource.cVwhere.cVcondition [iV];
			cVprotocolumn = cDestination.mutable_cvwhere ()->mutable_cvcondition ()->Add ();
			cVprotocolumn->set_svcolumn (cVthriftconstcolumn->sVcolumn);
			cVprotocolumn->set_ivconditiontype ((iCconditiontype) cVthriftconstcolumn->iVconditiontype);
			CoreTutl::cp2cv (	cVthriftconstcolumn->cVvalue.iVtype, 
							&cVthriftconstcolumn->cVvalue, cVprotocolumn->mutable_cvvalue ());
		}
		for (iV = 0; iV < cSource.cVwhere.cVorderby.size (); iV++) {
			cVthriftconstorderby = &cSource.cVwhere.cVorderby [iV];
			cVprotoorderby = cDestination.mutable_cvwhere ()->mutable_cvorderby ()->Add ();
			cVprotoorderby->set_svcolumn (cVthriftconstorderby->sVcolumn);
			cVprotoorderby->set_ivordertype ((iCordertype) cVthriftconstorderby->iVordertype);
		}
	}

	/*OSQL*/
	void CoreTutl::osql_ (const ThrfL2os& cSource, PrtoL2os& cDestination) {
		PrtoL2cv* cVprotocolumnvalue;
		const ThrfL2cv* cVthriftconstcolumnvalue;

		cDestination.set_svnamespace (cSource.sVnamespace);
		cDestination.set_svosqlstring (cSource.sVosqlstring);
		cDestination.set_ivtimestamp (cSource.iVtimestamp);
		cDestination.set_bvfulltablescan (cSource.bVfulltablescan);
		for (unsigned int iV = 0; iV < cSource.cVinputbindings.size (); iV++) {
			cVthriftconstcolumnvalue = &cSource.cVinputbindings [iV];
			cVprotocolumnvalue = cDestination.mutable_cvinputbindings ()->Add ();
			cVprotocolumnvalue->set_ivtype ((iCcolumntype) cVthriftconstcolumnvalue->iVtype);					
			CoreTutl::cp2cv (	cVthriftconstcolumnvalue->iVtype, cVthriftconstcolumnvalue, cVprotocolumnvalue);
		}
		cDestination.set_bvonlysecondary (cSource.bVonlysecondary);
		cDestination.set_bvdisableteleport (cSource.bVdisableteleport);
	}

	/*DEBUG*/
	AnsiString CoreTutl::debug (const PrtoLmtb& cMutable) {

		return "MUTABLE: " + cMutable.svnamespace () + "_" + cMutable.svtable () + "\n";
	}

	/*DEBUG*/
	AnsiString CoreTutl::debug (const PrtoLkey& cKey) {
		AnsiString sVreturn;

		return "KEY: " + cKey.svmain () + "\n";
	}

	AnsiString CoreTutl::debug (const PrtoL2qr& cDestination) {
		
		return debug (cDestination.cvmutable ()) + debug (cDestination.cvkey_start ());
	}