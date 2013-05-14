/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
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
 */

#include "ns3/test.h"
#include "ns3/callback.h"
#include <stdint.h>

using namespace ns3;

// ===========================================================================
// Test the basic Callback mechanism
// ===========================================================================
class BasicCallbackTestCase : public TestCase
{
public:
  BasicCallbackTestCase ();
  virtual ~BasicCallbackTestCase () {}

  void Target1 (void) { m_test1 = true; }
  int Target2 (void) { m_test2 = true; return 2; }
  void Target3 (double a) { m_test3 = true; }
  int Target4 (double a, int b) { m_test4 = true; return 4; }

private:
  virtual void DoRun (void);
  virtual void DoSetup (void);

  bool m_test1;
  bool m_test2;
  bool m_test3;
  bool m_test4;
};

static bool gBasicCallbackTest5;
static bool gBasicCallbackTest6;
static bool gBasicCallbackTest7;

void 
BasicCallbackTarget5 (void)
{
  gBasicCallbackTest5 = true;
}

void 
BasicCallbackTarget6 (int)
{
  gBasicCallbackTest6 = true;
}

int 
BasicCallbackTarget7 (int a)
{
  gBasicCallbackTest7 = true;
  return a;
}

BasicCallbackTestCase::BasicCallbackTestCase ()
  : TestCase ("Check basic Callback mechansim")
{
}

void
BasicCallbackTestCase::DoSetup (void)
{
  m_test1 = false;
  m_test2 = false;
  m_test3 = false;
  m_test4 = false;
  gBasicCallbackTest5 = false;
  gBasicCallbackTest6 = false;
  gBasicCallbackTest7 = false;
}

void
BasicCallbackTestCase::DoRun (void)
{
  //
  // Make sure we can declare and compile a Callback pointing to a member 
  // function returning void and execute it.
  //
  Callback<void> target1 (this, &BasicCallbackTestCase::Target1);
  target1 ();
  NS_TEST_ASSERT_MSG_EQ (m_test1, true, "Callback did not fire");

  //
  // Make sure we can declare and compile a Callback pointing to a member 
  // function that returns an int and execute it.
  //
  Callback<int> target2;
  target2 = Callback<int> (this, &BasicCallbackTestCase::Target2);
  target2 ();
  NS_TEST_ASSERT_MSG_EQ (m_test2, true, "Callback did not fire");

  //
  // Make sure we can declare and compile a Callback pointing to a member 
  // function that returns void, takes a double parameter, and execute it.
  //
  Callback<void, double> target3 = Callback<void, double> (this, &BasicCallbackTestCase::Target3);
  target3 (0.0);
  NS_TEST_ASSERT_MSG_EQ (m_test3, true, "Callback did not fire");

  //
  // Make sure we can declare and compile a Callback pointing to a member 
  // function that returns void, takes two parameters, and execute it.
  //
  Callback<int, double, int> target4 = Callback<int, double, int> (this, &BasicCallbackTestCase::Target4);
  target4 (0.0, 1);
  NS_TEST_ASSERT_MSG_EQ (m_test4, true, "Callback did not fire");

  //
  // Make sure we can declare and compile a Callback pointing to a non-member 
  // function that returns void, and execute it.  This is a lower level call
  // than MakeCallback so we have got to include at least two arguments to make
  // sure that the constructor is properly disambiguated.  If the arguments are
  // not needed, we just pass in dummy values.
  //
  Callback<void> target5 = Callback<void> (&BasicCallbackTarget5, true, true);
  target5 ();
  NS_TEST_ASSERT_MSG_EQ (gBasicCallbackTest5, true, "Callback did not fire");

  //
  // Make sure we can declare and compile a Callback pointing to a non-member 
  // function that returns void, takes one integer argument and execute it.
  // We also need to provide two dummy arguments to the constructor here.
  //
  Callback<void, int> target6 = Callback<void, int> (&BasicCallbackTarget6, true, true);
  target6 (1);
  NS_TEST_ASSERT_MSG_EQ (gBasicCallbackTest6, true, "Callback did not fire");

  //
  // Make sure we can declare and compile a Callback pointing to a non-member 
  // function that returns int, takes one integer argument and execute it.
  // We also need to provide two dummy arguments to the constructor here.
  //
  Callback<int, int> target7 = Callback<int, int> (&BasicCallbackTarget7, true, true);
  target7 (1);
  NS_TEST_ASSERT_MSG_EQ (gBasicCallbackTest7, true, "Callback did not fire");
}

// ===========================================================================
// Test the MakeCallback mechanism
// ===========================================================================
class MakeCallbackTestCase : public TestCase
{
public:
  MakeCallbackTestCase ();
  virtual ~MakeCallbackTestCase () {}

