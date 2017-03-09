/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2017 Universita' degli Studi di Napoli Federico II
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
 * Author: Stefano Avallone <stavallo@unina.it>
 *
 */

#include "ns3/test.h"
#include "ns3/uinteger.h"
#include "ns3/pointer.h"
#include "ns3/string.h"
#include "ns3/double.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/node-container.h"
#include "ns3/traffic-control-layer.h"
#include "ns3/traffic-control-helper.h"
#include "ns3/simple-net-device.h"
#include "ns3/simple-channel.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/net-device-queue-interface.h"
#include "ns3/config.h"

using namespace ns3;

/**
 * \ingroup traffic-control-test
 * \ingroup tests
 *
 * \brief Queue Disc Test Item
 */
class QueueDiscTestItem : public QueueDiscItem {
public:
  /**
   * Constructor
   *
   * \param p the packet stored in this item
   */
  QueueDiscTestItem (Ptr<Packet> p);
  virtual ~QueueDiscTestItem ();
  virtual void AddHeader (void);
  virtual bool Mark(void);

private:
  QueueDiscTestItem ();
  /**
   * \brief Copy constructor
   * Disable default implementation to avoid misuse
   */
  QueueDiscTestItem (const QueueDiscTestItem &);
  /**
   * \brief Assignment operator
   * \return this object
   * Disable default implementation to avoid misuse
   */
  QueueDiscTestItem &operator = (const QueueDiscTestItem &);
};

QueueDiscTestItem::QueueDiscTestItem (Ptr<Packet> p)
  : QueueDiscItem (p, Mac48Address (), 0)
{
}

QueueDiscTestItem::~QueueDiscTestItem ()
{
}

void
QueueDiscTestItem::AddHeader (void)
{
}

bool
QueueDiscTestItem::Mark (void)
{
  return false;
}

/**
 * \ingroup traffic-control-test
 * \ingroup tests
 *
 * \brief Traffic Control Flow Control Test Case
 */
class TcFlowControlTestCase : public TestCase
{
public:
  /// Device queue operating mode
  enum TestType
    {
      PACKET_MODE,
      BYTE_MODE
    };

  /**
   * Constructor
   *
   * \param tt the test type
   */
  TcFlowControlTestCase (TestType tt);
  virtual ~TcFlowControlTestCase ();
private:
  virtual void DoRun (void);
  /**
   * Instruct a node to send a specified number of packets
   * \param n the node
   * \param nPackets the number of packets to send
   */
  void SendPackets (Ptr<Node> n, uint16_t nPackets);
  /**
   * Check if the device queue stores the expected number of packets
   * \param dev the device
   * \param nPackets the expected number of packets stored in the device queue
   * \param msg the message to print if a different number of packets are stored
   */
  void CheckPacketsInDeviceQueue (Ptr<NetDevice> dev, uint16_t nPackets, const char* msg);
  /**
   * Check if the device queue is in the expected status (stopped or not)
   * \param dev the device
   * \param value the expected status of the queue (true means stopped)
   * \param msg the message to print if the status of the device queue is different
   */
  void CheckDeviceQueueStopped (Ptr<NetDevice> dev, bool value, const char* msg);
  /**
   * Check if the queue disc stores the expected number of packets
   * \param dev the device the queue disc is installed on
   * \param nPackets the expected number of packets stored in the queue disc
   * \param msg the message to print if a different number of packets are stored
   */
  void CheckPacketsInQueueDisc (Ptr<NetDevice> dev, uint16_t nPackets, const char* msg);
  TestType m_type;       //!< the test type
};

TcFlowControlTestCase::TcFlowControlTestCase (TestType tt)
  : TestCase ("Test the operation of the flow control mechanism"),
    m_type (tt)
{
}

TcFlowControlTestCase::~TcFlowControlTestCase ()
{
}

void
TcFlowControlTestCase::SendPackets (Ptr<Node> n, uint16_t nPackets)
{
  Ptr<TrafficControlLayer> tc = n->GetObject<TrafficControlLayer> ();
  for (uint16_t i = 0; i < nPackets; i++)
    {
      tc->Send (n->GetDevice (0), Create<QueueDiscTestItem> (Create<Packet> (1000)));
    }
}

