/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include <string>
#include "ns3/default-value.h"
#include "ns3/command-line.h"
#include "ns3/debug.h"

using namespace ns3;

//
// This sample file demonstrates how to take some simple member
// variables and hook them into the default variable system
// Typically, you will establish a static variable to maintain the current
// value of the default parameter.  Then as other code require the values of 
// the defaults, they query them with GetValue() to get the present value.
static BooleanDefaultValue defaultTestBool1 ("testBool1", "helpBool", true);
static NumericDefaultValue<int> defaultTestInt1 ("testInt1", "helpInt1", 33);
static NumericDefaultValue<uint32_t> defaultTestInt2 ("testInt2", "helpInt2", 47);

// 
// This test class demonstrates the declaration of variables that
// may be overridden by the default-value system
//
// You will see in the core ns-3 modules that many member variables
// can be overridden in this manner 
//
class TestClass {
public:
  TestClass();
  virtual ~TestClass () {}

  bool m_testBool1;
  int m_testInt1;
  uint32_t m_testInt2;
};

//
// In the constructor, you can assign default values in the initializer
// list such as below; note that the instance of the created TestClass
// will have the values as dictated by the current value of the default.
// This means that the behavior of this class can be changed on the fly with
// calls to bind.
//
TestClass::TestClass () : 
  m_testBool1(defaultTestBool1.GetValue()),
  m_testInt1(defaultTestInt1.GetValue()),
  m_testInt2(defaultTestInt2.GetValue())
{
}
using std::cout;
int main (int argc, char* argv[])
{
  //The following allows the default values established so far to be hooked
  //into the command line argument processing unit.  Essentially, the command
  //line processor is aware of the DefaultValues that have been registered, and
  //will accept command line overrides of these.  The call automatically
  //provides a --help option in addition to allowing overrides of defaults.
  uint32_t loops = 0;
  CommandLine::AddArgValue("loops","a test of the command line",loops);
  CommandLine::Parse(argc,argv);
  
  //utilize the loops variable to show that it can be read from the command line
  if(loops>0)
  {
    cout<<"You requested "<<loops<<" iterations of a loop";
    for(uint32_t i=0;i<loops;++i)
      cout<<"iteration "<<i;
  }
  
  // Before objects are instantiated in your simulation script, you have
  // the opportunity to overwrite any default value in the system.  
  // The Bind () method allows you to specify the name (string) of the
  // global variable and value (string) to overwrite the default.
  // Here, the default value of 33 for testInt1 is overwritten with 57
  // 
  DefaultValue::Bind("testInt1", "57");

  TestClass* testclass = new TestClass ();
  NS_DEBUG_UNCOND("TestBool1 default value (" << testclass->m_testBool1 << ")");
  NS_DEBUG_UNCOND("TestInt1 default value (" << testclass->m_testInt1 << ")");
  NS_DEBUG_UNCOND("TestInt2 default value (" << testclass->m_testInt2 << ")");
  delete testclass;

  return 0;
}
