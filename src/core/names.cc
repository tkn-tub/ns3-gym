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
#include "object.h"
#include "log.h"
#include "assert.h"
#include "abort.h"
#include "names.h"
#include "ns3/simulator.h"

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
  bool Rename (std::string oldname, std::string newname);
  bool Add (std::string context, std::string name, Ptr<Object> object);
  bool Rename (std::string context, std::string oldname, std::string newname);
  bool Add (Ptr<Object> context, std::string name, Ptr<Object> object);
  bool Rename (Ptr<Object> context, std::string oldname, std::string newname);
  std::string FindName (Ptr<Object> object);
  std::string FindPath (Ptr<Object> object);
  Ptr<Object> Find (std::string name);
  Ptr<Object> Find (std::string path, std::string name);
  Ptr<Object> Find (Ptr<Object> context, std::string name);

  static NamesPriv *Get (void);
  static void Delete (void);
private:
  static NamesPriv **DoGet (void);

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

  m_root.m_parent = 0;
  m_root.m_name = "";
  m_root.m_object = 0;
}

bool
NamesPriv::Add (std::string name, Ptr<Object> object)
{
  NS_LOG_FUNCTION (name << object);
  //
  // This is the simple, easy to use version of Add, so we want it to be flexible.
  // We don't want to force a user to always type the fully qualified namespace 
  // name, so we allow the namespace name to be omitted.  For example, calling
  // Add ("Client/ath0", obj) should result in exactly the same behavior as
  // Add ("/Names/Client/ath0", obj).  Calling Add ("Client", obj) should have
  // the same effect as Add ("Names/Client", obj)
  //
  // The first thing to do, then, is to "canonicalize" the input string to always
  // be a fully qualified name.
  //
  // If we are given a name that begins with "/Names/" we assume that this is a
  // fullname to the object we want to create.  We split the fullname into a 
  // context string and and a final segment and then call the "Real" Add.
  //
  std::string namespaceName = "/Names";
  std::string::size_type offset = name.find (namespaceName);
  if (offset != 0)
    {
      //
      // This must be a name that has the "/Names" namespace prefix omitted.  
      // Do some reasonableness checking on the rest of the name.
      //
      offset = name.find ("/");
      if (offset == 0)
        {
          NS_ASSERT_MSG (false, "NamesPriv::Add(): Name begins with '/' but not \"/Names\"");
          return false;
        }

      name = "/Names/" + name;
    }
  
  //
  // There must now be a fully qualified longname in the string.  All fully 
  // qualified names begin with "/Names".  We have to split off the final 
  // segment which will become the shortname of the object.  A '/' that
  // separates the context from the final segment had better be there since
  // we just made sure that at least the namespace name was there.
  //
  std::string::size_type i = name.rfind ("/");
  NS_ASSERT_MSG (i != std::string::npos, "NamesPriv::Add(): Internal error.  Can't find '/' in name");

  //
  // The slash we found cannot be the slash at the start of the namespaceName.
  // This would indicate there is no shortname in the path at all.  It can be
  // any other index.
  //
  NS_ASSERT_MSG (i != 0, "NamesPriv::Add(): Can't find a shortname in the name string");

  //
  // We now know where the context string starts and ends, and where the
  // shortname starts and ends.  All we have to do is to call our available
  // function for creating addubg a shortname under a context string.
  //
  return Add (name.substr (0, i), name.substr (i + 1), object);
}

