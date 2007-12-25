#include "array-trace-resolver.h"


namespace ns3 {

ArrayTraceResolverMatcher::ArrayTraceResolverMatcher (std::string element)
  : m_element (element)
{}
bool 
ArrayTraceResolverMatcher::Matches (uint32_t i) const
{
  if (m_element == "*")
    {
      return true;
    }
  std::string::size_type tmp;
  tmp = m_element.find ("|");
  if (tmp != std::string::npos)
    {
      std::string left = m_element.substr (0, tmp-0);
      std::string right = m_element.substr (tmp+1, m_element.size () - (tmp + 1));
      ArrayTraceResolverMatcher matcher = ArrayTraceResolverMatcher (left);
      if (matcher.Matches (i))
	{
	  return true;
	}
      matcher = ArrayTraceResolverMatcher (right);
      if (matcher.Matches (i))
	{
	  return true;
	}
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
          return true;
        }
      else
	{
	  return false;
	}
    }
  uint32_t value;
  if (StringToUint32 (m_element, &value) &&
      i == value)
    {
      return true;
    }
  return false;
}

bool
ArrayTraceResolverMatcher::StringToUint32 (std::string str, uint32_t *value) const
{
  std::istringstream iss;
  iss.str (str);
  iss >> (*value);
  return !iss.bad () && !iss.fail ();
}

}//namespace ns3


#ifdef RUN_SELF_TESTS

#include <stdarg.h>
#include "test.h"
#include "object.h"
#include "array-trace-resolver.h"
#include "callback-trace-source.h"
#include "composite-trace-resolver.h"

namespace ns3 {

class ObjectTraceTesterIndex : public TraceContextElement
{
public:
  ObjectTraceTesterIndex ();
  ObjectTraceTesterIndex (uint32_t index);
  void Print (std::ostream &os);
  static uint16_t GetUid (void);
  uint32_t Get (void) const;
  std::string GetTypeName (void) const;
private:
  uint32_t m_index;
};


ObjectTraceTesterIndex::ObjectTraceTesterIndex ()
  : m_index (0)
{}
ObjectTraceTesterIndex::ObjectTraceTesterIndex (uint32_t index)
  : m_index (index)
{}
void 
ObjectTraceTesterIndex::Print (std::ostream &os)
{
  os << "nodeid=" << m_index;
}
uint16_t 
ObjectTraceTesterIndex::GetUid (void)
{
  static uint16_t uid = AllocateUid<ObjectTraceTesterIndex> ("ObjectTraceTesterIndex");
  return uid;
}
uint32_t 
ObjectTraceTesterIndex::Get (void) const
{
  return m_index;
}
std::string 
ObjectTraceTesterIndex::GetTypeName (void) const
{
  return "ns3::ObjectTraceTesterIndex";
}


class ObjectTraceTester : public Object
{
public:
  void Do (uint32_t i);
protected:
  virtual Ptr<TraceResolver> GetTraceResolver (void) const;
private:
  CallbackTraceSource<uint32_t> m_test;
};

void 
ObjectTraceTester::Do (uint32_t i)
{
  m_test (i);
}

Ptr<TraceResolver>
ObjectTraceTester::GetTraceResolver (void) const
{
  Ptr<CompositeTraceResolver> resolver = Create<CompositeTraceResolver> ();
  resolver->AddSource ("test", 
		       TraceDoc ("Test"),
		       m_test);
  resolver->SetParentResolver (Object::GetTraceResolver ());
  return resolver;
}

class ArrayTraceResolverTest : public Test
{
public:
  ArrayTraceResolverTest ();
  virtual bool RunTests (void);
private:
  bool RunOne (uint32_t n, std::string str, 
	       uint32_t nExpected, ...);
  void OneItem (const TraceContext &context,
		uint32_t i);

