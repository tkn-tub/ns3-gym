/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "config.h"
#include "singleton.h"
#include "object.h"
#include "global-value.h"
#include "object-vector.h"
#include "names.h"
#include "pointer.h"
#include "log.h"

#include "test.h"
#include "integer.h"
#include "traced-value.h"
#include "trace-source-accessor.h"
#include "callback.h"

#include <sstream>

NS_LOG_COMPONENT_DEFINE ("Config");

namespace ns3 {

namespace Config {

MatchContainer::MatchContainer ()
{}
MatchContainer::MatchContainer (const std::vector<Ptr<Object> > &objects, 
                                const std::vector<std::string> &contexts,
                                std::string path)
  : m_objects (objects),
    m_contexts (contexts),
    m_path (path)
{}
MatchContainer::Iterator 
MatchContainer::Begin (void) const
{
  return m_objects.begin ();
}
MatchContainer::Iterator 
MatchContainer::End (void) const
{
  return m_objects.end ();
}
uint32_t 
MatchContainer::GetN (void) const
{
  return m_objects.size ();
}
Ptr<Object> 
MatchContainer::Get (uint32_t i) const
{
  return m_objects[i];
}
std::string 
MatchContainer::GetMatchedPath (uint32_t i) const
{
  return m_contexts[i];
}
std::string 
MatchContainer::GetPath (void) const
{
  return m_path;
}

void 
MatchContainer::Set (std::string name, const AttributeValue &value)
{
  for (Iterator tmp = Begin (); tmp != End (); ++tmp)
    {
      Ptr<Object> object = *tmp;
      object->SetAttribute (name, value);
    }
}
void 
MatchContainer::Connect (std::string name, const CallbackBase &cb)
{
  NS_ASSERT (m_objects.size () == m_contexts.size ());
  for (uint32_t i = 0; i < m_objects.size (); ++i)
    {
      Ptr<Object> object = m_objects[i];
      std::string ctx = m_contexts[i] + name;
      object->TraceConnect (name, ctx, cb);
    }
}
void 
MatchContainer::ConnectWithoutContext (std::string name, const CallbackBase &cb)
{
  for (Iterator tmp = Begin (); tmp != End (); ++tmp)
    {
      Ptr<Object> object = *tmp;
      object->TraceConnectWithoutContext (name, cb);
    }
}
void 
MatchContainer::Disconnect (std::string name, const CallbackBase &cb)
{
  NS_ASSERT (m_objects.size () == m_contexts.size ());
  for (uint32_t i = 0; i < m_objects.size (); ++i)
    {
      Ptr<Object> object = m_objects[i];
      std::string ctx = m_contexts[i] + name;
      object->TraceDisconnect (name, ctx, cb);
    }
}
void 
MatchContainer::DisconnectWithoutContext (std::string name, const CallbackBase &cb)
{
  for (Iterator tmp = Begin (); tmp != End (); ++tmp)
    {
      Ptr<Object> object = *tmp;
      object->TraceDisconnectWithoutContext (name, cb);
    }
}

} // namespace Config

class ArrayMatcher
{
public:
  ArrayMatcher (std::string element);
  bool Matches (uint32_t i) const;
private:
  bool StringToUint32 (std::string str, uint32_t *value) const;
  std::string m_element;
};


ArrayMatcher::ArrayMatcher (std::string element)
  : m_element (element)
{}
bool 
ArrayMatcher::Matches (uint32_t i) const
{
  if (m_element == "*")
    {
      NS_LOG_DEBUG ("Array "<<i<<" matches *");
      return true;
    }
  std::string::size_type tmp;
  tmp = m_element.find ("|");
  if (tmp != std::string::npos)
    {
      std::string left = m_element.substr (0, tmp-0);
      std::string right = m_element.substr (tmp+1, m_element.size () - (tmp + 1));
      ArrayMatcher matcher = ArrayMatcher (left);
      if (matcher.Matches (i))
	{
	  NS_LOG_DEBUG ("Array "<<i<<" matches "<<left);
	  return true;
	}
      matcher = ArrayMatcher (right);
      if (matcher.Matches (i))
	{
	  NS_LOG_DEBUG ("Array "<<i<<" matches "<<right);
	  return true;
	}
      NS_LOG_DEBUG ("Array "<<i<<" does not match "<<m_element);
      return false;
    }
  std::string::size_type leftBracket = m_element.find ("[");
  std::string::size_type rightBracket = m_element.find ("]");
  std::string::size_type dash = m_element.find ("-");
  if (leftBracket == 0 && rightBracket == m_element.size () - 1 &&
      dash > leftBracket && dash < rightBracket)
    {
      std::string lowerBound = m_element.substr (leftBracket + 1, dash - (leftBracket + 1));
      std::string upperBound = m_element.substr (dash + 1, rightBracket - (dash + 1));
      uint32_t min;
      uint32_t max;
      if (StringToUint32 (lowerBound, &min) && 
	  StringToUint32 (upperBound, &max) &&
	  i >= min && i <= max)
        {
	  NS_LOG_DEBUG ("Array "<<i<<" matches "<<m_element);
          return true;
        }
      else
	{
	  NS_LOG_DEBUG ("Array "<<i<<" does not "<<m_element);
	  return false;
	}
    }
  uint32_t value;
  if (StringToUint32 (m_element, &value) &&
      i == value)
    {
      NS_LOG_DEBUG ("Array "<<i<<" matches "<<m_element);
      return true;
    }
  NS_LOG_DEBUG ("Array "<<i<<" does not match "<<m_element);
  return false;
}

bool
ArrayMatcher::StringToUint32 (std::string str, uint32_t *value) const
{
  std::istringstream iss;
  iss.str (str);
  iss >> (*value);
  return !iss.bad () && !iss.fail ();
}


class Resolver
{
public:
  Resolver (std::string path);
  virtual ~Resolver ();