bool
NamesPriv::Rename (std::string oldname, std::string newname)
{
  NS_LOG_FUNCTION (oldname << newname);
  //
  // This is the simple, easy to use version of Rename, so we want it to be 
  // flexible.   We don't want to force a user to always type the fully 
  // qualified namespace name, so we allow the namespace name to be omitted.
  // For example, calling Rename ("Client/ath0", "eth0") should result in 
  // exactly the same behavior as Rename ("/Names/Client/ath0", "eth0").
  // Calling Rename ("Client", "Router") should have the same effect as 
  // Rename ("Names/Client", "Router")
  //
  // The first thing to do, then, is to "canonicalize" the input string to always
  // be a fully qualified name.
  //
  // If we are given a name that begins with "/Names/" we assume that this is a
  // fullname to the object we want to change.  We split the fullname into a 
  // context string and and a final segment and then call the "Real" Rename.
  //
  std::string namespaceName = "/Names";
  std::string::size_type offset = oldname.find (namespaceName);
  if (offset != 0)
    {
      //
      // This must be a name that has the "/Names" namespace prefix omitted.  
      // Do some reasonableness checking on the rest of the name.
      //
      offset = oldname.find ("/");
      if (offset == 0)
        {
          NS_ASSERT_MSG (false, "NamesPriv::Add(): Name begins with '/' but not \"/Names\"");
          return false;
        }

      oldname = "/Names/" + oldname;
    }
  
  //
  // There must now be a fully qualified longname in the oldname string.  All 
  // fully qualified names begin with "/Names".  We have to split off the final 
  // segment which will become the shortname we want to rename.  A '/' that
  // separates the context from the final segment had better be there since
  // we just made sure that at least the namespace name was there.
  //
  std::string::size_type i = oldname.rfind ("/");
  NS_ASSERT_MSG (i != std::string::npos, "NamesPriv::Add(): Internal error.  Can't find '/' in name");

  //
  // The slash we found cannot be the slash at the start of the namespaceName.
  // This would indicate there is no shortname in the path at all.  It can be
  // any other index.
  //
  NS_ASSERT_MSG (i != 0, "NamesPriv::Add(): Can't find a shortname in the name string");

  //
  // We now know where the context string starts and ends, and where the
  // shortname starts and ends.  All we have to do is to call our available
  // function for creating addubg a shortname under a context string.
  //
  return Rename (oldname.substr (0, i), oldname.substr (i + 1), newname);
}

bool
NamesPriv::Add (std::string path, std::string name, Ptr<Object> object)
{
  if (path == "/Names")
    {
      return Add (Ptr<Object> (0, false), name, object);
    }
  return Add (Find (path), name, object);
}

bool
NamesPriv::Rename (std::string path, std::string oldname, std::string newname)
{
  if (path == "/Names")
    {
      return Rename (Ptr<Object> (0, false), oldname, newname);
    }
  return Rename (Find (path), oldname, newname);
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
NamesPriv::Rename (Ptr<Object> context, std::string oldname, std::string newname)
{
  NS_LOG_FUNCTION (context << oldname << newname);

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

  if (IsDuplicateName (node, newname))
    {
      NS_LOG_LOGIC ("New name is already taken");
      return false;
    }

  std::map<std::string, NameNode *>::iterator i = node->m_nameMap.find (oldname);
  if (i == node->m_nameMap.end ())
    {
      NS_LOG_LOGIC ("Old name does not exist in name map");
      return false;
    }
  else
    {
      NS_LOG_LOGIC ("Old name exists in name map");

      //
      // The rename process consists of:
      // 1.  Geting the pointer to the name node from the map and remembering it;
      // 2.  Removing the map entry corresponding to oldname from the map;
      // 3.  Changing the name string in the name node;
      // 4.  Adding the name node back in the map under the newname.
      //
      NameNode *changeNode = i->second;
      node->m_nameMap.erase (i);
      changeNode->m_name = newname;
      node->m_nameMap[newname] = changeNode;
      return true;
    }
}

std::string
NamesPriv::FindName (Ptr<Object> object)
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
NamesPriv::FindPath (Ptr<Object> object)
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

  std::string path;

  do
    {
      path = "/" + p->m_name + path;
      NS_LOG_LOGIC ("path is " << path);
    }
  while ((p = p->m_parent) != 0);

  return path;
}


Ptr<Object>
NamesPriv::Find (std::string path)
{
  //
  // This is hooked in from simple, easy to use version of Find, so we want it
  // to be flexible.
  //
  // If we are provided a path that doesn't begin with "/Names", we assume 
  // that the caller has simply given us a path starting with a name that
  // is in the root namespace.  This allows peole to omit the "/Names" prefix.
  // and simply do a Find ("Client/eth0") instead of having to always do a
  // Find ("/Names/Client/eth0");
  //
  // So, if we are given a name that begins with "/Names/" the upshot is that we
  // just remove that prefix and treat the rest of the string as starting with a 
  // name in the root namespace.
  //
  std::string namespaceName = "/Names/";
  std::string remaining;

  std::string::size_type offset = path.find (namespaceName);
  if (offset == 0)
    {
      NS_LOG_LOGIC (path << " is a fully qualified name");
      remaining = path.substr (namespaceName.size ());
    }
  else
    {
      NS_LOG_LOGIC (path << " begins with a relative shortname");
      remaining = path;
    }

  NameNode *node = &m_root;

  //
  // The string <remaining> is now composed entirely of path segments in the
  // /Names name space and we have eaten the leading slash. e.g., 
  // remaining = "ClientNode/eth0"
  //
  // The start of the search is always at the root of the name space.
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

  NS_ASSERT_MSG (node, "NamesPriv::Find(): Internal error:  this can't happen");
  return 0;
}

