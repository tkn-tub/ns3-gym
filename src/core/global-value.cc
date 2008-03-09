#include "global-value.h"
#include "fatal-error.h"
#include "attribute.h"

namespace ns3 {

GlobalValue::GlobalValue (std::string name, std::string help,
			  Attribute initialValue,
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
GlobalValue::GetName (void) const
{
  return m_name;
}
std::string 
GlobalValue::GetHelp (void) const
{
  return m_help;
}
Attribute 
GlobalValue::GetValue (void) const
{
  return m_initialValue;
}
Ptr<const AttributeChecker> 
GlobalValue::GetChecker (void) const
{
  return m_checker;
}
  
bool
GlobalValue::SetValue (Attribute value)
{
  if (!m_checker->Check (value))
    {
      return false;
    }
  m_initialValue = value;
  return true;
}

void 
GlobalValue::Bind (std::string name, Attribute value)
{
  for (Iterator i = Begin (); i != End (); i++)
    {
      if ((*i)->GetName () == name)
	{
	  if (!(*i)->SetValue (value))
	    {
	      NS_FATAL_ERROR ("Invalid new value for global value: "<<name);
	    }
	  return;
	}
    }
  NS_FATAL_ERROR ("Non-existant global value: "<<name);
}
bool 
GlobalValue::BindFailSafe (std::string name, Attribute value)
{
  for (Iterator i = Begin (); i != End (); i++)
    {
      if ((*i)->GetName () == name)
	{
	  return (*i)->SetValue (value);
	}
    }
  return false;
}
GlobalValue::Iterator 
GlobalValue::Begin (void)
{
  return GetVector ()->begin ();
}
GlobalValue::Iterator 
GlobalValue::End (void)
{
  return GetVector ()->end ();
}
GlobalValue::Vector *
GlobalValue::GetVector (void)
{
  static Vector vector;
  return &vector;
}

} // namespace ns3

#ifdef RUN_SELF_TESTS

#include "test.h"
#include "uinteger.h"

namespace {

static ns3::GlobalValue g_uint = ns3::GlobalValue ("TestUint", "help text",
						     ns3::Uinteger (10),
						     ns3::MakeUintegerChecker<uint32_t> ());

}

namespace ns3 {

class GlobalValueTests : public Test
{
public:
  GlobalValueTests ();
  virtual bool RunTests (void);
private:
};


GlobalValueTests::GlobalValueTests ()
  : Test ("GlobalValue")
{}
bool 
GlobalValueTests::RunTests (void)
{
  bool result = true;

  NS_TEST_ASSERT_EQUAL (10, Uinteger (g_uint.GetValue ()).Get ());

  return result;
}

static GlobalValueTests g_initialValueTests;

} // namespace ns3

#endif /* RUN_SELF_TESTS */
