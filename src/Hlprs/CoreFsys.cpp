#include "CoreFsys.hpp"
#include "CoreButl.hpp"
#include "snappy.h"

using namespace orion;

  __fastcall CoreFsys::CoreFsys (PTR_POOL cPool) {
		
		sVfilename = NULL;
		cVpool = cPool;
		bVopen = false;
		iVbuffersize = -1;
		iVvalidbuffer = 0;
  }

  __fastcall CoreFsys::~CoreFsys () {

		close ();
		ustbf ();
  }

	/*CLOSE*/
	void __fastcall CoreFsys::close () {

		if (bVopen) fclose (cVfile);
		bVopen = false;
		cVfile = NULL;
	}

	/*SET FileName*/
	void __fastcall CoreFsys::setfn (AnsiString sFilename) {

		sVfilename = sFilename;
	}

	/*OPEN*/
	bool __fastcall CoreFsys::open_ (AnsiString sMode) {

		if (!bVopen) {
			cVfile = fopen (CoreButl::ffpen (sVfilename).c_str (), sMode.c_str ());
			bVopen = cVfile != NULL;
		}
		return bVopen;
	}

	/*SET BuFfer size*/
	void __fastcall CoreFsys::setbf (int iBuffersize, bool bCompress) {
		int iVtempbuffersize;

		iVtempbuffersize = iBuffersize;
		if (bCompress) iVtempbuffersize += snappy::MaxCompressedLength(iBuffersize);
		if (iVtempbuffersize > 0 && iVbuffersize < iVtempbuffersize) {
			ustbf ();
			iVbuffersize = iVtempbuffersize;			
			iVvalidbuffer = 0;
			vVbuffer = cVpool->mallc (CorePool::iCchar, iVbuffersize, "FSYS");
		}
	}

	/*UnSeT BuFfer*/
	void __fastcall CoreFsys::ustbf () {

		if (iVbuffersize > 0) {
			iVbuffersize = -1;
			cVpool->free_ (vVbuffer);
		}
	}

	/*OverWRiTE*/
	bool __fastcall  CoreFsys::owrte (void* vData, int iSize) {
			int iVsize;

			if (open_ ("wb")) {
				iVsize = fwrite (vData, 1, iSize, cVfile);
				close ();
				return (iVsize == iSize);
			} else return false;
	}

	/*OverWRiTE*/
	bool __fastcall  CoreFsys::owrte (AnsiString sFilename) {

			setfn (sFilename);
			owrte (vVbuffer, iVvalidbuffer);
			return owrte (vVbuffer, iVvalidbuffer);
	}

	/*get SIZE*/
	int64vcl __fastcall CoreFsys::fsize () {
		int64vcl iVreturn;
		unsigned int iVcurrent;

		if (!bVopen) return 0;
		iVcurrent = FTELL (cVfile);
		FSEEK (cVfile, 0, SEEK_END);
		iVreturn = FTELL (cVfile);
		FSEEK (cVfile, iVcurrent, SEEK_SET);
		return iVreturn;
	}

	/*READ*/
	bool __fastcall CoreFsys::read_ (int iSize) {
		int iVsize;

		if (open_ ("rb")) {
			if (iSize < 0) iVsize = fsize ();
			else iVsize = iSize;
			setbf (iVsize);
			iVvalidbuffer = fread (vVbuffer, 1, iVsize, cVfile);
			return iVvalidbuffer > 0;
		} else {
			return false;
		}
	}

	/*READ*/
	bool __fastcall CoreFsys::read_ (AnsiString sFilename) {

		setfn (sFilename);
		return read_ ();
	}

	/*OverWRiTE*/
	bool __fastcall CoreFsys::owrte (AnsiString sFilename, void* vData, int iSize) {

		setfn (sFilename);
		return owrte (vData, iSize);
	}

	/*BEGIN*/
	bool __fastcall CoreFsys::begin (int iBuffersize, AnsiString sFilename, bool bReadmode) {

		close ();
		setfn (sFilename);			
		setbf (iBuffersize);
		if (bReadmode) return open_ ("rb");
		else return open_ ("wb");
	}

	/*END__*/
	void __fastcall CoreFsys::end__ () {

		close ();
	}
	
	/*FLUSH*/
	void __fastcall CoreFsys::flush () {

		if (bVopen) fflush (cVfile);
	}

	/*IS OPeN*/
	bool __fastcall CoreFsys::isopn () {

		return bVopen;
	}

	/*Read CHuNK*/
	bool __fastcall CoreFsys::rchnk (int* iSize, bool bReadsizetoo, bool bCompress) {
		int iVsizeread;
		int iVsizeuncompressed = 0;
		unsigned int iVsize;
		void* vVbufferread;

		if (!bVopen) return false;
		if (bCompress) {
			if (!(fread (&iVsizeuncompressed, 1, sizeof (iVsizeuncompressed), cVfile) == 
					sizeof (iVsizeuncompressed))) return false;
		}
		if (bReadsizetoo) {
			if (!(fread (&iVsize, 1, sizeof (iVsize), cVfile) == sizeof (iVsize))) return false;
		} else iVsize = *iSize;
		if (iVsize > 0) {
			setbf (iVsize + iVsizeuncompressed);						
			if (bCompress) vVbufferread =  &((char*) vVbuffer) [iVbuffersize - iVsize - 1];
			else vVbufferread = vVbuffer;
			iVsizeread = fread (vVbufferread, 1, iVsize, cVfile);
			if (bCompress) {				
				if (iVsizeread == iVsize) {
					snappy::RawUncompress ((const char*) vVbufferread, iVsizeread, (char*) vVbuffer);
					iVsize = iVsizeuncompressed;
				} else iVsize = 0;
			} else iVsize = iVsizeread;
		}
		iVvalidbuffer = iVsize;
		*iSize = iVvalidbuffer;
		return true;
	}

	/*Write CHuNK*/
	bool __fastcall CoreFsys::wchnk (int iSize, bool bWritesizetoo, bool bCompress) {
		int iVsize;
		void* vVbufferwrite;
		bool bVreturn;
		size_t iVsizecompressed;

		if (!bVopen) return false;
		bVreturn = true;
		if (bCompress) {
			vVbufferwrite =  &((char*) vVbuffer) [iSize];
			snappy::RawCompress ((const char*) vVbuffer, iSize, (char*) vVbufferwrite, &iVsizecompressed);
			iVsize = iVsizecompressed;
			bVreturn &=	fwrite (&iSize, 1, sizeof (iSize), cVfile) == sizeof (iSize);
		} else {
			vVbufferwrite = vVbuffer;
			iVsize = iSize;
		}
		if (bWritesizetoo) bVreturn &=	fwrite (&iVsize, 1, sizeof (iVsize), cVfile) == sizeof (iVsize);
		bVreturn &= fwrite (vVbufferwrite, 1, iVsize, cVfile) == iVsize;
		return bVreturn;
	}

	/*Buffer Size*/
	int __fastcall CoreFsys::bsize () {

		return iVvalidbuffer;
	}

	/*GET POsition*/
	int64vcl __fastcall CoreFsys::getpo () {

		return bVopen ? FTELL (cVfile) : 0;
	}

	/*GET POsition*/
	bool __fastcall CoreFsys::setpo (int64vcl iPosition) {

		return bVopen && FSEEK (cVfile, iPosition, SEEK_SET) == 0;
	}

	/*Buffer Address*/
	void* __fastcall CoreFsys::baddr () {

		return vVbuffer;
	}

	/*SET Mutable Buffer*/
	void __fastcall CoreFsys::setmb (void* vData, int iBuffersize, bool bMutable) {

		if (bMutable) {
			ustbf ();
			vVbuffer = vData;
		} else {
			setbf (iBuffersize);
			memcpy (vVbuffer, vData, iBuffersize);
		}
		iVvalidbuffer = iBuffersize;
	}
