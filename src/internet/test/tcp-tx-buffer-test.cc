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
  void TestNextSeg ();
  void TestUpdateScoreboardWithCraftedSACK ();
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
  Simulator::Schedule (Seconds (0.0),
                       &TcpTxBufferTestCase::TestNextSeg, this);
  Simulator::Schedule (Seconds (0.0),
                       &TcpTxBufferTestCase::TestUpdateScoreboardWithCraftedSACK, this);

  Simulator::Run ();
  Simulator::Destroy ();
}

void
TcpTxBufferTestCase::TestNextSeg ()
{
  TcpTxBuffer txBuf;
  SequenceNumber32 head (1);
  SequenceNumber32 ret;
  uint32_t dupThresh = 3;
  uint32_t segmentSize = 150;
  Ptr<TcpOptionSack> sack = CreateObject<TcpOptionSack> ();

  // At the beginning the values of dupThresh and segmentSize don't matter
  NS_TEST_ASSERT_MSG_EQ (txBuf.NextSeg (&ret, 0, 0, false), false,
                         "NextSeq should not be returned at the beginning");

  txBuf.SetHeadSequence (head);
  NS_TEST_ASSERT_MSG_EQ (txBuf.NextSeg (&ret, 0, 0, false), false,
                         "NextSeq should not be returned with no data");

  // Add a single, 3000-bytes long, packet
  txBuf.Add (Create<Packet> (30000));
  NS_TEST_ASSERT_MSG_EQ (txBuf.NextSeg (&ret, 0, 0, false), true,
                         "No NextSeq with data at beginning");
  NS_TEST_ASSERT_MSG_EQ (ret.GetValue (), head.GetValue (),
                         "Different NextSeq than expected at the beginning");

  // Simulate sending 100 packets, 150 bytes long each, from seq 1
  for (uint32_t i=0; i<100; ++i)
    {
      NS_TEST_ASSERT_MSG_EQ (txBuf.NextSeg (&ret, dupThresh, segmentSize, false), true,
                             "No NextSeq with data while \"transmitting\"");
      NS_TEST_ASSERT_MSG_EQ (ret, head + (segmentSize * i),
                             "Different NextSeq than expected while \"transmitting\"");
      txBuf.CopyFromSequence (segmentSize, ret);
    }

  // Ok, now simulate we lost the first segment [1;151], and that we have
  // limited transmit. NextSeg should return (up to dupThresh-1) new pieces of data
  SequenceNumber32 lastRet = ret; // This is like m_highTx
  for (uint32_t i=1; i<dupThresh; ++i) // iterate dupThresh-1 times (limited transmit)
    {
      SequenceNumber32 begin = head + (segmentSize * i);
      SequenceNumber32 end = begin + segmentSize;
      sack->AddSackBlock (TcpOptionSack::SackBlock (begin, end));
      txBuf.Update (sack->GetSackList ());

      // new data expected and sent
      NS_TEST_ASSERT_MSG_EQ (txBuf.NextSeg (&ret, dupThresh, segmentSize, false), true,
                             "No NextSeq with SACK block while \"transmitting\"");
      NS_TEST_ASSERT_MSG_EQ (ret, lastRet + segmentSize,
                             "Different NextSeq than expected in limited transmit");
      txBuf.CopyFromSequence (segmentSize, ret);
      sack->ClearSackList ();
      lastRet = ret;
    }

  // Limited transmit was ok; now there is the dupThresh-th dupack.
  // Now we need to retransmit the first block..
  sack->AddSackBlock (TcpOptionSack::SackBlock (head + (segmentSize * (dupThresh)),
                                                head + (segmentSize * (dupThresh)) + segmentSize));
  txBuf.Update (sack->GetSackList ());
  NS_TEST_ASSERT_MSG_EQ (txBuf.NextSeg (&ret, dupThresh, segmentSize, false), true,
                         "No NextSeq with SACK block for Fast Recovery");
  NS_TEST_ASSERT_MSG_EQ (ret, head,
                         "Different NextSeq than expected for Fast Recovery");
  txBuf.CopyFromSequence (segmentSize, ret);
  sack->ClearSackList ();

  // Fast Retransmission was ok; now check some additional dupacks.
  for (uint32_t i=1; i<=4; ++i)
    {
      sack->AddSackBlock (TcpOptionSack::SackBlock (head + (segmentSize * (dupThresh+i)),
                                                    head + (segmentSize * (dupThresh+i)) + segmentSize));
      txBuf.Update (sack->GetSackList ());
      NS_TEST_ASSERT_MSG_EQ (txBuf.NextSeg (&ret, dupThresh, segmentSize, false), true,
                             "No NextSeq with SACK block after recv dupacks in FR");
      NS_TEST_ASSERT_MSG_EQ (ret, lastRet + segmentSize,
                             "Different NextSeq than expected after recv dupacks in FR");
      txBuf.CopyFromSequence (segmentSize, ret);
      sack->ClearSackList ();
      lastRet = ret;
    }

  // Well now we receive a partial ACK, corresponding to the segment we retransmitted.
  // Unfortunately, the next one is lost as well; but NextSeg should be smart enough
  // to give us the next segment (head + segmentSize) to retransmit.
  /* In this particular case, we are checking the fact that we have badly crafted
   * the SACK blocks. Talking in segment, we transmitted 1,2,3,4,5 ... and then
   * received dupack for 1. While receiving these, we crafted SACK block in the
   * way that 2,3,4,... were correctly received. Now, if we receive an ACK for 2,
   * we clearly crafted the corresponding ACK wrongly. TcpTxBuffer should be able
   * to "backoff" that flag on its HEAD (segment 2). We still don't know for segment
   * 3,4 .. so keep them.
   */
  head = head + segmentSize;
  txBuf.DiscardUpTo (head);
  NS_TEST_ASSERT_MSG_EQ (txBuf.NextSeg (&ret, dupThresh, segmentSize, false), true,
                         "No NextSeq with SACK block after receiving partial ACK");
  NS_TEST_ASSERT_MSG_EQ (ret, head,
                         "Different NextSeq than expected after receiving partial ACK");
  txBuf.CopyFromSequence (segmentSize, ret);

  // Now, check for one more dupack...
  sack->AddSackBlock (TcpOptionSack::SackBlock (head + (segmentSize * (dupThresh+6)),
                                                head + (segmentSize * (dupThresh+6)) + segmentSize));
  txBuf.Update (sack->GetSackList ());
  NS_TEST_ASSERT_MSG_EQ (txBuf.NextSeg (&ret, dupThresh, segmentSize, false), true,
                         "No NextSeq with SACK block after recv dupacks after partial ack");
  NS_TEST_ASSERT_MSG_EQ (ret, lastRet + segmentSize,
                         "Different NextSeq than expected after recv dupacks after partial ack");
  txBuf.CopyFromSequence (segmentSize, ret);
  sack->ClearSackList ();
  head = lastRet = ret + segmentSize;

  // And now ack everything we sent to date!
  txBuf.DiscardUpTo (head);

  // And continue normally until the end
  for (uint32_t i=0; i<93; ++i)
    {
      NS_TEST_ASSERT_MSG_EQ (txBuf.NextSeg (&ret, dupThresh, segmentSize, false), true,
                             "No NextSeq with data while \"transmitting\"");
      NS_TEST_ASSERT_MSG_EQ (ret, head + (segmentSize * i),
                             "Different NextSeq than expected while \"transmitting\"");
      txBuf.CopyFromSequence (segmentSize, ret);
    }

  txBuf.DiscardUpTo (ret+segmentSize);
  NS_TEST_ASSERT_MSG_EQ (txBuf.Size (), 0,
                         "Data inside the buffer");
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
TcpTxBufferTestCase::TestUpdateScoreboardWithCraftedSACK ()
{
  TcpTxBuffer txBuf;
  SequenceNumber32 head (1);
  txBuf.SetHeadSequence (head);

  // Add a single, 3000-bytes long, packet
  txBuf.Add (Create<Packet> (30000));

  // Simulate sending 100 packets, 150 bytes long each, from seq 1
  for (uint32_t i=0; i<100; ++i)
    {
      txBuf.CopyFromSequence (150, head + (150 * i));
    }

  // Now we have 100 packets sent, 100 waiting (well, that 100 are condensed in one)

  // Suppose now we receive 99 dupacks, because the first was lost.
  for (uint32_t i=0; i<99; ++i)
    {
      Ptr<const TcpOptionSack> sack = txBuf.CraftSackOption (head, 32); // 3 maximum sack block

      // For iteration 0 and 1 we have 1 and 2 sack blocks, respectively.
      // For all others, maximum 3
      if (i == 0)
        {
          NS_TEST_ASSERT_MSG_EQ (sack->GetNumSackBlocks (), 1,
                                 "Different block number than expected");
        }
      else if (i == 1)
        {
          NS_TEST_ASSERT_MSG_EQ (sack->GetNumSackBlocks (), 2,
                                 "Different block number than expected");
        }
      else if (i >= 2)
        {
          NS_TEST_ASSERT_MSG_EQ (sack->GetNumSackBlocks (), 3,
                                 "More blocks than expected");
        }

      TcpOptionSack::SackList sackList = sack->GetSackList ();
      TcpOptionSack::SackBlock block = sackList.front ();
      sackList.pop_front ();

      // The first block, assuming all the other are SACKed in order (from 2nd
      // onward) has seq = 1 + (150 * (i+1)) --> i+1 because the first sent
      // block cannot be SACKed
      NS_TEST_ASSERT_MSG_EQ (block.first, SequenceNumber32 (1 + (150*(i+1))),
                             "First sack block is wrong (on the left)");
      NS_TEST_ASSERT_MSG_EQ (block.second, block.first + 150,
                             "First sack block is wrong (on the right)");

      SequenceNumber32 left = block.first;
      for (TcpOptionSack::SackList::iterator it = sackList.begin (); it != sackList.end (); ++it)
        {
          block = (*it);

          // the blocks go backwards here. To understand better, an example
          // of SACK option list: [1351;1501], [1201;1351], [1051;1201]
          NS_TEST_ASSERT_MSG_EQ (block.first, left - 150,
                                 "First sack block is wrong (on the left)");
          NS_TEST_ASSERT_MSG_EQ (block.second, left,
                                 "First sack block is wrong (on the right)");
          left -= 150;
        }

      txBuf.Update (sack->GetSackList ());

      if (i == 0)
        {
          NS_TEST_ASSERT_MSG_EQ (false, txBuf.IsLost (SequenceNumber32 (1), 3, 150),
                                 "SequenceNumber 1 isLost, but for RFC 6675 is not");
        }
      else if (i == 1)
        {
          NS_TEST_ASSERT_MSG_EQ (false, txBuf.IsLost (SequenceNumber32 (1), 3, 150),
                                 "SequenceNumber 1 isLost, but for RFC 6675 is not");
          SequenceNumber32 lost (1 + (150 * i));
          NS_TEST_ASSERT_MSG_EQ (false, txBuf.IsLost (lost, 3, 150),
                                 "SequenceNumber " << lost <<
                                 "isLost, but for RFC 6675 is because is SACKed");
        }
      else if (i >= 2)
        {
          NS_TEST_ASSERT_MSG_EQ (true, txBuf.IsLost (SequenceNumber32 (1), 3, 150),
                                 "SequenceNumber 1 ! isLost, but for RFC 6675 is");
          SequenceNumber32 lost (1 + (150 * i));
          NS_TEST_ASSERT_MSG_EQ (false, txBuf.IsLost (lost, 3, 150),
                                 "SequenceNumber " << lost <<
                                 "isLost, but for RFC 6675 is because is SACKed");
        }
    }

  for (uint32_t i=0; i<100; ++i)
    {
      txBuf.DiscardUpTo (SequenceNumber32 (30001));
    }

  NS_TEST_ASSERT_MSG_EQ (txBuf.Size (), 0,
                         "Data inside the buffer");
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
