#include "CoreHutl.hpp"

using namespace orion;

  /*REAd DiRectory*/
  void __fastcall CoreHutl::readr ( AnsiString sPath, AnsiString sMask,
                                    AnsiString** sFilenames, int& iCount,
																		AnsiString sFoldermask) {
    int iV;
		bool bVcontinue;
    TList* lVlist;
    char bVoldpath [MAX_PATH];
		WIN32_FIND_DATA fVscanfiles;
		HANDLE cVfinder;
		char* bVtemp;

    lVlist = new TList;
    GetCurrentDirectory(MAX_PATH, bVoldpath);
		if (SetCurrentDirectory (sPath.c_str ())!= 0) {
			cVfinder = FindFirstFile(sMask.c_str (), &fVscanfiles); 
			if (cVfinder != INVALID_HANDLE_VALUE) {
				bVcontinue = true;
				while (bVcontinue) {				
					if (!(fVscanfiles.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
						bVtemp = new char [MAXFILE + MAXEXT];
						strcpy (bVtemp, fVscanfiles.cFileName);
						lVlist->Add(bVtemp);
					}
					try {
						bVcontinue = FindNextFile(cVfinder, &fVscanfiles) != 0;
					} catch (...) {
						bVcontinue = false;
					}
				}
				FindClose(cVfinder);
			}
			if (sFoldermask != NULL) {
				cVfinder = FindFirstFile(sFoldermask.c_str (), &fVscanfiles); 
				if (cVfinder != INVALID_HANDLE_VALUE) {
					bVcontinue = true;
					while (bVcontinue) {				
						if ((fVscanfiles.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && 
								fVscanfiles.cFileName [0] != '.') {
							bVtemp = new char [MAXFILE + MAXEXT];
							strcpy (bVtemp, fVscanfiles.cFileName);
							lVlist->Add(bVtemp);
						}
						try {
							bVcontinue = FindNextFile(cVfinder, &fVscanfiles) != 0;
						} catch (...) {
							bVcontinue = false;
						}
					}
					FindClose(cVfinder);
				}
			}
		}
    SetCurrentDirectory (bVoldpath);
		iCount = lVlist->Count;		
		if (sFilenames [0] != NULL) delete [] sFilenames [0];            
		if (iCount == 0) {
			sFilenames [0]= new AnsiString [1];
    } else {
    	sFilenames [0]= new AnsiString [iCount];
    	for (iV = 0; iV < iCount; iV++) {
      	bVtemp = (char*) lVlist->Items [iV];
      	sFilenames [0][iV] = (AnsiString) bVtemp;
      	delete [] bVtemp;
  		}
		}            
    bVtemp = NULL;
    delete lVlist;
    lVlist = NULL;
  }
	
  /*GET System Time*/
  bool __fastcall CoreHutl::getst (AnsiString sFilename, SYSTEMTIME* cWrite) {
		bool bVreturn;
    FILETIME cVaccess;
    FILETIME cVcreate;
    FILETIME cVwrite;
    SYSTEMTIME cVswrite;
    HANDLE hVhandle;

    bVreturn = false;
    hVhandle = CreateFile(  sFilename.c_str(), GENERIC_READ, FILE_SHARE_READ,
                            NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hVhandle != INVALID_HANDLE_VALUE) {
      GetFileTime(hVhandle, &cVcreate, &cVaccess, &cVwrite);
      FileTimeToSystemTime(&cVwrite, &cVswrite);
      SystemTimeToTzSpecificLocalTime(NULL, &cVswrite, cWrite);
      bVreturn = true;
    }
    CloseHandle(hVhandle);
    return bVreturn;
		return true;
  }
			
	/*APPlication NaMe*/
	AnsiString __fastcall CoreHutl::appnm (void* hInstance) {
		char bVmodulename [MAX_PATH + 1];
		TCHAR bVtmodulename [MAX_PATH + 1]; 
		AnsiString sVreturn;
	
		bVmodulename [MAX_PATH] = (int) 0;
		GetModuleFileName ((HMODULE) hInstance, bVtmodulename, MAX_PATH);
		return bVtmodulename;
	}

	/*GET Timestamp Partial*/
	int64vcl CoreHutl::gettp () {

		return __InterlockedIncrement (&iVglobaltimestamppartial);
	}