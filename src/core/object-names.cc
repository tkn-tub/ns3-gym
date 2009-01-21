/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
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
 */

#include <map>
#include "ns3/object.h"
#include "ns3/log.h"
#include "ns3/assert.h"
#include "ns3/abort.h"
#include "ns3/simulator.h"
#include "object-names.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("Names");

class NameNode
{
public:
  NameNode ();
  NameNode (const NameNode &nameNode);
  NameNode (NameNode *parent, std::string name, Ptr<Object> object);
  NameNode &operator = (const NameNode &rhs);

 ~NameNode ();

  NameNode *m_parent;
  std::string m_name;
  Ptr<Object> m_object;

  std::map<std::string, NameNode *> m_nameMap;
};

NameNode::NameNode ()
  : m_parent (0), m_name (""), m_object (0)
{
}

NameNode::NameNode (const NameNode &nameNode)
{
  m_parent = nameNode.m_parent;
  m_name = nameNode.m_name;
  m_object = nameNode.m_object;
  m_nameMap = nameNode.m_nameMap;
}

NameNode &
NameNode::operator = (const NameNode &rhs)
{
  m_parent = rhs.m_parent;
  m_name = rhs.m_name;
  m_object = rhs.m_object;
  m_nameMap = rhs.m_nameMap;
  return *this;
}

NameNode::NameNode (NameNode *parent, std::string name, Ptr<Object> object)
  : m_parent (parent), m_name (name), m_object (object)
{
}

NameNode::~NameNode ()
{
}

class NamesPriv 
{
public:
  NamesPriv ();
  ~NamesPriv ();

  bool Add (std::string name, Ptr<Object> obj);
  bool Add (Ptr<Object> context, std::string name, Ptr<Object> object);
  bool Add (std::string context, std::string name, Ptr<Object> object);
  std::string FindShortName (Ptr<Object> object);
  std::string FindFullName (Ptr<Object> object);
  Ptr<Object> FindObjectFromFullName (std::string name);
  Ptr<Object> FindObjectFromShortName (Ptr<Object> context, std::string name);

  static NamesPriv *Get (void);
  
private:
  static NamesPriv **DoGet (void);
  static void Delete (void);

  NameNode *IsNamed (Ptr<Object>);
  bool IsDuplicateName (NameNode *node, std::string name);

  NameNode m_root;
  std::map<Ptr<Object>, NameNode *> m_objectMap;
};

NamesPriv *
NamesPriv::Get (void)
{
  return *(DoGet ());
}

NamesPriv **
NamesPriv::DoGet (void)
{
  static NamesPriv *ptr = 0;

  if (ptr == 0)
    {
      ptr = new NamesPriv;
      Simulator::ScheduleDestroy (&NamesPriv::Delete);
    }

  return &ptr;
}

void 
NamesPriv::Delete (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  NamesPriv **ptr = DoGet ();
  delete *ptr;
  *ptr = 0;
}

NamesPriv::NamesPriv ()
{
  NS_LOG_FUNCTION_NOARGS ();

  m_root.m_parent = 0;
  m_root.m_name = "Names";
  m_root.m_object = 0;
}

NamesPriv::~NamesPriv ()
{
  NS_LOG_FUNCTION_NOARGS ();

  //
  // Every name is associated with an object in the object map, so freeing the
  // NameNodes in this map will free all of the memory allocated for the NameNodes
  //
  for (std::map<Ptr<Object>, NameNode *>::iterator i = m_objectMap.begin (); i != m_objectMap.end (); ++i)
    {
      delete i->second;
      i->second = 0;
    }
}