void
TcFlowControlTestCase::CheckPacketsInDeviceQueue (Ptr<NetDevice> dev, uint16_t nPackets, const char* msg)
{
  PointerValue ptr;
  dev->GetAttributeFailSafe ("TxQueue", ptr);
  Ptr<Queue<Packet> > queue = ptr.Get<Queue<Packet> > ();
  NS_TEST_EXPECT_MSG_EQ (queue->GetNPackets (), nPackets, msg);
}

void
TcFlowControlTestCase::CheckDeviceQueueStopped (Ptr<NetDevice> dev, bool value, const char* msg)
{
  Ptr<NetDeviceQueueInterface> ndqi = dev->GetObject<NetDeviceQueueInterface> ();
  NS_TEST_EXPECT_MSG_EQ (ndqi->GetTxQueue (0)->IsStopped (), value, msg);
}

void
TcFlowControlTestCase::CheckPacketsInQueueDisc (Ptr<NetDevice> dev, uint16_t nPackets, const char* msg)
{
  Ptr<TrafficControlLayer> tc = dev->GetNode ()->GetObject<TrafficControlLayer> ();
  Ptr<QueueDisc> qdisc = tc->GetRootQueueDiscOnDevice (dev);
  NS_TEST_EXPECT_MSG_EQ (qdisc->GetNPackets (), nPackets, msg);
}


