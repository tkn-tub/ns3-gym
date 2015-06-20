/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009, 2010 MIRKO BANCHI
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
 * Author: Mirko Banchi <mk.banchi@gmail.com>
 */

#include "ns3/test.h"
#include "ns3/log.h"
#include "ns3/qos-utils.h"
#include "ns3/ctrl-headers.h"
#include <list>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("BlockAckTest");

/**
 * This simple test verifies the correctness of buffering for packets received
 * under block ack. In order to completely understand this example is important to cite
 * section 9.10.3 in IEEE802.11 standard:
 *
 * "[...] The sequence number space is considered divided into two parts, one of which
 * is “old” and one of which is “new” by means of a boundary created by adding half the
 * sequence number range to the current start of receive window (modulo 2^12)."
 */
//-------------------------------------------------------------------------------------

/* ----- = old packets
 * +++++ = new packets
 *
 *  CASE A: startSeq < endSeq
 *                        -  -   +
 *  initial buffer state: 0 16 56000
 *
 *
 *    0                            4095
 *    |------|++++++++++++++++|-----|
 *           ^                ^
 *           | startSeq       | endSeq = 4000
 *
 *  first received packet's sequence control = 64016 (seqNum = 4001, fragNum = 0) -
 *  second received packet's sequence control = 63984 (seqNum = 3999, fragNum = 0) +
 *  4001 is older seq number so this packet should be inserted at the buffer's begin.
 *  3999 is previous element of older of new packets: it should be inserted at the end of buffer.
 *
 *  expected buffer state: 64016 0 16 56000 63984
 *
 */
class PacketBufferingCaseA : public TestCase
{
public:
  PacketBufferingCaseA ();
  virtual ~PacketBufferingCaseA ();
private:
  virtual void DoRun (void);
  std::list<uint16_t> m_expectedBuffer;
};

PacketBufferingCaseA::PacketBufferingCaseA ()
  : TestCase ("Check correct order of buffering when startSequence < endSeq")
{
  m_expectedBuffer.push_back (64016);
  m_expectedBuffer.push_back (0);
  m_expectedBuffer.push_back (16);
  m_expectedBuffer.push_back (56000);
  m_expectedBuffer.push_back (63984);
}

PacketBufferingCaseA::~PacketBufferingCaseA ()
{
}

void
PacketBufferingCaseA::DoRun (void)
{
  std::list<uint16_t> m_buffer;
  std::list<uint16_t>::iterator i,j;
  m_buffer.push_back (0);
  m_buffer.push_back (16);
  m_buffer.push_back (56000);

  uint16_t endSeq = 4000;

  uint16_t receivedSeq = 4001 * 16;
  uint32_t mappedSeq = QosUtilsMapSeqControlToUniqueInteger (receivedSeq, endSeq);
  for (i = m_buffer.begin (); i != m_buffer.end () && QosUtilsMapSeqControlToUniqueInteger ((*i), endSeq) < mappedSeq; i++)
    {
    }
  {
    m_buffer.insert (i, receivedSeq);
  }

  receivedSeq = 3999 * 16;
  mappedSeq = QosUtilsMapSeqControlToUniqueInteger (receivedSeq, endSeq);
  for (i = m_buffer.begin (); i != m_buffer.end () && QosUtilsMapSeqControlToUniqueInteger ((*i), endSeq) < mappedSeq; i++)
    {
    }
  {
    m_buffer.insert (i, receivedSeq);
  }

  for (i = m_buffer.begin (), j = m_expectedBuffer.begin (); i != m_buffer.end (); i++, j++)
    {
      NS_TEST_EXPECT_MSG_EQ (*i, *j, "error in buffer order");
    }
}


/* ----- = old packets
 * +++++ = new packets
 *
 *  CASE B: startSeq > endSeq
 *                         -    +    +
 *  initial buffer state: 256 64000 16
 *
 *
 *    0                            4095
 *    |++++++|----------------|++++++|
 *           ^                ^
 *           | endSeq = 10    | startSeq
 *
 *  first received packet's sequence control = 240 (seqNum = 15, fragNum = 0)  -
 *  second received packet's sequence control = 241 (seqNum = 15, fragNum = 1) -
 *  third received packet's sequence control = 64800 (seqNum = 4050, fragNum = 0) +
 *  240 is an old packet should be inserted at the buffer's begin.
 *  241 is an old packet: second segment of the above packet.
 *  4050 is a new packet: it should be inserted between 64000 and 16.
 *
 *  expected buffer state: 240 241 256 64000 64800 16
 *
 */
class PacketBufferingCaseB : public TestCase
{
public:
  PacketBufferingCaseB ();
  virtual ~PacketBufferingCaseB ();
private:
  virtual void DoRun (void);
  std::list<uint16_t> m_expectedBuffer;
};

PacketBufferingCaseB::PacketBufferingCaseB ()
  : TestCase ("Check correct order of buffering when startSequence > endSeq")
{
  m_expectedBuffer.push_back (240);
  m_expectedBuffer.push_back (241);
  m_expectedBuffer.push_back (256);
  m_expectedBuffer.push_back (64000);
  m_expectedBuffer.push_back (64800);
  m_expectedBuffer.push_back (16);
}

PacketBufferingCaseB::~PacketBufferingCaseB ()
{
}

