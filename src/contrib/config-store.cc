#include "config-store.h"
#include "ns3/string.h"
#include "ns3/config.h"
#include "ns3/object-vector.h"
#include "ns3/pointer.h"
#include "ns3/log.h"
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>

NS_LOG_COMPONENT_DEFINE ("ConfigStore");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (ConfigStore);

TypeId 
ConfigStore::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ConfigStore")
    .SetParent<ObjectBase> ()
    .AddAttribute ("LoadFilename", 
		   "The file where the configuration should be loaded from.",
		   String (""),
		   MakeStringAccessor (&ConfigStore::m_loadFilename),
		   MakeStringChecker ())
    .AddAttribute ("StoreFilename", 
		   "The file where the configuration should be stored to.",
		   String (""),
		   MakeStringAccessor (&ConfigStore::m_storeFilename),
		   MakeStringChecker ())
    ;
  return tid;
}
TypeId 
ConfigStore::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}


ConfigStore::ConfigStore ()
{
  ObjectBase::ConstructSelf (AttributeList ());
}

void 
ConfigStore::LoadFrom (std::string filename)
{
  std::ifstream is;
  is.open (filename.c_str (), std::ios::in);
  std::string path, value;
  while (is.good())
    {
      is >> path >> value;
      NS_LOG_DEBUG (path << "=" << value);
      Config::Set (path, String (value));
    }
}
void 
ConfigStore::StoreTo (std::string filename)
{
  std::ofstream os;
  os.open (filename.c_str (), std::ios::out);
  for (uint32_t i = 0; i < Config::GetRootNamespaceObjectN (); ++i)
    {
      Ptr<Object> object = Config::GetRootNamespaceObject (i);
      Store (os, object);
    }
  os.close ();
  NS_ASSERT (m_currentPath.empty ());
  NS_ASSERT (m_examined.empty ());
  exit (0);
}

bool
ConfigStore::IsExamined (Ptr<const Object> object)
{
  for (uint32_t i = 0; i < m_examined.size (); ++i)
    {
      if (object == m_examined[i])
	{
	  return true;
	}
    }
  return false;
}

std::string
ConfigStore::GetCurrentPath (std::string attr) const
{
  std::ostringstream oss;
  for (uint32_t i = 0; i < m_currentPath.size (); ++i)
    {
      oss << "/" << m_currentPath[i];
    }
  oss << "/" << attr;
  return oss.str ();
}

void
ConfigStore::Store (std::ostream &os, Ptr<const Object> object)
{
  if (IsExamined (object))
    {
      return;
    }
  TypeId tid = object->GetInstanceTypeId ();
  m_currentPath.push_back ("$" + tid.GetName ());
  NS_LOG_DEBUG ("store " << tid.GetName ());
  for (uint32_t i = 0; i < tid.GetAttributeN (); ++i)
    {
      Ptr<const AttributeChecker> checker = tid.GetAttributeChecker (i);
      const PointerChecker *ptrChecker = dynamic_cast<const PointerChecker *> (PeekPointer (checker));
      if (ptrChecker != 0)
	{
	  NS_LOG_DEBUG ("pointer attribute " << tid.GetAttributeName (i));
	  Ptr<const Object> tmp = Pointer (object->GetAttribute (tid.GetAttributeName (i)));
	  if (tmp != 0)
	    {
	      m_currentPath.push_back (tid.GetAttributeName (i));
	      m_examined.push_back (object);
	      Store (os, tmp);
	      m_examined.pop_back ();
	      m_currentPath.pop_back ();
	    }
	  continue;
	}
      // attempt to cast to an object vector.
      const ObjectVectorChecker *vectorChecker = dynamic_cast<const ObjectVectorChecker *> (PeekPointer (checker));
      if (vectorChecker != 0)
	{
	  NS_LOG_DEBUG ("vector attribute " << tid.GetAttributeName (i));
	  ObjectVector vector = object->GetAttribute (tid.GetAttributeName (i));
	  for (uint32_t j = 0; j < vector.GetN (); ++j)
	    {
	      NS_LOG_DEBUG ("vector attribute item " << j);
	      Ptr<const Object> tmp = vector.Get (j);
	      std::ostringstream oss;
	      oss << tid.GetAttributeName (i) << "/" << j;
	      m_currentPath.push_back (oss.str ());
	      m_examined.push_back (object);
	      Store (os, tmp);
	      m_examined.pop_back ();
	      m_currentPath.pop_back ();
	    }
	  continue;
	}
      uint32_t flags = tid.GetAttributeFlags (i);
      Ptr<const AttributeAccessor> accessor = tid.GetAttributeAccessor (i);
      if ((flags & TypeId::ATTR_GET) && accessor->HasGetter () &&
	  (flags & TypeId::ATTR_SET) && accessor->HasSetter ())
	{
	  std::string value = object->GetAttributeAsString (tid.GetAttributeName (i));
	  os << GetCurrentPath (tid.GetAttributeName (i)) << " " << value << std::endl;
	}
      else
	{
	  NS_LOG_DEBUG ("could not store " << tid.GetAttributeName (i));
	}
    }
  Object::AggregateIterator iter = object->GetAggregateIterator ();
  bool recursiveAggregate = false;
  while (iter.HasNext ())
    {
      Ptr<const Object> tmp = iter.Next ();
      if (IsExamined (tmp))
	{
	  recursiveAggregate = true;
	}
    }
  if (!recursiveAggregate)
    {
      iter = object->GetAggregateIterator ();
      while (iter.HasNext ())
	{
	  Ptr<const Object> tmp = iter.Next ();
	  m_examined.push_back (object);
	  Store (os, tmp);
	  m_examined.pop_back ();
	}
    }
  m_currentPath.pop_back ();
}


void 
ConfigStore::Configure (void)
{
  if (m_loadFilename != "")
    {
      LoadFrom (m_loadFilename);
    }
  if (m_storeFilename != "")
    {
      StoreTo (m_storeFilename);
    }
}

} // namespace ns3
