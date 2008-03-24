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
#include "log.h"
#include <sstream>

NS_LOG_COMPONENT_DEFINE ("Config");

namespace ns3 {

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
  void DoResolve (std::string path, Ptr<Object> root);
  void DoArrayResolve (std::string path, const ObjectVector &vector);
  void DoResolveOne (Ptr<Object> object, std::string name);
  std::string GetResolvedPath (std::string name) const;
  virtual void DoOne (Ptr<Object> object, std::string path, std::string name) = 0;
  std::vector<std::string> m_workStack;
  std::string m_path;
};

Resolver::Resolver (std::string path)
  : m_path (path)
{}
Resolver::~Resolver ()
{}

void 
Resolver::Resolve (Ptr<Object> root)
{
  DoResolve (m_path, root);
}

std::string
Resolver::GetResolvedPath (std::string name) const
{
  std::string fullPath = "";
  for (std::vector<std::string>::const_iterator i = m_workStack.begin (); i != m_workStack.end (); i++)
    {
      fullPath += "/" + *i;
    }
  fullPath += "/" + name;
  return fullPath;
}

void 
Resolver::DoResolveOne (Ptr<Object> object, std::string name)
{
  NS_LOG_DEBUG ("resolved="<<GetResolvedPath (name));
  DoOne (object, GetResolvedPath (name), name);
}