  void Resolve (Ptr<Object> root);
private:
  void Canonicalize (void);
  void DoResolve (std::string path, Ptr<Object> root);
  void DoArrayResolve (std::string path, const ObjectVectorValue &vector);
  void DoResolveOne (Ptr<Object> object);
  std::string GetResolvedPath (void) const;
  virtual void DoOne (Ptr<Object> object, std::string path) = 0;
  std::vector<std::string> m_workStack;
  std::string m_path;
};

Resolver::Resolver (std::string path)
  : m_path (path)
{
  Canonicalize ();
}
Resolver::~Resolver ()
{}
void
Resolver::Canonicalize (void)
{
  // ensure that we start and end with a '/'
  std::string::size_type tmp = m_path.find ("/");
  if (tmp != 0)
    {
      // no slash at start
      m_path = "/" + m_path;
    }
  tmp = m_path.find_last_of ("/");
  if (tmp != (m_path.size () - 1))
    {
      // no slash at end
      m_path = m_path + "/";
    }
}

void 
Resolver::Resolve (Ptr<Object> root)
{
  DoResolve (m_path, root);
}

std::string
Resolver::GetResolvedPath (void) const
{
  std::string fullPath = "/";
  for (std::vector<std::string>::const_iterator i = m_workStack.begin (); i != m_workStack.end (); i++)
    {
      fullPath += *i + "/";
    }
  return fullPath;
}

void 
Resolver::DoResolveOne (Ptr<Object> object)
{
  NS_LOG_DEBUG ("resolved="<<GetResolvedPath ());
  DoOne (object, GetResolvedPath ());
}

void
Resolver::DoResolve (std::string path, Ptr<Object> root)
{
  NS_LOG_FUNCTION (path << root);
  std::string::size_type tmp;
  tmp = path.find ("/");
  NS_ASSERT (tmp == 0);
  std::string::size_type next = path.find ("/", 1);

  if (next == std::string::npos)
    {
      //
      // If root is zero, we're beginning to see if we can use the object name 
      // service to resolve this path.  It is impossible to have a object name 
      // associated with the root of the object name service since that root
      // is not an object.  This path must be referring to something in another
      // namespace and it will have been found already since the name service
      // is always consulted last.
      // 
      if (root)
        {
          DoResolveOne (root);
        }
      return;
    }
  std::string item = path.substr (1, next-1);
  std::string pathLeft = path.substr (next, path.size ()-next);

  //
  // If root is zero, we're beginning to see if we can use the object name 
  // service to resolve this path.  In this case, we must see the name space 
  // "/Names" on the front of this path.  There is no object associated with 
  // the root of the "/Names" namespace, so we just ignore it and move on to 
  // the next segment.
  //
  if (root == 0)
    {
      std::string::size_type offset = path.find ("/Names");
      if (offset == 0)
        {
          m_workStack.push_back (item);
          DoResolve (pathLeft, root);
          m_workStack.pop_back ();
          return;
        }
    }

  //
  // We have an item (possibly a segment of a namespace path.  Check to see if
  // we can determine that this segment refers to a named object.  If root is
  // zero, this means to look in the root of the "/Names" name space, otherwise
  // it refers to a name space context (level).
  //
  Ptr<Object> namedObject = Names::Find<Object> (root, item);
  if (namedObject)
    {
      NS_LOG_DEBUG ("Name system resolved item = " << item << " to " << namedObject);
      m_workStack.push_back (item);
      DoResolve (pathLeft, namedObject);
      m_workStack.pop_back ();
      return;
    }

  //
  // We're done with the object name service hooks, so proceed down the path
  // of types and attributes; but only if root is nonzero.  If root is zero
  // and we find ourselves here, we are trying to check in the namespace for
  // a path that is not in the "/Names" namespace.  We will have previously
  // found any matches, so we just bail out.
  //
  if (root == 0)
    {
      return;
    }
  std::string::size_type dollarPos = item.find ("$");
  if (dollarPos == 0)
    {
      // This is a call to GetObject
      std::string tidString = item.substr (1, item.size () - 1);
      NS_LOG_DEBUG ("GetObject="<<tidString<<" on path="<<GetResolvedPath ());
      TypeId tid = TypeId::LookupByName (tidString);
      Ptr<Object> object = root->GetObject<Object> (tid);
      if (object == 0)
	{
	  NS_LOG_DEBUG ("GetObject ("<<tidString<<") failed on path="<<GetResolvedPath ());
	  return;
	}
      m_workStack.push_back (item);
      DoResolve (pathLeft, object);
      m_workStack.pop_back ();
    }
  else 
    {
      // this is a normal attribute.
      TypeId tid = root->GetInstanceTypeId ();
      struct TypeId::AttributeInfo info;
      if (!tid.LookupAttributeByName (item, &info))
	{
	  NS_LOG_DEBUG ("Requested item="<<item<<" does not exist on path="<<GetResolvedPath ());
	  return;
	}
      // attempt to cast to a pointer checker.
      const PointerChecker *ptr = dynamic_cast<const PointerChecker *> (PeekPointer (info.checker));
      if (ptr != 0)
	{
	  NS_LOG_DEBUG ("GetAttribute(ptr)="<<item<<" on path="<<GetResolvedPath ());
          PointerValue ptr;
          root->GetAttribute (item, ptr);
	  Ptr<Object> object = ptr.Get<Object> ();
	  if (object == 0)
	    {
	      NS_LOG_ERROR ("Requested object name=\""<<item<<
			    "\" exists on path=\""<<GetResolvedPath ()<<"\""
			    " but is null.");
	      return;
	    }
	  m_workStack.push_back (item);
	  DoResolve (pathLeft, object);
	  m_workStack.pop_back ();
	}
      // attempt to cast to an object vector.
      const ObjectVectorChecker *vectorChecker = dynamic_cast<const ObjectVectorChecker *> (PeekPointer (info.checker));
      if (vectorChecker != 0)
	{
	  NS_LOG_DEBUG ("GetAttribute(vector)="<<item<<" on path="<<GetResolvedPath ());
	  ObjectVectorValue vector;
          root->GetAttribute (item, vector);
	  m_workStack.push_back (item);
	  DoArrayResolve (pathLeft, vector);
	  m_workStack.pop_back ();
	}
      // this could be anything else and we don't know what to do with it.
      // So, we just ignore it.
    }
}

void 
Resolver::DoArrayResolve (std::string path, const ObjectVectorValue &vector)
{
  NS_ASSERT (path != "");
  std::string::size_type tmp;
  tmp = path.find ("/");
  NS_ASSERT (tmp == 0);
  std::string::size_type next = path.find ("/", 1);
  if (next == std::string::npos)
    {
      NS_FATAL_ERROR ("vector path includes no index data on path=\""<<path<<"\"");
    }
  std::string item = path.substr (1, next-1);
  std::string pathLeft = path.substr (next, path.size ()-next);

  ArrayMatcher matcher = ArrayMatcher (item);
  for (uint32_t i = 0; i < vector.GetN (); i++)
    {
      if (matcher.Matches (i))
	{
	  std::ostringstream oss;
	  oss << i;
	  m_workStack.push_back (oss.str ());
	  DoResolve (pathLeft, vector.Get (i));
	  m_workStack.pop_back ();
	}
    }
}


class ConfigImpl 
{
public:
  void Set (std::string path, const AttributeValue &value);
  void ConnectWithoutContext (std::string path, const CallbackBase &cb);
  void Connect (std::string path, const CallbackBase &cb);
  void DisconnectWithoutContext (std::string path, const CallbackBase &cb);
  void Disconnect (std::string path, const CallbackBase &cb);
  Config::MatchContainer LookupMatches (std::string path);

