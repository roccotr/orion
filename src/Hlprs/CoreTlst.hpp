#ifndef CORETLST
#define CORETLST

namespace orion {

class CoreTlst {
private:
  int iVallocated;
  void grow_ (int iNewsize);
public:
  int Capacity;
  int Count;
  void** Items;

  CoreTlst ();
  ~CoreTlst ();

  int Add (void* vItem, int iItem = -1);
  void Clear ();
  void Delete (int iItem);
  void*& operator[] (unsigned iV); 
};

class CoreTls2 {
public:
  int Capacity;
  int Count;
  CoreTlst Items;

	CoreTls2 () {Count = Items.Count; Capacity = Items.Capacity;}
  int Add (void* vItem, int iItem = -1);
  void Clear ();
  void Delete (int iItem); 
};

};

#endif