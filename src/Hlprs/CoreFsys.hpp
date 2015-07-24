#ifndef COREFSYS
#define COREFSYS
#include <glog/logging.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"
#include "CorePool.hpp"

namespace orion {

class CoreFsys
{

private:
	AnsiString sVfilename;
	bool bVopen;
	int iVbuffersize;
	int iVvalidbuffer;
	void* vVbuffer;
	FILE* cVfile;
	PTR_POOL cVpool;

	void __fastcall close ();
	void __fastcall setfn (AnsiString sFilename);
	bool __fastcall open_ (AnsiString sMode);
	void __fastcall ustbf ();
	bool __fastcall owrte (void* vData, int iSize); 
	bool __fastcall read_ (int iSize = -1);

public:

  __fastcall CoreFsys (PTR_POOL cPool);
  __fastcall ~CoreFsys ();
	bool __fastcall owrte (AnsiString sFilename);
	bool __fastcall read_ (AnsiString sFilename);
	bool __fastcall owrte (AnsiString sFilename, void* vData, int iSize);

	bool __fastcall begin (int iBuffersize, AnsiString sFilename, bool bReadmode = true);
	void __fastcall end__ ();
	void __fastcall flush ();
	bool __fastcall isopn ();
	int64vcl __fastcall fsize ();
	bool __fastcall rchnk (int* iSize, bool bReadsizetoo = false, bool bCompress = false);
	bool __fastcall wchnk (int iSize, bool bWritesizetoo = false, bool bCompress = false);
	int __fastcall bsize ();
	int64vcl __fastcall getpo ();
	bool __fastcall setpo (int64vcl iPosition);
	void* __fastcall baddr ();	
	void __fastcall setbf (int iBuffersize, bool bCompress = false);
	void __fastcall setmb (void* vData, int iBuffersize, bool bMutable = true);

	template<class T>
	bool __fastcall srlze (T* cProtocolbuffers);
	template<class T>
	bool __fastcall parse (T* cProtocolbuffers);
	template<class T>
	bool __fastcall sctds (T* cProtocolbuffers, bool bCompress = true, bool bFlush = false);
	template<class T>
	bool __fastcall pcfds (T* cProtocolbuffers, bool& bParsed, int64vcl iPosition = -1, bool bCompress = true);
	
};

typedef boost::shared_ptr<CoreFsys> PTR_FILESYSTEM;

};

#endif