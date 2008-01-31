#ifdef RUN_SELF_TESTS
#include "test.h"
#include "object.h"
#include "boolean-value.h"
#if 0
#include "int-value.h"
#include "uint-value.h"
#include "fp-value.h"
#include "enum-value.h"
#endif
namespace ns3 {

class ParamSpecTest : public Test
{
public:
  ParamSpecTest ();
  virtual bool RunTests (void);
private:
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

class ParamSpecObjectTest : public Object
{
public:
  enum TestEnum {
    TEST_A,
    TEST_B,
    TEST_C
  };
  static TypeId GetTypeId (void) {
    static TypeId tid = TypeId ("ParamSpecObjectTest")
      .SetParent<Object> ()
      .AddParameter ("TestBoolName", "help text",
		     MakeBooleanParamSpec (&ParamSpecObjectTest::m_boolTest, false))
      .AddParameter ("TestBoolA", "help text",
		     MakeBooleanParamSpec (&ParamSpecObjectTest::DoSetTestB,
					   &ParamSpecObjectTest::DoGetTestB, 
					   false))
      .AddParameter ("TestPtr", "help text", 
		     MakePtrParamSpec (&ParamSpecObjectTest::m_derived))
#if 0
      .AddParameter ("TestInt16", "help text",
		     MakeIntParamSpec (-2, &ParamSpecObjectTest::m_int16))
      .AddParameter ("TestInt16WithBounds", "help text",
		     MakeIntParamSpec (-2, -5, 10, 
				       &ParamSpecObjectTest::m_int16WithBounds))
      .AddParameter ("TestInt16SetGet", "help text",
		     MakeIntParamSpec (6, &ParamSpecObjectTest::DoSetInt16,
				       &ParamSpecObjectTest::DoGetInt16))
      .AddParameter ("TestUint8", "help text",
		     MakeUintParamSpec (1, &ParamSpecObjectTest::m_uint8))
      .AddParameter ("TestFloat", "help text",
		     MakeFpParamSpec (-1.1, &ParamSpecObjectTest::m_float))
      .AddParameter ("TestEnum", "help text",
		     MakeEnumParamSpec (&ParamSpecObjectTest::m_enum,
					TEST_A, "TestA",
					TEST_B, "TestB",
					TEST_C, "TestC"))
#endif
      ;
        
    return tid;
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
  bool m_boolTestA;
  bool m_boolTest;
  Ptr<Derived> m_derived;
  int16_t m_int16;
  int16_t m_int16WithBounds;
  int16_t m_int16SetGet;
  uint8_t m_uint8;
  float m_float;
  enum TestEnum m_enum;
};


#define CHECK_GET_STR(p,name,value)		\
  {						\
    std::string expected = value;		\
    std::string got;				\
    bool ok = p->Get (name, got);		\
    NS_TEST_ASSERT (ok);			\
    NS_TEST_ASSERT_EQUAL (got, expected);	\
  }
#define CHECK_GET_PARAM(p,name,type,value)		\
  {							\
    const type expected = value;			\
    type got = value;					\
    PValue v = p->Get (name);				\
    NS_TEST_ASSERT (v.DynCast<type *> () != 0);		\
    got = v;						\
    NS_TEST_ASSERT_EQUAL (got.Get (), expected.Get ());	\
  }


NS_OBJECT_ENSURE_REGISTERED (ParamSpecObjectTest);

ParamSpecTest::ParamSpecTest ()
  : Test ("ParamSpec")
{}
bool 
ParamSpecTest::RunTests (void)
{
  bool result = true;

  Parameters params;
  Ptr<ParamSpecObjectTest> p;
  NS_TEST_ASSERT (params.Set ("ParamSpecObjectTest::TestBoolName", "false"));
  p = CreateObject<ParamSpecObjectTest> (params);
  CHECK_GET_STR (p, "TestBoolName", "false");
  CHECK_GET_PARAM (p, "TestBoolName", BooleanValue, false);

  NS_TEST_ASSERT (p->Set ("TestBoolName", "true"));
  CHECK_GET_STR (p, "TestBoolName", "true");
  CHECK_GET_PARAM (p, "TestBoolName", BooleanValue, true);

  NS_TEST_ASSERT (p->Set ("TestBoolName", BooleanValue (false)));
  CHECK_GET_STR (p, "TestBoolName", "false");
  CHECK_GET_PARAM (p, "TestBoolName", BooleanValue, false);

  p = CreateObjectWith<ParamSpecObjectTest> ("TestBoolName", "true");
  CHECK_GET_STR (p, "TestBoolName", "true");
  CHECK_GET_PARAM (p, "TestBoolName", BooleanValue, true);

  p = CreateObjectWith<ParamSpecObjectTest> ("TestBoolName", BooleanValue (true));
  CHECK_GET_STR (p, "TestBoolName", "true");
  CHECK_GET_PARAM (p, "TestBoolName", BooleanValue, true);

  NS_TEST_ASSERT (p->Set ("TestBoolA", "false"));
  CHECK_GET_STR (p, "TestBoolA", "false");
  CHECK_GET_PARAM (p, "TestBoolA", BooleanValue, false);

  NS_TEST_ASSERT (p->Set ("TestBoolA", "true"));
  CHECK_GET_STR (p, "TestBoolA", "true");
  CHECK_GET_PARAM (p, "TestBoolA", BooleanValue, true);


  Ptr<Derived> derived = p->Get ("TestPtr");
  NS_TEST_ASSERT (derived == 0);
  derived = Create<Derived> ();
  NS_TEST_ASSERT (p->Set ("TestPtr", derived));
  Ptr<Derived> stored = p->Get ("TestPtr");
  NS_TEST_ASSERT (stored == derived);
  Ptr<Object> storedBase = p->Get ("TestPtr");
  NS_TEST_ASSERT (stored == storedBase);
  Ptr<ParamSpecObjectTest> x = p->Get ("TestPtr");
  NS_TEST_ASSERT (x == 0);

  p = CreateObjectWith<ParamSpecObjectTest> ("TestPtr", Create<Derived> ());
  NS_TEST_ASSERT (p != 0);
  derived = 0;
  derived = p->Get ("TestPtr");
  NS_TEST_ASSERT (derived != 0);

#if 0
  CHECK_GET_STR (p, "TestInt16", "-2");
  CHECK_GET_PARAM (p, "TestInt16", IntValue, -2);

  NS_TEST_ASSERT (p->Set ("TestInt16", "-5"));
  CHECK_GET_STR (p, "TestInt16", "-5");
  CHECK_GET_PARAM (p, "TestInt16", IntValue, -5);

  NS_TEST_ASSERT (p->Set ("TestInt16", IntValue (+2)));
  CHECK_GET_STR (p, "TestInt16", "2");
  CHECK_GET_PARAM (p, "TestInt16", IntValue, +2);

  NS_TEST_ASSERT (p->Set ("TestInt16", IntValue (-32768)));
  CHECK_GET_STR (p, "TestInt16", "-32768");
  CHECK_GET_PARAM (p, "TestInt16", IntValue, -32768);

  NS_TEST_ASSERT (!p->Set ("TestInt16", IntValue (-32769)));
  CHECK_GET_STR (p, "TestInt16", "-32768");
  CHECK_GET_PARAM (p, "TestInt16", IntValue, -32768);

  NS_TEST_ASSERT (p->Set ("TestInt16", IntValue (32767)));
  CHECK_GET_STR (p, "TestInt16", "32767");
  CHECK_GET_PARAM (p, "TestInt16", IntValue, 32767);

  NS_TEST_ASSERT (!p->Set ("TestInt16", IntValue (32768)));
  CHECK_GET_STR (p, "TestInt16", "32767");
  CHECK_GET_PARAM (p, "TestInt16", IntValue, 32767);

  NS_TEST_ASSERT (p->Set ("TestInt16WithBounds", IntValue (10)));
  CHECK_GET_STR (p, "TestInt16WithBounds", "10");
  CHECK_GET_PARAM (p, "TestInt16WithBounds", IntValue, 10);
  NS_TEST_ASSERT (!p->Set ("TestInt16WithBounds", IntValue (11)));
  CHECK_GET_STR (p, "TestInt16WithBounds", "10");
  CHECK_GET_PARAM (p, "TestInt16WithBounds", IntValue, 10);

  NS_TEST_ASSERT (p->Set ("TestInt16WithBounds", IntValue (-5)));
  CHECK_GET_STR (p, "TestInt16WithBounds", "-5");
  CHECK_GET_PARAM (p, "TestInt16WithBounds", IntValue, -5);
  NS_TEST_ASSERT (!p->Set ("TestInt16WithBounds", IntValue (-6)));
  CHECK_GET_STR (p, "TestInt16WithBounds", "-5");
  CHECK_GET_PARAM (p, "TestInt16WithBounds", IntValue, -5);

  CHECK_GET_STR (p, "TestInt16SetGet", "6");
  CHECK_GET_PARAM (p, "TestInt16SetGet", IntValue, 6);
  NS_TEST_ASSERT (p->Set ("TestInt16SetGet", IntValue (0)));
  CHECK_GET_STR (p, "TestInt16SetGet", "0");
  CHECK_GET_PARAM (p, "TestInt16SetGet", IntValue, 0);
  
  CHECK_GET_STR (p, "TestUint8", "1");
  CHECK_GET_PARAM (p, "TestUint8", UintValue, 1);
  NS_TEST_ASSERT (p->Set ("TestUint8", UintValue (0)));
  CHECK_GET_STR (p, "TestUint8", "0");
  CHECK_GET_PARAM (p, "TestUint8", UintValue, 0);
  NS_TEST_ASSERT (p->Set ("TestUint8", UintValue (255)));
  CHECK_GET_STR (p, "TestUint8", "255");
  CHECK_GET_PARAM (p, "TestUint8", UintValue, 255);
  NS_TEST_ASSERT (p->Set ("TestUint8", "255"));
  CHECK_GET_STR (p, "TestUint8", "255");
  CHECK_GET_PARAM (p, "TestUint8", UintValue, 255);
  NS_TEST_ASSERT (!p->Set ("TestUint8", "256"));
  CHECK_GET_STR (p, "TestUint8", "255");
  CHECK_GET_PARAM (p, "TestUint8", UintValue, 255);
  NS_TEST_ASSERT (!p->Set ("TestUint8", "-1"));
  CHECK_GET_STR (p, "TestUint8", "255");
  CHECK_GET_PARAM (p, "TestUint8", UintValue, 255);
  NS_TEST_ASSERT (!p->Set ("TestUint8", UintValue (-1)));
  CHECK_GET_STR (p, "TestUint8", "255");
  CHECK_GET_PARAM (p, "TestUint8", UintValue, 255);

  CHECK_GET_STR (p, "TestFloat", "-1.1");
  NS_TEST_ASSERT (p->Set ("TestFloat", FpValue ((float)+2.3)));
  CHECK_GET_PARAM (p, "TestFloat", FpValue, (float)+2.3);

  CHECK_GET_STR (p, "TestEnum", "TestA");
  CHECK_GET_PARAM (p, "TestEnum", EnumValue, ParamSpecObjectTest::TEST_A);
  NS_TEST_ASSERT (p->Set ("TestEnum", EnumValue (ParamSpecObjectTest::TEST_C)));
  CHECK_GET_STR (p, "TestEnum", "TestC");
  CHECK_GET_PARAM (p, "TestEnum", EnumValue, ParamSpecObjectTest::TEST_C);
  NS_TEST_ASSERT (p->Set ("TestEnum", "TestB"));
  CHECK_GET_STR (p, "TestEnum", "TestB");
  CHECK_GET_PARAM (p, "TestEnum", EnumValue, ParamSpecObjectTest::TEST_B);
  NS_TEST_ASSERT (!p->Set ("TestEnum", "TestD"));
  CHECK_GET_STR (p, "TestEnum", "TestB");
  CHECK_GET_PARAM (p, "TestEnum", EnumValue, ParamSpecObjectTest::TEST_B);
  NS_TEST_ASSERT (!p->Set ("TestEnum", EnumValue (5)));
  CHECK_GET_STR (p, "TestEnum", "TestB");
  CHECK_GET_PARAM (p, "TestEnum", EnumValue, ParamSpecObjectTest::TEST_B);
#endif

#if 0
  p->Set ("TestBoolName", "true");
  NS_TEST_ASSERT_EQUAL (p->Get ("TestBoolName"), "true");
  p->Set ("TestBoolName", "false");
  NS_TEST_ASSERT_EQUAL (p->Get ("TestBoolName"), "false");

  Parameters::GetGlobal ()->Set ("TestBoolName", "true");
  p = CreateObjectWith<ParamSpecObjectTest> ();
  NS_TEST_ASSERT_EQUAL (p->Get ("TestBoolName"), "true");

  Parameters::GetGlobal ()->Set ("TestBoolName", "false");
  p = CreateObjectWith<ParamSpecObjectTest> ();
  NS_TEST_ASSERT_EQUAL (p->Get ("TestBoolName"), "false");
#endif
  return result;
}



static ParamSpecTest g_parameterTest;

} // namespace ns3


#endif /* RUN_SELF_TESTS */
