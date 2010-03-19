/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *  Copyright (c) 2009-2010 TELEMATICS LAB - Poliotecnico di Bari
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
 *         Giuseppe Piro <g.piro@poliba.it>
 *                       <peppe.piro@gmail.com>
 */
#include "ns3/log.h"
#include "ns3/abort.h"
#include "ns3/test.h"
#include "ns3/uinteger.h"
#include "ns3/inet-socket-address.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-header.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/udp-client-server-helper.h"
#include "ns3/udp-header.h"
#include "ns3/simulator.h"
#include "ns3/wimax-helper.h"
#include "ns3/mobility-helper.h"
#include <iostream>
#include "ns3/global-route-manager.h"
#include "wimax-tlv.h"
#include "ipcs-classifier-record.h"
#include "service-flow.h"
#include "wimax-connection.h"

using namespace ns3;

/*
 * Test the wimax packet fragmentation.
 */
class Ns3WimaxFragmentationTestCase : public TestCase
{
public:
  Ns3WimaxFragmentationTestCase ();
  virtual ~Ns3WimaxFragmentationTestCase ();

private:
  virtual bool DoRun (void);

};

Ns3WimaxFragmentationTestCase::Ns3WimaxFragmentationTestCase ()
  : TestCase ("Test the packet fra/defragmentation.")
{
}

Ns3WimaxFragmentationTestCase::~Ns3WimaxFragmentationTestCase ()
{
}

bool
Ns3WimaxFragmentationTestCase::DoRun (void)
{
  GenericMacHeader gnrcMacHdr;
  ManagementMessageType msgType;
  FragmentationSubheader fragSubhdr;
  GenericMacHeader hdr;

  Cid cid;
  WimaxConnection *connectionTx = new WimaxConnection (cid, Cid::TRANSPORT);
  WimaxConnection *connectionRx = new WimaxConnection (cid, Cid::TRANSPORT);
  bool testResult = false;

  // A Packet of 1000 bytes has been created.
  // It will be fragmentated into 4 fragments and then defragmentated into fullPacket.
  Ptr<Packet> packet = Create<Packet> (1000);
  Ptr<Packet> fragment;
  Ptr<Packet> fullPacket = Create<Packet> ();

  // Enqueued packet
  hdr.SetLen (packet->GetSize () + hdr.GetSerializedSize ());
  hdr.SetCid (connectionTx->GetCid ());
  MacHeaderType::HeaderType packetType = MacHeaderType::HEADER_TYPE_GENERIC;

  connectionTx->Enqueue (packet, packetType, hdr);

  uint32_t availableByteForFragment = 280;
  for (int i = 0; i < 4; i++)
    {
      // dequeue a fragment
      if (connectionTx->GetQueue ()->GetFirstPacketRequiredByte (packetType) > availableByteForFragment)
        {
          fragment = connectionTx->Dequeue (packetType, availableByteForFragment);
        }
      else
        {
          fragment = connectionTx->Dequeue (packetType);
        }
// *** send packet -----> receive packet ----**

      // check if receive packet is a fragment
      fragment->RemoveHeader (gnrcMacHdr);
      uint8_t type = gnrcMacHdr.GetType ();
      if (type)
        {
          // Check if there is a fragmentation Subheader
          uint8_t tmpType = type;
          if (((tmpType >> 2) & 1) != 1)
            {
              // The packet is not a fragment
              testResult = true;
              break;
            }
        }

      // remove header from the received fragment
      fragment->RemoveHeader (fragSubhdr);
      uint32_t fc = fragSubhdr.GetFc ();


      if (fc == 1 && i != 0)
        {
          // the fragment in not the first one
          testResult = true;
          break;
        }
      if (fc == 2 && i != 3)
        {
          // the fragment in not the latest one
          testResult = true;
          break;
        }
      if ((fc == 3 && i != 1) && (fc == 3 && i != 2))
        {
          // the fragment in not the middle one
          testResult = true;
          break;
        }


      if (fc != 2)
        {
          // This is the first or middle fragment.
          // Take the fragment queue, store the fragment into the queue
          connectionRx->FragmentEnqueue (fragment);
        }
      else
        {
          // This is the latest fragment.
          // Take the fragment queue, defragment a packet and send it to the upper layer
          connectionRx->FragmentEnqueue (fragment);
          WimaxConnection::FragmentsQueue fragmentsQueue = connectionRx->GetFragmentsQueue ();

          // DEFRAGMENTATION
          for (std::list<Ptr<const Packet> >::const_iterator iter = fragmentsQueue.begin ();
               iter != fragmentsQueue.end (); ++iter)
            {
              // Create the whole Packet
              fullPacket->AddAtEnd (*iter);
            }
          connectionRx->ClearFragmentsQueue ();

          if (fullPacket->GetSize () != 1000)
            {
              // The defragmentation is correct.
              testResult = true; // Test is passed
              break;
            }
        }
    }
  delete connectionTx;
  delete connectionRx;
  Simulator::Destroy ();
  return (testResult);
}
// ==============================================================================

class Ns3WimaxFragmentationTestSuite : public TestSuite
{
public:
  Ns3WimaxFragmentationTestSuite ();
};

Ns3WimaxFragmentationTestSuite::Ns3WimaxFragmentationTestSuite ()
  : TestSuite ("wimax-fragmentation", UNIT)
{
  AddTestCase (new Ns3WimaxFragmentationTestCase);
}

Ns3WimaxFragmentationTestSuite ns3WimaxFragmentationTestSuite;
