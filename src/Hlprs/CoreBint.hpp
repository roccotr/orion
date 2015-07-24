#ifndef COREBINT
#define COREBINT
#include <vector>
#include <cstdlib>
#include <glog/logging.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include "CoreVcl_.hpp"

namespace orion {

using namespace std;

class CoreBint {

	static const int iCbaseorder;
	static const int64vcl iCbase;

  vector<int64vcl> cVa;
  bool bVoverflow;

	void trim_ ();

public:
  CoreBint();
  ~CoreBint ();
  CoreBint (AnsiString &sV);

  void operator=(const CoreBint &cA);
  void operator=(const AnsiString& sA);
  CoreBint operator+(const CoreBint &cA) const;
  void operator+=(const CoreBint &cA);
  CoreBint operator-(const CoreBint &cA) const;
  CoreBint operator/(int64vcl cA) const;
  void operator/=(int64vcl cA);
  CoreBint operator*(int64vcl cA) const;
  void operator*=(int64vcl cA);
	void ans2b (AnsiString sAnsistring);  
	AnsiString bin2a ();  
	AnsiString write ();
	void read_(AnsiString sAnsistring);
	bool isovf ();

	AnsiString debug ();

};

};

#endif
