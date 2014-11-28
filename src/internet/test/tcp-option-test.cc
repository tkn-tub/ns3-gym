/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014 Natale Patriciello <natale.patriciello@gmail.com>
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
 *
 */

#include "ns3/test.h"
#include "ns3/core-module.h"
#include "ns3/tcp-option.h"
#include "../src/internet/model/tcp-option-winscale.h"
#include "../src/internet/model/tcp-option-ts.h"

#include <string.h>

namespace ns3 {

class TcpOptionWSTestCase : public TestCase
{
public:
  TcpOptionWSTestCase (std::string name, uint8_t scale);

  void TestSerialize ();
  void TestDeserialize ();

private:
  virtual void DoRun (void);
  virtual void DoTeardown (void);

  uint8_t m_scale;
  Buffer m_buffer;
};


TcpOptionWSTestCase::TcpOptionWSTestCase (std::string name, uint8_t scale)
  : TestCase (name)
{
  m_scale = scale;
}

void
TcpOptionWSTestCase::DoRun ()
{
  TestSerialize ();
  TestDeserialize ();
}

void
TcpOptionWSTestCase::TestSerialize ()
{
  TcpOptionWinScale opt;

  opt.SetScale (m_scale);
  NS_TEST_EXPECT_MSG_EQ (m_scale, opt.GetScale (), "Scale isn't saved correctly");

  m_buffer.AddAtStart (opt.GetSerializedSize ());

  opt.Serialize (m_buffer.Begin ());
}

void
TcpOptionWSTestCase::TestDeserialize ()
{
  TcpOptionWinScale opt;

  Buffer::Iterator start = m_buffer.Begin ();
  uint8_t kind = start.PeekU8 ();

  NS_TEST_EXPECT_MSG_EQ (kind, TcpOption::WINSCALE, "Different kind found");

  opt.Deserialize (start);

  NS_TEST_EXPECT_MSG_EQ (m_scale, opt.GetScale (), "Different scale found");
}

void
TcpOptionWSTestCase::DoTeardown ()
{
}

class TcpOptionTSTestCase : public TestCase
{
public:
  TcpOptionTSTestCase (std::string name, uint32_t timestamp, uint32_t echo);

  void TestSerialize ();
  void TestDeserialize ();

private:
  virtual void DoRun (void);
  virtual void DoTeardown (void);

  uint32_t m_timestamp;
  uint32_t m_echo;
  Buffer m_buffer;
};


TcpOptionTSTestCase::TcpOptionTSTestCase (std::string name, uint32_t timestamp,
                                          uint32_t echo)
  : TestCase (name)
{
  m_timestamp = timestamp;
  m_echo = echo;
}

void
TcpOptionTSTestCase::DoRun ()
{
  TestSerialize ();
  TestDeserialize ();
}

void
TcpOptionTSTestCase::TestSerialize ()
{
  TcpOptionTS opt;

  opt.SetTimestamp (m_timestamp);
  opt.SetEcho (m_echo);

  NS_TEST_EXPECT_MSG_EQ (m_timestamp, opt.GetTimestamp (), "TS isn't saved correctly");
  NS_TEST_EXPECT_MSG_EQ (m_echo, opt.GetEcho (), "echo isn't saved correctly");

  m_buffer.AddAtStart (opt.GetSerializedSize ());

  opt.Serialize (m_buffer.Begin ());
}

void
TcpOptionTSTestCase::TestDeserialize ()
{
  TcpOptionTS opt;

  Buffer::Iterator start = m_buffer.Begin ();
  uint8_t kind = start.PeekU8 ();

  NS_TEST_EXPECT_MSG_EQ (kind, TcpOption::TS, "Different kind found");

  opt.Deserialize (start);

  NS_TEST_EXPECT_MSG_EQ (m_timestamp, opt.GetTimestamp (), "Different TS found");
  NS_TEST_EXPECT_MSG_EQ (m_echo, opt.GetEcho (), "Different echo found");
}

void
TcpOptionTSTestCase::DoTeardown ()
{
}

static class TcpOptionTestSuite : public TestSuite
{
public:
  TcpOptionTestSuite ()
    : TestSuite ("tcp-option", UNIT)
  {
    for (uint8_t i = 0; i < 15; ++i)
      {
        AddTestCase (new TcpOptionWSTestCase ("Testing window "
                                              "scale value", i), TestCase::QUICK);
      }

    Ptr<UniformRandomVariable> x = CreateObject<UniformRandomVariable> ();

    for (uint32_t i = 0; i < 1000; ++i)
      {
        AddTestCase (new TcpOptionTSTestCase ("Testing serialization of random "
                                              "values for timestamp",
                                              x->GetInteger (),
                                              x->GetInteger ()), TestCase::QUICK);
      }

  }

} g_TcpOptionTestSuite;

} // namespace ns3
