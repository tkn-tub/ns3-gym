#include "object-map.h"
#include <ns3/log.h>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("ObjectMapValue");

ObjectMapValue::ObjectMapValue ()
{}

ObjectMapValue::Iterator
ObjectMapValue::Begin (void) const
{
  NS_LOG_FUNCTION (this);
  return m_objects.begin ();
}
ObjectMapValue::Iterator
ObjectMapValue::End (void) const
{
  NS_LOG_FUNCTION (this);
  return m_objects.end ();
}
uint32_t 
ObjectMapValue::GetN (void) const
{
  NS_LOG_FUNCTION (this);
  return m_objects.size ();
}
Ptr<Object> 
ObjectMapValue::Get (uint32_t i) const
{
  NS_LOG_FUNCTION (this);
  return m_objects.find(i)->second;
}

Ptr<AttributeValue>
ObjectMapValue::Copy (void) const
{
  NS_LOG_FUNCTION (this);
  return ns3::Create<ObjectMapValue> (*this);
}
std::string 
ObjectMapValue::SerializeToString (Ptr<const AttributeChecker> checker) const
{
  NS_LOG_FUNCTION (this);
  std::ostringstream oss;
  for (ObjectMapValue::Iterator it = Begin () ; it != End(); it++ )
  {
    oss << (*it).first << "-" << (*it).second;
    if (it != End () )
    {
      oss << " ";
    }
  }
  return oss.str ();
}

bool 
ObjectMapValue::DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker)
{
  NS_FATAL_ERROR ("cannot deserialize a map of object pointers.");
  return true;
}

bool 
ObjectMapAccessor::Set (ObjectBase * object, const AttributeValue & value) const
{
  // not allowed.
  return false;
}
bool 
ObjectMapAccessor::Get (const ObjectBase * object, AttributeValue &value) const
{
  NS_LOG_FUNCTION (this);
  ObjectMapValue *v = dynamic_cast<ObjectMapValue *> (&value);
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

      std::pair<uint32_t, Ptr<Object> > element = DoGet (object, i);
      NS_LOG_FUNCTION(i << object << element.first << element.second);

      v->m_objects.insert (  DoGet (object, i));
    }

  return true;
}
bool 
ObjectMapAccessor::HasGetter (void) const
{
  return true;
}
bool 
ObjectMapAccessor::HasSetter (void) const
{
  return false;
}

} // name
