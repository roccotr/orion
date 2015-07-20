#ifndef COREANSX
#define COREANSX

#define iCansialloc 8
#define iCansiallocmask iCansialloc-1
#include <string>

namespace orion {

class CoreAnsx
{
private:
	char bVfence;
	char bVguard;
  char* bVcontent;
  int iVlength;
  int iVallocatedlength;

public:
  static int alloc (int iLength);
private:
	void start (const char* bString, int iOffset, int iLength);
  void start (const CoreAnsx& cString, int iOffset);

public:
  CoreAnsx ();
  CoreAnsx (const char* bString, int iLength = -1);
  CoreAnsx (const CoreAnsx& cString);
  CoreAnsx (std::string cString);
  CoreAnsx (char bValue, bool bForcelength = false);
  CoreAnsx (double dValue);
	CoreAnsx (long int iValue);
	CoreAnsx (long unsigned int iValue);
	CoreAnsx (int iValue);

  ~CoreAnsx ();

  CoreAnsx& operator =(const CoreAnsx& cString);
  CoreAnsx& operator +=(const CoreAnsx& cString);
	CoreAnsx operator +(const CoreAnsx& cString) const;
  bool operator ==(const CoreAnsx& sString) const;
  bool operator !=(const CoreAnsx& sString) const;
  bool operator >(const CoreAnsx& sString) const;
  bool operator <(const CoreAnsx& sString) const;
  bool operator >=(const CoreAnsx& sString) const;
  bool operator <=(const CoreAnsx& sString) const;
  char* c_str () const;
  int strcmp (const CoreAnsx& sString, int iLength = 0) const;
	std::string to_string () const;
	int  Length()  const;
	CoreAnsx UpperCase () const;
  CoreAnsx LowerCase () const;
  CoreAnsx SubString(int iIndex, int iCount) const;
  CoreAnsx Trim () const;
  double ToDouble() const;
  int ToInt() const;	
	int Pos(const CoreAnsx& sString) const;
	int AnsiPos(const CoreAnsx& subStr) const;
	CoreAnsx& sprintf(const char* cFormat, double d);
  static CoreAnsx _FloatToStrF (	double dValue, int iDummy,
																	int iPrecision, int iDigits);
	static std::string to_string (char* sData, int iData);
	static CoreAnsx debug (char* bString, int iLength, bool bAscii = true);
	CoreAnsx debug (bool bAscii = true);
	CoreAnsx prntb ();
};

};

#endif