#include "type-traits.h"
#include "test.h"

#ifdef RUN_SELF_TESTS

namespace ns3 {

class TypeTraitsTest : public Test
{
public:
  TypeTraitsTest ();
  virtual bool RunTests (void);
};

TypeTraitsTest::TypeTraitsTest ()
  : Test ("TypeTraits")
{}
bool 
TypeTraitsTest::RunTests (void)
{
  bool result = true;

  //TypeTraits<int &>::ReferencedType ir;
  //TypeTraits<const int>::NonConstType uci;
  NS_TEST_ASSERT_EQUAL (TypeTraits<void (TypeTraitsTest::*) (void)>::IsPointerToMember, 1);
  NS_TEST_ASSERT_EQUAL (TypeTraits<void (TypeTraitsTest::*) (void) const>::IsPointerToMember, 1);
  NS_TEST_ASSERT_EQUAL (TypeTraits<void (TypeTraitsTest::*) (int)>::IsPointerToMember, 1);
  NS_TEST_ASSERT_EQUAL (TypeTraits<void (TypeTraitsTest::*) (int) const>::IsPointerToMember, 1);
  NS_TEST_ASSERT_EQUAL (TypeTraits<void (TypeTraitsTest::*) (void) const>::PointerToMemberTraits::nArgs, 0);
  NS_TEST_ASSERT_EQUAL (TypeTraits<void (TypeTraitsTest::*) (int) const>::PointerToMemberTraits::nArgs, 1);

  return result;
}

static TypeTraitsTest g_typeTraitsTest;

} // namespace ns3

#endif /* RUN_SELF_TESTS */

