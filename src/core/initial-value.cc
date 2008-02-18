#include "initial-value.h"
#include "fatal-error.h"
#include "uint-value.h"
#include "int-value.h"
#include "fp-value.h"
#include "enum-value.h"

namespace ns3 {

InitialValue::InitialValue (std::string name, std::string help,
			    PValue initialValue,
			    Ptr<const AttributeChecker> checker)
  : m_name (name),
    m_help (help),
    m_initialValue (initialValue),
    m_checker (checker)
{
  if (m_checker == 0)
    {
      NS_FATAL_ERROR ("Checker should no be zero.");
    }
  GetVector ()->push_back (this);
}

std::string 
InitialValue::GetName (void) const
{
  return m_name;
}
std::string 
InitialValue::GetHelp (void) const
{
  return m_help;
}
PValue 
InitialValue::GetInitialValue (void) const
{
  return m_initialValue;
}
Ptr<const AttributeChecker> 
InitialValue::GetChecker (void) const
{
  return m_checker;
}
  
void 
InitialValue::SetInitialValue (PValue value)
{
  if (!m_checker->Check (value))
    {
      NS_FATAL_ERROR ("Invalid new value.");
    }
  m_initialValue = value;
}

void 
InitialValue::Bind (std::string name, PValue value)
{
  for (Iterator i = Begin (); i != End (); i++)
    {
      if ((*i)->GetName () == name)
	{
	  (*i)->SetInitialValue (value);
	  return;
	}
    }
  NS_FATAL_ERROR ("InitialValue name=\""<<name<<"\" not found.");
}
InitialValue::Iterator 
InitialValue::Begin (void)
{
  return GetVector ()->begin ();
}
InitialValue::Iterator 
InitialValue::End (void)
{
  return GetVector ()->end ();
}
InitialValue::Vector *
InitialValue::GetVector (void)
{
  static Vector vector;
  return &vector;
}

} // namespace ns3

#ifdef RUN_SELF_TESTS

#include "test.h"
#include "uint-value.h"

namespace {

static ns3::InitialValue g_uint = ns3::InitialValue ("TestUint", "help text",
						     ns3::UintValue (10),
						     ns3::MakeUintChecker<uint32_t> ());

}

namespace ns3 {

class InitialValueTests : public Test
{
public:
  InitialValueTests ();
  virtual bool RunTests (void);
private:
};


InitialValueTests::InitialValueTests ()
  : Test ("InitialValue")
{}
bool 
InitialValueTests::RunTests (void)
{
  bool result = true;

  NS_TEST_ASSERT_EQUAL (10, UintValue (g_uint.GetInitialValue ()).Get ());

  return result;
}

static InitialValueTests g_initialValueTests;

} // namespace ns3

#endif /* RUN_SELF_TESTS */
