#include "IfceArs_.hpp"

using namespace boost;
using namespace orion; 

	CoreSmph cVariesglobalmutex;
	int iVarieslibraryrefs = 0;

	IfceArs_::IfceArs_ (AnsiString sAddress, int iPort, int iWorkers, int iBulksize, bool& bValid) {
		
		bValid = false;
		FLAGS_log_dir = "c:/templog/ordbc";
		cVariesglobalmutex.locks ();
		if (iVarieslibraryrefs == 0) google::InitGoogleLogging("Ordbc");
		iVarieslibraryrefs++;
		cVariesglobalmutex.relse ();
		cVpool = PTR_POOL (new CorePool ());
		cVorionconnectionfactory = PTR_ORIONCONNECTIONFACTORY  (new CoreCocf ());
		cVorionconnectionfactory->init_ (cVpool, sAddress, iPort, CoreCocf::bCprotobuf);
		cVariesconnectionfactory = PTR_ARIESCONNECTION (new CoreCaco (cVorionconnectionfactory, iWorkers, iBulksize));
		cVariesconnectionfactory->addrf ();
		if (cVariesconnectionfactory->valid ()) {
			cVariespipeline = PTR_ARIESPIPELINE (new CoreCacp (cVariesconnectionfactory));
			bValid = true;
		}
	}

	IfceArs_::~IfceArs_ () {

		cVariespipeline.reset ();
		cVariesconnectionfactory->subrf ();
		cVariesconnectionfactory.reset ();
		cVorionconnectionfactory.reset ();
		cVpool.reset ();
	}

	/*RUN*/
	bool IfceArs_::run__ (ThrfSrvc cVservice, ThrfSrvc& cVserviceresult, bool bOptimistic) {
		PrtoSrvc cVprotoservice;
		PrtoSrvr cVprotoserviceresult;

		cVprotoservice.set_ivservicetype ((iCservicetype) cVservice.iVservicetype);
		switch (cVservice.iVservicetype) {
			case iEservicetype::STATEMENT:
				CoreTutl::statm (cVservice.cVstatement, *cVprotoservice.mutable_cvstatement ());
				break;
			case iEservicetype::QUERY:
				CoreTutl::query (cVservice.cVquery, *cVprotoservice.mutable_cvquery ());
				break;
			case iEservicetype::OSQL:
				CoreTutl::osql_ (cVservice.cVosql, *cVprotoservice.mutable_cvosql ());
				break;
		}

		return cVariespipeline->run__ (cVprotoserviceresult, cVprotoservice, bOptimistic);
	}
	


		/*
		jbyteArray cVresult;
		char* cVdirect;
		jboolean bVcopy;
		int iVlength;
		ThrfSrvc cVservice;
		PrtoSrvc cVprotoservice;
		PrtoSrvr cVprotoserviceresult;

		iVlength = cEnv->GetArrayLength (cArray);
		cVdirect = (char*) cEnv->GetPrimitiveArrayCritical (cArray, &bVcopy);
		boost::shared_ptr<TMemoryBuffer> cVtransport (new TMemoryBuffer ((uint8_t*) cVdirect, iVlength));
		boost::shared_ptr<TProtocol> cVprotocol (new TBinaryProtocol (cVtransport));
		cVservice.read (cVprotocol.get ());
		cEnv->ReleasePrimitiveArrayCritical (cArray, cVdirect, NULL);
		cVprotoservice.set_ivservicetype ((iCservicetype) cVservice.iVservicetype);

		switch (cVservice.iVservicetype) {
			case iEservicetype::STATEMENT:
				CoreTutl::statm (cVservice.cVstatement, *cVprotoservice.mutable_cvstatement ());
				break;
			case iEservicetype::QUERY:
				CoreTutl::query (cVservice.cVquery, *cVprotoservice.mutable_cvquery ());
				break;
			case iEservicetype::OSQL:
				CoreTutl::osql_ (cVservice.cVosql, *cVprotoservice.mutable_cvosql ());
				break;
		}

		if (cVariespipeline->run__ (cVprotoserviceresult, cVprotoservice, bOptimistic == JNI_TRUE) && bOptimistic == JNI_FALSE) {

			ThrfSrvr cVserviceresult;
			uint8_t* bVserviceresult;
			uint32_t iVlengthresult;

			switch (cVservice.iVservicetype) {
				case iEservicetype::STATEMENT:
					cVserviceresult.bVreturn = cVprotoserviceresult.bvreturn ();
					break;
				case iEservicetype::QUERY:
					cVserviceresult.cVdmlresult.clear ();
					cVserviceresult.bVreturn = cVprotoserviceresult.bvreturn ();
					if (cVprotoserviceresult.bvreturn ()) 
						CoreTutl::wrtrs (cVprotoserviceresult.mutable_cvdmlresult (), cVserviceresult.cVdmlresult);
					break;
				case iEservicetype::OSQL:
					cVserviceresult.bVreturn = cVprotoserviceresult.bvreturn ();
					cVserviceresult.cVdmlresult.clear ();
					if (cVprotoserviceresult.bvreturn ()) 
						CoreTutl::wrtrs (cVprotoserviceresult.mutable_cvdmlresult (), cVserviceresult.cVdmlresult);
					break;
			}
			boost::shared_ptr<TMemoryBuffer> cVtransport (new TMemoryBuffer ());
			boost::shared_ptr<TProtocol> cVprotocol (new TBinaryProtocol (cVtransport));
			cVserviceresult.write (cVprotocol.get ());
			cVtransport->getBuffer (&bVserviceresult, &iVlengthresult);
			cVresult = cEnv->NewByteArray (iVlengthresult);	
			cVdirect = (char*) cEnv->GetPrimitiveArrayCritical (cVresult, &bVcopy);
			memcpy (cVdirect, bVserviceresult, iVlengthresult);
			cEnv->ReleasePrimitiveArrayCritical (cVresult, cVdirect, NULL);
		} else cVresult = cEnv->NewByteArray (0);

		return cVresult;
		*/


				/*
			ThrfSrvr cVserviceresult;
			uint8_t* bVserviceresult;
			uint32_t iVlengthresult;

			boost::shared_ptr<TMemoryBuffer> cVtransport (new TMemoryBuffer ());
			boost::shared_ptr<TProtocol> cVprotocol (new TBinaryProtocol (cVtransport));
			cVserviceresult.write (cVprotocol.get ());
			cVtransport->getBuffer (&bVserviceresult, &iVlengthresult);
			cVresult = cEnv->NewByteArray (iVlengthresult);	
			cVdirect = (char*) cEnv->GetPrimitiveArrayCritical (cVresult, &bVcopy);
			memcpy (cVdirect, bVserviceresult, iVlengthresult);
			cEnv->ReleasePrimitiveArrayCritical (cVresult, cVdirect, NULL);*/

		/*
		LOG(WARNING) << "ANCORA " << iVreferences << " " << cVariespipeline;
		FlushLogFiles(GLOG_WARNING);


		return cEnv->NewByteArray (0);
		*/