  void RegisterRootNamespaceObject (Ptr<Object> obj);
  void UnregisterRootNamespaceObject (Ptr<Object> obj);

  uint32_t GetRootNamespaceObjectN (void) const;
  Ptr<Object> GetRootNamespaceObject (uint32_t i) const;
  
private:
  void ParsePath (std::string path, std::string *root, std::string *leaf) const;
  typedef std::vector<Ptr<Object> > Roots;
  Roots m_roots;
};

void 
ConfigImpl::ParsePath (std::string path, std::string *root, std::string *leaf) const
{
  std::string::size_type slash = path.find_last_of ("/");
  NS_ASSERT (slash != std::string::npos);
  *root = path.substr (0, slash);
  *leaf = path.substr (slash+1, path.size ()-(slash+1));
  NS_LOG_FUNCTION (path << *root << *leaf);
}

void 
ConfigImpl::Set (std::string path, const AttributeValue &value)
{
  std::string root, leaf;
  ParsePath (path, &root, &leaf);
  Config::MatchContainer container = LookupMatches (root);
  container.Set (leaf, value);
}
void 
ConfigImpl::ConnectWithoutContext (std::string path, const CallbackBase &cb)
{
  std::string root, leaf;
  ParsePath (path, &root, &leaf);
  Config::MatchContainer container = LookupMatches (root);
  container.ConnectWithoutContext (leaf, cb);
}
void 
ConfigImpl::DisconnectWithoutContext (std::string path, const CallbackBase &cb)
{
  std::string root, leaf;
  ParsePath (path, &root, &leaf);
  Config::MatchContainer container = LookupMatches (root);
  container.DisconnectWithoutContext (leaf, cb);
}
void 
ConfigImpl::Connect (std::string path, const CallbackBase &cb)
{
  std::string root, leaf;
  ParsePath (path, &root, &leaf);
  Config::MatchContainer container = LookupMatches (root);
  container.Connect (leaf, cb);
}
void 
ConfigImpl::Disconnect (std::string path, const CallbackBase &cb)
{
  std::string root, leaf;
  ParsePath (path, &root, &leaf);
  Config::MatchContainer container = LookupMatches (root);
  container.Disconnect (leaf, cb);
}

Config::MatchContainer 
ConfigImpl::LookupMatches (std::string path)
{
  NS_LOG_FUNCTION (path);
  class LookupMatchesResolver : public Resolver 
  {
  public:
    LookupMatchesResolver (std::string path)
      : Resolver (path)
    {}
    virtual void DoOne (Ptr<Object> object, std::string path) {
      m_objects.push_back (object);
      m_contexts.push_back (path);
    }
    std::vector<Ptr<Object> > m_objects;
    std::vector<std::string> m_contexts;
  } resolver = LookupMatchesResolver (path);
  for (Roots::const_iterator i = m_roots.begin (); i != m_roots.end (); i++)
    {
      resolver.Resolve (*i);
    }

  //
  // See if we can do something with the object name service.  Starting with
  // the root pointer zeroed indicates to the resolver that it should start
  // looking at the root of the "/Names" namespace during this go.
  //
  resolver.Resolve (0);

  return Config::MatchContainer (resolver.m_objects, resolver.m_contexts, path);
}

void 
ConfigImpl::RegisterRootNamespaceObject (Ptr<Object> obj)
{
  m_roots.push_back (obj);
}

void 
ConfigImpl::UnregisterRootNamespaceObject (Ptr<Object> obj)
{
  for (std::vector<Ptr<Object> >::iterator i = m_roots.begin (); i != m_roots.end (); i++)
    {
      if (*i == obj)
	{
	  m_roots.erase (i);
	  return;
	}
    }
}

uint32_t 
ConfigImpl::GetRootNamespaceObjectN (void) const
{
  return m_roots.size ();
}
Ptr<Object> 
ConfigImpl::GetRootNamespaceObject (uint32_t i) const
{
  return m_roots[i];
}

namespace Config {

void Set (std::string path, const AttributeValue &value)
{
  Singleton<ConfigImpl>::Get ()->Set (path, value);
}
void SetDefault (std::string name, const AttributeValue &value)
{
  AttributeList::GetGlobal ()->Set (name, value);
}
bool SetDefaultFailSafe (std::string name, const AttributeValue &value)
{
  return AttributeList::GetGlobal ()->SetFailSafe (name, value);
}
void SetGlobal (std::string name, const AttributeValue &value)
{
  GlobalValue::Bind (name, value);
}
bool SetGlobalFailSafe (std::string name, const AttributeValue &value)
{
  return GlobalValue::BindFailSafe (name, value);
}
void ConnectWithoutContext (std::string path, const CallbackBase &cb)
{
  Singleton<ConfigImpl>::Get ()->ConnectWithoutContext (path, cb);
}
void DisconnectWithoutContext (std::string path, const CallbackBase &cb)
{
  Singleton<ConfigImpl>::Get ()->DisconnectWithoutContext (path, cb);
}
void 
Connect (std::string path, const CallbackBase &cb)
{
  Singleton<ConfigImpl>::Get ()->Connect (path, cb);
}
void 
Disconnect (std::string path, const CallbackBase &cb)
{
  Singleton<ConfigImpl>::Get ()->Disconnect (path, cb);
}
Config::MatchContainer LookupMatches (std::string path)
{
  return Singleton<ConfigImpl>::Get ()->LookupMatches (path);
}

void RegisterRootNamespaceObject (Ptr<Object> obj)
{
  Singleton<ConfigImpl>::Get ()->RegisterRootNamespaceObject (obj);
}

void UnregisterRootNamespaceObject (Ptr<Object> obj)
{
  Singleton<ConfigImpl>::Get ()->UnregisterRootNamespaceObject (obj);
}

uint32_t GetRootNamespaceObjectN (void)
{
  return Singleton<ConfigImpl>::Get ()->GetRootNamespaceObjectN ();
}

Ptr<Object> GetRootNamespaceObject (uint32_t i)
{
  return Singleton<ConfigImpl>::Get ()->GetRootNamespaceObject (i);
}

} // namespace Config

// ===========================================================================
// An object with some attributes that we can play with using config.
// ===========================================================================
class ConfigTestObject : public Object
{
public:
  static TypeId GetTypeId (void);

