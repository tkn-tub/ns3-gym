#include "attribute-iterator.h"
#include "ns3/config.h"
#include "ns3/log.h"
#include "ns3/pointer.h"
#include "ns3/object-vector.h"
#include "ns3/string.h"
#include <fstream>


NS_LOG_COMPONENT_DEFINE ("AttributeIterator");

namespace ns3 {


AttributeIterator::AttributeIterator ()
{}

AttributeIterator::~AttributeIterator ()
{}

void 
AttributeIterator::Iterate (void)
{
  for (uint32_t i = 0; i < Config::GetRootNamespaceObjectN (); ++i)
    {
      Ptr<Object> object = Config::GetRootNamespaceObject (i);
      DoIterate (object);
    }
  NS_ASSERT (m_currentPath.empty ());
  NS_ASSERT (m_examined.empty ());
}

bool
AttributeIterator::IsExamined (Ptr<const Object> object)
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
AttributeIterator::GetCurrentPath (std::string attr) const
{
  std::ostringstream oss;
  for (uint32_t i = 0; i < m_currentPath.size (); ++i)
    {
      oss << "/" << m_currentPath[i];
    }
  if (attr != "")
    {
      oss << "/" << attr;
    }
  return oss.str ();
}

void 
AttributeIterator::Push (std::string name)
{
  m_currentPath.push_back (name);
  DoPush (name, GetCurrentPath (""));
}
void 
AttributeIterator::Pop (void)
{
  DoPop ();
  m_currentPath.pop_back ();
}
void
AttributeIterator::Visit (Ptr<Object> object, std::string name)
{
  DoVisit (object, name, GetCurrentPath (name));
}


void
AttributeIterator::DoIterate (Ptr<Object> object)
{
  if (IsExamined (object))
    {
      return;
    }
  TypeId tid = object->GetInstanceTypeId ();
  Push ("$" + tid.GetName ());
  NS_LOG_DEBUG ("store " << tid.GetName ());
  for (uint32_t i = 0; i < tid.GetAttributeN (); ++i)
    {
      Ptr<const AttributeChecker> checker = tid.GetAttributeChecker (i);
      const PointerChecker *ptrChecker = dynamic_cast<const PointerChecker *> (PeekPointer (checker));
      if (ptrChecker != 0)
	{
	  NS_LOG_DEBUG ("pointer attribute " << tid.GetAttributeName (i));
	  PointerValue ptr;
	  object->GetAttribute (tid.GetAttributeName (i), ptr);
	  Ptr<Object> tmp = ptr.Get<Object> ();
	  if (tmp != 0)
	    {
	      Push (tid.GetAttributeName (i));
	      m_examined.push_back (object);
	      DoIterate (tmp);
	      m_examined.pop_back ();
	      Pop ();
	    }
	  continue;
	}
      // attempt to cast to an object vector.
      const ObjectVectorChecker *vectorChecker = dynamic_cast<const ObjectVectorChecker *> (PeekPointer (checker));
      if (vectorChecker != 0)
	{
	  NS_LOG_DEBUG ("vector attribute " << tid.GetAttributeName (i));
	  ObjectVectorValue vector;
	  object->GetAttribute (tid.GetAttributeName (i), vector);
	  Push (tid.GetAttributeName (i));
	  for (uint32_t j = 0; j < vector.GetN (); ++j)
	    {
	      NS_LOG_DEBUG ("vector attribute item " << j);
	      Ptr<Object> tmp = vector.Get (j);
	      std::ostringstream oss;
	      oss << j;
	      Push (oss.str ());
	      m_examined.push_back (object);
	      DoIterate (tmp);
	      m_examined.pop_back ();
	      Pop ();
	    }
	  Pop ();
	  continue;
	}
      uint32_t flags = tid.GetAttributeFlags (i);
      Ptr<const AttributeAccessor> accessor = tid.GetAttributeAccessor (i);
      if ((flags & TypeId::ATTR_GET) && accessor->HasGetter () &&
	  (flags & TypeId::ATTR_SET) && accessor->HasSetter ())
	{
	  Visit (object, tid.GetAttributeName (i));
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
	  Ptr<Object> tmp = const_cast<Object *> (PeekPointer (iter.Next ()));
	  m_examined.push_back (object);
	  DoIterate (tmp);
	  m_examined.pop_back ();
	}
    }
  Pop ();
}



TextFileAttributeIterator::TextFileAttributeIterator (std::ostream &os)
  : m_os (os)
{}
void 
TextFileAttributeIterator::DoVisit (Ptr<Object> object, std::string name, std::string path)
{
  StringValue str;
  object->GetAttribute (name, str);
  m_os << path << " " << str.Get () << std::endl;
}
void 
TextFileAttributeIterator::DoPush (std::string name, std::string path)
{}
void 
TextFileAttributeIterator::DoPop (void)
{}

void 
TextFileAttributeIterator::Save (void)
{
  Iterate ();
}



} // namespace ns3
