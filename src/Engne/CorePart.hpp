#ifndef COREPART
#define COREPART
#include <glog/logging.h>
#include <protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"
#include "CoreEnpo.hpp"
#include "CoreTble.hpp"

namespace orion {

class CorePart {

public:

	typedef struct CoreCplc {
		AnsiString sVindex;
		AnsiString sVnodeid;
		AnsiString sVtoken;

	} CUSTOMPARTITIONLOC;

  enum iCpartitiontype {
		iCnopartition = 0,
		iCrandompartition = 1,
		iCbyteorderpartition = 2,
		iCcustompartition = 3
  };

	typedef struct CorePtcl {
		CorePart::iCpartitiontype iVpartitiontype;
		CoreList<CUSTOMPARTITIONLOC> cVpartitionpool;
		AnsiString sVupperbound;
		AnsiString sVlowerbound;
		AnsiString sVjoincustompartition;

		CorePtcl () {
			iVpartitiontype = CorePart::iCcustompartition;
			sVjoincustompartition = "";
		}
	};

	typedef struct CorePttl {
		bool bVmanualpartition;
		int iVcustomreplicationfactor;

		CorePtcl cVpartition;
		CorePtcl cVpartitionindex;

		/*INITIALIZE*/
		CorePttl () {
			bVmanualpartition = false;
			iVcustomreplicationfactor = -1;
		}

	} PARTITIONLOC;

private:
	const static AnsiString sCchar0;
	const static AnsiString sCignored;

	PTR_ENDPOINT cVendpoint;

	void setpd (	CorePtcl* cCustompartition,
								AnsiString sNodeid, AnsiString sToken, AnsiString sPartition, 
								int iReplicationfactor); 

public:
  __fastcall CorePart ();
  __fastcall ~CorePart ();

	void __fastcall init_ (PTR_ENDPOINT cEndpoint);
	int __fastcall desow (	iCpartitiontype iPartitiontype, AnsiString sKey, 
													bool bIndex, CorePart::PARTITIONLOC* cPartitionloc, 
													bool bScan = false);
	bool __fastcall next_ (	int iFirstseed, 
													iCpartitiontype iPartitiontype, 
													CoreTble::iCquerytype iQuerytype,
													int& iCursor);
	
	bool __fastcall setpd (	CorePart::PARTITIONLOC* cPartitiondata, AnsiString sNodeid, AnsiString sToken,
													AnsiString sPartition = "", AnsiString sPartitionindex = "");
	static void __fastcall pptpb (	CoreList<CorePart::CUSTOMPARTITIONLOC>* cPartitionpool, 
																	google::protobuf::RepeatedPtrField<PrtoPtel>* cProtocolbuffer);
	void __fastcall pbtpp (	google::protobuf::RepeatedPtrField<PrtoPtel>* cProtocolbuffer,
													CorePtcl* cCustompartition, int iReplicationfactor);
	AnsiString __fastcall rebpp ( CoreList<CorePart::CUSTOMPARTITIONLOC>* cPartitionpool);
	static CorePart::iCpartitiontype __fastcall getpt (CorePart::PARTITIONLOC* cPartitiondata, bool bIndex);

	static AnsiString __fastcall gettk (AnsiString sKey);
	static AnsiString split (AnsiString sLeft, AnsiString sRight, int iNum, int iDen, AnsiString& sHex);
	static AnsiString gethx (AnsiString sBound);
	
};

typedef boost::shared_ptr<CorePart> PTR_PARTITIONER;

};

#endif