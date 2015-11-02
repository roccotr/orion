#include "IfceOrn_.hpp"
#include "IfceArs_.hpp"
#include <cstdlib>
#include <time.h>
#include <glog/logging.h>
#include <gflags/gflags.h>
#include "com_tomting_orion_ordbc_Ordbc.h"


	/*NATIVE NEW ARIES*/
	JNIEXPORT jlong JNICALL Java_com_tomting_orion_ordbc_Ordbc_nativeNewAries (	JNIEnv* cEnv, jobject, jstring sAddress, 
																																									jint iPort, jint iWorkers, jint iBulksize) {
	
		AnsiString sVaddress;
		const char* bVaddress;
		
		bVaddress = cEnv->GetStringUTFChars (sAddress, NULL);
		sVaddress = bVaddress;
		cEnv->ReleaseStringUTFChars (sAddress, bVaddress);

		bool bVvalid;
		IfceArs_* cVinstance = new IfceArs_ (sVaddress, iPort, iWorkers, iBulksize, bVvalid);
		if (bVvalid) return (long) cVinstance;		
		delete cVinstance;
		return 0;
	}

	/*NATIVE DELETE ARIES*/
	JNIEXPORT jboolean JNICALL JNICALL Java_com_tomting_orion_ordbc_Ordbc_nativeDeleteAries (JNIEnv* cEnv, jobject, jlong cOpaquehandle) {

		IfceArs_* cVinstance = (IfceArs_*) cOpaquehandle;
		if (cVinstance != NULL) delete cVinstance;
		return true;
	}

	/*NATIVE RUN ARIES*/
	JNIEXPORT jbyteArray JNICALL Java_com_tomting_orion_ordbc_Ordbc_nativeRunAries (JNIEnv* cEnv, jobject, 
		jlong cOpaquehandle, jbyteArray cArray, jboolean bOptimistic) {
		jboolean bVcopy;
		ThrfSrvc cVservice;
		ThrfSrvc cVserviceresult;

		int iVarray = cEnv->GetArrayLength (cArray);
		char* cVarray = (char*) cEnv->GetPrimitiveArrayCritical (cArray, &bVcopy);
		boost::shared_ptr<TMemoryBuffer> cVtransportinput (new TMemoryBuffer ((uint8_t*) cVarray, iVarray));
		boost::shared_ptr<TProtocol> cVprotocolinput (new TBinaryProtocol (cVtransportinput));
		cVservice.read (cVprotocolinput.get ());
		cEnv->ReleasePrimitiveArrayCritical (cArray, cVarray, NULL);

		bool bVoptimistic = bOptimistic == JNI_TRUE;
		if (
				((IfceArs_*) cOpaquehandle)->run__ (cVservice, cVserviceresult, bVoptimistic) 
				&& !bOptimistic) {

			uint8_t* bVserviceresult;
			uint32_t iVserviceresult;
			boost::shared_ptr<TMemoryBuffer> cVtransportresult (new TMemoryBuffer ());
			boost::shared_ptr<TProtocol> cVprotocolresult (new TBinaryProtocol (cVtransportresult));
			cVserviceresult.write (cVprotocolresult.get ());
			cVtransportresult->getBuffer (&bVserviceresult, &iVserviceresult);

			jbyteArray cVresult = cEnv->NewByteArray (iVserviceresult);	
			cVarray = (char*) cEnv->GetPrimitiveArrayCritical (cVresult, &bVcopy);
			memcpy (cVarray, bVserviceresult, iVserviceresult);
			cEnv->ReleasePrimitiveArrayCritical (cVresult, cVarray, NULL);
			return cVresult;
		} else return cEnv->NewByteArray (0);
	}

	/*PING*/
	JNIEXPORT jstring JNICALL Java_com_tomting_orion_ordbc_Ordbc_ping (JNIEnv* cEnv, jobject, jstring sPinged) {
		const char* bVtorelease = cEnv->GetStringUTFChars(sPinged, NULL);
		AnsiString sVping = bVtorelease;
		cEnv->ReleaseStringUTFChars(sPinged, bVtorelease);
		printf ("ping: %s\n", sVping.c_str ());
		fflush (stdout);
		return cEnv->NewStringUTF (sVping.UpperCase ().c_str ());
	}

	/*NATIVE NEW EMBEDDED*/
	JNIEXPORT jlong JNICALL Java_com_tomting_orion_ordbc_Ordbc_nativeNewEmbedded
		(JNIEnv* cEnv, jobject, jbyteArray cOptions, jstring sPath) {
		jboolean bVcopy;
		PrtoIoop cVoptions;

		const char* bVpath = cEnv->GetStringUTFChars(sPath, NULL);
		AnsiString sVpath = bVpath;
		cEnv->ReleaseStringUTFChars(sPath, bVpath);
		int iVoptions = cEnv->GetArrayLength (cOptions);
		char* bVoptions = (char*) cEnv->GetPrimitiveArrayCritical (cOptions, &bVcopy);
		cVoptions.ParseFromArray (bVoptions, iVoptions);
		cEnv->ReleasePrimitiveArrayCritical (cOptions, bVoptions, NULL);

		struct Options options (cVoptions);
		DB* cVinstance = new DB (options, sVpath.to_string ());
		return (long) cVinstance;
	}

	/*NATIVE DESTROY EMBEDDED*/
	JNIEXPORT jboolean JNICALL Java_com_tomting_orion_ordbc_Ordbc_nativeDestroyEmbedded
		(JNIEnv* cEnv, jobject, jbyteArray cOptions, jstring sPath) {
		jboolean bVcopy;
		PrtoIoop cVoptions;

		const char* bVpath = cEnv->GetStringUTFChars(sPath, NULL);
		AnsiString sVpath = bVpath;
		cEnv->ReleaseStringUTFChars(sPath, bVpath);
		int iVoptions = cEnv->GetArrayLength (cOptions);
		char* bVoptions = (char*) cEnv->GetPrimitiveArrayCritical (cOptions, &bVcopy);
		cVoptions.ParseFromArray (bVoptions, iVoptions);
		cEnv->ReleasePrimitiveArrayCritical (cOptions, bVoptions, NULL);

		struct Options options (cVoptions);
		Status cVstatus = DB::DestroyDB (options, sVpath.to_string ());
		return cVstatus.ok ();
	}

	/*NATIVE DELETE EMBEDDED*/
	JNIEXPORT jboolean JNICALL Java_com_tomting_orion_ordbc_Ordbc_nativeDeleteEmbedded 
		(JNIEnv* cEnv, jobject, jlong cOpaquehandle) {

		DB* cVinstance = (DB*) cOpaquehandle;
		delete cVinstance;
		return true;
	}

	/*NATIVE RUN EMBEDDED*/
	JNIEXPORT jbyteArray Java_com_tomting_orion_ordbc_Ordbc_nativeRunEmbedded
		(JNIEnv* cEnv, jobject, jlong cOpaquehandle, jint iServicetype, jbyteArray cService) {
		jbyteArray cVresult;
		iCembeddedservicetype iVservicetype = (iCembeddedservicetype) iServicetype;
		DB* cVinstance = (DB*) cOpaquehandle;
		Status cVstatus;

		jboolean bVcopy;
		int iVservice = cEnv->GetArrayLength (cService);
		char* bVservice = (char*) cEnv->GetPrimitiveArrayCritical (cService, &bVcopy);
	
		switch (iVservicetype) {
			case EMBEDDEDWRITE:
				{
					PrtoIwop cVwriteoptions;
					cVwriteoptions.ParseFromArray (bVservice, iVservice);
					switch (cVwriteoptions.ivlevel ()) {
						case L1:
							cVstatus.status.set_bvresult (
								cVinstance->cVplanner->lcstm (
									cVwriteoptions.mutable_cvstatementl1 (), true));
							break;
						case L2:
							cVstatus.status.set_bvresult (
								cVinstance->cVplanner->statm (
									cVwriteoptions.mutable_cvstatementl2 (), 
									cVwriteoptions.bvoptimizewriteindex (),
									cVwriteoptions.bvupdateonlyindex (),
									cVwriteoptions.bvlocalfilter()));
							break;
					}
				}
				break;
			case EMBEDDEDSTORAGE:
				{
					PrtoIgop cVstorageoptions;
					cVstorageoptions.ParseFromArray (bVservice, iVservice);
					switch (cVstorageoptions.ivlevel ()) {
						case L1:
							cVstatus.status.set_bvresult (
								cVinstance->cVplanner->crttb (cVstorageoptions.mutable_cvdmll1 ()));
							break;
						case L2:
							cVstatus.status.set_bvresult (
								cVinstance->cVplanner->crttb (cVstorageoptions.mutable_cvdmll2 ()));
							break;
					}
				}
				break;
			case EMBEDDEDREAD:
				{
					PrtoIrop cVreadoptions;
					cVreadoptions.ParseFromArray (bVservice, iVservice);
					switch (cVreadoptions.ivlevel ()) {
						case L1:
							cVstatus.status.set_bvresult (
								cVinstance->cVplanner->lcqry (
									cVreadoptions.mutable_cvqueryl1 (), 
									cVstatus.status.mutable_cvsnapshotl1 ()->mutable_cvvalue (), true));
							if (!cVstatus.status.bvresult ()) cVstatus.status.clear_cvsnapshotl1 ();
							else 
								cVstatus.status.mutable_cvsnapshotl1 ()->mutable_cvkey ()->set_svmain (
									cVreadoptions.mutable_cvqueryl1 ()->cvkey ().svmain ());
							break;
						case L2:
							cVstatus.status.set_bvresult (
								cVinstance->cVplanner->query (
									cVreadoptions.mutable_cvqueryl2 (),
									cVstatus.status.mutable_cvsnapshotl2 ()));
							break;
					}
				}
				break;
			case EMBEDDEDOSQL:
				{
					PrtoIqop cVosqloptions;
					cVosqloptions.ParseFromArray (bVservice, iVservice);
					cVstatus.status.set_bvresult (
						cVinstance->cVplanner->osql_ (
							cVosqloptions.mutable_cvosqll2 (), 
							cVstatus.status.mutable_cvsnapshotl2 ()));
				}
				break;
		}
		cEnv->ReleasePrimitiveArrayCritical (cService, bVservice, NULL);
		string sVstatusserialized = cVstatus.status.SerializeAsString ();
		cVresult = cEnv->NewByteArray (sVstatusserialized.size ());	
		char*	bVresult = (char*) cEnv->GetPrimitiveArrayCritical (cVresult, &bVcopy);
		memcpy (bVresult, sVstatusserialized.data (), sVstatusserialized.size ());
		cEnv->ReleasePrimitiveArrayCritical (cVresult, bVresult, NULL);
		return cVresult;
	}