  void AddNodeA (Ptr<ConfigTestObject> a);
  void AddNodeB (Ptr<ConfigTestObject> b);

  void SetNodeA (Ptr<ConfigTestObject> a);
  void SetNodeB (Ptr<ConfigTestObject> b);

  int8_t GetA (void) const;
  int8_t GetB (void) const;

private:
  std::vector<Ptr<ConfigTestObject> > m_nodesA;
  std::vector<Ptr<ConfigTestObject> > m_nodesB;
  Ptr<ConfigTestObject> m_nodeA;
  Ptr<ConfigTestObject> m_nodeB;
  int8_t m_a;
  int8_t m_b;
  TracedValue<int16_t> m_trace;
};

TypeId
ConfigTestObject::GetTypeId (void)
{
  static TypeId tid = TypeId ("ConfigTestObject")
    .SetParent<Object> ()
    .AddAttribute ("NodesA", "",
		   ObjectVectorValue (),
		   MakeObjectVectorAccessor (&ConfigTestObject::m_nodesA),
		   MakeObjectVectorChecker<ConfigTestObject> ())
    .AddAttribute ("NodesB", "",
		   ObjectVectorValue (),
		   MakeObjectVectorAccessor (&ConfigTestObject::m_nodesB),
		   MakeObjectVectorChecker<ConfigTestObject> ())
    .AddAttribute ("NodeA", "",
                   PointerValue (),
		   MakePointerAccessor (&ConfigTestObject::m_nodeA),
		   MakePointerChecker<ConfigTestObject> ())
    .AddAttribute ("NodeB", "",
                   PointerValue (),
		   MakePointerAccessor (&ConfigTestObject::m_nodeB),
		   MakePointerChecker<ConfigTestObject> ())
    .AddAttribute ("A", "",
		   IntegerValue (10),
		   MakeIntegerAccessor (&ConfigTestObject::m_a),
		   MakeIntegerChecker<int8_t> ())
    .AddAttribute ("B", "",
		   IntegerValue (9),
		   MakeIntegerAccessor (&ConfigTestObject::m_b),
		   MakeIntegerChecker<int8_t> ())
    .AddAttribute ("Source", "XX",
		   IntegerValue (-1),
		   MakeIntegerAccessor (&ConfigTestObject::m_trace),
		   MakeIntegerChecker<int16_t> ())
    .AddTraceSource ("Source", "XX",
		     MakeTraceSourceAccessor (&ConfigTestObject::m_trace))
    ;
  return tid;
}

void
ConfigTestObject::SetNodeA (Ptr<ConfigTestObject> a)
{
  m_nodeA = a;
}

void
ConfigTestObject::SetNodeB (Ptr<ConfigTestObject> b)
{
  m_nodeB = b;
}

void 
ConfigTestObject::AddNodeA (Ptr<ConfigTestObject> a)
{
  m_nodesA.push_back (a);
}

void 
ConfigTestObject::AddNodeB (Ptr<ConfigTestObject> b)
{
  m_nodesB.push_back (b);
}

int8_t 
ConfigTestObject::GetA (void) const
{
  return m_a;
}

int8_t 
ConfigTestObject::GetB (void) const
{
  return m_b;
}

// ===========================================================================
// Test for the ability to register and use a root namespace
// ===========================================================================
class RootNamespaceConfigTestCase : public TestCase
{
public:
  RootNamespaceConfigTestCase ();
  virtual ~RootNamespaceConfigTestCase () {}

private:
  virtual bool DoRun (void);
};

RootNamespaceConfigTestCase::RootNamespaceConfigTestCase ()
  : TestCase ("Check ability to register a root namespace and use it")
{
}

bool
RootNamespaceConfigTestCase::DoRun (void)
{
  IntegerValue iv;
  //
  // Create an object and register its attributes directly in the root 
  // namespace.
  //
  Ptr<ConfigTestObject> root = CreateObject<ConfigTestObject> ();
  Config::RegisterRootNamespaceObject (root);

  //
  // We should find the default values there.
  //
  root->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), 10, "Object Attribute \"A\" not initialized as expected");

  //
  // Now use the config mechanism to set the attribute; and we should find the
  // new value.
  //
  Config::Set ("/A", IntegerValue (1));
  root->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), 1, "Object Attribute \"A\" not set correctly");

  //
  // We should find the default values of "B" too.
  //
  root->GetAttribute ("B", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), 9, "Object Attribute \"B\" not initialized as expected");

  //
  // Now use the config mechanism to set the attribute; and we should find the
  // new value.
  //
  Config::Set ("/B", IntegerValue (-1));
  root->GetAttribute ("B", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), -1, "Object Attribute \"B\" not set correctly");

  return GetErrorStatus ();
}