void
TcFlowControlTestCase::DoRun (void)
{
  NodeContainer n;
  n.Create (2);

  n.Get (0)->AggregateObject (CreateObject<TrafficControlLayer> ());
  n.Get (1)->AggregateObject (CreateObject<TrafficControlLayer> ());

  Ptr<Queue<Packet> > queue;

  if (m_type == PACKET_MODE)
    {
      queue = CreateObjectWithAttributes<DropTailQueue<Packet> > ("Mode", EnumValue (QueueBase::QUEUE_MODE_PACKETS),
                                                                  "MaxPackets", UintegerValue (5));
    }
  else
    {
      queue = CreateObjectWithAttributes<DropTailQueue<Packet> > ("Mode", EnumValue (QueueBase::QUEUE_MODE_BYTES),
                                                                  "MaxBytes", UintegerValue (5000));
    }

  // link the two nodes
  Ptr<SimpleNetDevice> txDev, rxDev;
  txDev = CreateObjectWithAttributes<SimpleNetDevice> ("TxQueue", PointerValue (queue),
                                                       "DataRate", DataRateValue (DataRate ("1Mb/s")));
  rxDev = CreateObject<SimpleNetDevice> ();
  n.Get (0)->AddDevice (txDev);
  n.Get (1)->AddDevice (rxDev);
  Ptr<SimpleChannel> channel1 = CreateObject<SimpleChannel> ();
  txDev->SetChannel (channel1);
  rxDev->SetChannel (channel1);

  txDev->SetMtu (2500);

  TrafficControlHelper tch = TrafficControlHelper::Default ();
  tch.Install (txDev);

  // transmit 10 packets at time 0
  Simulator::Schedule (Time (Seconds (0)), &TcFlowControlTestCase::SendPackets,
                      this, n.Get (0), 10);

  if (m_type == PACKET_MODE)
    {
      /*
       * When the device queue is in packet mode, all the packets enqueued in the
       * queue disc are correctly transmitted, even if the device queue is stopped
       * when the last packet is received from the upper layers
       */

      // The transmission of each packet takes 1000B/1Mbps = 8ms
      // After 1ms, we have 5 packets in the device queue (stopped) and 4 in the queue disc
      Simulator::Schedule (Time (MilliSeconds (1)), &TcFlowControlTestCase::CheckPacketsInDeviceQueue,
                          this, txDev, 5, "There must be 5 packets in the device queue after 1ms");
      Simulator::Schedule (Time (MilliSeconds (1)), &TcFlowControlTestCase::CheckDeviceQueueStopped,
                          this, txDev, true, "The device queue must be stopped after 1ms");
      Simulator::Schedule (Time (MilliSeconds (1)), &TcFlowControlTestCase::CheckPacketsInQueueDisc,
                          this, txDev, 4, "There must be 4 packets in the queue disc after 1ms");

      // After 9ms, we have 5 packets in the device queue (stopped) and 3 in the queue disc
      Simulator::Schedule (Time (MilliSeconds (9)), &TcFlowControlTestCase::CheckPacketsInDeviceQueue,
                          this, txDev, 5, "There must be 5 packets in the device queue after 9ms");
      Simulator::Schedule (Time (MilliSeconds (9)), &TcFlowControlTestCase::CheckDeviceQueueStopped,
                          this, txDev, true, "The device queue must be stopped after 9ms");
      Simulator::Schedule (Time (MilliSeconds (9)), &TcFlowControlTestCase::CheckPacketsInQueueDisc,
                          this, txDev, 3, "There must be 3 packets in the queue disc after 9ms");

      // After 17ms, we have 5 packets in the device queue (stopped) and 2 in the queue disc
      Simulator::Schedule (Time (MilliSeconds (17)), &TcFlowControlTestCase::CheckPacketsInDeviceQueue,
                          this, txDev, 5, "There must be 5 packets in the device queue after 17ms");
      Simulator::Schedule (Time (MilliSeconds (17)), &TcFlowControlTestCase::CheckDeviceQueueStopped,
                          this, txDev, true, "The device queue must be stopped after 17ms");
      Simulator::Schedule (Time (MilliSeconds (17)), &TcFlowControlTestCase::CheckPacketsInQueueDisc,
                          this, txDev, 2, "There must be 2 packets in the queue disc after 17ms");

      // After 25ms, we have 5 packets in the device queue (stopped) and 1 in the queue disc
      Simulator::Schedule (Time (MilliSeconds (25)), &TcFlowControlTestCase::CheckPacketsInDeviceQueue,
                          this, txDev, 5, "There must be 5 packets in the device queue after 25ms");
      Simulator::Schedule (Time (MilliSeconds (25)), &TcFlowControlTestCase::CheckDeviceQueueStopped,
                          this, txDev, true, "The device queue must be stopped after 25ms");
      Simulator::Schedule (Time (MilliSeconds (25)), &TcFlowControlTestCase::CheckPacketsInQueueDisc,
                          this, txDev, 1, "There must be 1 packet in the queue disc after 25ms");

      // After 33ms, we have 5 packets in the device queue (stopped) and the queue disc is empty
      Simulator::Schedule (Time (MilliSeconds (33)), &TcFlowControlTestCase::CheckPacketsInDeviceQueue,
                          this, txDev, 5, "There must be 5 packets in the device queue after 33ms");
      Simulator::Schedule (Time (MilliSeconds (33)), &TcFlowControlTestCase::CheckDeviceQueueStopped,
                          this, txDev, true, "The device queue must be stopped after 33ms");
      Simulator::Schedule (Time (MilliSeconds (33)), &TcFlowControlTestCase::CheckPacketsInQueueDisc,
                          this, txDev, 0, "The queue disc must be empty after 33ms");

      // After 41ms, we have 4 packets in the device queue (not stopped) and the queue disc is empty
      Simulator::Schedule (Time (MilliSeconds (41)), &TcFlowControlTestCase::CheckPacketsInDeviceQueue,
                          this, txDev, 4, "There must be 4 packets in the device queue after 41ms");
      Simulator::Schedule (Time (MilliSeconds (41)), &TcFlowControlTestCase::CheckDeviceQueueStopped,
                          this, txDev, false, "The device queue must not be stopped after 41ms");
      Simulator::Schedule (Time (MilliSeconds (41)), &TcFlowControlTestCase::CheckPacketsInQueueDisc,
                          this, txDev, 0, "The queue disc must be empty after 41ms");

      // After 81ms, all packets must have been transmitted (the device queue and the queue disc are empty)
      Simulator::Schedule (Time (MilliSeconds (81)), &TcFlowControlTestCase::CheckPacketsInDeviceQueue,
                          this, txDev, 0, "The device queue must be empty after 81ms");
      Simulator::Schedule (Time (MilliSeconds (81)), &TcFlowControlTestCase::CheckDeviceQueueStopped,
                          this, txDev, false, "The device queue must not be stopped after 81ms");
      Simulator::Schedule (Time (MilliSeconds (81)), &TcFlowControlTestCase::CheckPacketsInQueueDisc,
                          this, txDev, 0, "The queue disc must be empty after 81ms");
    }
  else
    {
      /*
       * When the device queue is in byte mode, all the packets enqueued in the
       * queue disc are correctly transmitted, even if the device queue is stopped
       * when the last packet is received from the upper layers
       */

      // The transmission of each packet takes 1000B/1Mbps = 8ms
      // After 1ms, we have 3 packets in the device queue (stopped) and 6 in the queue disc
      Simulator::Schedule (Time (MilliSeconds (1)), &TcFlowControlTestCase::CheckPacketsInDeviceQueue,
                          this, txDev, 3, "There must be 3 packets in the device queue after 1ms");
      Simulator::Schedule (Time (MilliSeconds (1)), &TcFlowControlTestCase::CheckDeviceQueueStopped,
                          this, txDev, true, "The device queue must be stopped after 1ms");
      Simulator::Schedule (Time (MilliSeconds (1)), &TcFlowControlTestCase::CheckPacketsInQueueDisc,
                          this, txDev, 6, "There must be 6 packets in the queue disc after 1ms");

      // After 9ms, we have 3 packets in the device queue (stopped) and 5 in the queue disc
      Simulator::Schedule (Time (MilliSeconds (9)), &TcFlowControlTestCase::CheckPacketsInDeviceQueue,
                          this, txDev, 3, "There must be 3 packets in the device queue after 9ms");
      Simulator::Schedule (Time (MilliSeconds (9)), &TcFlowControlTestCase::CheckDeviceQueueStopped,
                          this, txDev, true, "The device queue must be stopped after 9ms");
      Simulator::Schedule (Time (MilliSeconds (9)), &TcFlowControlTestCase::CheckPacketsInQueueDisc,
                          this, txDev, 5, "There must be 5 packets in the queue disc after 9ms");

      // After 17ms, we have 3 packets in the device queue (stopped) and 4 in the queue disc
      Simulator::Schedule (Time (MilliSeconds (17)), &TcFlowControlTestCase::CheckPacketsInDeviceQueue,
                          this, txDev, 3, "There must be 3 packets in the device queue after 17ms");
      Simulator::Schedule (Time (MilliSeconds (17)), &TcFlowControlTestCase::CheckDeviceQueueStopped,
                          this, txDev, true, "The device queue must be stopped after 17ms");
      Simulator::Schedule (Time (MilliSeconds (17)), &TcFlowControlTestCase::CheckPacketsInQueueDisc,
                          this, txDev, 4, "There must be 4 packets in the queue disc after 17ms");

      // After 25ms, we have 3 packets in the device queue (stopped) and 3 in the queue disc
      Simulator::Schedule (Time (MilliSeconds (25)), &TcFlowControlTestCase::CheckPacketsInDeviceQueue,
                          this, txDev, 3, "There must be 3 packets in the device queue after 25ms");
      Simulator::Schedule (Time (MilliSeconds (25)), &TcFlowControlTestCase::CheckDeviceQueueStopped,
                          this, txDev, true, "The device queue must be stopped after 25ms");
      Simulator::Schedule (Time (MilliSeconds (25)), &TcFlowControlTestCase::CheckPacketsInQueueDisc,
                          this, txDev, 3, "There must be 3 packets in the queue disc after 25ms");

      // After 33ms, we have 3 packets in the device queue (stopped) and 2 in the queue disc
      Simulator::Schedule (Time (MilliSeconds (33)), &TcFlowControlTestCase::CheckPacketsInDeviceQueue,
                          this, txDev, 3, "There must be 3 packets in the device queue after 33ms");
      Simulator::Schedule (Time (MilliSeconds (33)), &TcFlowControlTestCase::CheckDeviceQueueStopped,
                          this, txDev, true, "The device queue must be stopped after 33ms");
      Simulator::Schedule (Time (MilliSeconds (33)), &TcFlowControlTestCase::CheckPacketsInQueueDisc,
                          this, txDev, 2, "There must be 2 packets in the queue disc after 33ms");

      // After 41ms, we have 3 packets in the device queue (stopped) and 1 in the queue disc
      Simulator::Schedule (Time (MilliSeconds (41)), &TcFlowControlTestCase::CheckPacketsInDeviceQueue,
                          this, txDev, 3, "There must be 3 packets in the device queue after 41ms");
      Simulator::Schedule (Time (MilliSeconds (41)), &TcFlowControlTestCase::CheckDeviceQueueStopped,
                          this, txDev, true, "The device queue must be stopped after 41ms");
      Simulator::Schedule (Time (MilliSeconds (41)), &TcFlowControlTestCase::CheckPacketsInQueueDisc,
                          this, txDev, 1, "There must be 1 packet in the queue disc after 41ms");

      // After 49ms, we have 3 packets in the device queue (stopped) and the queue disc is empty
      Simulator::Schedule (Time (MilliSeconds (49)), &TcFlowControlTestCase::CheckPacketsInDeviceQueue,
                          this, txDev, 3, "There must be 3 packets in the device queue after 49ms");
      Simulator::Schedule (Time (MilliSeconds (49)), &TcFlowControlTestCase::CheckDeviceQueueStopped,
                          this, txDev, true, "The device queue must be stopped after 49ms");
      Simulator::Schedule (Time (MilliSeconds (49)), &TcFlowControlTestCase::CheckPacketsInQueueDisc,
                          this, txDev, 0, "The queue disc must be empty after 49ms");

      // After 57ms, we have 2 packets in the device queue (not stopped) and the queue disc is empty
      Simulator::Schedule (Time (MilliSeconds (57)), &TcFlowControlTestCase::CheckPacketsInDeviceQueue,
                          this, txDev, 2, "There must be 2 packets in the device queue after 57ms");
      Simulator::Schedule (Time (MilliSeconds (57)), &TcFlowControlTestCase::CheckDeviceQueueStopped,
                          this, txDev, false, "The device queue must not be stopped after 57ms");
      Simulator::Schedule (Time (MilliSeconds (57)), &TcFlowControlTestCase::CheckPacketsInQueueDisc,
                          this, txDev, 0, "The queue disc must be empty after 57ms");

      // After 81ms, all packets must have been transmitted (the device queue and the queue disc are empty)
      Simulator::Schedule (Time (MilliSeconds (81)), &TcFlowControlTestCase::CheckPacketsInDeviceQueue,
                          this, txDev, 0, "The device queue must be empty after 81ms");
      Simulator::Schedule (Time (MilliSeconds (81)), &TcFlowControlTestCase::CheckDeviceQueueStopped,
                          this, txDev, false, "The device queue must not be stopped after 81ms");
      Simulator::Schedule (Time (MilliSeconds (81)), &TcFlowControlTestCase::CheckPacketsInQueueDisc,
                          this, txDev, 0, "The queue disc must be empty after 81ms");
    }

  Simulator::Run ();
  Simulator::Destroy ();
}

/**
 * \ingroup traffic-control-test
 * \ingroup tests
 *
 * \brief Traffic Control Flow Control Test Suite
 */
static class TcFlowControlTestSuite : public TestSuite
{
public:
  TcFlowControlTestSuite ()
    : TestSuite ("tc-flow-control", UNIT)
  {
    AddTestCase (new TcFlowControlTestCase (TcFlowControlTestCase::PACKET_MODE), TestCase::QUICK);
    AddTestCase (new TcFlowControlTestCase (TcFlowControlTestCase::BYTE_MODE), TestCase::QUICK);
  }
} g_tcFlowControlTestSuite; ///< the test suite
