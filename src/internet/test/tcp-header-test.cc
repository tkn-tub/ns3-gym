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

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include "ns3/test.h"
#include "ns3/core-module.h"
#include "ns3/tcp-header.h"
#include "ns3/buffer.h"

#include "../src/internet/model/tcp-option-rfc793.h"

namespace ns3 {

class TcpHeaderGetSetTestCase : public TestCase
{
public:
  TcpHeaderGetSetTestCase (uint16_t sourcePort, uint16_t destinationPort,
                           SequenceNumber32 sequenceNumber, SequenceNumber32 ackNumber,
                           uint8_t flags, uint16_t m_windowSize, uint16_t m_urgentPointer,
                           std::string name);
protected:
  uint16_t m_sourcePort;        //!< Source port
  uint16_t m_destinationPort;   //!< Destination port
  SequenceNumber32 m_sequenceNumber;  //!< Sequence number
  SequenceNumber32 m_ackNumber;       //!< ACK number
  uint8_t m_flags;              //!< Flags (really a uint6_t)
  uint16_t m_windowSize;        //!< Window size
  uint16_t m_urgentPointer;     //!< Urgent pointer

private:
  virtual void DoRun (void);
  virtual void DoTeardown (void);

  TcpHeader m_header;
  Buffer m_buffer;
};

TcpHeaderGetSetTestCase::TcpHeaderGetSetTestCase (
  uint16_t sourcePort, uint16_t destinationPort,
  SequenceNumber32 sequenceNumber, SequenceNumber32 ackNumber,
  uint8_t flags, uint16_t windowSize, uint16_t urgentPointer,
  std::string name) : TestCase (name)
{
  m_sourcePort = sourcePort;
  m_destinationPort = destinationPort;
  m_sequenceNumber = sequenceNumber;
  m_ackNumber = ackNumber;
  m_flags = flags;
  m_windowSize = windowSize;
  m_urgentPointer = urgentPointer;

  m_header.SetSourcePort (m_sourcePort);
  m_header.SetDestinationPort (m_destinationPort);
  m_header.SetSequenceNumber (m_sequenceNumber);
  m_header.SetAckNumber (m_ackNumber);
  m_header.SetFlags (m_flags);
  m_header.SetWindowSize (m_windowSize);
  m_header.SetUrgentPointer (m_urgentPointer);

  NS_TEST_ASSERT_MSG_EQ (m_header.GetLength (), 5, "TcpHeader without option is"
                         " not 5 word");

  m_buffer.AddAtStart (m_header.GetSerializedSize ());
  m_header.Serialize (m_buffer.Begin ());
}

void TcpHeaderGetSetTestCase::DoRun (void)
{
  NS_TEST_ASSERT_MSG_EQ (m_sourcePort, m_header.GetSourcePort (),
                         "Different source port found");
  NS_TEST_ASSERT_MSG_EQ (m_destinationPort, m_header.GetDestinationPort (),
                         "Different destination port found");
  NS_TEST_ASSERT_MSG_EQ (m_sequenceNumber, m_header.GetSequenceNumber (),
                         "Different sequence number found");
  NS_TEST_ASSERT_MSG_EQ (m_ackNumber, m_header.GetAckNumber (),
                         "Different ack number found");
  NS_TEST_ASSERT_MSG_EQ (m_flags, m_header.GetFlags (),
                         "Different flags found");
  NS_TEST_ASSERT_MSG_EQ (m_windowSize, m_header.GetWindowSize (),
                         "Different window size found");
  NS_TEST_ASSERT_MSG_EQ (m_urgentPointer, m_header.GetUrgentPointer (),
                         "Different urgent pointer found");

  NS_TEST_ASSERT_MSG_EQ (m_header.GetLength (), 5, "TcpHeader without option is"
                         " not 5 word");

  TcpHeader copyHeader;

  copyHeader.Deserialize (m_buffer.Begin ());

  NS_TEST_ASSERT_MSG_EQ (m_sourcePort, copyHeader.GetSourcePort (),
                         "Different source port found in deserialized header");
  NS_TEST_ASSERT_MSG_EQ (m_destinationPort, copyHeader.GetDestinationPort (),
                         "Different destination port found in deserialized header");
  NS_TEST_ASSERT_MSG_EQ (m_sequenceNumber, copyHeader.GetSequenceNumber (),
                         "Different sequence number found in deserialized header");
  NS_TEST_ASSERT_MSG_EQ (m_ackNumber, copyHeader.GetAckNumber (),
                         "Different ack number found in deserialized header");
  NS_TEST_ASSERT_MSG_EQ (m_flags, copyHeader.GetFlags (),
                         "Different flags found in deserialized header");
  NS_TEST_ASSERT_MSG_EQ (m_windowSize, copyHeader.GetWindowSize (),
                         "Different window size found in deserialized header");
  NS_TEST_ASSERT_MSG_EQ (m_urgentPointer, copyHeader.GetUrgentPointer (),
                         "Different urgent pointer found in deserialized header");
}

void TcpHeaderGetSetTestCase::DoTeardown (void)
{

}

class TcpHeaderWithRFC793OptionTestCase : public TestCase
{
public:
  TcpHeaderWithRFC793OptionTestCase (std::string name);

private:
  virtual void DoRun (void);
  virtual void DoTeardown (void);