// ===========================================================================
// Test for the ability to add an object under the root namespace.
// ===========================================================================
class UnderRootNamespaceConfigTestCase : public TestCase
{
public:
  UnderRootNamespaceConfigTestCase ();
  virtual ~UnderRootNamespaceConfigTestCase () {}

private:
  virtual bool DoRun (void);
};

UnderRootNamespaceConfigTestCase::UnderRootNamespaceConfigTestCase ()
  : TestCase ("Check ability to register an object under the root namespace and use it")
{
}

bool
UnderRootNamespaceConfigTestCase::DoRun (void)
{
  IntegerValue iv;
  //
  // Create an object and register its attributes directly in the root 
  // namespace.
  //
  Ptr<ConfigTestObject> root = CreateObject<ConfigTestObject> ();
  Config::RegisterRootNamespaceObject (root);

  Ptr<ConfigTestObject> a = CreateObject<ConfigTestObject> ();
  root->SetNodeA (a);

  //
  // We should find the default values there.
  //
  a->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), 10, "Object Attribute \"A\" not initialized as expected");

  //
  // Now use the config mechanism to set the attribute; and we should find the
  // new value.
  //
  Config::Set ("/NodeA/A", IntegerValue (1));
  a->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), 1, "Object Attribute \"A\" not set correctly");

  //
  // We should find the default values of "B" too.
  //
  a->GetAttribute ("B", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), 9, "Object Attribute \"B\" not initialized as expected");

  //
  // Now use the config mechanism to set the attribute; and we should find the
  // new value.
  //
  Config::Set ("/NodeA/B", IntegerValue (-1));
  a->GetAttribute ("B", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), -1, "Object Attribute \"B\" not set correctly");

  //
  // Try and set through a nonexistent path.  Should do nothing.
  //
  Config::Set ("/NodeB/A", IntegerValue (1234));
  a->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), 1, "Object Attribute \"A\" unexpectedly set via bad path");

  Config::Set ("/NodeB/B", IntegerValue (1234));
  a->GetAttribute ("B", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), -1, "Object Attribute \"B\" unexpectedly set via bad path");

  //
  // Step down one level of recursion and try again
  //
  Ptr<ConfigTestObject> b = CreateObject<ConfigTestObject> ();

  //
  // We should find the default values there.
  //
  b->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), 10, "Object Attribute \"A\" not initialized as expected");
  b->GetAttribute ("B", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), 9, "Object Attribute \"B\" not initialized as expected");

  //
  // Now tell A that it has a B; and we should be able to set this new object's
  // Attributes.
  //
  a->SetNodeB (b);

  Config::Set ("/NodeA/NodeB/A", IntegerValue (4));
  Config::Set ("/NodeA/NodeB/B", IntegerValue (-4));
  b->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), 4, "Object Attribute \"A\" not set as expected");
  b->GetAttribute ("B", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), -4, "Object Attribute \"B\" not set as expected");

  return GetErrorStatus ();
}

