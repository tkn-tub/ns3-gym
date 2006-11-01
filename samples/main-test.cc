/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/test.h"

using namespace ns3;

#ifdef RUN_SELF_TESTS

// declare subclass of base class Test
class MyTest : public Test {
public:
  MyTest (bool ok);
  virtual ~MyTest ();
  virtual bool RunTests (void);
private:
  bool m_ok;
};

// implement MyTest
MyTest::MyTest (bool ok)
  : Test ("My"),
    m_ok (ok)
{}
MyTest::~MyTest ()
{}
bool
MyTest::RunTests (void)
{
  return m_ok;
}

// instantiate MyTest once
static MyTest g_my_test = MyTest (true);

#endif /* RUN_SELF_TESTS */

int main (int argc, char *argv[])
{
  // run tests
  TestManager::EnableVerbose ();
  TestManager::RunTests ();
  return 0;
}