void
PacketBufferingCaseB::DoRun (void)
{
  std::list<uint16_t> m_buffer;
  std::list<uint16_t>::iterator i,j;
  m_buffer.push_back (256);
  m_buffer.push_back (64000);
  m_buffer.push_back (16);

  uint16_t endSeq = 10;

  uint16_t receivedSeq = 15 * 16;
  uint32_t mappedSeq = QosUtilsMapSeqControlToUniqueInteger (receivedSeq, endSeq);
  for (i = m_buffer.begin (); i != m_buffer.end () && QosUtilsMapSeqControlToUniqueInteger ((*i), endSeq) < mappedSeq; i++)
    {
    }
  {
    m_buffer.insert (i, receivedSeq);
  }

  receivedSeq = 15 * 16 + 1;
  mappedSeq = QosUtilsMapSeqControlToUniqueInteger (receivedSeq, endSeq);
  for (i = m_buffer.begin (); i != m_buffer.end () && QosUtilsMapSeqControlToUniqueInteger ((*i), endSeq) < mappedSeq; i++)
    {
    }
  {
    m_buffer.insert (i, receivedSeq);
  }

  receivedSeq = 4050 * 16;
  mappedSeq = QosUtilsMapSeqControlToUniqueInteger (receivedSeq, endSeq);
  for (i = m_buffer.begin (); i != m_buffer.end () && QosUtilsMapSeqControlToUniqueInteger ((*i), endSeq) < mappedSeq; i++)
    {
    }
  {
    m_buffer.insert (i, receivedSeq);
  }

  for (i = m_buffer.begin (), j = m_expectedBuffer.begin (); i != m_buffer.end (); i++, j++)
    {
      NS_TEST_EXPECT_MSG_EQ (*i, *j, "error in buffer order");
    }
}


//Test for block ack header
class CtrlBAckResponseHeaderTest : public TestCase
{
public:
  CtrlBAckResponseHeaderTest ();
private:
  virtual void DoRun ();
  CtrlBAckResponseHeader m_blockAckHdr;
};

CtrlBAckResponseHeaderTest::CtrlBAckResponseHeaderTest ()
  : TestCase ("Check the correctness of block ack compressed bitmap")
{
}

void
CtrlBAckResponseHeaderTest::DoRun (void)
{
  m_blockAckHdr.SetType (COMPRESSED_BLOCK_ACK);

  //Case 1: startSeq < endSeq
  //          179        242
  m_blockAckHdr.SetStartingSequence (179);
  for (uint32_t i = 179; i < 220; i++)
    {
      m_blockAckHdr.SetReceivedPacket (i);
    }
  for (uint32_t i = 225; i <= 242; i++)
    {
      m_blockAckHdr.SetReceivedPacket (i);
    }
  NS_TEST_EXPECT_MSG_EQ (m_blockAckHdr.GetCompressedBitmap (), 0xffffc1ffffffffffLL, "error in compressed bitmap");
  m_blockAckHdr.SetReceivedPacket (1500);
  NS_TEST_EXPECT_MSG_EQ (m_blockAckHdr.GetCompressedBitmap (), 0xffffc1ffffffffffLL, "error in compressed bitmap");
  NS_TEST_EXPECT_MSG_EQ (m_blockAckHdr.IsPacketReceived (220), false, "error in compressed bitmap");
  NS_TEST_EXPECT_MSG_EQ (m_blockAckHdr.IsPacketReceived (225), true, "error in compressed bitmap");
  NS_TEST_EXPECT_MSG_EQ (m_blockAckHdr.IsPacketReceived (1500), false, "error in compressed bitmap");

  m_blockAckHdr.ResetBitmap ();

  //Case 2: startSeq > endSeq
  //          4090       58
  m_blockAckHdr.SetStartingSequence (4090);
  for (uint32_t i = 4090; i != 10; i = (i + 1) % 4096)
    {
      m_blockAckHdr.SetReceivedPacket (i);
    }
  for (uint32_t i = 22; i < 25; i++)
    {
      m_blockAckHdr.SetReceivedPacket (i);
    }
  NS_TEST_EXPECT_MSG_EQ (m_blockAckHdr.GetCompressedBitmap (), 0x00000000007000ffffLL, "error in compressed bitmap");
  m_blockAckHdr.SetReceivedPacket (80);
  NS_TEST_EXPECT_MSG_EQ (m_blockAckHdr.GetCompressedBitmap (), 0x00000000007000ffffLL, "error in compressed bitmap");
  NS_TEST_EXPECT_MSG_EQ (m_blockAckHdr.IsPacketReceived (4090), true, "error in compressed bitmap");
  NS_TEST_EXPECT_MSG_EQ (m_blockAckHdr.IsPacketReceived (4095), true, "error in compressed bitmap");
  NS_TEST_EXPECT_MSG_EQ (m_blockAckHdr.IsPacketReceived (10), false, "error in compressed bitmap");
  NS_TEST_EXPECT_MSG_EQ (m_blockAckHdr.IsPacketReceived (35), false, "error in compressed bitmap");
  NS_TEST_EXPECT_MSG_EQ (m_blockAckHdr.IsPacketReceived (80), false, "error in compressed bitmap");
}


class BlockAckTestSuite : public TestSuite
{
public:
  BlockAckTestSuite ();
};

BlockAckTestSuite::BlockAckTestSuite ()
  : TestSuite ("wifi-block-ack", UNIT)
{
  AddTestCase (new PacketBufferingCaseA, TestCase::QUICK);
  AddTestCase (new PacketBufferingCaseB, TestCase::QUICK);
  AddTestCase (new CtrlBAckResponseHeaderTest, TestCase::QUICK);
}

static BlockAckTestSuite g_blockAckTestSuite;
