#ifndef CORECHLS
#define CORECHLS
#include "CoreList.hpp"


namespace orion {

template<typename T>
struct CoreChls {

public:
  CoreList<T> cVlist;
  CoreChls* cVnext;
  CoreChls* cVprev;

  /*INITIALIZE*/
  CoreChls () {
    cVnext = cVprev = NULL;
  }


};

};


#endif