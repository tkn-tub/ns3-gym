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
#ifdef RUN_SELF_TESTS
#include "test.h"
#include "object.h"
#include "boolean.h"
#include "integer.h"
#include "uinteger.h"
#include "enum.h"
#include "string.h"
#include "random-variable.h"
#include "double.h"
#include "object-vector.h"
#include "traced-value.h"
#include "trace-source-accessor.h"
#include "pointer.h"

namespace ns3 {

class ValueClassTest 
{
public:
  ValueClassTest () {}
private:
  int m_v;
};
bool operator != (const ValueClassTest &a, const ValueClassTest &b)
{
  return true;
}
std::ostream & operator << (std::ostream &os, ValueClassTest v)
{
  return os;
}
std::istream & operator >> (std::istream &is, ValueClassTest &v)
{
  return is;
}
ATTRIBUTE_HELPER_HEADER_2 (ValueClassTest);
ATTRIBUTE_HELPER_CPP (ValueClassTest);

class AttributeTest : public Test
{
public:
  AttributeTest ();
  virtual bool RunTests (void);
private:
  void NotifySource1 (int8_t old, int8_t n) {
    m_got1 = n;
  }
  void NotifySource2 (double a, int b, float c) {
    m_got2 = a;
  }
  void NotifySourceValue (ValueClassTest old, ValueClassTest n) {
    m_gotValue = n;
  }
  int64_t m_got1;
  double m_got2;
  ValueClassTest m_gotValue;
};

class Derived : public Object
{
public:
  static TypeId GetTypeId (void) {
    static TypeId tid = TypeId ("Derived")
      .SetParent<Object> ()
      ;
    return tid;
  }
};

class AttributeObjectTest : public Object
{
public:
  enum Test_e {
    TEST_A,
    TEST_B,
    TEST_C
  };
  static TypeId GetTypeId (void) {
    static TypeId tid = TypeId ("ns3::AttributeObjectTest")
      .SetParent<Object> ()
      .HideFromDocumentation ()
      .AddAttribute ("TestBoolName", "help text",
		     BooleanValue (false),
		     MakeBooleanAccessor (&AttributeObjectTest::m_boolTest),
		     MakeBooleanChecker ())
      .AddAttribute ("TestBoolA", "help text",
		     BooleanValue (false),
		     MakeBooleanAccessor (&AttributeObjectTest::DoSetTestB,
					   &AttributeObjectTest::DoGetTestB),
		     MakeBooleanChecker ())
      .AddAttribute ("TestInt16", "help text",
		     IntegerValue (-2),
		     MakeIntegerAccessor (&AttributeObjectTest::m_int16),
		     MakeIntegerChecker<int16_t> ())
      .AddAttribute ("TestInt16WithBounds", "help text",
		     IntegerValue (-2),
		     MakeIntegerAccessor (&AttributeObjectTest::m_int16WithBounds),
		     MakeIntegerChecker<int16_t> (-5, 10))
      .AddAttribute ("TestInt16SetGet", "help text",
		     IntegerValue (6),
		     MakeIntegerAccessor (&AttributeObjectTest::DoSetInt16,
				       &AttributeObjectTest::DoGetInt16),
		     MakeIntegerChecker<int16_t> ())
      .AddAttribute ("TestUint8", "help text",
		     UintegerValue (1),
		     MakeUintegerAccessor (&AttributeObjectTest::m_uint8),
		     MakeUintegerChecker<uint8_t> ())
      .AddAttribute ("TestEnum", "help text",
		     EnumValue (TEST_A),
		     MakeEnumAccessor (&AttributeObjectTest::m_enum),
		     MakeEnumChecker (TEST_A, "TestA",
				      TEST_B, "TestB",
				      TEST_C, "TestC"))
      .AddAttribute ("TestRandom", "help text",
		     RandomVariableValue (ConstantVariable (1.0)),
		     MakeRandomVariableAccessor (&AttributeObjectTest::m_random),
		     MakeRandomVariableChecker ())
      .AddAttribute ("TestFloat", "help text",
		     DoubleValue (-1.1),
		     MakeDoubleAccessor (&AttributeObjectTest::m_float),
		     MakeDoubleChecker<float> ())
      .AddAttribute ("TestVector1", "help text",
		     ObjectVectorValue (),
		     MakeObjectVectorAccessor (&AttributeObjectTest::m_vector1),
		     MakeObjectVectorChecker<Derived> ())
      .AddAttribute ("TestVector2", "help text",
		     ObjectVectorValue (),
		     MakeObjectVectorAccessor (&AttributeObjectTest::DoGetVectorN,
						&AttributeObjectTest::DoGetVector),
		     MakeObjectVectorChecker<Derived> ())
      .AddAttribute ("IntegerTraceSource1", "help text",
		     IntegerValue (-2),
		     MakeIntegerAccessor (&AttributeObjectTest::m_intSrc1),
		     MakeIntegerChecker<int8_t> ())
      .AddAttribute ("IntegerTraceSource2", "help text",
		     IntegerValue (-2),
		     MakeIntegerAccessor (&AttributeObjectTest::DoSetIntSrc,
					  &AttributeObjectTest::DoGetIntSrc),
		     MakeIntegerChecker<int8_t> ())
      .AddAttribute ("ValueClassSource", "help text",
		     ValueClassTestValue (ValueClassTest ()),
		     MakeValueClassTestAccessor (&AttributeObjectTest::m_valueSrc),
		     MakeValueClassTestChecker ())
      .AddTraceSource ("Source1", "help test",
		       MakeTraceSourceAccessor (&AttributeObjectTest::m_intSrc1))
      .AddTraceSource ("Source2", "help text",
		       MakeTraceSourceAccessor (&AttributeObjectTest::m_cb))
      .AddTraceSource ("ValueSource", "help text",
		       MakeTraceSourceAccessor (&AttributeObjectTest::m_valueSrc))
      .AddAttribute ("Pointer", "XXX",
                     PointerValue (),
                     MakePointerAccessor (&AttributeObjectTest::m_ptr),
                     MakePointerChecker<Derived> ())
      ;
        
    return tid;
  }

