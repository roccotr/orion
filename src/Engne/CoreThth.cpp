#include "CoreThth.hpp" 
#include "CoreThsv.hpp"
#include "CoreFsys.hpp"
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

#include "CoreCocf.hpp"
#include "TmplShst.cpp"
#include "TmplTsrl.cpp"

  __fastcall CoreThth::CoreThth (	PTR_POOL cPool, CoreSckt* cSocket, void* cThriftserver,
																	bool bSharedmemory, AnsiString sSharedmemoryprefix, int iSharedmemorysuffix) : 
																	TThread (true) {
		
		bValive = false;
		bVsharedmemory = bSharedmemory; 
		cVpool = cPool;
		cVsocket = cSocket;
		cVthriftserver = cThriftserver;
		if (bVsharedmemory) {
			sVsharedmemoryname = sSharedmemoryprefix + "_" + (AnsiString) iSharedmemorysuffix;
			cVsharedsend = boost::shared_ptr<CoreShst<ThrfBsrr> > (new CoreShst<ThrfBsrr> (true, sVsharedmemoryname + "_RES", true));
			cVsharedreceive = boost::shared_ptr<CoreShst<ThrfBsrc> > (new CoreShst<ThrfBsrc> (true, sVsharedmemoryname + "_SVC", true));
			//Sleep (500);
			Sleep (0);
		} else sVsharedmemoryname = "";  
		Resume ();
	}

  __fastcall CoreThth::~CoreThth () {

	}

  /*EXECUTE*/
  void __fastcall CoreThth::Execute () {
		AnsiString sProtocol; 
		char bProtocol;

		bValive = true;
		if (cVsocket->mrcve (sProtocol)) {
			bProtocol = sProtocol.c_str () [0];
			cVsocket->msend (sVsharedmemoryname);
			if (bProtocol == CoreCocf::bCprotobuf) loopp ();
			else loop_ ();
		}
		bValive = false; 
	}

	/*LOOP*/
	void __fastcall CoreThth::loop_ () {
		int iVdim;
		CoreThsv* cVserver = (CoreThsv*) cVthriftserver;

		while (!Terminated) {
			ThrfBsrc cVdeserialized;
			ThrfBsrr cVserialized;

			if (!bVsharedmemory) {
				boost::shared_ptr<CoreTsrl> cVdeserializer (new CoreTsrl ());

				iVdim = cVsocket->recve ();
				if (iVdim <= 0) {
					Terminate ();
					break;
				}
				cVsocket->recve ((char*) cVdeserializer->frbub (iVdim), iVdim);
				cVdeserializer->frbue (iVdim);
				cVdeserializer->parse<ThrfBsrc> (cVdeserialized);
			} else {
				cVdeserialized = cVsharedreceive->pop_s ();
			}
			switch (cVdeserialized.iVbulkservicetype) {
				case iEbulkservicetype::SINGLETHRIFT:

					//printf ("-> *%s*\n",cVdeserialized.cVsinglethrift.cVstatement.cVkey.sVmain.c_str ());

					cVserver->thriftbatch (&cVserialized.cVsinglethrift, &cVdeserialized.cVsinglethrift);
					break;
				case iEbulkservicetype::BULKTHRIFT:
					cVserver->thriftbatch (cVserialized.cVbulkthrift, cVdeserialized.cVbulkthrift);
					break;
				case iEbulkservicetype::BULKSTRING:
					//TODO
					break;
				case iEbulkservicetype::PING:
					Terminate ();
					return;
			}

			if (!bVsharedmemory) {
				boost::shared_ptr<CoreTsrl> cVserializer (new CoreTsrl ());

				iVdim = cVserializer->srlze<ThrfBsrr> (cVserialized, false);
				cVsocket->send_ ((char*) cVserializer->tobuf (), iVdim);
				cVsocket->lflsh ();
			} else {
				cVsharedsend->pushs (cVserialized);
			}
		}
	}

	/*LOOP*/
	void __fastcall CoreThth::loopp () {
		int iVdim;
		CoreThsv* cVserver = (CoreThsv*) cVthriftserver;
		PTR_FILESYSTEM cVfilesystem (new CoreFsys (cVpool));
		PrtoBsrc cVdeserialized;
		PrtoBsrr cVserialized;

		while (!Terminated) {
			cVdeserialized.Clear ();
			cVserialized.Clear ();

			if (!bVsharedmemory) {
				iVdim = cVsocket->recve ();
				if (iVdim <= 0) {
					Terminate ();
					break;
				}
				cVfilesystem->setbf (iVdim);
				cVsocket->recve ((char*) cVfilesystem->baddr (), iVdim);
				cVdeserialized.ParseFromArray (cVfilesystem->baddr (), iVdim);
			} else {
				iVdim = cVsharedreceive->popbb ();
				cVdeserialized.ParseFromArray (cVsharedreceive->popbe (), iVdim);
			}

			switch (cVdeserialized.ivbulkservicetype ()) {
				case SINGLEPROTO:

					//printf ("-> *%s*\n", cVdeserialized.mutable_cvsingleproto ()->mutable_cvstatement ()->mutable_cvkey ()->svmain ().c_str ());

					cVserver->protobatch  (cVserialized.mutable_cvsingleproto (), cVdeserialized.mutable_cvsingleproto ());
					break;
				case BULKPROTO:

					/*
					for (int iV = 0; iV < cVdeserialized.mutable_cvbulkproto ()->size (); iV++) {
						printf ("-> *%s*\n", cVdeserialized.mutable_cvbulkproto (iV)->mutable_cvstatement ()->mutable_cvkey ()->svmain ().c_str ());
					}*/

					cVserver->protobatch (cVserialized.mutable_cvbulkproto (), cVdeserialized.mutable_cvbulkproto ());
					break;
				case BULKSTRING:
					//TODO
					break;
				case iEbulkservicetype::PING:
					Terminate (); 
					return;
			}
				
			if (!bVsharedmemory) {
				iVdim = cVserialized.ByteSize ();
				cVfilesystem->setbf (iVdim);
				cVserialized.SerializeToArray (cVfilesystem->baddr (), iVdim);
				cVsocket->send_ ((char*) cVfilesystem->baddr (), iVdim);
				cVsocket->lflsh ();
			} else {
				iVdim = cVserialized.ByteSize ();
				cVserialized.SerializeWithCachedSizesToArray ((google::protobuf::uint8*) cVsharedsend->pshbb (iVdim));
				cVsharedsend->pshbe ();
			}
		}
	}

	/*IS ALiVe*/
	bool __fastcall CoreThth::isalv () {
		return bValive;
	}
