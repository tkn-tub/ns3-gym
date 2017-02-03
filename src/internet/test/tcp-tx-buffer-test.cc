/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
#include "ns3/tcp-tx-buffer.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/log.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TcpTxBufferTestSuite");

class TcpTxBufferTestCase : public TestCase
{
public:
  TcpTxBufferTestCase ();

private:
  virtual void DoRun (void);
  virtual void DoTeardown (void);

  void TestNewBlock ();
  void TestTransmittedBlock ();
};

TcpTxBufferTestCase::TcpTxBufferTestCase ()
  : TestCase ("TcpTxBuffer Test")
{
}

void
TcpTxBufferTestCase::DoRun ()
{
  /*
   * Cases for new block:
   * -> is exactly the same as stored
   * -> starts over the boundary, but ends earlier
   * -> starts over the boundary, but ends after
   */
  Simulator::Schedule (Seconds (0.0), &TcpTxBufferTestCase::TestNewBlock, this);

  /*
   * Cases for transmitted block:
   * -> is exactly the same as previous
   * -> starts over the boundary, but ends earlier
   * -> starts over the boundary, but ends after
   * -> starts inside a packet, ends right
   * -> starts inside a packet, ends earlier in the same packet
   * -> starts inside a packet, ends in another packet
   */
  Simulator::Schedule (Seconds (0.0),
                       &TcpTxBufferTestCase::TestTransmittedBlock, this);
  Simulator::Run ();
  Simulator::Destroy ();
}

void
TcpTxBufferTestCase::TestNewBlock ()
{
  // Manually recreating all the conditions
  TcpTxBuffer txBuf;
  txBuf.SetHeadSequence (SequenceNumber32 (1));

  // get a packet which is exactly the same stored
  Ptr<Packet> p1 = Create<Packet> (100);
  txBuf.Add (p1);

  NS_TEST_ASSERT_MSG_EQ (txBuf.SizeFromSequence (SequenceNumber32 (1)), 100,
                         "TxBuf miscalculates size");
  //NS_TEST_ASSERT_MSG_EQ (txBuf.BytesInFlight (), 0,
  //                       "TxBuf miscalculates size of in flight segments");

  Ptr<Packet> ret = txBuf.CopyFromSequence (100, SequenceNumber32 (1));
  NS_TEST_ASSERT_MSG_EQ (ret->GetSize (), 100,
                         "Returned packet has different size than requested");
  NS_TEST_ASSERT_MSG_EQ (txBuf.SizeFromSequence (SequenceNumber32 (1)), 100,
                         "TxBuf miscalculates size");
  //NS_TEST_ASSERT_MSG_EQ (txBuf.BytesInFlight (), 100,
  //                       "TxBuf miscalculates size of in flight segments");

  txBuf.DiscardUpTo (SequenceNumber32 (101));
  NS_TEST_ASSERT_MSG_EQ (txBuf.SizeFromSequence (SequenceNumber32 (101)), 0,
                         "TxBuf miscalculates size");
  //NS_TEST_ASSERT_MSG_EQ (txBuf.BytesInFlight (), 0,
  //                       "TxBuf miscalculates size of in flight segments");

  // starts over the boundary, but ends earlier

  Ptr<Packet> p2 = Create<Packet> (100);
  txBuf.Add (p2);

  ret = txBuf.CopyFromSequence (50, SequenceNumber32 (101));
  NS_TEST_ASSERT_MSG_EQ (ret->GetSize (), 50,
                         "Returned packet has different size than requested");
  NS_TEST_ASSERT_MSG_EQ (txBuf.SizeFromSequence (SequenceNumber32 (151)), 50,
                         "TxBuf miscalculates size");
  //NS_TEST_ASSERT_MSG_EQ (txBuf.BytesInFlight (), 50,
  //                       "TxBuf miscalculates size of in flight segments");

  // starts over the boundary, but ends after
  Ptr<Packet> p3 = Create<Packet> (100);
  txBuf.Add (p3);

  ret = txBuf.CopyFromSequence (70, SequenceNumber32 (151));
  NS_TEST_ASSERT_MSG_EQ (ret->GetSize (), 70,
                         "Returned packet has different size than requested");
  NS_TEST_ASSERT_MSG_EQ (txBuf.SizeFromSequence (SequenceNumber32 (221)), 80,
                         "TxBuf miscalculates size");
  //NS_TEST_ASSERT_MSG_EQ (txBuf.BytesInFlight (), 120,
  //                       "TxBuf miscalculates size of in flight segments");

  ret = txBuf.CopyFromSequence (3000, SequenceNumber32 (221));
  NS_TEST_ASSERT_MSG_EQ (ret->GetSize (), 80,
                         "Returned packet has different size than requested");
  NS_TEST_ASSERT_MSG_EQ (txBuf.SizeFromSequence (SequenceNumber32 (301)), 0,
                         "TxBuf miscalculates size");
  //NS_TEST_ASSERT_MSG_EQ (txBuf.BytesInFlight (), 200,
  //                       "TxBuf miscalculates size of in flight segments");

  // Clear everything
  txBuf.DiscardUpTo (SequenceNumber32 (381));
  NS_TEST_ASSERT_MSG_EQ (txBuf.Size (), 0,
                         "Size is different than expected");
}

void
TcpTxBufferTestCase::TestTransmittedBlock ()
{
}

void
TcpTxBufferTestCase::DoTeardown ()
{
}

class TcpTxBufferTestSuite : public TestSuite
{
public:
  TcpTxBufferTestSuite ()
    : TestSuite ("tcp-tx-buffer", UNIT)
  {
    AddTestCase (new TcpTxBufferTestCase, TestCase::QUICK);
  }
};
static TcpTxBufferTestSuite  g_tcpTxBufferTestSuite;
