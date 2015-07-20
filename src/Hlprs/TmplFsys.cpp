#include "CoreFsys.hpp"
#include <google/protobuf/stubs/common.h>
#include <math.h>

using namespace orion;

	/*SeRiaLiZe*/
	template<class T>
	bool __fastcall CoreFsys::srlze (T* cProtocolbuffers) {
		int iVserializedbuffer;

		iVserializedbuffer = cProtocolbuffers->ByteSize ();
		setbf (iVserializedbuffer);
		cProtocolbuffers->SerializeWithCachedSizesToArray ((google::protobuf::uint8*) vVbuffer);		
		iVvalidbuffer = iVserializedbuffer;
		return true;
	}

	/*SeRiaLiZe*/
	template<class T>
	bool __fastcall CoreFsys::parse (T* cProtocolbuffers) {
				
		return cProtocolbuffers->ParseFromArray (vVbuffer, iVvalidbuffer);
	}

	/*Serialized Chunk To DiSk*/
	template<class T>
	bool __fastcall CoreFsys::sctds (T* cProtocolbuffers, bool bCompress, bool bFlush) {
		int iVserializedbuffer;

		iVserializedbuffer = cProtocolbuffers->ByteSize ();
		setbf (iVserializedbuffer, true);
		cProtocolbuffers->SerializeWithCachedSizesToArray ((google::protobuf::uint8*) vVbuffer);		
		if (wchnk (iVserializedbuffer, true, bCompress)) {
			if (bFlush) flush ();
			return true;
		}
		return false;
	}

	/*Parse Chunk From DiSk*/
	template<class T>
	bool __fastcall CoreFsys::pcfds (T* cProtocolbuffers, bool& bParsed, int64vcl iPosition, bool bCompress) {
		int iVserializedbuffer;

		if (iPosition >= 0 && !setpo (iPosition)) return false;
		if (!rchnk (&iVserializedbuffer, true, bCompress)) return false;	
		bParsed = cProtocolbuffers->ParseFromArray (vVbuffer, iVserializedbuffer);
		return true;
	}