void
Resolver::DoResolve (std::string path, Ptr<Object> root)
{
  NS_ASSERT (path != "");
  std::string::size_type pos = path.find ("/");
  if (pos != 0)
    {
      NS_FATAL_ERROR ("path does not start with a \"/\": \""<<path<<"\"");
      return;
    }
  std::string::size_type next = path.find ("/", 1);
  if (next == std::string::npos)
    {
      std::string attributeName = path.substr (1, path.size ()-1);
      NS_LOG_DEBUG ("handle attr="<<attributeName);
      DoOne (root, GetResolvedPath (attributeName), attributeName);
      return;
    }
  std::string item = path.substr (1, next-1);
  std::string pathLeft = path.substr (next, path.size ()-next);

  std::string::size_type dollarPos = item.find ("$");
  if (dollarPos == 0)
    {
      // This is a call to GetObject
      std::string tidString = item.substr (1, item.size () - 1);
      NS_LOG_DEBUG ("GetObject="<<tidString<<"on path="<<GetResolvedPath (""));
      TypeId tid = TypeId::LookupByName (tidString);
      Ptr<Object> object = root->GetObject<Object> (tid);
      if (object == 0)
	{
	  NS_LOG_DEBUG ("GetObject ("<<tidString<<") failed on path="<<GetResolvedPath (""));
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
	  NS_LOG_DEBUG ("Requested item="<<item<<" does not exist on path="<<GetResolvedPath (""));
	  return;
	}
      // attempt to cast to a pointer checker.
      const PtrChecker *ptr = dynamic_cast<const PtrChecker *> (PeekPointer (info.checker));
      if (ptr != 0)
	{
	  NS_LOG_DEBUG ("GetAttribute(ptr)="<<item<<" on path="<<GetResolvedPath (""));
	  // XXX: This is not completely right because anything could be stored in a
	  // Ptr<>. We really need to fix this by thinking seriously about our
	  // object hierarchy.
	  Ptr<Object> object = root->GetAttribute (item);
	  if (object == 0)
	    {
	      NS_LOG_ERROR ("Requested object name=\""<<item<<
			    "\" exists on path=\""<<GetResolvedPath ("")<<"\""
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
	  NS_LOG_DEBUG ("GetAttribute(vector)="<<item<<" on path="<<GetResolvedPath (""));
	  ObjectVector vector = root->GetAttribute (item);
	  m_workStack.push_back (item);
	  DoArrayResolve (pathLeft, vector);
	  m_workStack.pop_back ();
	}
      // this could be anything else and we don't know what to do with it.
      // So, we just ignore it.
    }
}

void 
Resolver::DoArrayResolve (std::string path, const ObjectVector &vector)
{
  NS_ASSERT (path != "");
  std::string::size_type pos = path.find ("/");
  if (pos != 0)
    {
      NS_FATAL_ERROR ("path does not start with a \"/\": \""<<path<<"\"");
      return;
    }
  std::string::size_type next = path.find ("/", 1);
  if (next == std::string::npos)
    {
      NS_LOG_DEBUG ("vector path includes no index data on path=\""<<path<<"\"");
      return;
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
  void Set (std::string path, Attribute value);
  void ConnectWithoutContext (std::string path, const CallbackBase &cb);
  void Connect (std::string path, const CallbackBase &cb);
  void DisconnectWithoutContext (std::string path, const CallbackBase &cb);
  void Disconnect (std::string path, const CallbackBase &cb);

  void RegisterRootNamespaceObject (Ptr<Object> obj);
  void UnregisterRootNamespaceObject (Ptr<Object> obj);
  
private:
  typedef std::vector<Ptr<Object> > Roots;
  Roots m_roots;
};

void 
ConfigImpl::Set (std::string path, Attribute value)
{
  class SetResolver : public Resolver 
  {
  public:
    SetResolver (std::string path, Attribute value)
      : Resolver (path),
	m_value (value) {}
  private:
    virtual void DoOne (Ptr<Object> object, std::string path, std::string name) {
      object->SetAttribute (name, m_value);
    }
    Attribute m_value;
  } resolver = SetResolver (path, value);
  for (Roots::const_iterator i = m_roots.begin (); i != m_roots.end (); i++)
    {
      resolver.Resolve (*i);
    }
}
void 
ConfigImpl::ConnectWithoutContext (std::string path, const CallbackBase &cb)
{
  class ConnectResolver : public Resolver 
  {
  public:
    ConnectResolver (std::string path, const CallbackBase &cb)
      : Resolver (path),
	m_cb (cb) {}
  private:
    virtual void DoOne (Ptr<Object> object, std::string path, std::string name) {
      object->TraceConnectWithoutContext (name, m_cb);
    }
    CallbackBase m_cb;
  } resolver = ConnectResolver (path, cb);
  for (Roots::const_iterator i = m_roots.begin (); i != m_roots.end (); i++)
    {
      resolver.Resolve (*i);
    }
}
void 
ConfigImpl::DisconnectWithoutContext (std::string path, const CallbackBase &cb)
{
  class DisconnectResolver : public Resolver 
  {
  public:
    DisconnectResolver (std::string path, const CallbackBase &cb)
      : Resolver (path),
	m_cb (cb) {}
  private:
    virtual void DoOne (Ptr<Object> object, std::string path, std::string name) {
      object->TraceDisconnectWithoutContext (name, m_cb);
    }
    CallbackBase m_cb;
  } resolver = DisconnectResolver (path, cb);
  for (Roots::const_iterator i = m_roots.begin (); i != m_roots.end (); i++)
    {
      resolver.Resolve (*i);
    }
}
void 
ConfigImpl::Connect (std::string path, const CallbackBase &cb)
{
  class ConnectWithContextResolver : public Resolver 
  {
  public:
    ConnectWithContextResolver (std::string path, const CallbackBase &cb)
      : Resolver (path),
	m_cb (cb) {}
  private:
    virtual void DoOne (Ptr<Object> object, std::string path, std::string name) {
      object->TraceConnect (name, path, m_cb);
    }
    CallbackBase m_cb;
  } resolver = ConnectWithContextResolver (path, cb);
  for (Roots::const_iterator i = m_roots.begin (); i != m_roots.end (); i++)
    {
      resolver.Resolve (*i);
    }
}
void 
ConfigImpl::Disconnect (std::string path, const CallbackBase &cb)
{
  class DisconnectWithContextResolver : public Resolver 
  {
  public:
    DisconnectWithContextResolver (std::string path, const CallbackBase &cb)
      : Resolver (path),
	m_cb (cb) {}
  private:
    virtual void DoOne (Ptr<Object> object, std::string path, std::string name) {
      object->TraceDisconnect (name, path, m_cb);
    }
    CallbackBase m_cb;
  } resolver = DisconnectWithContextResolver (path, cb);
  for (Roots::const_iterator i = m_roots.begin (); i != m_roots.end (); i++)
    {
      resolver.Resolve (*i);
    }
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


namespace Config {

void Set (std::string path, Attribute value)
{
  Singleton<ConfigImpl>::Get ()->Set (path, value);
}
void SetDefault (std::string name, Attribute value)
{
  AttributeList::GetGlobal ()->Set (name, value);
}
bool SetDefaultFailSafe (std::string name, Attribute value)
{
  return AttributeList::GetGlobal ()->SetFailSafe (name, value);
}
void SetGlobal (std::string name, Attribute value)
{
  GlobalValue::Bind (name, value);
}
bool SetGlobalFailSafe (std::string name, Attribute value)
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

void RegisterRootNamespaceObject (Ptr<Object> obj)
{
  Singleton<ConfigImpl>::Get ()->RegisterRootNamespaceObject (obj);
}

void UnregisterRootNamespaceObject (Ptr<Object> obj)
{
  Singleton<ConfigImpl>::Get ()->UnregisterRootNamespaceObject (obj);
}

} // namespace Config

} // namespace ns3

#ifdef RUN_SELF_TESTS

#include "test.h"
#include "integer.h"
#include "traced-value.h"
#include "trace-source-accessor.h"
#include "callback.h"

namespace ns3 {

class MyNode : public Object
{
public:
  static TypeId GetTypeId (void);

  void AddNodeA (Ptr<MyNode> a);
  void AddNodeB (Ptr<MyNode> b);

  void SetNodeA (Ptr<MyNode> a);
  void SetNodeB (Ptr<MyNode> b);

  int8_t GetA (void) const;
  int8_t GetB (void) const;

private:
  std::vector<Ptr<MyNode> > m_nodesA;
  std::vector<Ptr<MyNode> > m_nodesB;
  Ptr<MyNode> m_nodeA;
  Ptr<MyNode> m_nodeB;
  int8_t m_a;
  int8_t m_b;
  TracedValue<int16_t> m_trace;
};

TypeId MyNode::GetTypeId (void)
{
  static TypeId tid = TypeId ("MyNode")
    .SetParent<Object> ()
    .AddAttribute ("NodesA", "",
		   ObjectVector (),
		   MakeObjectVectorAccessor (&MyNode::m_nodesA),
		   MakeObjectVectorChecker ())
    .AddAttribute ("NodesB", "",
		   ObjectVector (),
		   MakeObjectVectorAccessor (&MyNode::m_nodesB),
		   MakeObjectVectorChecker ())
    .AddAttribute ("NodeA", "",
		   Ptr<MyNode> (0),
		   MakePtrAccessor (&MyNode::m_nodeA),
		   MakePtrChecker<MyNode> ())
    .AddAttribute ("NodeB", "",
		   Ptr<MyNode> (0),
		   MakePtrAccessor (&MyNode::m_nodeB),
		   MakePtrChecker<MyNode> ())
    .AddAttribute ("A", "",
		   Integer (10),
		   MakeIntegerAccessor (&MyNode::m_a),
		   MakeIntegerChecker<int8_t> ())
    .AddAttribute ("B", "",
		   Integer (9),
		   MakeIntegerAccessor (&MyNode::m_b),
		   MakeIntegerChecker<int8_t> ())
    .AddAttribute ("Source", "XX",
		   Integer (-1),
		   MakeIntegerAccessor (&MyNode::m_trace),
		   MakeIntegerChecker<int16_t> ())
    .AddTraceSource ("Source", "XX",
		     MakeTraceSourceAccessor (&MyNode::m_trace))
    ;
  return tid;
}

void
MyNode::SetNodeA (Ptr<MyNode> a)
{
  m_nodeA = a;
}
void
MyNode::SetNodeB (Ptr<MyNode> b)
{
  m_nodeB = b;
}
void 
MyNode::AddNodeA (Ptr<MyNode> a)
{
  m_nodesA.push_back (a);
}
void 
MyNode::AddNodeB (Ptr<MyNode> b)
{
  m_nodesB.push_back (b);
}
int8_t 
MyNode::GetA (void) const
{
  return m_a;
}
int8_t 
MyNode::GetB (void) const
{
  return m_b;
}


class ConfigTest : public Test
{
public:
  ConfigTest ();
  virtual bool RunTests (void);
private:
  void ChangeNotification (int16_t old, int16_t newValue);
  void ChangeNotificationWithPath (std::string path, int16_t old, int16_t newValue);
  int16_t m_traceNotification;
  std::string m_tracePath;
};

static ConfigTest g_configTestUnique;

ConfigTest::ConfigTest ()
  : Test ("Config")
{}

void
ConfigTest::ChangeNotification (int16_t oldValue, int16_t newValue)
{
  m_traceNotification = newValue;
}

void 
ConfigTest::ChangeNotificationWithPath (std::string path, int16_t old, int16_t newValue)
{
  m_traceNotification = newValue;
  m_tracePath = path;
}

bool
ConfigTest::RunTests (void)
{
  bool result = true;

  Ptr<MyNode> root = CreateObject<MyNode> ();
  Config::RegisterRootNamespaceObject (root);
  Config::Set ("/A", Integer (1));
  Config::Set ("/B", Integer (-1));
  Integer v = root->GetAttribute ("A");
  NS_TEST_ASSERT_EQUAL (v.Get (), 1);
  v = root->GetAttribute ("B");
  NS_TEST_ASSERT_EQUAL (v.Get (), -1);

  Ptr<MyNode> a = CreateObject<MyNode> ();
  root->SetNodeA (a);
  Config::Set ("/NodeA/A", Integer (2));
  Config::Set ("/NodeA/B", Integer (-2));
  v = a->GetAttribute ("A");
  NS_TEST_ASSERT_EQUAL (v.Get (), 2);
  v = a->GetAttribute ("B");
  NS_TEST_ASSERT_EQUAL (v.Get (), -2);
  Config::Set ("/NodeB/A", Integer (3));
  Config::Set ("/NodeB/B", Integer (-3));
  v = a->GetAttribute ("A");
  NS_TEST_ASSERT_EQUAL (v.Get (), 2);
  v = a->GetAttribute ("B");
  NS_TEST_ASSERT_EQUAL (v.Get (), -2);

  Ptr<MyNode> b = CreateObject<MyNode> ();
  a->SetNodeB (b);
  Config::Set ("/NodeA/NodeB/A", Integer (4));
  Config::Set ("/NodeA/NodeB/B", Integer (-4));
  v = b->GetAttribute ("A");
  NS_TEST_ASSERT_EQUAL (v.Get (), 4);
  v = b->GetAttribute ("B");
  NS_TEST_ASSERT_EQUAL (v.Get (), -4);

  Ptr<MyNode> c = CreateObject<MyNode> ();
  root->SetNodeB (c);
  Config::Set ("/NodeB/A", Integer (5));
  Config::Set ("/NodeB/B", Integer (-5));
  v = c->GetAttribute ("A");
  NS_TEST_ASSERT_EQUAL (v.Get (), 5);
  v = c->GetAttribute ("B");
  NS_TEST_ASSERT_EQUAL (v.Get (), -5);


  Ptr<MyNode> d0 = CreateObject<MyNode> ();
  Ptr<MyNode> d1 = CreateObject<MyNode> ();
  Ptr<MyNode> d2 = CreateObject<MyNode> ();
  Ptr<MyNode> d3 = CreateObject<MyNode> ();
  b->AddNodeB (d0);
  b->AddNodeB (d1);
  b->AddNodeB (d2);
  b->AddNodeB (d3);
  Config::Set ("/NodeA/NodeB/NodesB/0/A", Integer (-11));
  v = d0->GetAttribute ("A");
  NS_TEST_ASSERT_EQUAL (v.Get (), -11);
  v = d0->GetAttribute ("B");
  NS_TEST_ASSERT_EQUAL (v.Get (), 9);
  v = d1->GetAttribute ("A");
  NS_TEST_ASSERT_EQUAL (v.Get (), 10);
  v = d1->GetAttribute ("B");
  NS_TEST_ASSERT_EQUAL (v.Get (), 9);
  Config::Set ("/NodeA/NodeB/NodesB/0|1/A", Integer (-12));
  v = d0->GetAttribute ("A");
  NS_TEST_ASSERT_EQUAL (v.Get (), -12);
  v = d1->GetAttribute ("A");
  NS_TEST_ASSERT_EQUAL (v.Get (), -12);
  Config::Set ("/NodeA/NodeB/NodesB/|0|1|/A", Integer (-13));
  v = d0->GetAttribute ("A");
  NS_TEST_ASSERT_EQUAL (v.Get (), -13);
  v = d1->GetAttribute ("A");
  NS_TEST_ASSERT_EQUAL (v.Get (), -13);
  Config::Set ("/NodeA/NodeB/NodesB/[0-2]/A", Integer (-14));
  v = d0->GetAttribute ("A");
  NS_TEST_ASSERT_EQUAL (v.Get (), -14);
  v = d1->GetAttribute ("A");
  NS_TEST_ASSERT_EQUAL (v.Get (), -14);
  v = d2->GetAttribute ("A");
  NS_TEST_ASSERT_EQUAL (v.Get (), -14);
  Config::Set ("/NodeA/NodeB/NodesB/[1-3]/A", Integer (-15));
  v = d0->GetAttribute ("A");
  NS_TEST_ASSERT_EQUAL (v.Get (), -14);
  v = d1->GetAttribute ("A");
  NS_TEST_ASSERT_EQUAL (v.Get (), -15);
  v = d2->GetAttribute ("A");
  NS_TEST_ASSERT_EQUAL (v.Get (), -15);
  v = d3->GetAttribute ("A");
  NS_TEST_ASSERT_EQUAL (v.Get (), -15);
  Config::Set ("/NodeA/NodeB/NodesB/[0-1]|3/A", Integer (-16));
  v = d0->GetAttribute ("A");
  NS_TEST_ASSERT_EQUAL (v.Get (), -16);
  v = d1->GetAttribute ("A");
  NS_TEST_ASSERT_EQUAL (v.Get (), -16);
  v = d2->GetAttribute ("A");
  NS_TEST_ASSERT_EQUAL (v.Get (), -15);
  v = d3->GetAttribute ("A");
  NS_TEST_ASSERT_EQUAL (v.Get (), -16);


  Config::ConnectWithoutContext ("/NodeA/NodeB/NodesB/[0-1]|3/Source", 
		   MakeCallback (&ConfigTest::ChangeNotification, this));
  m_traceNotification = 0;
  // this should trigger no notification
  d2->SetAttribute ("Source", Integer (-2));
  NS_TEST_ASSERT_EQUAL (m_traceNotification, 0);
  m_traceNotification = 0;
  // this should trigger a notification
  d1->SetAttribute ("Source", Integer (-3));
  NS_TEST_ASSERT_EQUAL (m_traceNotification, -3);
  Config::DisconnectWithoutContext ("/NodeA/NodeB/NodesB/[0-1]|3/Source", 
		      MakeCallback (&ConfigTest::ChangeNotification, this));
  m_traceNotification = 0;
  // this should _not_ trigger a notification
  d1->SetAttribute ("Source", Integer (-4));
  NS_TEST_ASSERT_EQUAL (m_traceNotification, 0);

  
  Config::Connect ("/NodeA/NodeB/NodesB/[0-1]|3/Source", 
			      MakeCallback (&ConfigTest::ChangeNotificationWithPath, this));
  m_traceNotification = 0;
  // this should trigger no notification
  d2->SetAttribute ("Source", Integer (-2));
  NS_TEST_ASSERT_EQUAL (m_traceNotification, 0);
  m_traceNotification = 0;
  m_tracePath = "";
  // this should trigger a notification
  d1->SetAttribute ("Source", Integer (-3));
  NS_TEST_ASSERT_EQUAL (m_traceNotification, -3);
  NS_TEST_ASSERT_EQUAL (m_tracePath, "/NodeA/NodeB/NodesB/1/Source")
  m_traceNotification = 0;
  m_tracePath = "";
  // this should trigger a notification
  d3->SetAttribute ("Source", Integer (-3));
  NS_TEST_ASSERT_EQUAL (m_traceNotification, -3);
  NS_TEST_ASSERT_EQUAL (m_tracePath, "/NodeA/NodeB/NodesB/3/Source");
  Config::Disconnect ("/NodeA/NodeB/NodesB/[0-1]|3/Source", 
				 MakeCallback (&ConfigTest::ChangeNotificationWithPath, this));
  m_traceNotification = 0;
  // this should _not_ trigger a notification
  d1->SetAttribute ("Source", Integer (-4));
  NS_TEST_ASSERT_EQUAL (m_traceNotification, 0);



  return result;
}

} // namespace ns3


#endif /* RUN_SELF_TEST */
