#include "CoreTlst.hpp"
#include "CoreVcl_.hpp"
#include "CoreVutl.hpp"
#include <string.h>
#include <stdio.h>

#define iCtlstcapacityframe 4

using namespace orion;

  CoreTlst::CoreTlst () {
    Count = 0;
    Items = NULL;
    iVallocated = Capacity = 0;
  }

  CoreTlst::~CoreTlst () {
    Clear ();
  }

	/*GROW*/
  void CoreTlst::grow_ (int iNewsize) { 
		int iVnewallocated;
    
    iVnewallocated = iVallocated;
    while (iNewsize > iVnewallocated) iVnewallocated += iCtlstcapacityframe;
    iVnewallocated = iVnewallocated > Capacity ? iVnewallocated : Capacity;
    if (iVnewallocated != iVallocated) {
      iVallocated = Capacity = iVnewallocated;
			Items = (void**) CoreVutl::reall (Items, (sizeof (void*)) * iVallocated); 
    }
    Count = iNewsize;
  }

	/*ADD*/
  int CoreTlst::Add (void* vItem, int iItem) {
    int iVtemp;
		int iVprevious;
		int iVelementstocopy;

		iVprevious = Count;
    iVtemp = Count + 1;
    grow_ (iVtemp);
		if (iItem < 0 || iItem == iVprevious) Items [iVtemp - 1] = vItem;
		else {			
			iVelementstocopy = Count - iItem -1;
			if (iVelementstocopy > 0)
				memmove (&Items [iItem + 1], &Items [iItem], (sizeof (void*)) * iVelementstocopy);
			Items [iItem] = vItem;
		}
    return 1;
  }

	/*CLEAR*/
  void CoreTlst::Clear () {		
		if (Items != NULL) free (Items);

    Items = NULL;
    iVallocated = Count = 0;
  }

	/*DELETE*/
  void CoreTlst::Delete (int iItem) {
		int iVelementstocopy;

    if (iItem >= 0 && iItem < Count) {
      Count--;

			iVelementstocopy = Count - iItem;
			if (iVelementstocopy > 0)
				memmove (&Items [iItem], &Items [iItem + 1], (sizeof (void*)) * iVelementstocopy);
    }
    if (Count == 0) Clear ();
  }

	void*& CoreTlst::operator[] (unsigned iV) {
		#ifdef VSTUDIODEBUG
			if (iV >= unsigned (Count)) {
				OutputDebugString("CoreTlst error\n");
			}
		#endif
		return Items [iV];
	}

	/*ADD*/
  int CoreTls2::Add (void* vItem, int iItem) {
		int iVreturn;
		
		Items.Capacity = Capacity;
		iVreturn = Items.Add (vItem, iItem);
		Count = Items.Count;
		return iVreturn;
	}

	/*CLEAR*/
  void CoreTls2::Clear () {
		Items.Clear ();
		Count = Items.Count;
	}

	/*DELETE*/
	void CoreTls2::Delete (int iItem) {
		Items.Delete (iItem);
		Count = Items.Count;
	}