bool
NamesPriv::Add (std::string name, Ptr<Object> object)
{
  NS_LOG_FUNCTION (name << object);
  //
  // This is the simple, easy to use version of Add, so we want it to be flexible.
  //
  // If we are provided a name that doesn't begin with "/Names", we assume 
  // that the caller has given us a shortname that she wants added to the root
  // namespace.  This results in a call to the "real" Add with context set to 
  // zero, indicating what we want to do.
  //
  // If we are given a name that begins with "/Names/" we assume that this is a
  // fullname to the object we want to create.  We split the fullname into a 
  // context string and and a final segment and then call the "Real" Add.
  //
  std::string namespaceName = "/Names";
  std::string::size_type offset = name.find (namespaceName);
  if (offset == 0)
    {
      //
      // This must be a fully qualified longname.  All fully qualified names begin
      // with "/Names".  We have to split off the final segment which will become
      // the shortname of the object.
      //
      std::string::size_type i = name.rfind ("/");
      NS_ASSERT_MSG (i != std::string::npos, "NamesPriv::Add(): Internal error.  Can't find '/' in name");

      //
      // The slash we found cannot be the slash at the start of the namespaceName.
      // This would indicate there is no shortname in the path at all.
      //
      NS_ASSERT_MSG (i != 0, "NamesPriv::Add(): Can't find a shortname in the name string");

      //
      // We now know where the context string starts and ends, and where the
      // shortname starts and ends.  All we have to do is to call our available
      // function for creating addubg a shortname under a context string.
      //
      return Add (name.substr (0, i), name.substr (i + 1), object);
    }
  else
    {
      //
      // This must be a shortname.  Shortnames can't have ANY '/' characters in
      // them since they are interpreted as a final segment of a fullname.  A 
      // shortname in this context means creating a name in the root namespace.
      // We indicate this by passing a zero context to the "real" add.
      //
      NS_ASSERT_MSG (offset == std::string::npos, "NamesPriv::Add(): Unexpected '/' in shortname");
      return Add (Ptr<Object> (0, false), name, object);
    }
}

bool
NamesPriv::Add (Ptr<Object> context, std::string name, Ptr<Object> object)
{
  NS_LOG_FUNCTION (context << name << object);

  if (IsNamed (object))
    {
      NS_LOG_LOGIC ("Object is already named");
      return false;
    }

  NameNode *node = 0;
  if (context)
    {
      node = IsNamed (context);
      NS_ASSERT_MSG (node, "NamesPriv::Name(): context must point to a previously named node");
    }
  else
    {
      node = &m_root;
    }

  if (IsDuplicateName (node, name))
    {
      NS_LOG_LOGIC ("Name is already taken");
      return false;
    }

  NameNode *newNode = new NameNode(node, name, object);
  node->m_nameMap[name] = newNode;
  m_objectMap[object] = newNode;

  return true;
}

bool
NamesPriv::Add (std::string context, std::string name, Ptr<Object> object)
{
  if (context == "/Names")
    {
      return Add (Ptr<Object> (0, false), name, object);
    }
  return Add (FindObjectFromFullName (context), name, object);
}

std::string
NamesPriv::FindShortName (Ptr<Object> object)
{
  NS_LOG_FUNCTION (object);

  std::map<Ptr<Object>, NameNode *>::iterator i = m_objectMap.find (object);
  if (i == m_objectMap.end ())
    {
      NS_LOG_LOGIC ("Object does not exist in object map");
      return "";
    }
  else
    {
      NS_LOG_LOGIC ("Object exists in object map");
      return i->second->m_name;
    }
}

std::string
NamesPriv::FindFullName (Ptr<Object> object)
{
  NS_LOG_FUNCTION (object);

  std::map<Ptr<Object>, NameNode *>::iterator i = m_objectMap.find (object);
  if (i == m_objectMap.end ())
    {
      NS_LOG_LOGIC ("Object does not exist in object map");
      return "";
    }

  NameNode *p = i->second;
  NS_ASSERT_MSG (p, "NamesPriv::FindFullName(): Internal error: Invalid NameNode pointer from map");

  std::string fullname;

  do
    {
      fullname = "/" + p->m_name + fullname;
      NS_LOG_LOGIC ("fullname is " << fullname);
    }
  while ((p = p->m_parent) != 0);

  return fullname;
}


