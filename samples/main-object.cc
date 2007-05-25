#include "ns3/object.h"

using namespace ns3;

class AnObject : public Object
{
public:
  static const InterfaceId iid;
  AnObject ();
protected:
  virtual void DoDispose (void);
};

const InterfaceId AnObject::iid = MakeInterfaceId ("AnObject", Object::iid);

AnObject::AnObject ()
{
  // enable our interface
  SetInterfaceId (AnObject::iid);
}
void
AnObject::DoDispose (void)
{
  // Do your work here.
  // chain up
  Object::DoDispose ();
}

class AnotherObject : public Object
{
public:
  static const InterfaceId iid;
  AnotherObject (int a);
private:
  virtual void DoDispose (void);
};

const InterfaceId AnotherObject::iid = MakeInterfaceId ("AnotherObject", Object::iid);

AnotherObject::AnotherObject (int a)
{
  // enable our interface
  SetInterfaceId (AnotherObject::iid);
}
void
AnotherObject::DoDispose (void)
{
  // Do your work here.
  // chain up
  Object::DoDispose ();
}



class YetAnotherObject : public Object
{
public:
  static const InterfaceId iid;
  YetAnotherObject (int a);
private:
  virtual void DoDispose (void);
};

const InterfaceId YetAnotherObject::iid = MakeInterfaceId ("YetAnotherObject", Object::iid);

YetAnotherObject::YetAnotherObject (int a)
{
  // enable our interface
  SetInterfaceId (YetAnotherObject::iid);
  // aggregated directly to another object.
  AddInterface (MakeNewObject<AnObject> ());
}
void
YetAnotherObject::DoDispose (void)
{
  // Do your work here.
  // chain up
  Object::DoDispose ();
}



int main (int argc, char *argv[])
{
  Ptr<Object> p;
  Ptr<AnObject> anObject;
  Ptr<AnotherObject> anotherObject;
  Ptr<YetAnotherObject> yetAnotherObject;

  p = MakeNewObject<AnObject> ();
  // p gives you access to AnObject's interface
  anObject = p->QueryInterface<AnObject> (AnObject::iid);
  NS_ASSERT (anObject != 0);
  // p does not give you access to AnotherObject's interface
  anotherObject = p->QueryInterface<AnotherObject> (AnotherObject::iid);
  NS_ASSERT (anotherObject == 0);

  anotherObject = MakeNewObject<AnotherObject> (1);
  // AnotherObject does not give you access to AnObject's interface
  anObject = anotherObject->QueryInterface<AnObject> (AnObject::iid);
  NS_ASSERT (anObject == 0);

  // aggregate the two objects
  p->AddInterface (anotherObject);
  // p gives you access to AnObject's interface
  anObject = p->QueryInterface<AnObject> (AnObject::iid);
  NS_ASSERT (anObject != 0);
  // p gives you access to AnotherObject's interface
  anotherObject = p->QueryInterface<AnotherObject> (AnotherObject::iid);
  NS_ASSERT (anotherObject != 0);


  yetAnotherObject = MakeNewObject<YetAnotherObject> (2);
  // gives you acess to AnObject interface too.
  anObject = yetAnotherObject->QueryInterface<AnObject> (AnObject::iid);
  NS_ASSERT (anObject != 0);


  return 0;
}

