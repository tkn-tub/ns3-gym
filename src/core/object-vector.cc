#include "object-vector.h"

namespace ns3 {

ObjectVector::ObjectVector ()
{}

ObjectVector::Iterator 
ObjectVector::Begin (void) const
{
  return m_objects.begin ();
}
ObjectVector::Iterator 
ObjectVector::End (void) const
{
  return m_objects.end ();
}
uint32_t 
ObjectVector::GetN (void) const
{
  return m_objects.size ();
}
Ptr<Object> 
ObjectVector::Get (uint32_t i) const
{
  return m_objects[i];
}

ObjectVector::ObjectVector (Attribute value)
{
  const ObjectVectorValue *v = value.DynCast<const ObjectVectorValue *> ();
  if (v == 0)
    {
      NS_FATAL_ERROR ("Expected value of type ObjectVectorValue.");
    }
  *this = v->Get ();
}

ObjectVector::operator Attribute () const
{
  return Attribute (ns3::Create<ObjectVectorValue> ());
}

ObjectVectorValue::ObjectVectorValue ()
  : m_vector ()
{}

ObjectVectorValue::ObjectVectorValue (const ObjectVector &vector)
  : m_vector (vector)
{}

ObjectVector 
ObjectVectorValue::Get (void) const
{
  return m_vector;
}

Attribute 
ObjectVectorValue::Copy (void) const
{
  return Attribute (ns3::Create<ObjectVectorValue> (*this));
}
std::string 
ObjectVectorValue::SerializeToString (Ptr<const AttributeChecker> checker) const
{
  // XXX
  return "";
}
bool 
ObjectVectorValue::DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker)
{
  // XXX ?? Can we implement this correctly ?? I doubt it very much.
  return true;
}

bool 
ObjectVectorAccessor::Set (ObjectBase * object, Attribute value) const
{
  // not allowed.
  return false;
}
bool 
ObjectVectorAccessor::Get (const ObjectBase * object, Attribute value) const
{
  ObjectVectorValue *v = value.DynCast<ObjectVectorValue *> ();
  if (v == 0)
    {
      return false;
    }
  v->m_vector.m_objects.clear ();
  uint32_t n;
  bool ok = DoGetN (object, &n);
  if (!ok)
    {
      return false;
    }
  for (uint32_t i = 0; i < n; i++)
    {
      Ptr<Object> o = DoGet (object, i);
      v->m_vector.m_objects.push_back (o);
    }
  return true;
}

} // name
