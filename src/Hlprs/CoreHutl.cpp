#include "CoreHutl.hpp"
#include "TmplList.cpp"


using namespace boost::filesystem;

	l64 CoreHutl::iVglobaltimestamppartial = 0;


  /*REAd DiRectory*/
	void __fastcall CoreHutl::readr (	AnsiString sPath, AnsiString sMask,
																		CoreList<AnsiString>& cFilenames) {
		AnsiString* sVfilenames;
		int iVcount;

		sVfilenames = NULL;
		readr (sPath, sMask, &sVfilenames, iVcount);			
		cFilenames.clear ();
		for (int iV = 0; iV < iVcount; iV++) *cFilenames.set__ () = sVfilenames [iV];
		if (sVfilenames != NULL) delete [] sVfilenames;
	}

	/*CReate DIRectory*/
	bool __fastcall CoreHutl::crdir (AnsiString sPath) {
		
		path cVpath (sPath.c_str ());
		return create_directories (cVpath); 
	}

  /*IS Path Valid*/
	bool __fastcall CoreHutl::ispvl (AnsiString sPath) {
		
		path cVpath (sPath.c_str ());
		return exists (cVpath); 
	}

	/*ReMove DIRectory*/
	bool __fastcall CoreHutl::rmdir (AnsiString sPath) {

		if (ispvl (sPath)) {
			path cVpath (sPath.c_str ());
			remove_all (cVpath);
			return true;
		} else return false;
	}

