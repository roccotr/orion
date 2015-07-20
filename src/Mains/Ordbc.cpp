#include "CoreTutl.hpp"
#include "CoreCacp.hpp"
#include <cstdlib>
#include <time.h>
#include <glog/logging.h>
#include <gflags/gflags.h>
#include "com_tomting_orion_ordbc_Ordbc.h"

using namespace google;

static PTR_POOL cVpool;
static PTR_ORIONCONNECTIONFACTORY cVorionconnectionfactory;
static PTR_ARIESCONNECTION cVariesconnectionfactory;
static PTR_ARIESPIPELINE cVariespipeline;
static CoreSmph cVbuildmutex;
static int iVreferences = 0;


JNIEXPORT jboolean JNICALL Java_com_tomting_orion_ordbc_Ordbc_initializeOrdbc (	JNIEnv* cEnv, jobject cObject, jstring sAddress, 
																																								jint iPort, jint iWorkers, jint iBulksize) {
	
	bool bVreturn;
	AnsiString sVaddress;
	const char* bVaddress;
		
	bVaddress = cEnv->GetStringUTFChars (sAddress, NULL);
	sVaddress = bVaddress;
	cEnv->ReleaseStringUTFChars (sAddress, bVaddress);
	cVbuildmutex.locks ();
	if (iVreferences == 0) {


		FLAGS_log_dir = "c:/templog/ordbc";
		google::InitGoogleLogging("Ordbc");

		LOG(WARNING) << "references " << iVreferences << " " << cVariespipeline;
		FlushLogFiles(GLOG_WARNING);

		cVpool = boost::shared_ptr<CorePool> (new CorePool ());
		cVorionconnectionfactory = boost::shared_ptr<CoreCocf>  (new CoreCocf ());
		cVorionconnectionfactory->init_ (cVpool, sVaddress, iPort, CoreCocf::bCprotobuf);
		cVariesconnectionfactory = boost::shared_ptr<CoreCaco> (new CoreCaco (cVorionconnectionfactory, iWorkers, iBulksize));
		cVariesconnectionfactory->addrf ();
		bVreturn = cVariesconnectionfactory->valid ();
		if (bVreturn) cVariespipeline = boost::shared_ptr<CoreCacp> (new CoreCacp (cVariesconnectionfactory));

		LOG(WARNING) << "allocation " << iVreferences << " " << cVariespipeline << " " << GetCurrentThreadId();
		FlushLogFiles(GLOG_WARNING);

	}
	iVreferences++;
	cVbuildmutex.relse ();
	return bVreturn;
																																						
}

JNIEXPORT jboolean JNICALL Java_com_tomting_orion_ordbc_Ordbc_destroyOrdbc (JNIEnv *, jobject) {

	cVbuildmutex.locks ();
	iVreferences--;
	if (iVreferences == 0) {

		LOG(WARNING) << "deallocation" << iVreferences << " " << cVariespipeline << " " << GetCurrentThreadId();
		FlushLogFiles(GLOG_WARNING);

		cVariespipeline.reset ();
		cVariesconnectionfactory->subrf ();
		cVariesconnectionfactory.reset ();
		cVorionconnectionfactory.reset ();
		cVpool.reset ();

		LOG(WARNING) << "close " << iVreferences << " " << cVariespipeline;
		FlushLogFiles(GLOG_WARNING);
	}
	cVbuildmutex.relse ();
	return true;
}


JNIEXPORT jbyteArray JNICALL Java_com_tomting_orion_ordbc_Ordbc_run (JNIEnv* cEnv, jobject, jbyteArray cArray, jboolean bOptimistic) {
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


	return cEnv->NewByteArray (0);*/


}




