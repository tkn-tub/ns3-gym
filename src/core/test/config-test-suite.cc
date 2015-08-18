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
#include "ns3/config.h"
#include "ns3/test.h"
#include "ns3/integer.h"
#include "ns3/traced-value.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/callback.h"

#include "ns3/singleton.h"
#include "ns3/object.h"
#include "ns3/object-vector.h"
#include "ns3/names.h"
#include "ns3/pointer.h"
#include "ns3/log.h"


#include <sstream>

using namespace ns3;

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
                     MakeTraceSourceAccessor (&ConfigTestObject::m_trace),
                     "ns3::TracedValueCallback::Int16")
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

// Other test objects
//

class DerivedConfigTestObject : public ConfigTestObject
{
public:
  static TypeId GetTypeId (void);
  DerivedConfigTestObject (void) {}
  virtual ~DerivedConfigTestObject (void) {}
};

TypeId
DerivedConfigTestObject::GetTypeId (void)
{
  static TypeId tid = TypeId ("DerivedConfigTestObject")
    .SetParent<ConfigTestObject> ()
    ;
  return tid;
}

class BaseConfigObject : public Object
{
public:
  static TypeId GetTypeId (void);
  BaseConfigObject (void) : m_x(15) {}
  virtual ~BaseConfigObject (void) {}
private:
  int8_t m_x;
  void Increment (void) { m_x++; } // silence unused variable warning
};

TypeId
BaseConfigObject::GetTypeId (void)
{
  static TypeId tid = TypeId ("BaseConfigObject")
    .SetParent<Object> ()
    .AddAttribute ("X", "",
                   IntegerValue (10),
                   MakeIntegerAccessor (&BaseConfigObject::m_x),
                   MakeIntegerChecker<int8_t> ())
    ;
  return tid;
}

class DerivedConfigObject : public BaseConfigObject
{
public:
  static TypeId GetTypeId (void);
  DerivedConfigObject (void) {}
  virtual ~DerivedConfigObject (void) {}
};

TypeId
DerivedConfigObject::GetTypeId (void)
{
  static TypeId tid = TypeId ("DerivedConfigObject")
    .SetParent<BaseConfigObject> ()
    ;
  return tid;
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
  virtual void DoRun (void);
};

RootNamespaceConfigTestCase::RootNamespaceConfigTestCase ()
  : TestCase ("Check ability to register a root namespace and use it")
{
}

void
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
  virtual void DoRun (void);
};

UnderRootNamespaceConfigTestCase::UnderRootNamespaceConfigTestCase ()
  : TestCase ("Check ability to register an object under the root namespace and use it")
{
}

void
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


  //
  // Try '*' for attributes
  //
  Config::Set ("/*/A", IntegerValue (2));
  a->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), 2, "Object Attribute \"A\" not set correctly");
  b->GetAttribute ("A", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), 4, "Object Attribute \"A\" not set correctly");
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
  virtual void DoRun (void);
};

ObjectVectorConfigTestCase::ObjectVectorConfigTestCase ()
  : TestCase ("Check ability to configure vectors of Object using regular expressions")
{
}

void
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
}

// ===========================================================================
// Test for the ability to trace configure with vectors of objects.
// ===========================================================================
class ObjectVectorTraceConfigTestCase : public TestCase
{
public:
  ObjectVectorTraceConfigTestCase ();
  virtual ~ObjectVectorTraceConfigTestCase () {}

  void Trace (int16_t oldValue, int16_t newValue) { m_newValue = newValue; }
  void TraceWithPath (std::string path, int16_t old, int16_t newValue) { m_newValue = newValue; m_path = path; }

private:
  virtual void DoRun (void);

  int16_t m_newValue;
  std::string m_path;
};

ObjectVectorTraceConfigTestCase::ObjectVectorTraceConfigTestCase ()
  : TestCase ("Check ability to trace connect through vectors of Object using regular expressions")
{
}

void
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
}

// ===========================================================================
// Test for the ability to search attributes of parent classes
// when Resolver searches for attributes in a derived class object.
// This test passes with the patch found in
// https://www.nsnam.org/bugzilla/show_bug.cgi?id=1673
// (also reported in https://www.nsnam.org/bugzilla/show_bug.cgi?id=1959)
// ===========================================================================
class SearchAttributesOfParentObjectsTestCase : public TestCase
{
public:
  SearchAttributesOfParentObjectsTestCase ();
  virtual ~SearchAttributesOfParentObjectsTestCase () {}

private:
  virtual void DoRun (void);

};

SearchAttributesOfParentObjectsTestCase::SearchAttributesOfParentObjectsTestCase ()
  : TestCase ("Check that attributes of base class are searchable from paths including objects of derived class")
{
}

void
SearchAttributesOfParentObjectsTestCase::DoRun (void)
{
  IntegerValue iv;
  //
  // Create a root namespace object that doesn't have attributes but
  // whose parent class has 'NodeA' attribute
  //
  Ptr<DerivedConfigTestObject> root = CreateObject<DerivedConfigTestObject> ();
  Config::RegisterRootNamespaceObject (root);

  //
  //  Instantiate /NodeA
  //
  Ptr<DerivedConfigTestObject> a = CreateObject<DerivedConfigTestObject> ();
  root->SetNodeA (a);

  //
  // BaseConfigObject has attribute X, but we aggregate DerivedConfigObject 
  // instead
  //
  Ptr<DerivedConfigObject> derived = CreateObject<DerivedConfigObject> ();
  a->AggregateObject (derived);
  Config::Set ("/NodeA/$DerivedConfigObject/X", IntegerValue (42));
  derived->GetAttribute ("X", iv);
  NS_TEST_ASSERT_MSG_EQ (iv.Get (), 42, "Object Attribute \"X\" not settable in derived class");

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
  : TestSuite ("config", UNIT)
{
  AddTestCase (new RootNamespaceConfigTestCase, TestCase::QUICK);
  AddTestCase (new UnderRootNamespaceConfigTestCase, TestCase::QUICK);
  AddTestCase (new ObjectVectorConfigTestCase, TestCase::QUICK);
  AddTestCase (new SearchAttributesOfParentObjectsTestCase, TestCase::QUICK);
}

static ConfigTestSuite configTestSuite;
