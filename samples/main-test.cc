/* -*-    Mode:C++; c-basic-offset:4; tab-width:4; indent-tabs-mode:f -*- */

#include "ns3/test.h"

using namespace ns3;

#ifndef RUN_SELF_TESTS

class MyTest : public Test {
public:
	MyTest (bool ok);
	virtual ~MyTest ();
	virtual bool runTests (void);
private:
	bool m_ok;
};

MyTest::MyTest (bool ok)
	: Test ("My"),
	  m_ok (ok)
{}
MyTest::~MyTest ()
{}
bool
MyTest::runTests (void)
{
	return m_ok;
}

MyTest g_my_test = MyTest (false);

#endif /* RUN_SELF_TESTS */

int main (int argc, char *argv[])
{
	TestManager::enableVerbose ();
	TestManager::runTests ();
	return 0;
}