  void Target1 (void) { m_test1 = true; }
  int Target2 (void) { m_test2 = true; return 2; }
  void Target3 (double a) { m_test3 = true; }
  int Target4 (double a, int b) { m_test4 = true; return 4; }

private:
  virtual void DoRun (void);
  virtual void DoSetup (void);

  bool m_test1;
  bool m_test2;
  bool m_test3;
  bool m_test4;
};

static bool gMakeCallbackTest5;
static bool gMakeCallbackTest6;
static bool gMakeCallbackTest7;

void 
MakeCallbackTarget5 (void)
{
  gMakeCallbackTest5 = true;
}

void 
MakeCallbackTarget6 (int)
{
  gMakeCallbackTest6 = true;
}

int 
MakeCallbackTarget7 (int a)
{
  gMakeCallbackTest7 = true;
  return a;
}

MakeCallbackTestCase::MakeCallbackTestCase ()
  : TestCase ("Check MakeCallback() mechanism")
{
}

void
MakeCallbackTestCase::DoSetup (void)
{
  m_test1 = false;
  m_test2 = false;
  m_test3 = false;
  m_test4 = false;
  gMakeCallbackTest5 = false;
  gMakeCallbackTest6 = false;
  gMakeCallbackTest7 = false;
}

void
MakeCallbackTestCase::DoRun (void)
{
  //
  // Make sure we can declare and make a Callback pointing to a member 
  // function returning void and execute it.
  //
  Callback<void> target1 = MakeCallback (&MakeCallbackTestCase::Target1, this);
  target1 ();
  NS_TEST_ASSERT_MSG_EQ (m_test1, true, "Callback did not fire");

  //
  // Make sure we can declare and make a Callback pointing to a member 
  // function that returns an int and execute it.
  //
  Callback<int> target2 = MakeCallback (&MakeCallbackTestCase::Target2, this);
  target2 ();
  NS_TEST_ASSERT_MSG_EQ (m_test2, true, "Callback did not fire");

  //
  // Make sure we can declare and make a Callback pointing to a member 
  // function that returns void, takes a double parameter, and execute it.
  //
  Callback<void, double> target3 = MakeCallback (&MakeCallbackTestCase::Target3, this);
  target3 (0.0);
  NS_TEST_ASSERT_MSG_EQ (m_test3, true, "Callback did not fire");

  //
  // Make sure we can declare and make a Callback pointing to a member 
  // function that returns void, takes two parameters, and execute it.
  //
  Callback<int, double, int> target4 = MakeCallback (&MakeCallbackTestCase::Target4, this);
  target4 (0.0, 1);
  NS_TEST_ASSERT_MSG_EQ (m_test4, true, "Callback did not fire");

  //
  // Make sure we can declare and make a Callback pointing to a non-member 
  // function that returns void, and execute it.  This uses a higher level call
  // than in the basic tests so we do not need to include any dummy arguments
  // here.
  //
  Callback<void> target5 = MakeCallback (&MakeCallbackTarget5);
  target5 ();
  NS_TEST_ASSERT_MSG_EQ (gMakeCallbackTest5, true, "Callback did not fire");

  //
  // Make sure we can declare and compile a Callback pointing to a non-member 
  // function that returns void, takes one integer argument and execute it.
  // This uses a higher level call than in the basic tests so we do not need to 
  // include any dummy arguments here.
  //
  Callback<void, int> target6 = MakeCallback (&MakeCallbackTarget6);
  target6 (1);
  NS_TEST_ASSERT_MSG_EQ (gMakeCallbackTest6, true, "Callback did not fire");

  //
  // Make sure we can declare and compile a Callback pointing to a non-member 
  // function that returns int, takes one integer argument and execute it.
  // This uses a higher level call than in the basic tests so we do not need to 
  // include any dummy arguments here.
  //
  Callback<int, int> target7 = MakeCallback (&MakeCallbackTarget7);
  target7 (1);
  NS_TEST_ASSERT_MSG_EQ (gMakeCallbackTest7, true, "Callback did not fire");
}

// ===========================================================================
// Test the MakeBoundCallback mechanism
// ===========================================================================
class MakeBoundCallbackTestCase : public TestCase
{
public:
  MakeBoundCallbackTestCase ();
  virtual ~MakeBoundCallbackTestCase () {}

private:
  virtual void DoRun (void);
  virtual void DoSetup (void);
};

static int gMakeBoundCallbackTest1;
static bool *gMakeBoundCallbackTest2;
static bool *gMakeBoundCallbackTest3a;
static int gMakeBoundCallbackTest3b;

void 
MakeBoundCallbackTarget1 (int a)
{
  gMakeBoundCallbackTest1 = a;
}

void 
MakeBoundCallbackTarget2 (bool *a)
{
  gMakeBoundCallbackTest2 = a;
}