  typedef std::vector<uint32_t> Got;
  Got m_got;
};

ArrayTraceResolverTest::ArrayTraceResolverTest ()
  : Test ("ArrayTraceResolver")
{}
bool
ArrayTraceResolverTest::RunOne (uint32_t n, std::string str, 
				uint32_t nExpected, ...)
{
  bool result = true;
  std::vector<uint32_t> expected;
  va_list ap;
  va_start (ap, nExpected);
  for (uint32_t k = 0; k < nExpected; k++)
    {
      uint32_t v = va_arg (ap, uint32_t);
      expected.push_back (v);
    }
  va_end (ap);
  std::sort (expected.begin (), expected.end ());
  std::sort (m_got.begin (), m_got.end ());

  std::vector<Ptr<ObjectTraceTester> > vec;
  for (uint32_t i = 0; i < n; i++)
    {
      vec.push_back (Create<ObjectTraceTester> ());
    }
  ArrayTraceResolver<ObjectTraceTesterIndex> resolver;
  resolver.SetIterators (vec.begin (), vec.end ());

  TraceContext context;
  resolver.Connect (str, MakeCallback (&ArrayTraceResolverTest::OneItem, this), context);
  uint32_t l = 0;
  for (std::vector<Ptr<ObjectTraceTester> >::const_iterator j = vec.begin (); j != vec.end (); j++)
    {
      (*j)->Do (l);
      l++;
    }
  NS_TEST_ASSERT_EQUAL (m_got.size (), expected.size ());
  for (uint32_t m = 0; m < expected.size (); m++)
    {
      NS_TEST_ASSERT_EQUAL (m_got[m], expected[m]);
    }
  m_got.clear ();
  resolver.Disconnect (str, MakeCallback (&ArrayTraceResolverTest::OneItem, this));
  for (std::vector<Ptr<ObjectTraceTester> >::const_iterator j = vec.begin (); j != vec.end (); j++)
    {
      (*j)->Do (l);
      l++;
    }
  NS_TEST_ASSERT_EQUAL (m_got.size (), 0);
  m_got.clear ();

  return result;
}
void
ArrayTraceResolverTest::OneItem (const TraceContext &context,
				 uint32_t i)
{
  ObjectTraceTesterIndex index;
  bool found = context.GetElement (index);
  if (!found)
    {
      return;
    }
  if (index.Get () != i)
    {
      return;
    }
  m_got.push_back (i);
}

bool 
ArrayTraceResolverTest::RunTests (void)
{
  bool result = true;

  NS_TEST_ASSERT (RunOne (0, "/*/test", 0));
  NS_TEST_ASSERT (RunOne (1, "/*/test", 1, 0));
  NS_TEST_ASSERT (RunOne (1, "/0/test", 1, 0));
  NS_TEST_ASSERT (RunOne (1, "/[0-0]/test", 1, 0));
  NS_TEST_ASSERT (RunOne (1, "/0|0/test", 1, 0));
  NS_TEST_ASSERT (RunOne (2, "/*/test", 2, 0, 1));
  NS_TEST_ASSERT (RunOne (2, "/0|1/test", 2, 0, 1));
  NS_TEST_ASSERT (RunOne (2, "/1/test", 1, 1));
  NS_TEST_ASSERT (RunOne (2, "/|1|/test", 1, 1));
  NS_TEST_ASSERT (RunOne (2, "/0/test", 1, 0));
  NS_TEST_ASSERT (RunOne (2, "/0|/test", 1, 0));
  NS_TEST_ASSERT (RunOne (2, "/|0/test", 1, 0));
  NS_TEST_ASSERT (RunOne (2, "/[0-1]/test", 2, 0, 1));
  NS_TEST_ASSERT (RunOne (2, "/[0-0]/test", 1, 0));
  NS_TEST_ASSERT (RunOne (2, "/[1-1]/test", 1, 1));
  NS_TEST_ASSERT (RunOne (2, "/0|[1-1]/test", 2, 0, 1));
  NS_TEST_ASSERT (RunOne (3, "/1|0/test", 2, 0, 1));
  NS_TEST_ASSERT (RunOne (3, "/2|0/test", 2, 0, 2));
  NS_TEST_ASSERT (RunOne (3, "/2|1/test", 2, 1, 2));
  NS_TEST_ASSERT (RunOne (3, "/[0-1]/test", 2, 0, 1));
  NS_TEST_ASSERT (RunOne (3, "/[1-2]/test", 2, 1, 2));
  NS_TEST_ASSERT (RunOne (3, "/[0-2]/test", 3, 0, 1, 2));
  NS_TEST_ASSERT (RunOne (3, "/[1-2]|0/test", 3, 0, 1, 2));
  NS_TEST_ASSERT (RunOne (3, "/[1-1]|0|[2-2]/test", 3, 0, 1, 2));
  NS_TEST_ASSERT (RunOne (3, "/[1-2]||/test", 2, 1, 2));
  NS_TEST_ASSERT (RunOne (3, "/[1-2]||/test", 2, 2, 1));
  NS_TEST_ASSERT (RunOne (3, "/||||/test", 0));
  NS_TEST_ASSERT (RunOne (20, "/[5-10]|[2-3]|[15-17]/test", 11, 2, 3, 5, 6, 7, 8, 9, 10, 15, 16, 17));
  NS_TEST_ASSERT (RunOne (3, "/[1-2]|[0-1]/test", 3, 0, 1, 2));

  return result;
}



static ArrayTraceResolverTest g_arrayTraceResolverTest;


} // namespace ns3

#endif /* RUN_SELF_TESTS */
