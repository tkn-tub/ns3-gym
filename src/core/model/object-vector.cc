#include "object-vector.h"

namespace ns3 {

ObjectVectorValue::ObjectVectorValue ()
{}

ObjectVectorValue::Iterator 
ObjectVectorValue::Begin (void) const
{
  return m_objects.begin ();
}
ObjectVectorValue::Iterator 
ObjectVectorValue::End (void) const
{
  return m_objects.end ();
}
uint32_t 
ObjectVectorValue::GetN (void) const
{
  return m_objects.size ();
}
Ptr<Object> 
ObjectVectorValue::Get (uint32_t i) const
{
  return m_objects[i];
}

Ptr<AttributeValue>
ObjectVectorValue::Copy (void) const
{
  return ns3::Create<ObjectVectorValue> (*this);
}
std::string 
ObjectVectorValue::SerializeToString (Ptr<const AttributeChecker> checker) const
{
  std::ostringstream oss;
  for (uint32_t i = 0; i < m_objects.size (); ++i)
    {
      oss << m_objects[i];
      if (i != m_objects.size () - 1)
	{
	  oss << " ";
	}
    }
  return oss.str ();
}
bool 
ObjectVectorValue::DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker)
{
  NS_FATAL_ERROR ("cannot deserialize a vector of object pointers.");
  return true;
}

bool 
ObjectVectorAccessor::Set (ObjectBase * object, const AttributeValue & value) const
{
  // not allowed.
  return false;
}
bool 
ObjectVectorAccessor::Get (const ObjectBase * object, AttributeValue &value) const
{
  ObjectVectorValue *v = dynamic_cast<ObjectVectorValue *> (&value);
  if (v == 0)
    {
      return false;
    }
  v->m_objects.clear ();
  uint32_t n;
  bool ok = DoGetN (object, &n);
  if (!ok)
    {
      return false;
    }
  for (uint32_t i = 0; i < n; i++)
    {
      Ptr<Object> o = DoGet (object, i);
      v->m_objects.push_back (o);
    }
  return true;
}
bool 
ObjectVectorAccessor::HasGetter (void) const
{
  return true;
}
bool 
ObjectVectorAccessor::HasSetter (void) const
{
  return false;
}

} // name