int
MakeBoundCallbackTarget3 (bool *a, int b)
{
  gMakeBoundCallbackTest3a = a;
  gMakeBoundCallbackTest3b = b;
  return 1234;
}

MakeBoundCallbackTestCase::MakeBoundCallbackTestCase ()
  : TestCase ("Check MakeBoundCallback() mechanism")
{
}

void
MakeBoundCallbackTestCase::DoSetup (void)
{
  gMakeBoundCallbackTest1 = 0;
  gMakeBoundCallbackTest2 = 0;
  gMakeBoundCallbackTest3a = 0;
  gMakeBoundCallbackTest3b = 0;
}

void
MakeBoundCallbackTestCase::DoRun (void)
{
  //
  // This is slightly tricky to explain.  A bound Callback allows us to package
  // up arguments for use later.  The arguments are bound when the callback is
  // created and the code that fires the Callback does not know they are there.
  // 
  // Since the callback is *declared* according to the way it will be used, the
  // arguments are not seen there.  However, the target function of the callback
  // will have the provided arguments present.  The MakeBoundCallback template
  // function is what connects the two together and where you provide the 
  // arguments to be bound.
  //
  // Here we declare a Callback that returns a void and takes no parameters.
  // MakeBoundCallback connects this Callback to a target function that returns
  // void and takes an integer argument.  That integer argument is bound to the 
  // value 1234.  When the Callback is fired, no integer argument is provided
  // directly.  The argument is provided by bound Callback mechanism. 
  //
  Callback<void> target1 = MakeBoundCallback (&MakeBoundCallbackTarget1, 1234);
  target1 ();
  NS_TEST_ASSERT_MSG_EQ (gMakeBoundCallbackTest1, 1234, "Callback did not fire or binding not correct");

  //
  // Make sure we can bind a pointer value (a common use case).
  //
  bool a;
  Callback<void> target2 = MakeBoundCallback (&MakeBoundCallbackTarget2, &a);
  target2 ();
  NS_TEST_ASSERT_MSG_EQ (gMakeBoundCallbackTest2, &a, "Callback did not fire or binding not correct");

  //
  // Make sure we can mix and match bound and unbound arguments.  This callback
  // returns an integer so we should see that appear.
  //
  Callback<int, int> target3 = MakeBoundCallback (&MakeBoundCallbackTarget3, &a);
  int result = target3 (2468);
  NS_TEST_ASSERT_MSG_EQ (result, 1234, "Return value of callback not correct");
  NS_TEST_ASSERT_MSG_EQ (gMakeBoundCallbackTest3a, &a, "Callback did not fire or binding not correct");
  NS_TEST_ASSERT_MSG_EQ (gMakeBoundCallbackTest3b, 2468, "Callback did not fire or argument not correct");
}

// ===========================================================================
// Test the Nullify mechanism
// ===========================================================================
class NullifyCallbackTestCase : public TestCase
{
public:
  NullifyCallbackTestCase ();
  virtual ~NullifyCallbackTestCase () {}

  void Target1 (void) { m_test1 = true; }

private:
  virtual void DoRun (void);
  virtual void DoSetup (void);

  bool m_test1;
};

NullifyCallbackTestCase::NullifyCallbackTestCase ()
  : TestCase ("Check Nullify() and IsNull()")
{
}

void
NullifyCallbackTestCase::DoSetup (void)
{
  m_test1 = false;
}

void
NullifyCallbackTestCase::DoRun (void)
{
  //
  // Make sure we can declare and make a Callback pointing to a member 
  // function returning void and execute it.
  //
  Callback<void> target1 = MakeCallback (&NullifyCallbackTestCase::Target1, this);
  target1 ();
  NS_TEST_ASSERT_MSG_EQ (m_test1, true, "Callback did not fire");

  NS_TEST_ASSERT_MSG_EQ (target1.IsNull (), false, "Working Callback reports IsNull()");

  target1.Nullify ();

  NS_TEST_ASSERT_MSG_EQ (target1.IsNull (), true, "Nullified Callback reports not IsNull()");
}

// ===========================================================================
// Make sure that various MakeCallback template functions compile and execute.
// Doesn't check an results of the execution.
// ===========================================================================
class MakeCallbackTemplatesTestCase : public TestCase
{
public:
  MakeCallbackTemplatesTestCase ();
  virtual ~MakeCallbackTemplatesTestCase () {}

  void Target1 (void) { m_test1 = true; }

private:
  virtual void DoRun (void);

  bool m_test1;
};

void TestFZero (void) {}
void TestFOne (int) {}
void TestFTwo (int, int) {}
void TestFThree (int, int, int) {}
void TestFFour (int, int, int, int) {}
void TestFFive (int, int, int, int, int) {}
void TestFSix (int, int, int, int, int, int) {}

