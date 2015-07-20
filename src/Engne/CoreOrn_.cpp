#include <glog/logging.h> 
#include "CoreOrn_.hpp"
#include "CoreButl.hpp"
#include "CoreXml_.hpp"
#include "DataXml_.hpp"

using namespace google;
using namespace orion;

	const AnsiString CoreOrn_::sCnodeid = "DEFAULTNODE";
	const AnsiString CoreOrn_::sClocaladdress = "127.0.0.1";
	const AnsiString CoreOrn_::sCdefault = "DEFAULT";
	const int CoreOrn_::iCthriftport = 9000;
	const int CoreOrn_::iCredologdim = 50000;
	const short int CoreOrn_::iCthriftlisteners = 15;
	const short int CoreOrn_::iCreplicationfactor = 2;
	const short int CoreOrn_::iCmaxcompactionlevel = 1024;

  __fastcall CoreOrn_::CoreOrn_(	PTR_POOL cPool, AnsiString sPath, AnsiString sFilename, 
																	CoreEnpo::iCstategossipnode iState, bool bResetlocal) {

		cVnodedata.sVnodeid = sCnodeid;
		cVnodedata.sVordbcaddress = CoreSckt::sCpipeaddess;
		cVnodedata.sVthriftaddress = sClocaladdress;
		cVnodedata.iVthriftport = iCthriftport;
		cVnodedata.iVordbcport = cVnodedata.iVthriftport;
		cVnodedata.iVthriftlisteners = iCthriftlisteners;
		cVnodedata.sVdatacenterid = "DEFAULTDC";
		cVnodedata.iVredologdim = iCredologdim;
		cVnodedata.iVreplicationfactor = iCreplicationfactor;
		cVnodedata.iVwriteconsistencylevel = CoreEnpo::iCone;
		cVnodedata.iVreadconsistencylevel = CoreEnpo::iCone;
		cVnodedata.iVdatacenterlevel = CoreEnpo::iCsimplestrategy;
		cVnodedata.iVconcurrentreads = 12;
		cVnodedata.iVconcurrentwrites = 12;
		cVnodedata.iVmaxcompactionlevel = CoreOrn_::iCmaxcompactionlevel;
		cVnodedata.iVgracetime = 3600;
		cVpool = cPool;
		cVnodedata.sVpath = sPath;
		cVnodedata.sVfilename = sFilename;
		cVnodedata.sVdatapath = cVnodedata.sVtabletpath = cVnodedata.sVredologpath = cVnodedata.sVpath;
		PTR_XML_ cVdocument = PTR_XML_ (new CoreXml_ ());
		CoreNode* cVnode = cVdocument->getfc (CoreButl::getff (cVpool, cVnodedata.sVpath + cVnodedata.sVfilename))->getcd(DataXml_::sCtagorion_);	
		if (cVnode != NULL){
			cVnodedata.sVnodeid = cVnode->getls (DataXml_::sCtagnodeid);
			cVnodedata.sVdatacenterid = cVnode->getls (DataXml_::sCtagdtacid, true, cVnodedata.sVdatacenterid);			
			if (cVnode->exslc (DataXml_::sCtagpath__)) {
				cVnodedata.sVpath = cVnode->getls (DataXml_::sCtagpath__);
				cVnodedata.sVdatapath = cVnodedata.sVtabletpath = cVnodedata.sVredologpath = cVnodedata.sVpath;
			}
			cVnodedata.iVreplicationfactor = cVnode->getli (DataXml_::sCtagrepfac, true, cVnodedata.iVreplicationfactor);
			cVnodedata.iVwriteconsistencylevel = cVnode->getli (DataXml_::sCtagwcolev, true, cVnodedata.iVwriteconsistencylevel);
			cVnodedata.iVreadconsistencylevel = cVnode->getli (DataXml_::sCtagrcolev, true, cVnodedata.iVreadconsistencylevel);
			cVnodedata.iVdatacenterlevel = cVnode->getli (DataXml_::sCtagdtalev, true, cVnodedata.iVdatacenterlevel);
			cVnodedata.sVdatapath += cVnode->getls (DataXml_::sCtagdtaspt);
			cVnodedata.sVtabletpath += cVnode->getls (DataXml_::sCtagtblspt);
			cVnodedata.sVredologpath += cVnode->getls (DataXml_::sCtagrlgspt);
			cVnodedata.sVthriftaddress = cVnode->getls (DataXml_::sCtagthrfad, true, cVnodedata.sVthriftaddress);
			cVnodedata.sVthriftgossiperaddress = cVnode->getls (DataXml_::sCtagthrgad, true, cVnodedata.sVthriftgossiperaddress);
			cVnodedata.sVordbcaddress = cVnode->getls (DataXml_::sCtagordcad, true, cVnodedata.sVordbcaddress);
			cVnodedata.iVthriftport = cVnode->getli (DataXml_::sCtagthrfpt, true, cVnodedata.iVthriftport);
			cVnodedata.iVthriftthreadedport = cVnode->getli (DataXml_::sCtagthrtpt, true, cVnodedata.iVthriftport + 1);			
			cVnodedata.iVthriftgossiperport = cVnode->getli (DataXml_::sCtagthrgpt, true, cVnodedata.iVthriftgossiperport);
			cVnodedata.iVthriftlisteners = cVnode->getli (DataXml_::sCtagthrfls, true, cVnodedata.iVthriftlisteners);
			cVnodedata.iVordbcport = cVnode->getli (DataXml_::sCtagordcpt, true, cVnodedata.iVordbcport);
			cVnodedata.iVredologdim = cVnode->getli (DataXml_::sCtagrldim_, true, cVnodedata.iVredologdim);
			cVnodedata.iVmaxcompactionlevel = cVnode->getli (DataXml_::sCtagmaxcol, true, cVnodedata.iVmaxcompactionlevel);
			cVnodedata.iVconcurrentwrites = cVnode->getli (DataXml_::sCtagconwrt, true, cVnodedata.iVconcurrentwrites);
			cVnodedata.iVconcurrentreads = cVnode->getli (DataXml_::sCtagconrea, true, cVnodedata.iVconcurrentreads);		
			cVnodedata.iVgracetime = cVnode->getli (DataXml_::sCtaggracet, true, cVnodedata.iVgracetime);	
		}


#ifndef iCdisableprintlog
		printf ("Orion: %s\n", ((AnsiString) iCversion + 
														(AnsiString) " " + CoreButl::getsvr () + 
														(AnsiString) " " + (AnsiString) iCmemtableengine).c_str ());
		printf ("Node: %s\n", cVnodedata.sVnodeid.c_str ());
		printf ("Path: %s\n", cVnodedata.sVpath.c_str ());
		printf ("RF:   %i\n", cVnodedata.iVreplicationfactor);
#endif

		
		LOG(WARNING) << "Orion:     " << iCversion << " " << CoreButl::getsvr ().c_str ();
		LOG(WARNING) << "Node:      " << cVnodedata.sVnodeid.c_str ();
		LOG(WARNING) << "Path:      " << cVnodedata.sVpath.c_str ();
		FlushLogFiles(GLOG_WARNING);

		cVplanner = boost::shared_ptr<CorePlan> (new CorePlan (cVpool, cVnodedata));
		cVplanner->init_ (cVplanner, iState, bResetlocal);
	}

  __fastcall CoreOrn_::~CoreOrn_ () {
		cVplanner->end__ ();
		cVplanner.reset ();
	}

  /*GET PLanner*/
	PTR_PLANNER __fastcall CoreOrn_::getpl () {

		return cVplanner;
	}

	/*GET NodeData*/
	void __fastcall  CoreOrn_::getnd (CoreEnpo::NODEDATA& cNodedata) {

		cNodedata = cVnodedata;
	}