Ptr<Object>
NamesPriv::FindObjectFromFullName (std::string name)
{
  std::string namespaceName = "/Names/";
  std::string::size_type offset = name.find (namespaceName);
  if (offset == std::string::npos)
    {
      NS_LOG_LOGIC (name << " is not in the " << namespaceName << " name space");
      return 0;
    }

  std::string remaining = name.substr (namespaceName.size ());
  NameNode *node = &m_root;

  //
  // remaining is now composed entirely of path segments in the /Names name space.
  // and we have eaten the leading slash. e.g., remaining = "ClientNode/eth0"
  // The start of the search is at the root of the name space.
  //
  for (;;)
    {
      NS_LOG_LOGIC ("Looking for the object of name " << remaining);
      offset = remaining.find ("/");
      if (offset == std::string::npos)
        {
          //
          // There are no remaining slashes so this is the last segment of the 
          // specified name.  We're done when we find it
          //
          std::map<std::string, NameNode *>::iterator i = node->m_nameMap.find (remaining);
          if (i == node->m_nameMap.end ())
            {
              NS_LOG_LOGIC ("Name does not exist in name map");
              return 0;
            }
          else
            {
              NS_LOG_LOGIC ("Name parsed, found object");
              return i->second->m_object;
            }
        }
      else
        {
          //
          // There are more slashes so this is an intermediate segment of the 
          // specified name.  We need to "recurse" when we find this segment.
          //
          offset = remaining.find ("/");
          std::string segment = remaining.substr(0, offset);

          std::map<std::string, NameNode *>::iterator i = node->m_nameMap.find (segment);
          if (i == node->m_nameMap.end ())
            {
              NS_LOG_LOGIC ("Name does not exist in name map");
              return 0;
            }
          else
            {
              node = i->second;
              remaining = remaining.substr (offset + 1);
              NS_LOG_LOGIC ("Intermediate segment parsed");
              continue;
            }
        }
    }

  NS_ASSERT_MSG (node, "NamesPriv::FindObjectFromFullName(): Internal error:  this can't happen");
  return 0;
}

Ptr<Object>
NamesPriv::FindObjectFromShortName (Ptr<Object> context, std::string name)
{
  NS_LOG_FUNCTION (context << name);

  NameNode *node = 0;

  if (context == 0)
    {
      NS_LOG_LOGIC ("Zero context implies root NameNode");
      node = &m_root;
    }
  else
    {
      node = IsNamed (context);
      if (node == 0)
        {
          NS_LOG_LOGIC ("Context does not point to a previously named node");
          return 0;
        }
    }

  std::map<std::string, NameNode *>::iterator i = node->m_nameMap.find (name);
  if (i == node->m_nameMap.end ())
    {
      NS_LOG_LOGIC ("Name does not exist in name map");
      return 0;
    }
  else
    {
      NS_LOG_LOGIC ("Name exists in name map");
      return i->second->m_object;
    }
}

NameNode *
NamesPriv::IsNamed (Ptr<Object> object)
{
  NS_LOG_FUNCTION (object);

  std::map<Ptr<Object>, NameNode *>::iterator i = m_objectMap.find (object);
  if (i == m_objectMap.end ())
    {
      NS_LOG_LOGIC ("Object does not exist in object map, returning NameNode 0");
      return 0;
    }
  else
    {
      NS_LOG_LOGIC ("Object exists in object map, returning NameNode " << &i->second);
      return i->second;
    }
}

bool
NamesPriv::IsDuplicateName (NameNode *node, std::string name)
{
  NS_LOG_FUNCTION (node << name);

  std::map<std::string, NameNode *>::iterator i = node->m_nameMap.find (name);
  if (i == node->m_nameMap.end ())
    {
      NS_LOG_LOGIC ("Name does not exist in name map");
      return false;
    }
  else
    {
      NS_LOG_LOGIC ("Name exists in name map");
      return true;
    }
}

