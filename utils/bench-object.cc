#include <vector>
#include <stdlib.h>
#include "ns3/interface-object.h"

using namespace ns3;

class BaseA : public ns3::InterfaceObject
{
public:
  static const ns3::MyInterfaceId iid;
  BaseA ()
  {
    SetInterfaceId (BaseA::iid);
  }
  virtual void Dispose (void) {}
};

const ns3::MyInterfaceId BaseA::iid = 
ns3::MakeInterfaceId ("BaseABench", InterfaceObject::iid);



int main (int argc, char *argv[])
{
  int nobjects = atoi (argv[1]);
  int nswaps = atoi (argv[2]);

  std::vector< Ptr<BaseA> > objlist;

  for (int i = 0; i < nobjects; ++i)
    objlist.push_back (MakeNewObject<BaseA> ());

  for (int swapCounter = nswaps; swapCounter; --swapCounter)
    {
      int x1 = swapCounter % nobjects;
      int x2 = (swapCounter+1) % nobjects;
      Ptr<BaseA> obj1 = objlist[x1];
      Ptr<BaseA> obj2 = objlist[x2];
      objlist[x2] = obj1;
      objlist[x1] = obj2;
    }
}

