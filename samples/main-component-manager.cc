#include "ns3/object.h"
#include "ns3/component-manager.h"

using namespace ns3;

class AnObject : public Object
{
public:
  static const InterfaceId iid;
  static const ClassId cid;
  AnObject (int a, double b);
protected:
  virtual void DoDispose (void);
};

const InterfaceId AnObject::iid = MakeInterfaceId ("AnObject", Object::iid);
const ClassId AnObject::cid = MakeClassId<AnObject, int, double> ("AnObject", AnObject::iid);

AnObject::AnObject (int a, double b)
{}
void
AnObject::DoDispose (void)
{
  // Do your work here.
  // chain up
  Object::DoDispose ();
}


int main (int argc, char *argv[])
{
  Ptr<AnObject> anObject = ComponentManager::Create<AnObject,int,double> (AnObject::cid, AnObject::iid, 10, 20.0);
  NS_ASSERT (anObject != 0);
  return 0;
}