Ptr<Object>
NamesPriv::Find (std::string path, std::string name)
{
  NS_LOG_FUNCTION (path << name);

  if (path == "/Names")
    {
      return Find (Ptr<Object> (0, false), name);
    }
  return Find (Find (path), name);
}

Ptr<Object>
NamesPriv::Find (Ptr<Object> context, std::string name)
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

void
Names::Delete (void)
{
  NamesPriv::Delete ();
}

bool
Names::Add (std::string name, Ptr<Object> object)
{
  return NamesPriv::Get ()->Add (name, object);
}

bool
Names::Rename (std::string oldname, std::string newname)
{
  return NamesPriv::Get ()->Rename (oldname, newname);
}

bool
Names::Add (std::string path, std::string name, Ptr<Object> object)
{
  return NamesPriv::Get ()->Add (path, name, object);
}

bool
Names::Rename (std::string path, std::string oldname, std::string newname)
{
  return NamesPriv::Get ()->Rename (path, oldname, newname);
}

bool
Names::Add (Ptr<Object> context, std::string name, Ptr<Object> object)
{
  return NamesPriv::Get ()->Add (context, name, object);
}

bool
Names::Rename (Ptr<Object> context, std::string oldname, std::string newname)
{
  return NamesPriv::Get ()->Rename (context, oldname, newname);
}

std::string
Names::FindName (Ptr<Object> object)
{
  return NamesPriv::Get ()->FindName (object);
}

std::string
Names::FindPath (Ptr<Object> object)
{
  return NamesPriv::Get ()->FindPath (object);
}

Ptr<Object>
Names::FindInternal (std::string name)
{
  return NamesPriv::Get ()->Find (name);
}

Ptr<Object>
Names::FindInternal (std::string path, std::string name)
{
  return NamesPriv::Get ()->Find (path, name);
}

