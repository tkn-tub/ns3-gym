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
  m_parameters.SetWithTid (m_tid, name, value);
}

void 
ObjectFactory::Set (std::string name, std::string value)
{
  m_parameters.SetWithTid (m_tid, name, value);
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