  /**
   * \brief Check an header with only one kind of option
   *
   */
  void OneOptionAtTime ();
  void CheckNoPadding ();
  void CheckCorrectDeserialize ();
};


TcpHeaderWithRFC793OptionTestCase::TcpHeaderWithRFC793OptionTestCase (std::string name)
  : TestCase (name)
{

}

void
TcpHeaderWithRFC793OptionTestCase::DoRun ()
{
  OneOptionAtTime ();
  CheckNoPadding ();
  CheckCorrectDeserialize ();
}

void
TcpHeaderWithRFC793OptionTestCase::CheckCorrectDeserialize ()
{
  TcpHeader source, destination;
  TcpOptionNOP temp;
  Buffer buffer;
  buffer.AddAtStart (40);

  Buffer::Iterator i = buffer.Begin ();
  source.AppendOption (&temp);

  source.Serialize (i);

  i.ReadU8 ();
  i.WriteU8 (59);

  i = buffer.Begin ();
  destination.Deserialize (i);

  NS_TEST_ASSERT_MSG_EQ (destination.HasOption (59), false, "Kind 59 registered");
}

void
TcpHeaderWithRFC793OptionTestCase::CheckNoPadding ()
{
  {
    TcpOptionNOP oNop1, oNop2, oNop3, oNop4;
    TcpHeader header;
    Buffer buffer;

    NS_TEST_ASSERT_MSG_EQ (header.GetLength (), 5, "TcpHeader without option is"
                           " not 5 word");
    header.AppendOption (&oNop1);
    header.AppendOption (&oNop2);
    header.AppendOption (&oNop3);
    header.AppendOption (&oNop4);

    NS_TEST_ASSERT_MSG_EQ (header.GetLength (), 6, "Four byte added as option "
                           "are not a word");
    NS_TEST_ASSERT_MSG_EQ (header.GetSerializedSize (), 24, "Four byte added as option "
                           "are not a word");

    buffer.AddAtStart (header.GetSerializedSize ());
    header.Serialize (buffer.Begin ());

    NS_TEST_ASSERT_MSG_EQ (header.GetSerializedSize (),
                           buffer.GetSize (), "Header not correctly serialized");

    // Inserted 4 byte NOP, no padding should be present
    Buffer::Iterator i = buffer.Begin ();
    i.Next (20);

    for (uint32_t j = 0; j < 4; ++j)
      {
        std::stringstream ss;
        ss << j;
        uint8_t value = i.ReadU8 ();
        NS_TEST_ASSERT_MSG_EQ (value, TcpOption::NOP,
                               "NOP not present at position " + ss.str ());
      }
  }
}

void
TcpHeaderWithRFC793OptionTestCase::OneOptionAtTime ()
{
  {
    TcpOptionEnd oEnd;
    TcpHeader header;
    Buffer buffer;

    NS_TEST_ASSERT_MSG_EQ (header.GetLength (), 5, "TcpHeader without option is"
                           " not 5 word");
    header.AppendOption (&oEnd);
    NS_TEST_ASSERT_MSG_EQ (header.GetLength (), 5, "Length has changed also for"
                           " END option");
    NS_TEST_ASSERT_MSG_EQ (header.GetSerializedSize (), 20, "Length has changed also for"
                           " END option");


    buffer.AddAtStart (header.GetSerializedSize ());
    header.Serialize (buffer.Begin ());

    NS_TEST_ASSERT_MSG_EQ (header.GetSerializedSize (),
                           buffer.GetSize (), "Header not correctly serialized");
  }

  {
    TcpOptionNOP oNop;
    TcpHeader header;
    Buffer buffer;

    NS_TEST_ASSERT_MSG_EQ (header.GetLength (), 5, "TcpHeader without option is"
                           " not 5 word");
    header.AppendOption (&oNop);
    NS_TEST_ASSERT_MSG_EQ (header.GetLength (), 6, "NOP option not handled correctly");
    NS_TEST_ASSERT_MSG_EQ (header.GetSerializedSize (), 24, "Different length found for"
                           "NOP option");

    buffer.AddAtStart (header.GetSerializedSize ());
    header.Serialize (buffer.Begin ());

    NS_TEST_ASSERT_MSG_EQ (header.GetSerializedSize (),
                           buffer.GetSize (), "Header not correctly serialized");

    // Inserted only 1 byte NOP, and so implementation should pad; so
    // the other 3 bytes should be NOP (so 4 byte NOP)
    Buffer::Iterator i = buffer.Begin ();
    i.Next (20);

    uint8_t value = i.ReadU8 ();
    NS_TEST_ASSERT_MSG_EQ (value, TcpOption::NOP, "NOP not present");
    for (uint32_t j = 1; j < 4; ++j)
      {
        std::stringstream ss;
        ss << j;

        value = i.ReadU8 ();
        NS_TEST_ASSERT_MSG_EQ (value, 0,
                               "Header not padded with 0 at position " + ss.str ());
      }
  }

  {
    TcpOptionMSS oMSS;
    oMSS.SetMSS (50);
    TcpHeader header, dest;
    Buffer buffer;

    NS_TEST_ASSERT_MSG_EQ (header.GetLength (), 5, "TcpHeader without option is"
                           " not 5 word");
    header.AppendOption (&oMSS);
    NS_TEST_ASSERT_MSG_EQ (header.GetLength (), 6, "MSS option not handled correctly");
    NS_TEST_ASSERT_MSG_EQ (header.GetSerializedSize (), 24, "Different length found for"
                           "MSS option");

    buffer.AddAtStart (header.GetSerializedSize ());
    header.Serialize (buffer.Begin ());

    NS_TEST_ASSERT_MSG_EQ (header.GetSerializedSize (),
                           buffer.GetSize (), "Header not correctly serialized");

    dest.Deserialize (buffer.Begin ());
    NS_TEST_ASSERT_MSG_EQ (header.HasOption (TcpOption::MSS),
                           true, "MSS option not correctly serialized");
  }
}


void
TcpHeaderWithRFC793OptionTestCase::DoTeardown ()
{
  uint32_t foo;
  foo = UINT32_MAX;
  foo--;

}

#define GET_RANDOM_UINT32(RandomVariable) \
  static_cast<uint32_t> (RandomVariable->GetInteger (0, UINT32_MAX))

#define GET_RANDOM_UINT16(RandomVariable) \
  static_cast<uint16_t> (RandomVariable->GetInteger (0, UINT16_MAX))

#define GET_RANDOM_UINT8(RandomVariable) \
  static_cast<uint8_t> (RandomVariable->GetInteger (0, UINT8_MAX))

#define GET_RANDOM_UINT6(RandomVariable) \
  static_cast<uint8_t> (RandomVariable->GetInteger (0, UINT8_MAX >> 2))

static class TcpHeaderTestSuite : public TestSuite
{
public:
  TcpHeaderTestSuite ()
    : TestSuite ("tcp-header", UNIT)
  {
    Ptr<UniformRandomVariable> x = CreateObject<UniformRandomVariable> ();

    for (uint32_t i = 0; i < 1000; ++i)
      {
        std::stringstream ss;
        ss << i;

        AddTestCase (new TcpHeaderGetSetTestCase (GET_RANDOM_UINT16 (x),
                                                  GET_RANDOM_UINT16 (x),
                                                  SequenceNumber32 (GET_RANDOM_UINT32 (x)),
                                                  SequenceNumber32 (GET_RANDOM_UINT32 (x)),
                                                  GET_RANDOM_UINT6 (x),
                                                  GET_RANDOM_UINT16 (x),
                                                  GET_RANDOM_UINT16 (x),
                                                  "Case number " + ss.str ()),
                     TestCase::QUICK);
      }

    AddTestCase (new TcpHeaderWithRFC793OptionTestCase ("Test for options in RFC 793"),
                 TestCase::QUICK);
  }

} g_TcpHeaderTestSuite;

} // namespace ns3
