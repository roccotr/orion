#include "CoreHutl.hpp"
#include "CoreXutl.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fts.h>
#include <dlfcn.h>
#include <string>
#include <limits.h>
#include <unistd.h>
using namespace orion;


/*MATCH*/
	bool CoreHutl::match (char *bString, char *bWild) {
		char *bVcp;
		char *bVmp;
	
		bVcp = NULL;
		bVmp = NULL;
		while ((*bString) && (*bWild != '*')) {
			if ((toupper (*bWild) != toupper (*bString)) && (*bWild != '?')) return false;
			bWild++;
			bString++;
		}
		while (*bString) {
			if (*bWild == '*') {
				if (!*++bWild) return true;
				bVmp = bWild;
				bVcp = bString+1;
			} else if ((toupper (*bWild) == toupper (*bString)) || (*bWild == '?')) {
				bWild++;
				bString++;
			} else {
				bWild = bVmp;
				bString = bVcp++;
			}
		}
		while (*bWild == '*') bWild++;
		return (*bWild == 0);
	}

	/*REAd DiRectory*/	
	void CoreHutl::readr (	AnsiString sPath, AnsiString sMask,
													AnsiString** sFilenames, int& iCount,
													AnsiString sFoldermask) {
    int iV;
		char** bVpathargv;		
    char* bVtemp;
    TList* lVlist;
		FTS* cVfts;
		FTSENT* cVfindnext;
				
    lVlist = new TList ();
		bVpathargv = new char* [2];
		bVpathargv [0] = new char [sPath.Length () + 1];
		bVpathargv [1] = NULL;
		strcpy (bVpathargv [0], sPath.c_str ());
		
		cVfts = fts_open (bVpathargv, FTS_NOCHDIR|FTS_PHYSICAL|FTS_NOSTAT, NULL);
		if (cVfts != NULL) {
			do {
				cVfindnext = fts_read (cVfts);
				if (cVfindnext != NULL) {
					if (cVfindnext->fts_level == 1 && cVfindnext->fts_info & FTS_F) {
						if (CoreHutl::match (cVfindnext->fts_name, sMask.c_str ())) {
							bVtemp = new char [strlen (cVfindnext->fts_name) + 1];
							strcpy (bVtemp, cVfindnext->fts_name);
							lVlist->Add(bVtemp);							
					  }
						fts_set (cVfts, cVfindnext, FTS_SKIP);
					}
				}
			} while (cVfindnext != NULL);		
		}		
		if (cVfts != NULL) fts_close (cVfts);
		if (sFoldermask != NULL) {
			cVfts = fts_open (bVpathargv, FTS_NOCHDIR, NULL);
			if (cVfts != NULL) {
				do {
					cVfindnext = fts_read (cVfts);
					if (cVfindnext != NULL) {
						if (cVfindnext->fts_level == 1 && cVfindnext->fts_info & FTS_D) {
							if (CoreHutl::match (cVfindnext->fts_name, sFoldermask.c_str ())) {
								bVtemp = new char [strlen (cVfindnext->fts_name) + 1];
								strcpy (bVtemp, cVfindnext->fts_name);
								lVlist->Add(bVtemp);							
							}
							fts_set (cVfts, cVfindnext, FTS_SKIP);
						}
					}
				} while (cVfindnext != NULL);		
			}		
			if (cVfts != NULL) fts_close (cVfts);
			
			
		}
		
		
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
		delete [] bVpathargv [0];
		bVpathargv [0] = NULL;
		delete [] bVpathargv;
		bVpathargv = NULL;
    delete lVlist;
    lVlist = NULL;		
	}
	
	/*APPlication NaMe*/

	AnsiString __fastcall CoreHutl::appnm (void* hInstance) {
		char bVmodulename [PATH_MAX + 1];
		bVmodulename [PATH_MAX] = (int) 0;
		ssize_t count = readlink( "/proc/self/exe", bVmodulename, PATH_MAX+1 );
		AnsiString sVreturn( bVmodulename, (count > 0) ? count+1 : 0 );
		//printf("PATH APPLICATION: %s\n",sVreturn.c_str());
		return sVreturn;
	}

	int64vcl CoreHutl::gettp(){
		return 0;
	}
	
