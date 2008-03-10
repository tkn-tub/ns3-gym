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

std::ostream & operator << (std::ostream &os, const ObjectFactory &factory)
{
  // XXX
  return os;
}
std::istream & operator >> (std::istream &is, ObjectFactory &factory)
{
  // XXX
  return is;
}


ATTRIBUTE_HELPER_CPP (ObjectFactory);

} // namespace ns3
