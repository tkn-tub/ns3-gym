/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
//
// Copyright (c) 2008-2010 INESC Porto
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: Gustavo J. A. M. Carneiro  <gjc@inescporto.pt> <gjcarneiro@gmail.com>
//

#include "ns3/sequence-number.h"
#include "ns3/test.h"
#include "ns3/object.h"
#include "ns3/traced-value.h"
#include "ns3/trace-source-accessor.h"

namespace ns3 {

class SequenceNumberTestObj : public Object
{
  TracedValue<SequenceNumber32> m_testTracedSequenceNumber;


public:

  SequenceNumberTestObj () 
  {
    m_testTracedSequenceNumber = SequenceNumber32 (0);
  }

  static TypeId GetTypeId (void)
  {
    static TypeId tid = TypeId ("ns3::SequenceNumberTestObj")
      .SetParent<Object> ()
      .AddTraceSource ("TestTracedSequenceNumber",
                       "A traceable sequence number",
                       MakeTraceSourceAccessor (&SequenceNumberTestObj::m_testTracedSequenceNumber))
      .AddConstructor<SequenceNumberTestObj> ()
    ;
    return tid;
  }

  TypeId GetInstanceTypeId (void) const
  {
    return GetTypeId ();
  }

  void IncSequenceNumber ()
  {
    m_testTracedSequenceNumber += 1;
  }


};

class SequenceNumberTestCase : public TestCase
{
  SequenceNumber32 m_oldval;
  SequenceNumber32 m_newval;

  void SequenceNumberTracer (SequenceNumber32 oldval, SequenceNumber32 newval);

public:

  SequenceNumberTestCase ();
  virtual ~SequenceNumberTestCase ();
  virtual void DoRun (void);
};

SequenceNumberTestCase::SequenceNumberTestCase ()
  : TestCase ("SequenceNumber")
{
  m_oldval = 0;
  m_newval = 0;
}

SequenceNumberTestCase::~SequenceNumberTestCase ()
{
}

void
SequenceNumberTestCase::SequenceNumberTracer (SequenceNumber32 oldval, SequenceNumber32 newval)
{
  m_oldval = oldval;
  m_newval = newval;
}

void SequenceNumberTestCase::DoRun (void)
{
#define NS_TEST_ASSERT_EQUAL(a,b) NS_TEST_ASSERT_MSG_EQ (a,b, "foo")
#define NS_TEST_ASSERT(a) NS_TEST_ASSERT_MSG_EQ (bool(a), true, "foo")

  {
    SequenceNumber32 num1 (3), num2 (5);
    uint32_t value;

    value = (num1 + num2).GetValue ();
    NS_TEST_ASSERT_EQUAL (value, 8);

    num1 += num2.GetValue ();
    NS_TEST_ASSERT_EQUAL (num1, SequenceNumber32 (8));

    ++num1;
    NS_TEST_ASSERT_EQUAL (num1, SequenceNumber32 (9));

    --num1;
    NS_TEST_ASSERT_EQUAL (num1, SequenceNumber32 (8));

    num1++;
    NS_TEST_ASSERT_EQUAL (num1, SequenceNumber32 (9));

    num1--;
    NS_TEST_ASSERT_EQUAL (num1, SequenceNumber32 (8));

  }

  {
    SequenceNumber16 num1 (60900), num2 (5), num3 (10000);

    NS_TEST_ASSERT (num1 == num1);

    NS_TEST_ASSERT (num2 != num1);

    NS_TEST_ASSERT (num3 > num2);
    NS_TEST_ASSERT (num3 >= num2);
    NS_TEST_ASSERT (num1 < num3);
    NS_TEST_ASSERT (num1 <= num3);

    NS_TEST_ASSERT (num1 < num2);
    NS_TEST_ASSERT (num1 <= num2);
    NS_TEST_ASSERT (num2 > num1);
    NS_TEST_ASSERT (num2 >= num1);

    NS_TEST_ASSERT (num1+num2 > num1);
    NS_TEST_ASSERT (num1+num2 >= num1);
    NS_TEST_ASSERT (num1 < num1+num2);
    NS_TEST_ASSERT (num1 <= num1+num2);

    NS_TEST_ASSERT (num1 < num1+num3);
    NS_TEST_ASSERT (num1 <= num1+num3);
    NS_TEST_ASSERT (num1+num3 > num1);
    NS_TEST_ASSERT (num1+num3 >= num1);
  }

  {
    NS_TEST_ASSERT_EQUAL ((SequenceNumber16 (1000) + SequenceNumber16 (6000)) - SequenceNumber16 (1000), 6000);
    NS_TEST_ASSERT_EQUAL ((SequenceNumber16 (60000) + SequenceNumber16 (6000)) - SequenceNumber16 (60000), 6000);
    NS_TEST_ASSERT_EQUAL (SequenceNumber16 (1000) - SequenceNumber16 (6000), -5000);
    NS_TEST_ASSERT_EQUAL ((SequenceNumber16 (60000) + SequenceNumber16 (1000)) - SequenceNumber16 (65000), -4000);
  }

  {
    SequenceNumber32 num1 (3);

    NS_TEST_ASSERT_EQUAL (num1 + 10, SequenceNumber32 (13));
    num1 += -1;
    NS_TEST_ASSERT_EQUAL (num1, SequenceNumber32 (2));

    NS_TEST_ASSERT_EQUAL (num1 - (num1 - 100), 100);
  }

  {
    Ptr<SequenceNumberTestObj> obj = CreateObject<SequenceNumberTestObj> ();
    obj->TraceConnectWithoutContext ("TestTracedSequenceNumber", MakeCallback (&SequenceNumberTestCase::SequenceNumberTracer, this));
    obj->IncSequenceNumber ();
    NS_TEST_ASSERT_EQUAL (m_oldval, SequenceNumber32 (0));
    NS_TEST_ASSERT_EQUAL (m_newval, SequenceNumber32 (1));
    obj->Dispose ();
  }

}

static class SequenceNumberTestSuite : public TestSuite
{
public:
  SequenceNumberTestSuite ()
    : TestSuite ("SequenceNumber", UNIT) 
  {
    AddTestCase (new SequenceNumberTestCase ());
  }
} g_seqNumTests;

}

