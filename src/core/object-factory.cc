#include "object-factory.h"
#include <sstream>

namespace ns3 {

ObjectFactory::ObjectFactory ()
{}

void 
ObjectFactory::SetTypeId (TypeId tid)
{
  m_tid = tid;
}
void 
ObjectFactory::SetTypeId (std::string tid)
{
  m_tid = TypeId::LookupByName (tid);
}
void 
ObjectFactory::SetTypeId (const char *tid)
{
  m_tid = TypeId::LookupByName (tid);
}
void 
ObjectFactory::Set (std::string name, Attribute value)
{
  if (name == "")
    {
      return;
    }
  if (!m_parameters.SetWithTid (m_tid, name, value))
    {
      NS_FATAL_ERROR ("Error setting attribute name="<<name);
    }
}

TypeId 
ObjectFactory::GetTypeId (void) const
{
  return m_tid;
}

Ptr<Object> 
ObjectFactory::Create (void) const
{
  Ptr<Object> object = m_tid.CreateObject (m_parameters);
  return object;
}

} // namespace ns3