// ===========================================================================
// Test for the ability to deal configure with vectors of objects.
// ===========================================================================
class ObjectVectorConfigTestCase : public TestCase
{
public:
  ObjectVectorConfigTestCase ();
  virtual ~ObjectVectorConfigTestCase () {}

private:
  virtual bool DoRun (void);
};

ObjectVectorConfigTestCase::ObjectVectorConfigTestCase ()
  : TestCase ("Check ability to configure vectors of Object using regular expressions")
{
}

bool
ObjectVectorConfigTestCase::DoRun (void)
{
  IntegerValue iv;

  //
  // Create a root namespace object
  //
  Ptr<ConfigTestObject> root = CreateObject<ConfigTestObject> ();
  Config::RegisterRootNamespaceObject (root);

  //
  // Create an object under the root.
  //
  Ptr<ConfigTestObject> a = CreateObject<ConfigTestObject> ();
  root->SetNodeA (a);

  //
  // Create an object one level down.
  //
  Ptr<ConfigTestObject> b = CreateObject<ConfigTestObject> ();
  a->SetNodeB (b);

  //
  // Add four objects to the ObjectVector Attribute at the bottom of the 
  // object hierarchy.  By this point, we believe that the Attributes
  // will be initialized correctly.
  //
  Ptr<ConfigTestObject> obj0 = CreateObject<ConfigTestObject> ();
  Ptr<ConfigTestObject> obj1 = CreateObject<ConfigTestObject> ();
  Ptr<ConfigTestObject> obj2 = CreateObject<ConfigTestObject> ();
  Ptr<ConfigTestObject> obj3 = CreateObject<ConfigTestObject> ();
  b->AddNodeB (obj0);
  b->AddNodeB (obj1);
  b->AddNodeB (obj2);
  b->AddNodeB (obj3);

  //
  // Set an Attribute of the zeroth Object in the vector by explicitly writing
  // the '0' and make sure that only the one thing changed.
  //
  Config::Set ("/NodeA/NodeB/NodesB/0/A", IntegerValue (-11));
  obj0->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), -11, "Object Attribute \"A\" not set as expected");

  obj1->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), 10, "Object Attribute \"A\" unexpectedly set");

  obj2->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), 10, "Object Attribute \"A\" unexpectedly set");

  obj3->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), 10, "Object Attribute \"A\" unexpectedly set");

  //
  // Start using regular expression-like syntax to set Attributes.  First try
  // the OR syntax.  Make sure that the two objects changed and nothing else
  //
  Config::Set ("/NodeA/NodeB/NodesB/0|1/A", IntegerValue (-12));
  obj0->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), -12, "Object Attribute \"A\" not set as expected");

  obj1->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), -12, "Object Attribute \"A\" not set as expected");

  obj2->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), 10, "Object Attribute \"A\" unexpectedly set");

  obj3->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), 10, "Object Attribute \"A\" unexpectedly set");

  //
  // Make sure that extra '|' are allowed at the start and end of the regular expression
  //
  Config::Set ("/NodeA/NodeB/NodesB/|0|1|/A", IntegerValue (-13));
  obj0->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), -13, "Object Attribute \"A\" not set as expected");

  obj1->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), -13, "Object Attribute \"A\" not set as expected");

  obj2->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), 10, "Object Attribute \"A\" unexpectedly set");

  obj3->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), 10, "Object Attribute \"A\" unexpectedly set");

  //
  // Try the [x-y] syntax
  //
  Config::Set ("/NodeA/NodeB/NodesB/[0-2]/A", IntegerValue (-14));
  obj0->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), -14, "Object Attribute \"A\" not set as expected");

  obj1->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), -14, "Object Attribute \"A\" not set as expected");

  obj2->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), -14, "Object Attribute \"A\" not set as expected");

  obj3->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), 10, "Object Attribute \"A\" unexpectedly set");

  //
  // Try the [x-y] syntax at the other limit
  //
  Config::Set ("/NodeA/NodeB/NodesB/[1-3]/A", IntegerValue (-15));
  obj0->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), -14, "Object Attribute \"A\" unexpectedly set");

  obj1->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), -15, "Object Attribute \"A\" not set as expected");

  obj2->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), -15, "Object Attribute \"A\" not set as expected");

  obj3->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), -15, "Object Attribute \"A\" not set as expected");

  //
  // Combine the [x-y] syntax and the OR sntax
  //
  Config::Set ("/NodeA/NodeB/NodesB/[0-1]|3/A", IntegerValue (-16));
  obj0->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), -16, "Object Attribute \"A\" not set as expected");

  obj1->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), -16, "Object Attribute \"A\" not set as expected");

  obj2->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), -15, "Object Attribute \"A\" unexpectedly set");

  obj3->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), -16, "Object Attribute \"A\" not set as expected");

  return GetErrorStatus ();
}