  void AddToVector1 (void) {
    m_vector1.push_back (CreateObject<Derived> ());
  }
  void AddToVector2 (void) {
    m_vector2.push_back (CreateObject<Derived> ());
  }

  void InvokeCb (double a, int b, float c) {
    m_cb (a,b,c);
  }

private:
  void DoSetTestB (bool v) {
    m_boolTestA = v;
  }
  bool DoGetTestB (void) const {
    return m_boolTestA;
  }
  int16_t DoGetInt16 (void) const {
    return m_int16SetGet;
  }
  void DoSetInt16 (int16_t v) {
    m_int16SetGet = v;
  }
  uint32_t DoGetVectorN (void) const {
    return m_vector2.size ();
  }
  Ptr<Derived> DoGetVector (uint32_t i) const {
    return m_vector2[i];
  }
  void DoSetIntSrc (int8_t v) {
    m_intSrc2 = v;
  }
  int8_t DoGetIntSrc (void) const {
    return m_intSrc2;
  }
  bool m_boolTestA;
  bool m_boolTest;
  int16_t m_int16;
  int16_t m_int16WithBounds;
  int16_t m_int16SetGet;
  uint8_t m_uint8;
  float m_float;
  enum Test_e m_enum;
  RandomVariable m_random;
  std::vector<Ptr<Derived> > m_vector1;
  std::vector<Ptr<Derived> > m_vector2;
  TracedValue<int8_t> m_intSrc1;
  TracedValue<int8_t> m_intSrc2;
  TracedCallback<double, int, float> m_cb;
  TracedValue<ValueClassTest> m_valueSrc;
  Ptr<Derived> m_ptr;
};


#define CHECK_GET_STR(p,name,value)                               \
  {                                                               \
    std::string expected = value;                                 \
    StringValue got;                                              \
    bool ok = p->GetAttributeFailSafe (name, got);                \
    NS_TEST_ASSERT (ok);                                          \
    NS_TEST_ASSERT_EQUAL (got.Get (), expected);                  \
  }
#define CHECK_GET_PARAM(p,name,type,value)		\
  {							\
    const type expected = value;			\
    type got;                                           \
    bool ok = p->GetAttributeFailSafe (name, got);      \
    NS_TEST_ASSERT (ok);                                \
    NS_TEST_ASSERT_EQUAL (got.Get (), expected.Get ());	\
  }

NS_OBJECT_ENSURE_REGISTERED (AttributeObjectTest);

AttributeTest::AttributeTest ()
  : Test ("Attribute")
{}
bool 
AttributeTest::RunTests (void)
{
  bool result = true;

  AttributeList params;
  Ptr<AttributeObjectTest> p;
  NS_TEST_ASSERT (params.SetFailSafe ("ns3::AttributeObjectTest::TestBoolName", StringValue ("false")));
  p = CreateObject<AttributeObjectTest> (params);
  CHECK_GET_STR (p, "TestBoolName", "false");
  CHECK_GET_PARAM (p, "TestBoolName", BooleanValue, false);

  NS_TEST_ASSERT (p->SetAttributeFailSafe("TestBoolName", StringValue ("true")));
  CHECK_GET_STR (p, "TestBoolName", "true");
  CHECK_GET_PARAM (p, "TestBoolName", BooleanValue, true);

  NS_TEST_ASSERT (p->SetAttributeFailSafe("TestBoolName", BooleanValue (false)));
  CHECK_GET_STR (p, "TestBoolName", "false");
  CHECK_GET_PARAM (p, "TestBoolName", BooleanValue, false);

  p = CreateObject<AttributeObjectTest> ("TestBoolName", StringValue ("true"));
  CHECK_GET_STR (p, "TestBoolName", "true");
  CHECK_GET_PARAM (p, "TestBoolName", BooleanValue, true);

  p = CreateObject<AttributeObjectTest> ("TestBoolName", BooleanValue (true));
  CHECK_GET_STR (p, "TestBoolName", "true");
  CHECK_GET_PARAM (p, "TestBoolName", BooleanValue, true);

  NS_TEST_ASSERT (p->SetAttributeFailSafe("TestBoolA", StringValue ("false")));
  CHECK_GET_STR (p, "TestBoolA", "false");
  CHECK_GET_PARAM (p, "TestBoolA", BooleanValue, false);

  NS_TEST_ASSERT (p->SetAttributeFailSafe("TestBoolA", StringValue ("true")));
  CHECK_GET_STR (p, "TestBoolA", "true");
  CHECK_GET_PARAM (p, "TestBoolA", BooleanValue, true);


  CHECK_GET_STR (p, "TestInt16", "-2");
  CHECK_GET_PARAM (p, "TestInt16", IntegerValue, -2);

  NS_TEST_ASSERT (p->SetAttributeFailSafe("TestInt16", StringValue ("-5")));
  CHECK_GET_STR (p, "TestInt16", "-5");
  CHECK_GET_PARAM (p, "TestInt16", IntegerValue, -5);

  NS_TEST_ASSERT (p->SetAttributeFailSafe("TestInt16", IntegerValue (+2)));
  CHECK_GET_STR (p, "TestInt16", "2");
  CHECK_GET_PARAM (p, "TestInt16", IntegerValue, +2);

  NS_TEST_ASSERT (p->SetAttributeFailSafe("TestInt16", IntegerValue (-32768)));
  CHECK_GET_STR (p, "TestInt16", "-32768");
  CHECK_GET_PARAM (p, "TestInt16", IntegerValue, -32768);

  NS_TEST_ASSERT (!p->SetAttributeFailSafe("TestInt16", IntegerValue (-32769)));
  CHECK_GET_STR (p, "TestInt16", "-32768");
  CHECK_GET_PARAM (p, "TestInt16", IntegerValue, -32768);

  NS_TEST_ASSERT (p->SetAttributeFailSafe("TestInt16", IntegerValue (32767)));
  CHECK_GET_STR (p, "TestInt16", "32767");
  CHECK_GET_PARAM (p, "TestInt16", IntegerValue, 32767);

  NS_TEST_ASSERT (!p->SetAttributeFailSafe("TestInt16", IntegerValue (32768)));
  CHECK_GET_STR (p, "TestInt16", "32767");
  CHECK_GET_PARAM (p, "TestInt16", IntegerValue, 32767);

  NS_TEST_ASSERT (p->SetAttributeFailSafe("TestInt16WithBounds", IntegerValue (10)));
  CHECK_GET_STR (p, "TestInt16WithBounds", "10");
  CHECK_GET_PARAM (p, "TestInt16WithBounds", IntegerValue, 10);
  NS_TEST_ASSERT (!p->SetAttributeFailSafe("TestInt16WithBounds", IntegerValue (11)));
  CHECK_GET_STR (p, "TestInt16WithBounds", "10");
  CHECK_GET_PARAM (p, "TestInt16WithBounds", IntegerValue, 10);

  NS_TEST_ASSERT (p->SetAttributeFailSafe("TestInt16WithBounds", IntegerValue (-5)));
  CHECK_GET_STR (p, "TestInt16WithBounds", "-5");
  CHECK_GET_PARAM (p, "TestInt16WithBounds", IntegerValue, -5);
  NS_TEST_ASSERT (!p->SetAttributeFailSafe("TestInt16WithBounds", IntegerValue (-6)));
  CHECK_GET_STR (p, "TestInt16WithBounds", "-5");
  CHECK_GET_PARAM (p, "TestInt16WithBounds", IntegerValue, -5);

  CHECK_GET_STR (p, "TestInt16SetGet", "6");
  CHECK_GET_PARAM (p, "TestInt16SetGet", IntegerValue, 6);
  NS_TEST_ASSERT (p->SetAttributeFailSafe("TestInt16SetGet", IntegerValue (0)));
  CHECK_GET_STR (p, "TestInt16SetGet", "0");
  CHECK_GET_PARAM (p, "TestInt16SetGet", IntegerValue, 0);

  CHECK_GET_STR (p, "TestUint8", "1");
  CHECK_GET_PARAM (p, "TestUint8", UintegerValue, 1);
  NS_TEST_ASSERT (p->SetAttributeFailSafe("TestUint8", UintegerValue (0)));
  CHECK_GET_STR (p, "TestUint8", "0");
  CHECK_GET_PARAM (p, "TestUint8", UintegerValue, 0);
  NS_TEST_ASSERT (p->SetAttributeFailSafe("TestUint8", UintegerValue (255)));
  CHECK_GET_STR (p, "TestUint8", "255");
  CHECK_GET_PARAM (p, "TestUint8", UintegerValue, 255);
  NS_TEST_ASSERT (p->SetAttributeFailSafe("TestUint8", StringValue ("255")));
  CHECK_GET_STR (p, "TestUint8", "255");
  CHECK_GET_PARAM (p, "TestUint8", UintegerValue, 255);
  NS_TEST_ASSERT (!p->SetAttributeFailSafe("TestUint8", StringValue ("256")));
  CHECK_GET_STR (p, "TestUint8", "255");
  CHECK_GET_PARAM (p, "TestUint8", UintegerValue, 255);
  NS_TEST_ASSERT (!p->SetAttributeFailSafe("TestUint8", StringValue ("-1")));
  CHECK_GET_STR (p, "TestUint8", "255");
  CHECK_GET_PARAM (p, "TestUint8", UintegerValue, 255);
  NS_TEST_ASSERT (!p->SetAttributeFailSafe("TestUint8", UintegerValue ((uint64_t)-1)));
  CHECK_GET_STR (p, "TestUint8", "255");
  CHECK_GET_PARAM (p, "TestUint8", UintegerValue, 255);

  CHECK_GET_STR (p, "TestFloat", "-1.1");
  NS_TEST_ASSERT (p->SetAttributeFailSafe("TestFloat", DoubleValue ((float)+2.3)));
  CHECK_GET_PARAM (p, "TestFloat", DoubleValue, (float)+2.3);

  CHECK_GET_STR (p, "TestEnum", "TestA");
  CHECK_GET_PARAM (p, "TestEnum", EnumValue, AttributeObjectTest::TEST_A);
  NS_TEST_ASSERT (p->SetAttributeFailSafe("TestEnum", EnumValue (AttributeObjectTest::TEST_C)));
  CHECK_GET_STR (p, "TestEnum", "TestC");
  CHECK_GET_PARAM (p, "TestEnum", EnumValue, AttributeObjectTest::TEST_C);
  NS_TEST_ASSERT (p->SetAttributeFailSafe("TestEnum", StringValue ("TestB")));
  CHECK_GET_STR (p, "TestEnum", "TestB");
  CHECK_GET_PARAM (p, "TestEnum", EnumValue, AttributeObjectTest::TEST_B);
  NS_TEST_ASSERT (!p->SetAttributeFailSafe("TestEnum", StringValue ("TestD")));
  CHECK_GET_STR (p, "TestEnum", "TestB");
  CHECK_GET_PARAM (p, "TestEnum", EnumValue, AttributeObjectTest::TEST_B);
  NS_TEST_ASSERT (!p->SetAttributeFailSafe("TestEnum", EnumValue (5)));
  CHECK_GET_STR (p, "TestEnum", "TestB");
  CHECK_GET_PARAM (p, "TestEnum", EnumValue, AttributeObjectTest::TEST_B);

  RandomVariableValue ran;
  p->GetAttribute ("TestRandom", ran);
  NS_TEST_ASSERT (p->SetAttributeFailSafe("TestRandom", RandomVariableValue (UniformVariable (0.0, 1.0))));
  NS_TEST_ASSERT (p->SetAttributeFailSafe("TestRandom", RandomVariableValue (ConstantVariable (10.0))));

  {
    ObjectVectorValue vector;
    p->GetAttribute ("TestVector1", vector);
    NS_TEST_ASSERT_EQUAL (vector.GetN (), 0);
    p->AddToVector1 ();
    NS_TEST_ASSERT_EQUAL (vector.GetN (), 0);
    p->GetAttribute ("TestVector1", vector);
    NS_TEST_ASSERT_EQUAL (vector.GetN (), 1);
    Ptr<Object> a = vector.Get (0);
    NS_TEST_ASSERT_UNEQUAL (a, 0);
    p->AddToVector1 ();
    NS_TEST_ASSERT_EQUAL (vector.GetN (), 1);
    p->GetAttribute ("TestVector1", vector);
    NS_TEST_ASSERT_EQUAL (vector.GetN (), 2);
  }

  {
    ObjectVectorValue vector;
    p->GetAttribute ("TestVector2", vector);
    NS_TEST_ASSERT_EQUAL (vector.GetN (), 0);
    p->AddToVector2 ();
    NS_TEST_ASSERT_EQUAL (vector.GetN (), 0);
    p->GetAttribute ("TestVector2", vector);
    NS_TEST_ASSERT_EQUAL (vector.GetN (), 1);
    Ptr<Object> a = vector.Get (0);
    NS_TEST_ASSERT_UNEQUAL (a, 0);
    p->AddToVector2 ();
    NS_TEST_ASSERT_EQUAL (vector.GetN (), 1);
    p->GetAttribute ("TestVector2", vector);
    NS_TEST_ASSERT_EQUAL (vector.GetN (), 2);
  }

  NS_TEST_ASSERT (AttributeList::GetGlobal ()->SetFailSafe ("ns3::AttributeObjectTest::TestBoolName", StringValue ("true")));
  p = CreateObject<AttributeObjectTest> ();
  BooleanValue boolV;
  p->GetAttribute ("TestBoolName", boolV);
  NS_TEST_ASSERT_EQUAL (boolV.Get (), true);

  NS_TEST_ASSERT (AttributeList::GetGlobal ()->SetFailSafe ("ns3::AttributeObjectTest::TestBoolName", StringValue ("false")));
  p = CreateObject<AttributeObjectTest> ();
  p->GetAttribute ("TestBoolName", boolV);
  NS_TEST_ASSERT_EQUAL (boolV.Get (), false);

  IntegerValue i;
  p->GetAttribute ("IntegerTraceSource1", i);
  NS_TEST_ASSERT_EQUAL (i.Get (), -2);
  NS_TEST_ASSERT (p->SetAttributeFailSafe ("IntegerTraceSource1", IntegerValue (+5)));
  p->GetAttribute ("IntegerTraceSource1", i);
  NS_TEST_ASSERT_EQUAL (i.Get (), +5);
  NS_TEST_ASSERT (p->SetAttributeFailSafe ("IntegerTraceSource1", IntegerValue (127)));
  NS_TEST_ASSERT (!p->SetAttributeFailSafe ("IntegerTraceSource1", IntegerValue (128)));
  NS_TEST_ASSERT (p->SetAttributeFailSafe ("IntegerTraceSource1", IntegerValue (-128)));
  NS_TEST_ASSERT (!p->SetAttributeFailSafe ("IntegerTraceSource1", IntegerValue (-129)));

  p->GetAttribute ("IntegerTraceSource2", i);
  NS_TEST_ASSERT_EQUAL (i.Get (), -2);
  NS_TEST_ASSERT (p->SetAttributeFailSafe ("IntegerTraceSource2", IntegerValue (+5)));
  p->GetAttribute ("IntegerTraceSource2", i);
  NS_TEST_ASSERT_EQUAL (i.Get (), +5);
  NS_TEST_ASSERT (p->SetAttributeFailSafe ("IntegerTraceSource2", IntegerValue (127)));
  NS_TEST_ASSERT (!p->SetAttributeFailSafe ("IntegerTraceSource2", IntegerValue (128)));
  NS_TEST_ASSERT (p->SetAttributeFailSafe ("IntegerTraceSource2", IntegerValue (-128)));
  NS_TEST_ASSERT (!p->SetAttributeFailSafe ("IntegerTraceSource2", IntegerValue (-129)));

  m_got1 = -2;
  NS_TEST_ASSERT (p->SetAttributeFailSafe ("IntegerTraceSource1", IntegerValue (-1)));
  NS_TEST_ASSERT (p->TraceConnectWithoutContext ("Source1", MakeCallback (&AttributeTest::NotifySource1, this)));
  NS_TEST_ASSERT (p->SetAttributeFailSafe ("IntegerTraceSource1", IntegerValue (0)));
  NS_TEST_ASSERT_EQUAL (m_got1, 0);
  NS_TEST_ASSERT (p->TraceDisconnectWithoutContext ("Source1", MakeCallback (&AttributeTest::NotifySource1, this)));
  NS_TEST_ASSERT (p->SetAttributeFailSafe ("IntegerTraceSource1", IntegerValue (1)));
  NS_TEST_ASSERT_EQUAL (m_got1, 0);

  m_got2 = 4.3;
  p->InvokeCb (1.0, -5, 0.0);
  NS_TEST_ASSERT_EQUAL (m_got2, 4.3);
  NS_TEST_ASSERT (p->TraceConnectWithoutContext ("Source2", MakeCallback (&AttributeTest::NotifySource2, this)));
  NS_TEST_ASSERT_EQUAL (m_got2, 4.3);
  p->InvokeCb (1.0, -5, 0.0);
  NS_TEST_ASSERT_EQUAL (m_got2, 1.0);
  NS_TEST_ASSERT (p->TraceDisconnectWithoutContext ("Source2", MakeCallback (&AttributeTest::NotifySource2, this)));
  p->InvokeCb (-1.0, -5, 0.0);
  NS_TEST_ASSERT_EQUAL (m_got2, 1.0);

  NS_TEST_ASSERT (p->TraceConnectWithoutContext ("ValueSource", MakeCallback (&AttributeTest::NotifySourceValue, this)));

  PointerValue ptr;
  p->GetAttribute ("Pointer", ptr);
  Ptr<Derived> derived = ptr.Get<Derived> ();
  NS_TEST_ASSERT (derived == 0);
  derived = Create<Derived> ();
  NS_TEST_ASSERT (p->SetAttributeFailSafe("Pointer", PointerValue (derived)));
  p->GetAttribute ("Pointer", ptr);
  Ptr<Derived> stored = ptr.Get<Derived> ();
  NS_TEST_ASSERT (stored == derived);
  p->GetAttribute ("Pointer", ptr);
  Ptr<Object> storedBase = ptr.Get<Object> ();
  NS_TEST_ASSERT (stored == storedBase);
  p->GetAttribute ("Pointer", ptr);
  Ptr<AttributeObjectTest> x = ptr.Get<AttributeObjectTest> ();
  NS_TEST_ASSERT (x == 0);

  p = CreateObject<AttributeObjectTest> ("Pointer", PointerValue (Create<Derived> ()));
  NS_TEST_ASSERT (p != 0);
  derived = 0;
  p->GetAttribute ("Pointer", ptr);
  derived = ptr.Get<Derived> ();
  NS_TEST_ASSERT (derived != 0);

  return result;
}



static AttributeTest g_parameterTest;

} // namespace ns3


#endif /* RUN_SELF_TESTS */