bool
Names::Add (std::string name, Ptr<Object> object)
{
  return NamesPriv::Get ()->Add (name, object);
}

bool
Names::Add (Ptr<Object> context, std::string name, Ptr<Object> object)
{
  return NamesPriv::Get ()->Add (context, name, object);
}

bool
Names::Add (std::string context, std::string name, Ptr<Object> object)
{
  return NamesPriv::Get ()->Add (context, name, object);
}

std::string
Names::FindShortName (Ptr<Object> object)
{
  return NamesPriv::Get ()->FindShortName (object);
}

std::string
Names::FindFullName (Ptr<Object> object)
{
  return NamesPriv::Get ()->FindFullName (object);
}

Ptr<Object>
Names::FindObjectFromFullNameInternal (std::string name)
{
  return NamesPriv::Get ()->FindObjectFromFullName (name);
}

Ptr<Object>
Names::FindObjectFromShortNameInternal (Ptr<Object> context, std::string name)
{
  return NamesPriv::Get ()->FindObjectFromShortName (context, name);
}

} //namespace ns3

#ifdef RUN_SELF_TESTS

#include "test.h"
#include "object-factory.h"

namespace ns3 {

class TestObject : public Object
{
public:
  static TypeId GetTypeId (void) 
  {
    static TypeId tid = TypeId ("TestObject")
      .SetParent (Object::GetTypeId ())
      .HideFromDocumentation ()
      .AddConstructor<TestObject> ();
    return tid;
  }
  TestObject () {}
  virtual void Dispose (void) {}
};

class NamesTest : public Test
{
public:
  NamesTest ();
  virtual bool RunTests (void);
};

NamesTest::NamesTest ()
  : Test ("Names")
{
}

bool 
NamesTest::RunTests (void)
{
  bool result = true;

  // 
  // Name a couple of objects at the root level
  //
  Ptr<TestObject> client = CreateObject<TestObject> ();
  result = Names::Add ("Client", client);
  NS_TEST_ASSERT_EQUAL (result, true);

  Ptr<TestObject> server = CreateObject<TestObject> ();
  result = Names::Add ("Server", server);
  NS_TEST_ASSERT_EQUAL (result, true);

  //
  // We shouldn't be able to add another name to a previously named object
  //
  result = Names::Add ("Not Client", client);
  NS_TEST_ASSERT_EQUAL (result, false);

  //
  // We shouldn't be able to duplicate a name at the root level.
  //
  Ptr<TestObject> secondClient = CreateObject<TestObject> ();
  result = Names::Add ("Client", secondClient);
  NS_TEST_ASSERT_EQUAL (result, false);

  //
  // We should be able to add a new name in the first object's context
  //
  Ptr<TestObject> clientEth0 = CreateObject<TestObject> ();
  result = Names::Add (client, "eth0", clientEth0);
  NS_TEST_ASSERT_EQUAL (result, true);

  //
  // We shouldn't be able to duplicate a name in that context.
  //
  Ptr<TestObject> secondClientEth0 = CreateObject<TestObject> ();
  result = Names::Add (client, "eth0", secondClientEth0);
  NS_TEST_ASSERT_EQUAL (result, false);

  //
  // We should be able to add the same name in the second object's context
  //
  Ptr<TestObject> serverEth0 = CreateObject<TestObject> ();
  result = Names::Add (server, "eth0", serverEth0);
  NS_TEST_ASSERT_EQUAL (result, true);

  //
  // We should be able to find the short names for the objects we created
  //
  std::string found;

  found = Names::FindShortName (client);
  NS_TEST_ASSERT_EQUAL (found, "Client");

  found = Names::FindShortName (server);
  NS_TEST_ASSERT_EQUAL (found, "Server");

  found = Names::FindShortName (clientEth0);
  NS_TEST_ASSERT_EQUAL (found, "eth0");

  found = Names::FindShortName (serverEth0);
  NS_TEST_ASSERT_EQUAL (found, "eth0");

  //
  // We should be able to find the full names for the objects we created
  //
  found = Names::FindFullName (client);
  NS_TEST_ASSERT_EQUAL (found, "/Names/Client");

  found = Names::FindFullName (server);
  NS_TEST_ASSERT_EQUAL (found, "/Names/Server");

  found = Names::FindFullName (clientEth0);
  NS_TEST_ASSERT_EQUAL (found, "/Names/Client/eth0");

  found = Names::FindFullName (serverEth0);
  NS_TEST_ASSERT_EQUAL (found, "/Names/Server/eth0");

  // 
  // We should be able to find the objects from the short names
  //
  Ptr<TestObject> foundObject;

  foundObject = Names::FindObjectFromShortName<TestObject> (0, "Client");
  NS_TEST_ASSERT_EQUAL (foundObject, client);

  foundObject = Names::FindObjectFromShortName<TestObject> (0, "Server");
  NS_TEST_ASSERT_EQUAL (foundObject, server);

  foundObject = Names::FindObjectFromShortName<TestObject> (client, "eth0");
  NS_TEST_ASSERT_EQUAL (foundObject, clientEth0);

  foundObject = Names::FindObjectFromShortName<TestObject> (server, "eth0");
  NS_TEST_ASSERT_EQUAL (foundObject, serverEth0);

  // 
  // We should be able to find the objects from their full names
  //
  foundObject = Names::Find<TestObject> ("/Names/Client");
  NS_TEST_ASSERT_EQUAL (foundObject, client);

  foundObject = Names::Find<TestObject> ("/Names/Server");
  NS_TEST_ASSERT_EQUAL (foundObject, server);

  foundObject = Names::Find<TestObject> ("/Names/Client/eth0");
  NS_TEST_ASSERT_EQUAL (foundObject, clientEth0);

  foundObject = Names::Find<TestObject> ("/Names/Server/eth0");
  NS_TEST_ASSERT_EQUAL (foundObject, serverEth0);

  //
  // We also have some syntactically sugary methods, so make sure they do what
  // they should as well.
  //
  Ptr<TestObject> bridge = CreateObject<TestObject> ();
  result = Names::Add ("/Names", "Bridge", bridge);
  NS_TEST_ASSERT_EQUAL (result, true);

  Ptr<TestObject> bridgeEth0 = CreateObject<TestObject> ();
  result = Names::Add ("/Names/Bridge", "eth0", bridgeEth0);
  NS_TEST_ASSERT_EQUAL (result, true);

  foundObject = Names::Find<TestObject> ("/Names/Bridge");
  NS_TEST_ASSERT_EQUAL (foundObject, bridge);

  foundObject = Names::Find<TestObject> ("/Names/Bridge/eth0");
  NS_TEST_ASSERT_EQUAL (foundObject, bridgeEth0);

  Ptr<TestObject> wireless = CreateObject<TestObject> ();
  result = Names::Add ("/Names/Wireless", wireless);
  NS_TEST_ASSERT_EQUAL (result, true);

  Ptr<TestObject> wirelessAth0 = CreateObject<TestObject> ();
  result = Names::Add ("/Names/Wireless/ath0", wirelessAth0);
  NS_TEST_ASSERT_EQUAL (result, true);

  foundObject = Names::Find<TestObject> ("/Names/Wireless");
  NS_TEST_ASSERT_EQUAL (foundObject, wireless);

  foundObject = Names::Find<TestObject> ("/Names/Wireless/ath0");
  NS_TEST_ASSERT_EQUAL (foundObject, wirelessAth0);

  //
  // Run the simulator and destroy it to get the Destroy method called on the
  // private implementation object.  We depend on seeing a valgrind-clean run of
  // the unit tests to really determine if the clean up was really successful.
  //
  Simulator::Run ();
  Simulator::Destroy ();
  
  return true;
}

static NamesTest g_namesTests;

} // namespace ns3

#endif /* RUN_SELF_TESTS */