// ===========================================================================
// Test for the ability to trace configure with vectors of objects.
// ===========================================================================
class ObjectVectorTraceConfigTestCase : public TestCase
{
public:
  ObjectVectorTraceConfigTestCase ();
  virtual ~ObjectVectorTraceConfigTestCase () {}

  void Trace (int16_t oldValue, int16_t newValue) {m_newValue = newValue;}
  void TraceWithPath (std::string path, int16_t old, int16_t newValue) {m_newValue = newValue; m_path = path;}

private:
  virtual bool DoRun (void);

  int16_t m_newValue;
  std::string m_path;
};

ObjectVectorTraceConfigTestCase::ObjectVectorTraceConfigTestCase ()
  : TestCase ("Check ability to trace connect through vectors of Object using regular expressions")
{
}

bool
ObjectVectorTraceConfigTestCase::DoRun (void)
{
  IntegerValue iv;

  //
  // Create a root namespace object
  //
  Ptr<ConfigTestObject> root = CreateObject<ConfigTestObject> ();
  Config::RegisterRootNamespaceObject (root);

  //
  // Create an object under the root.
  //
  Ptr<ConfigTestObject> a = CreateObject<ConfigTestObject> ();
  root->SetNodeA (a);

  //
  // Create an object one level down.
  //
  Ptr<ConfigTestObject> b = CreateObject<ConfigTestObject> ();
  a->SetNodeB (b);

  //
  // Add four objects to the ObjectVector Attribute at the bottom of the 
  // object hierarchy.  By this point, we believe that the Attributes
  // will be initialized correctly.
  //
  Ptr<ConfigTestObject> obj0 = CreateObject<ConfigTestObject> ();
  Ptr<ConfigTestObject> obj1 = CreateObject<ConfigTestObject> ();
  Ptr<ConfigTestObject> obj2 = CreateObject<ConfigTestObject> ();
  Ptr<ConfigTestObject> obj3 = CreateObject<ConfigTestObject> ();
  b->AddNodeB (obj0);
  b->AddNodeB (obj1);
  b->AddNodeB (obj2);
  b->AddNodeB (obj3);

  //
  // Do a trace connect to some of the sources.  We already checked parsing of
  // the regular expressions, so we'll concentrate on the tracing part of the 
  // puzzle here.
  //
  Config::ConnectWithoutContext ("/NodeA/NodeB/NodesB/[0-1]|3/Source", 
                                 MakeCallback (&ObjectVectorTraceConfigTestCase::Trace, this));

  // 
  // If we bug the trace source referred to by index '0' above, we should see 
  // the trace fire.
  //
  m_newValue = 0;
  obj0->SetAttribute ("Source", IntegerValue (-1));
  NS_TEST_ASSERT_MSG_EQ (m_newValue, -1, "Trace 0 did not fire as expected");

  // 
  // If we bug the trace source referred to by index '1' above, we should see 
  // the trace fire.
  //
  m_newValue = 0;
  obj1->SetAttribute ("Source", IntegerValue (-2));
  NS_TEST_ASSERT_MSG_EQ (m_newValue, -2, "Trace 1 did not fire as expected");

  // 
  // If we bug the trace source referred to by index '2' which is skipped above,
  // we should not see the trace fire.
  //
  m_newValue = 0;
  obj2->SetAttribute ("Source", IntegerValue (-3));
  NS_TEST_ASSERT_MSG_EQ (m_newValue, 0, "Trace 2 fired unexpectedly");

  // 
  // If we bug the trace source referred to by index '3' above, we should see 
  // the trace fire.
  //
  m_newValue = 0;
  obj3->SetAttribute ("Source", IntegerValue (-4));
  NS_TEST_ASSERT_MSG_EQ (m_newValue, -4, "Trace 3 did not fire as expected");

  //
  // Do a trace connect (with context) to some of the sources.
  //
  Config::Connect ("/NodeA/NodeB/NodesB/[0-1]|3/Source", 
                   MakeCallback (&ObjectVectorTraceConfigTestCase::TraceWithPath, this));

  // 
  // If we bug the trace source referred to by index '0' above, we should see 
  // the trace fire with the expected context path.
  //
  m_newValue = 0;
  m_path = "";
  obj0->SetAttribute ("Source", IntegerValue (-1));
  NS_TEST_ASSERT_MSG_EQ (m_newValue, -1, "Trace 0 did not fire as expected");
  NS_TEST_ASSERT_MSG_EQ (m_path, "/NodeA/NodeB/NodesB/0/Source", "Trace 0 did not provide expected context");

  // 
  // If we bug the trace source referred to by index '1' above, we should see 
  // the trace fire with the expected context path.
  //
  m_newValue = 0;
  m_path = "";
  obj1->SetAttribute ("Source", IntegerValue (-2));
  NS_TEST_ASSERT_MSG_EQ (m_newValue, -2, "Trace 1 did not fire as expected");
  NS_TEST_ASSERT_MSG_EQ (m_path, "/NodeA/NodeB/NodesB/1/Source", "Trace 1 did not provide expected context");

  // 
  // If we bug the trace source referred to by index '2' which is skipped above,
  // we should not see the trace fire.
  //
  m_newValue = 0;
  m_path = "";
  obj2->SetAttribute ("Source", IntegerValue (-3));
  NS_TEST_ASSERT_MSG_EQ (m_newValue, 0, "Trace 2 fired unexpectedly");

  // 
  // If we bug the trace source referred to by index '3' above, we should see 
  // the trace fire with the expected context path.
  //
  m_newValue = 0;
  m_path = "";
  obj3->SetAttribute ("Source", IntegerValue (-4));
  NS_TEST_ASSERT_MSG_EQ (m_newValue, -4, "Trace 3 did not fire as expected");
  NS_TEST_ASSERT_MSG_EQ (m_path, "/NodeA/NodeB/NodesB/1/Source", "Trace 1 did not provide expected context");

  return GetErrorStatus ();
}

// ===========================================================================
// The Test Suite that glues all of the Test Cases together.
// ===========================================================================
class ConfigTestSuite : public TestSuite
{
public:
  ConfigTestSuite ();
};

ConfigTestSuite::ConfigTestSuite ()
  : TestSuite ("config", BVT)
{
  AddTestCase (new RootNamespaceConfigTestCase);
  AddTestCase (new UnderRootNamespaceConfigTestCase);
  AddTestCase (new ObjectVectorConfigTestCase);
}

ConfigTestSuite configTestSuite;

} // namespace ns3
