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

ObjectVector::ObjectVector (PValue value)
{
  const ObjectVectorValue *v = value.DynCast<const ObjectVectorValue *> ();
  if (v == 0)
    {
      NS_FATAL_ERROR ("Expected value of type ObjectVectorValue.");
    }
  *this = v->Get ();
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

PValue 
ObjectVectorValue::Copy (void) const
{
  return PValue::Create<ObjectVectorValue> (*this);
}
std::string 
ObjectVectorValue::SerializeToString (Ptr<const ParamSpec> spec) const
{
  // XXX
  return "";
}
bool 
ObjectVectorValue::DeserializeFromString (std::string value, Ptr<const ParamSpec> spec)
{
  // XXX ?? Can we implement this correctly ?? I doubt it very much.
  return true;
}

bool 
ObjectVectorParamSpec::Set (ObjectBase * object, PValue value) const
{
  // not allowed.
  return false;
}
bool 
ObjectVectorParamSpec::Get (const ObjectBase * object, PValue value) const
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
bool 
ObjectVectorParamSpec::Check (PValue value) const
{
  const ObjectVectorValue *v = value.DynCast<const ObjectVectorValue *> ();
  return v != 0;
}
PValue 
ObjectVectorParamSpec::GetInitialValue (void) const
{
  return PValue::Create<ObjectVectorValue> ();
}
PValue 
ObjectVectorParamSpec::CreateValue (void) const
{
  return PValue::Create<ObjectVectorValue> ();
}


} // name