void TestFROne (int &) {}
void TestFRTwo (int &, int &) {}
void TestFRThree (int &, int &, int &) {}
void TestFRFour (int &, int &, int &, int &) {}
void TestFRFive (int &, int &, int &, int &, int &) {}
void TestFRSix (int &, int &, int &, int &, int &, int &) {}

class CallbackTestParent
{
public:
  void PublicParent (void) {}
protected:
  void ProtectedParent (void) {}
  static void StaticProtectedParent (void) {}
private:
  void PrivateParent (void) {}
};

class CallbackTestClass : public CallbackTestParent
{
public:
  void TestZero (void) {}
  void TestOne (int) {}
  void TestTwo (int, int) {}
  void TestThree (int, int, int) {}
  void TestFour (int, int, int, int) {}
  void TestFive (int, int, int, int, int) {}
  void TestSix (int, int, int, int, int, int) {}
  void TestCZero (void) const {}
  void TestCOne (int) const {}
  void TestCTwo (int, int) const {}
  void TestCThree (int, int, int) const {}
  void TestCFour (int, int, int, int) const {}
  void TestCFive (int, int, int, int, int) const {}
  void TestCSix (int, int, int, int, int, int) const {}

  void CheckParentalRights (void)
  {
    MakeCallback (&CallbackTestParent::StaticProtectedParent);
    MakeCallback (&CallbackTestParent::PublicParent, this);
    MakeCallback (&CallbackTestClass::ProtectedParent, this);
    // as expected, fails.
    // MakeCallback (&CallbackTestParent::PrivateParent, this);
    // unexpected, but fails too.  It does fumble me.
    // MakeCallback (&CallbackTestParent::ProtectedParent, this);
  }

};

MakeCallbackTemplatesTestCase::MakeCallbackTemplatesTestCase ()
  : TestCase ("Check various MakeCallback() template functions")
{
}

void
MakeCallbackTemplatesTestCase::DoRun (void)
{
  CallbackTestClass that;

  MakeCallback (&CallbackTestClass::TestZero, &that);
  MakeCallback (&CallbackTestClass::TestOne, &that);
  MakeCallback (&CallbackTestClass::TestTwo, &that);
  MakeCallback (&CallbackTestClass::TestThree, &that);
  MakeCallback (&CallbackTestClass::TestFour, &that);
  MakeCallback (&CallbackTestClass::TestFive, &that);
  MakeCallback (&CallbackTestClass::TestSix, &that);

  MakeCallback (&CallbackTestClass::TestCZero, &that);
  MakeCallback (&CallbackTestClass::TestCOne, &that);
  MakeCallback (&CallbackTestClass::TestCTwo, &that);
  MakeCallback (&CallbackTestClass::TestCThree, &that);
  MakeCallback (&CallbackTestClass::TestCFour, &that);
  MakeCallback (&CallbackTestClass::TestCFive, &that);
  MakeCallback (&CallbackTestClass::TestCSix, &that);

  MakeCallback (&TestFZero);
  MakeCallback (&TestFOne);
  MakeCallback (&TestFTwo);
  MakeCallback (&TestFThree);
  MakeCallback (&TestFFour);
  MakeCallback (&TestFFive);
  MakeCallback (&TestFSix);

  MakeCallback (&TestFROne);
  MakeCallback (&TestFRTwo);
  MakeCallback (&TestFRThree);
  MakeCallback (&TestFRFour);
  MakeCallback (&TestFRFive);
  MakeCallback (&TestFRSix);

  MakeBoundCallback (&TestFOne, 1);
  MakeBoundCallback (&TestFTwo, 1);
  MakeBoundCallback (&TestFThree, 1);
  MakeBoundCallback (&TestFFour, 1);
  MakeBoundCallback (&TestFFive, 1);

  MakeBoundCallback (&TestFROne, 1);
  MakeBoundCallback (&TestFRTwo, 1);
  MakeBoundCallback (&TestFRThree, 1);
  MakeBoundCallback (&TestFRFour, 1);
  MakeBoundCallback (&TestFRFive, 1);

  that.CheckParentalRights ();
}

// ===========================================================================
// The Test Suite that glues all of the Test Cases together.
// ===========================================================================
class CallbackTestSuite : public TestSuite
{
public:
  CallbackTestSuite ();
};

CallbackTestSuite::CallbackTestSuite ()
  : TestSuite ("callback", UNIT)
{
  AddTestCase (new BasicCallbackTestCase, TestCase::QUICK);
  AddTestCase (new MakeCallbackTestCase, TestCase::QUICK);
  AddTestCase (new MakeBoundCallbackTestCase, TestCase::QUICK);
  AddTestCase (new NullifyCallbackTestCase, TestCase::QUICK);
  AddTestCase (new MakeCallbackTemplatesTestCase, TestCase::QUICK);
}

static CallbackTestSuite CallbackTestSuite;