Ptr<Object>
Names::FindInternal (Ptr<Object> context, std::string name)
{
  return NamesPriv::Get ()->Find (context, name);
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

  found = Names::FindName (client);
  NS_TEST_ASSERT_EQUAL (found, "Client");

  found = Names::FindName (server);
  NS_TEST_ASSERT_EQUAL (found, "Server");

  found = Names::FindName (clientEth0);
  NS_TEST_ASSERT_EQUAL (found, "eth0");

  found = Names::FindName (serverEth0);
  NS_TEST_ASSERT_EQUAL (found, "eth0");

  //
  // We should be able to find the full names for the objects we created
  //
  found = Names::FindPath (client);
  NS_TEST_ASSERT_EQUAL (found, "/Names/Client");

  found = Names::FindPath (server);
  NS_TEST_ASSERT_EQUAL (found, "/Names/Server");

  found = Names::FindPath (clientEth0);
  NS_TEST_ASSERT_EQUAL (found, "/Names/Client/eth0");

  found = Names::FindPath (serverEth0);
  NS_TEST_ASSERT_EQUAL (found, "/Names/Server/eth0");

  // 
  // We should be able to find the objects from the short names.  Note that
  // the Ptr<Object> (0, false) below is to differentiate a null object pointer
  // from a null string pointer -- not normally needed in real use-cases.
  // 
  //
  Ptr<TestObject> foundObject;

  foundObject = Names::Find<TestObject> (Ptr<Object> (0, false), "Client");
  NS_TEST_ASSERT_EQUAL (foundObject, client);

  foundObject = Names::Find<TestObject> (Ptr<Object> (0, false), "Server");
  NS_TEST_ASSERT_EQUAL (foundObject, server);

  foundObject = Names::Find<TestObject> (client, "eth0");
  NS_TEST_ASSERT_EQUAL (foundObject, clientEth0);

  foundObject = Names::Find<TestObject> (server, "eth0");
  NS_TEST_ASSERT_EQUAL (foundObject, serverEth0);

  // 
  // We should be able to find the objects from their full path names
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
  // We should be able to omit the root of the namespace from the full path names
  //
  foundObject = Names::Find<TestObject> ("Client");
  NS_TEST_ASSERT_EQUAL (foundObject, client);

  foundObject = Names::Find<TestObject> ("Server");
  NS_TEST_ASSERT_EQUAL (foundObject, server);

  foundObject = Names::Find<TestObject> ("Client/eth0");
  NS_TEST_ASSERT_EQUAL (foundObject, clientEth0);

  foundObject = Names::Find<TestObject> ("Server/eth0");
  NS_TEST_ASSERT_EQUAL (foundObject, serverEth0);

  //
  // We should be able to add objects while including the root of the namespace
  // in the name.
  //
  Ptr<TestObject> router1 = CreateObject<TestObject> ();
  result = Names::Add ("/Names/Router1", router1);
  NS_TEST_ASSERT_EQUAL (result, true);

  //
  // We should be able to add objects while not including the root of the namespace
  // in the name.
  //
  Ptr<TestObject> router2 = CreateObject<TestObject> ();
  result = Names::Add ("Router2", router2);
  NS_TEST_ASSERT_EQUAL (result, true);

  //
  // We should be able to add sub-objects while including the root of the namespace
  // in the name.
  //
  Ptr<TestObject> router1Eth0 = CreateObject<TestObject> ();
  result = Names::Add ("/Names/Router1/eth0", router1Eth0);
  NS_TEST_ASSERT_EQUAL (result, true);

  //
  // We should be able to add sub-objects while not including the root of the namespace
  // in the name.
  //
  Ptr<TestObject> router2Eth0 = CreateObject<TestObject> ();
  result = Names::Add ("Router2/eth0", router2Eth0);
  NS_TEST_ASSERT_EQUAL (result, true);

  //
  // We should be able to find these objects in the same two ways
  //
  foundObject = Names::Find<TestObject> ("/Names/Router1");
  NS_TEST_ASSERT_EQUAL (foundObject, router1);

  foundObject = Names::Find<TestObject> ("Router1");
  NS_TEST_ASSERT_EQUAL (foundObject, router1);

  foundObject = Names::Find<TestObject> ("/Names/Router2");
  NS_TEST_ASSERT_EQUAL (foundObject, router2);

  foundObject = Names::Find<TestObject> ("Router2");
  NS_TEST_ASSERT_EQUAL (foundObject, router2);

  foundObject = Names::Find<TestObject> ("/Names/Router1/eth0");
  NS_TEST_ASSERT_EQUAL (foundObject, router1Eth0);

  foundObject = Names::Find<TestObject> ("Router1/eth0");
  NS_TEST_ASSERT_EQUAL (foundObject, router1Eth0);

  foundObject = Names::Find<TestObject> ("/Names/Router2/eth0");
  NS_TEST_ASSERT_EQUAL (foundObject, router2Eth0);

  foundObject = Names::Find<TestObject> ("Router2/eth0");
  NS_TEST_ASSERT_EQUAL (foundObject, router2Eth0);

  //
  // We have a pile of names defined.  We should be able to rename them in the
  // usual ways.
  //
  result = Names::Rename ("/Names/Router1", "RouterX");
  NS_TEST_ASSERT_EQUAL (result, true);

  foundObject = Names::Find<TestObject> ("/Names/RouterX");
  NS_TEST_ASSERT_EQUAL (foundObject, router1);

  result = Names::Rename ("Router2", "RouterY");
  NS_TEST_ASSERT_EQUAL (result, true);

  foundObject = Names::Find<TestObject> ("RouterY");
  NS_TEST_ASSERT_EQUAL (foundObject, router2);

  result = Names::Rename ("/Names/RouterX/eth0", "ath0");
  NS_TEST_ASSERT_EQUAL (result, true);

  foundObject = Names::Find<TestObject> ("/Names/RouterX/ath0");
  NS_TEST_ASSERT_EQUAL (foundObject, router1Eth0);

  foundObject = Names::Find<TestObject> ("RouterX/ath0");
  NS_TEST_ASSERT_EQUAL (foundObject, router1Eth0);

  //
  // We should not be able to rename an object into conflict with another
  // object.
  //

  result = Names::Rename ("/Names/RouterX", "RouterY");
  NS_TEST_ASSERT_EQUAL (result, false);

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
