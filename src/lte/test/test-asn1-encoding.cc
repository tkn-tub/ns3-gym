/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011, 2012 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Lluis Parcerisa <lparcerisa@cttc.cat>
 */

#include "ns3/log.h"
#include "ns3/string.h"
#include "ns3/double.h"
#include "ns3/enum.h"
#include "ns3/boolean.h"
#include "ns3/test.h"
#include "ns3/ptr.h"
#include "ns3/packet.h"

#include "ns3/lte-rrc-header.h"
#include "ns3/lte-rrc-sap.h"

NS_LOG_COMPONENT_DEFINE ("Asn1EncodingTest");

namespace ns3 {

class TestUtils
{
public:
  // Function to output packet contents in hex format
  static void printPacketContentsHex (Packet *pkt)
  {
    uint32_t psize = pkt->GetSize ();
    uint8_t buffer[psize];
    pkt->CopyData (buffer, psize); 
    for (uint32_t i = 0; i < psize; i++)
      {
        printf ("%02x ",buffer[i]);
      }
    printf ("\n\n");
  }
  // Function to output packet contents in binary format
  static void printPacketContentsBin (Packet *pkt)
  {
    uint32_t psize = pkt->GetSize ();
    uint8_t buffer[psize];
    pkt->CopyData (buffer, psize); 
    for (uint32_t i = 0; i < psize; i++)
      {
        std::cout << std::bitset<8> (buffer[i]) << " ";
      }
    std::cout << std::endl << std::endl;
  }
};

// --------------------------- CLASS RrcHeaderTestCase -----------------------------
/**
 * This class provides common functions to be inherited
 * by the children TestCases
 */
class RrcHeaderTestCase : public TestCase
{
public:
  RrcHeaderTestCase (std::string s);
  virtual void DoRun (void) = 0;
  LteRrcSap::RadioResourceConfigDedicated CreateRadioResourceConfigDedicated ();
  void AssertEqualRadioResourceConfigDedicated (LteRrcSap::RadioResourceConfigDedicated rrcd1, LteRrcSap::RadioResourceConfigDedicated rrcd2);
};

RrcHeaderTestCase :: RrcHeaderTestCase (std::string s) : TestCase (s)
{
}

LteRrcSap::RadioResourceConfigDedicated
RrcHeaderTestCase :: CreateRadioResourceConfigDedicated ()
{
  LteRrcSap::RadioResourceConfigDedicated rrd;

  rrd.drbToReleaseList = std::list<uint8_t> (4,2);

  LteRrcSap::SrbToAddMod srbToAddMod;
  srbToAddMod.srbIdentity = 2;

  LteRrcSap::LogicalChannelConfig logicalChannelConfig;
  logicalChannelConfig.priority = 9;
  logicalChannelConfig.prioritizedBitRateKbps = 128;
  logicalChannelConfig.bucketSizeDurationMs = 100;
  logicalChannelConfig.logicalChannelGroup = 3;
  srbToAddMod.logicalChannelConfig = logicalChannelConfig;

  rrd.srbToAddModList.insert (rrd.srbToAddModList.begin (),srbToAddMod);

  LteRrcSap::DrbToAddMod drbToAddMod;
  drbToAddMod.epsBearerIdentity = 1;
  drbToAddMod.drbIdentity = 1;
  drbToAddMod.logicalChannelIdentity = 5;
  LteRrcSap::RlcConfig rlcConfig;
  rlcConfig.choice = LteRrcSap::RlcConfig::UM_BI_DIRECTIONAL;
  drbToAddMod.rlcConfig = rlcConfig;

  LteRrcSap::LogicalChannelConfig logicalChannelConfig2;
  logicalChannelConfig2.priority = 7;
  logicalChannelConfig2.prioritizedBitRateKbps = 256;
  logicalChannelConfig2.bucketSizeDurationMs = 50;
  logicalChannelConfig2.logicalChannelGroup = 2;
  drbToAddMod.logicalChannelConfig = logicalChannelConfig2;

  rrd.drbToAddModList.insert (rrd.drbToAddModList.begin (),drbToAddMod);

  rrd.havePhysicalConfigDedicated = true;
  LteRrcSap::PhysicalConfigDedicated physicalConfigDedicated;
  physicalConfigDedicated.haveSoundingRsUlConfigDedicated = true;
  physicalConfigDedicated.soundingRsUlConfigDedicated.type = LteRrcSap::SoundingRsUlConfigDedicated::SETUP;
  physicalConfigDedicated.soundingRsUlConfigDedicated.srsBandwidth = 2;
  physicalConfigDedicated.soundingRsUlConfigDedicated.srsConfigIndex = 12;

  physicalConfigDedicated.haveAntennaInfoDedicated = true;
  physicalConfigDedicated.antennaInfo.transmissionMode = 3;

  rrd.physicalConfigDedicated = physicalConfigDedicated;

  return rrd;
}

void
RrcHeaderTestCase :: AssertEqualRadioResourceConfigDedicated (LteRrcSap::RadioResourceConfigDedicated rrcd1, LteRrcSap::RadioResourceConfigDedicated rrcd2)
{
  NS_TEST_ASSERT_MSG_EQ (rrcd1.srbToAddModList.size (), rrcd2.srbToAddModList.size (),"SrbToAddModList different sizes");
 
  std::list<LteRrcSap::SrbToAddMod> srcSrbToAddModList = rrcd1.srbToAddModList;
  std::list<LteRrcSap::SrbToAddMod>::iterator it1 = srcSrbToAddModList.begin ();
  std::list<LteRrcSap::SrbToAddMod> dstSrbToAddModList = rrcd2.srbToAddModList;
  std::list<LteRrcSap::SrbToAddMod>::iterator it2 = dstSrbToAddModList.begin ();

  for (; it1 != srcSrbToAddModList.end (); it1++, it2++)
    {
      NS_TEST_ASSERT_MSG_EQ (it1->srbIdentity,it2->srbIdentity, "srbIdentity");
      NS_TEST_ASSERT_MSG_EQ (it1->logicalChannelConfig.priority,it2->logicalChannelConfig.priority, "logicalChannelConfig.priority");
      NS_TEST_ASSERT_MSG_EQ (it1->logicalChannelConfig.prioritizedBitRateKbps,it2->logicalChannelConfig.prioritizedBitRateKbps, "logicalChannelConfig.prioritizedBitRateKbps");
      NS_TEST_ASSERT_MSG_EQ (it1->logicalChannelConfig.bucketSizeDurationMs,it2->logicalChannelConfig.bucketSizeDurationMs, "logicalChannelConfig.bucketSizeDurationMs");
      NS_TEST_ASSERT_MSG_EQ (it1->logicalChannelConfig.logicalChannelGroup,it2->logicalChannelConfig.logicalChannelGroup, "logicalChannelConfig.logicalChannelGroup");
    }

  NS_TEST_ASSERT_MSG_EQ (rrcd1.drbToAddModList.size (), rrcd2.drbToAddModList.size (),"DrbToAddModList different sizes");

  std::list<LteRrcSap::DrbToAddMod> srcDrbToAddModList = rrcd1.drbToAddModList;
  std::list<LteRrcSap::DrbToAddMod>::iterator it3 = srcDrbToAddModList.begin ();
  std::list<LteRrcSap::DrbToAddMod> dstDrbToAddModList = rrcd2.drbToAddModList;
  std::list<LteRrcSap::DrbToAddMod>::iterator it4 = dstDrbToAddModList.begin ();

  for (; it3 != srcDrbToAddModList.end (); it3++, it4++)
    {
      NS_TEST_ASSERT_MSG_EQ (it3->epsBearerIdentity,it4->epsBearerIdentity, "epsBearerIdentity");
      NS_TEST_ASSERT_MSG_EQ (it3->drbIdentity,it4->drbIdentity, "drbIdentity");
      NS_TEST_ASSERT_MSG_EQ (it3->rlcConfig.choice,it4->rlcConfig.choice, "rlcConfig.choice");
      NS_TEST_ASSERT_MSG_EQ (it3->logicalChannelIdentity,it4->logicalChannelIdentity, "logicalChannelIdentity");
      NS_TEST_ASSERT_MSG_EQ (it3->epsBearerIdentity,it4->epsBearerIdentity, "epsBearerIdentity");

      NS_TEST_ASSERT_MSG_EQ (it3->logicalChannelConfig.priority,it4->logicalChannelConfig.priority, "logicalChannelConfig.priority");
      NS_TEST_ASSERT_MSG_EQ (it3->logicalChannelConfig.prioritizedBitRateKbps,it4->logicalChannelConfig.prioritizedBitRateKbps, "logicalChannelConfig.prioritizedBitRateKbps");
      NS_TEST_ASSERT_MSG_EQ (it3->logicalChannelConfig.bucketSizeDurationMs,it4->logicalChannelConfig.bucketSizeDurationMs, "logicalChannelConfig.bucketSizeDurationMs");
      NS_TEST_ASSERT_MSG_EQ (it3->logicalChannelConfig.logicalChannelGroup,it4->logicalChannelConfig.logicalChannelGroup, "logicalChannelConfig.logicalChannelGroup");
    }

  NS_TEST_ASSERT_MSG_EQ (rrcd1.drbToReleaseList.size (), rrcd2.drbToReleaseList.size (),"DrbToReleaseList different sizes");

  std::list<uint8_t> srcDrbToReleaseList = rrcd1.drbToReleaseList;
  std::list<uint8_t> dstDrbToReleaseList = rrcd2.drbToReleaseList;
  std::list<uint8_t>::iterator it5 = srcDrbToReleaseList.begin ();
  std::list<uint8_t>::iterator it6 = dstDrbToReleaseList.begin ();

  for (; it5 != srcDrbToReleaseList.end (); it5++, it6++)
    {
      NS_TEST_ASSERT_MSG_EQ (*it5, *it6,"element != in DrbToReleaseList");
    }

  NS_TEST_ASSERT_MSG_EQ (rrcd1.havePhysicalConfigDedicated,rrcd2.havePhysicalConfigDedicated, "HavePhysicalConfigDedicated");

  if (rrcd1.havePhysicalConfigDedicated)
    {
      NS_TEST_ASSERT_MSG_EQ (rrcd1.physicalConfigDedicated.haveSoundingRsUlConfigDedicated,
                             rrcd2.physicalConfigDedicated.haveSoundingRsUlConfigDedicated,
                             "haveSoundingRsUlConfigDedicated");

      NS_TEST_ASSERT_MSG_EQ (rrcd1.physicalConfigDedicated.soundingRsUlConfigDedicated.type,
                             rrcd2.physicalConfigDedicated.soundingRsUlConfigDedicated.type,
                             "soundingRsUlConfigDedicated.type");
      NS_TEST_ASSERT_MSG_EQ (rrcd1.physicalConfigDedicated.soundingRsUlConfigDedicated.srsBandwidth,
                             rrcd2.physicalConfigDedicated.soundingRsUlConfigDedicated.srsBandwidth,
                             "soundingRsUlConfigDedicated.srsBandwidth");

      NS_TEST_ASSERT_MSG_EQ (rrcd1.physicalConfigDedicated.soundingRsUlConfigDedicated.srsConfigIndex,
                             rrcd2.physicalConfigDedicated.soundingRsUlConfigDedicated.srsConfigIndex,
                             "soundingRsUlConfigDedicated.srsConfigIndex");

      NS_TEST_ASSERT_MSG_EQ (rrcd1.physicalConfigDedicated.haveAntennaInfoDedicated,
                             rrcd2.physicalConfigDedicated.haveAntennaInfoDedicated,
                             "haveAntennaInfoDedicated");

      if (rrcd1.physicalConfigDedicated.haveAntennaInfoDedicated)
        {
          NS_TEST_ASSERT_MSG_EQ (rrcd1.physicalConfigDedicated.antennaInfo.transmissionMode,
                                 rrcd2.physicalConfigDedicated.antennaInfo.transmissionMode,
                                 "antennaInfo.transmissionMode");
        }
    }
}

// --------------------------- CLASS RrcConnectionRequestTestCase -----------------------------
class RrcConnectionRequestTestCase : public RrcHeaderTestCase
{
public:
  RrcConnectionRequestTestCase ();
  virtual void DoRun (void);
};

RrcConnectionRequestTestCase::RrcConnectionRequestTestCase () : RrcHeaderTestCase ("Testing RrcConnectionRequest")
{
}

void
RrcConnectionRequestTestCase::DoRun (void)
{
  std::cout << "============= RrcConnectionRequestTestCase ===========" << std::endl;
  // add header
  Ptr<Packet> packet = Create<Packet> ();
  packet->Print (std::cout);
  std::cout << std::endl;

  LteRrcSap::RrcConnectionRequest msg;
  msg.ueIdentity = 0x83fecafeca;

  RrcConnectionRequestHeader source;
  source.SetMessage (msg);

  std::cout << "--------- SOURCE INFO: -------" << std::endl;
  source.Print (std::cout);
  packet->AddHeader (source);
  std::cout << std::endl;

  // print serialized packet contents
  std::cout << "---- SERIALIZED PACKET CONTENTS (HEX): -------" << std::endl;
  std::cout << "Hex: ";
  TestUtils::printPacketContentsHex (GetPointer (packet));
  std::cout << "Bin: ";
  TestUtils::printPacketContentsBin (GetPointer (packet));

  // remove header
  RrcConnectionRequestHeader destination;
  packet->RemoveHeader (destination);
  std::cout << "--------- DESTINATION INFO: -------" << std::endl;
  destination.Print (std::cout);

  // Check that the destination and source headers contain the same values
  NS_TEST_ASSERT_MSG_EQ (source.getMmec (),destination.getMmec (), "Different m_mmec!");
  NS_TEST_ASSERT_MSG_EQ (source.getMtmsi (),destination.getMtmsi (), "Different m_mTmsi!");
}

// --------------------------- CLASS RrcConnectionSetupTestCase -----------------------------
class RrcConnectionSetupTestCase : public RrcHeaderTestCase
{
public:
  RrcConnectionSetupTestCase ();
  virtual void DoRun (void);
};

RrcConnectionSetupTestCase::RrcConnectionSetupTestCase () : RrcHeaderTestCase ("Testing RrcConnectionSetupTestCase")
{
}

void
RrcConnectionSetupTestCase::DoRun (void)
{
  std::cout << "============= RrcConnectionSetupTestCase ===========" << std::endl;
  // add header
  Ptr<Packet> packet = Create<Packet> ();
  packet->Print (std::cout);
  std::cout << std::endl;

  LteRrcSap::RrcConnectionSetup msg;
  msg.rrcTransactionIdentifier = 3;
  msg.radioResourceConfigDedicated = CreateRadioResourceConfigDedicated ();

  RrcConnectionSetupHeader source;
  source.SetMessage (msg);

  std::cout << "--------- SOURCE INFO: -------" << std::endl;
  source.Print (std::cout);
  packet->AddHeader (source);
  std::cout << std::endl;

  // print serialized packet contents
  std::cout << "---- SERIALIZED PACKET CONTENTS: -------" << std::endl;
  std::cout << "Hex: ";
  TestUtils::printPacketContentsHex (GetPointer (packet));
  std::cout << "Bin: ";
  TestUtils::printPacketContentsBin (GetPointer (packet));

  // remove header
  RrcConnectionSetupHeader destination;
  packet->RemoveHeader (destination);
  std::cout << "--------- DESTINATION INFO: -------" << std::endl;
  destination.Print (std::cout);

  // Check that the destination and source headers contain the same values
  NS_TEST_ASSERT_MSG_EQ (source.GetRrcTransactionIdentifier (),destination.GetRrcTransactionIdentifier (), "RrcTransactionIdentifier");

  AssertEqualRadioResourceConfigDedicated (source.GetRadioResourceConfigDedicated (),destination.GetRadioResourceConfigDedicated ());
}

// --------------------------- CLASS RrcConnectionSetupCompleteTestCase -----------------------------
class RrcConnectionSetupCompleteTestCase : public RrcHeaderTestCase
{
public:
  RrcConnectionSetupCompleteTestCase ();
  virtual void DoRun (void);
};

RrcConnectionSetupCompleteTestCase::RrcConnectionSetupCompleteTestCase () : RrcHeaderTestCase ("Testing RrcConnectionSetupCompleteTestCase")
{
}

void
RrcConnectionSetupCompleteTestCase::DoRun (void)
{
  std::cout << "============= RrcConnectionSetupCompleteTestCase ===========" << std::endl;
  // add header
  Ptr<Packet> packet = Create<Packet> ();
  packet->Print (std::cout);
  std::cout << std::endl;

  LteRrcSap::RrcConnectionSetupCompleted msg;
  msg.rrcTransactionIdentifier = 3;

  RrcConnectionSetupCompleteHeader source;
  source.SetMessage (msg);

  std::cout << "--------- SOURCE INFO: -------" << std::endl;
  source.Print (std::cout);
  packet->AddHeader (source);
  std::cout << std::endl;

  // print serialized packet contents
  std::cout << "---- SERIALIZED PACKET CONTENTS: -------" << std::endl;
  std::cout << "Hex: ";
  TestUtils::printPacketContentsHex (GetPointer (packet));
  std::cout << "Bin: ";
  TestUtils::printPacketContentsBin (GetPointer (packet));

  // remove header
  RrcConnectionSetupCompleteHeader destination;
  packet->RemoveHeader (destination);
  std::cout << "--------- DESTINATION INFO: -------" << std::endl;
  destination.Print (std::cout);

  // Check that the destination and source headers contain the same values
  NS_TEST_ASSERT_MSG_EQ (source.GetRrcTransactionIdentifier (),destination.GetRrcTransactionIdentifier (), "RrcTransactionIdentifier");
}

// --------------------------- CLASS RrcConnectionReconfigurationCompleteTestCase -----------------------------
class RrcConnectionReconfigurationCompleteTestCase : public RrcHeaderTestCase
{
public:
  RrcConnectionReconfigurationCompleteTestCase ();
  virtual void DoRun (void);
};

RrcConnectionReconfigurationCompleteTestCase::RrcConnectionReconfigurationCompleteTestCase ()
  : RrcHeaderTestCase ("Testing RrcConnectionReconfigurationCompleteTestCase")
{
}

void
RrcConnectionReconfigurationCompleteTestCase::DoRun (void)
{
  std::cout << "============= RrcConnectionReconfigurationCompleteTestCase ===========" << std::endl;
  // add header
  Ptr<Packet> packet = Create<Packet> ();
  packet->Print (std::cout);
  std::cout << std::endl;

  LteRrcSap::RrcConnectionReconfigurationCompleted msg;
  msg.rrcTransactionIdentifier = 2;

  RrcConnectionReconfigurationCompleteHeader source;
  source.SetMessage (msg);

  std::cout << "--------- SOURCE INFO: -------" << std::endl;
  source.Print (std::cout);
  packet->AddHeader (source);
  std::cout << std::endl;

  // print serialized packet contents
  std::cout << "---- SERIALIZED PACKET CONTENTS: -------" << std::endl;
  std::cout << "Hex: ";
  TestUtils::printPacketContentsHex (GetPointer (packet));
  std::cout << "Bin: ";
  TestUtils::printPacketContentsBin (GetPointer (packet));

  // remove header
  RrcConnectionReconfigurationCompleteHeader destination;
  packet->RemoveHeader (destination);
  std::cout << "--------- DESTINATION INFO: -------" << std::endl;
  destination.Print (std::cout);

  // Check that the destination and source headers contain the same values
  NS_TEST_ASSERT_MSG_EQ (source.GetRrcTransactionIdentifier (),destination.GetRrcTransactionIdentifier (), "RrcTransactionIdentifier");
}

// --------------------------- CLASS RrcConnectionReconfigurationTestCase -----------------------------
class RrcConnectionReconfigurationTestCase : public RrcHeaderTestCase
{
public:
  RrcConnectionReconfigurationTestCase ();
  virtual void DoRun (void);
};

RrcConnectionReconfigurationTestCase::RrcConnectionReconfigurationTestCase ()
  : RrcHeaderTestCase ("Testing RrcConnectionReconfigurationTestCase")
{
}

void
RrcConnectionReconfigurationTestCase::DoRun (void)
{
  std::cout << "============= RrcConnectionReconfigurationTestCase ===========" << std::endl;
  // add header
  Ptr<Packet> packet = Create<Packet> ();
  packet->Print (std::cout);
  std::cout << std::endl;

  LteRrcSap::RrcConnectionReconfiguration msg;
  msg.rrcTransactionIdentifier = 2;

  msg.haveMeasConfig = false;
  msg.haveMobilityControlInfo = true;
  msg.mobilityControlInfo.targetPhysCellId = 4;
  msg.mobilityControlInfo.haveCarrierFreq = true;
  msg.mobilityControlInfo.carrierFreq.dlCarrierFreq = 3;
  msg.mobilityControlInfo.carrierFreq.ulCarrierFreq = 5;
  msg.mobilityControlInfo.haveCarrierBandwidth = true;
  msg.mobilityControlInfo.carrierBandwidth.dlBandwidth = 5;
  msg.mobilityControlInfo.carrierBandwidth.ulBandwidth = 3;
  msg.mobilityControlInfo.newUeIdentity = 11;
  msg.mobilityControlInfo.haveRachConfigDedicated = true;
  msg.mobilityControlInfo.rachConfigDedicated.raPreambleIndex = 2; 
  msg.mobilityControlInfo.rachConfigDedicated.raPrachMaskIndex = 2;

  msg.haveRadioResourceConfigDedicated = true;

  msg.radioResourceConfigDedicated = CreateRadioResourceConfigDedicated ();

  RrcConnectionReconfigurationHeader source;
  source.SetMessage (msg);

  std::cout << "--------- SOURCE INFO: -------" << std::endl;
  source.Print (std::cout);
  packet->AddHeader (source);
  std::cout << std::endl;

  // print serialized packet contents
  std::cout << "---- SERIALIZED PACKET CONTENTS: -------" << std::endl;
  std::cout << "Hex: ";
  TestUtils::printPacketContentsHex (GetPointer (packet));
  std::cout << "Bin: ";
  TestUtils::printPacketContentsBin (GetPointer (packet));

  // remove header
  RrcConnectionReconfigurationHeader destination;
  packet->RemoveHeader (destination);
  std::cout << "--------- DESTINATION INFO: -------" << std::endl;
  destination.Print (std::cout);

  // Check that the destination and source headers contain the same values
  NS_TEST_ASSERT_MSG_EQ (source.GetRrcTransactionIdentifier (),destination.GetRrcTransactionIdentifier (), "RrcTransactionIdentifier");
  NS_TEST_ASSERT_MSG_EQ (source.GetHaveMeasConfig (),destination.GetHaveMeasConfig (), "GetHaveMeasConfig");
  NS_TEST_ASSERT_MSG_EQ (source.GetHaveMobilityControlInfo (),destination.GetHaveMobilityControlInfo (), "GetHaveMobilityControlInfo");
  NS_TEST_ASSERT_MSG_EQ (source.GetHaveRadioResourceConfigDedicated (),destination.GetHaveRadioResourceConfigDedicated (), "GetHaveRadioResourceConfigDedicated");

  if ( source.GetHaveMobilityControlInfo () )
    {
      NS_TEST_ASSERT_MSG_EQ (source.GetMobilityControlInfo ().targetPhysCellId,destination.GetMobilityControlInfo ().targetPhysCellId, "GetMobilityControlInfo().targetPhysCellId");
      NS_TEST_ASSERT_MSG_EQ (source.GetMobilityControlInfo ().haveCarrierFreq,destination.GetMobilityControlInfo ().haveCarrierFreq, "GetMobilityControlInfo().haveCarrierFreq");
      NS_TEST_ASSERT_MSG_EQ (source.GetMobilityControlInfo ().haveCarrierBandwidth,destination.GetMobilityControlInfo ().haveCarrierBandwidth, "GetMobilityControlInfo().haveCarrierBandwidth");
      NS_TEST_ASSERT_MSG_EQ (source.GetMobilityControlInfo ().newUeIdentity,destination.GetMobilityControlInfo ().newUeIdentity, "GetMobilityControlInfo().newUeIdentity");
      NS_TEST_ASSERT_MSG_EQ (source.GetMobilityControlInfo ().haveRachConfigDedicated,destination.GetMobilityControlInfo ().haveRachConfigDedicated, "GetMobilityControlInfo().haveRachConfigDedicated");

      if (source.GetMobilityControlInfo ().haveCarrierFreq)
        {
          NS_TEST_ASSERT_MSG_EQ (source.GetMobilityControlInfo ().carrierFreq.dlCarrierFreq,
                                 destination.GetMobilityControlInfo ().carrierFreq.dlCarrierFreq,
                                 "GetMobilityControlInfo().carrierFreq.dlCarrierFreq");
          NS_TEST_ASSERT_MSG_EQ (source.GetMobilityControlInfo ().carrierFreq.ulCarrierFreq,
                                 destination.GetMobilityControlInfo ().carrierFreq.ulCarrierFreq,
                                 "GetMobilityControlInfo().carrierFreq.ulCarrierFreq");
        }

      if (source.GetMobilityControlInfo ().haveCarrierBandwidth)
        {
          NS_TEST_ASSERT_MSG_EQ (source.GetMobilityControlInfo ().carrierBandwidth.dlBandwidth,
                                 destination.GetMobilityControlInfo ().carrierBandwidth.dlBandwidth,
                                 "GetMobilityControlInfo().carrierBandwidth.dlBandwidth");
          NS_TEST_ASSERT_MSG_EQ (source.GetMobilityControlInfo ().carrierBandwidth.ulBandwidth,
                                 destination.GetMobilityControlInfo ().carrierBandwidth.ulBandwidth,
                                 "GetMobilityControlInfo().carrierBandwidth.ulBandwidth");
        }

      if (source.GetMobilityControlInfo ().haveRachConfigDedicated)
        {
          NS_TEST_ASSERT_MSG_EQ (source.GetMobilityControlInfo ().rachConfigDedicated.raPreambleIndex,
                                 destination.GetMobilityControlInfo ().rachConfigDedicated.raPreambleIndex,
                                 "GetMobilityControlInfo().rachConfigDedicated.raPreambleIndex");
          NS_TEST_ASSERT_MSG_EQ (source.GetMobilityControlInfo ().rachConfigDedicated.raPrachMaskIndex,
                                 destination.GetMobilityControlInfo ().rachConfigDedicated.raPrachMaskIndex,
                                 "GetMobilityControlInfo().rachConfigDedicated.raPrachMaskIndex");
        }
    }

  if (source.GetHaveRadioResourceConfigDedicated ())
    {
      AssertEqualRadioResourceConfigDedicated (source.GetRadioResourceConfigDedicated (), destination.GetRadioResourceConfigDedicated ());
    }
}

// --------------------------- CLASS HandoverPreparationInfoTestCase -----------------------------
class HandoverPreparationInfoTestCase : public RrcHeaderTestCase
{
public:
  HandoverPreparationInfoTestCase ();
  virtual void DoRun (void);
};

HandoverPreparationInfoTestCase::HandoverPreparationInfoTestCase () : RrcHeaderTestCase ("Testing HandoverPreparationInfoTestCase")
{
}

void
HandoverPreparationInfoTestCase::DoRun (void)
{
  std::cout << "============= HandoverPreparationInfoTestCase ===========" << std::endl;
  // add header
  Ptr<Packet> packet = Create<Packet> ();
  packet->Print (std::cout);
  std::cout << std::endl;

  LteRrcSap::HandoverPreparationInfo msg;
  msg.asConfig.sourceDlCarrierFreq = 3;
  msg.asConfig.sourceUeIdentity = 11;
  msg.asConfig.sourceRadioResourceConfig = CreateRadioResourceConfigDedicated ();
  msg.asConfig.sourceMasterInformationBlock.dlBandwidth = 3;
  msg.asConfig.sourceMasterInformationBlock.systemFrameNumber = 1;

  msg.asConfig.sourceSystemInformationBlockType1.cellAccessRelatedInfo.csgIndication = true;
  msg.asConfig.sourceSystemInformationBlockType1.cellAccessRelatedInfo.cellIdentity = 5;
  msg.asConfig.sourceSystemInformationBlockType1.cellAccessRelatedInfo.csgIdentity = 4;
  msg.asConfig.sourceSystemInformationBlockType1.cellAccessRelatedInfo.plmnIdentityInfo.plmnIdentity = 123;

  HandoverPreparationInfoHeader source;
  source.SetMessage (msg);

  std::cout << "--------- SOURCE INFO: -------" << std::endl;
  source.Print (std::cout);
  packet->AddHeader (source);
  std::cout << std::endl;

  // print serialized packet contents
  std::cout << "---- SERIALIZED PACKET CONTENTS: -------" << std::endl;
  std::cout << "Hex: ";
  TestUtils::printPacketContentsHex (GetPointer (packet));
  std::cout << "Bin: ";
  TestUtils::printPacketContentsBin (GetPointer (packet));

  // remove header
  HandoverPreparationInfoHeader destination;
  packet->RemoveHeader (destination);
  std::cout << "--------- DESTINATION INFO: -------" << std::endl;
  destination.Print (std::cout);

  // Check that the destination and source headers contain the same values
  AssertEqualRadioResourceConfigDedicated (source.GetAsConfig ().sourceRadioResourceConfig, destination.GetAsConfig ().sourceRadioResourceConfig);
  NS_TEST_ASSERT_MSG_EQ (source.GetAsConfig ().sourceUeIdentity, destination.GetAsConfig ().sourceUeIdentity, "sourceUeIdentity");
  NS_TEST_ASSERT_MSG_EQ (source.GetAsConfig ().sourceMasterInformationBlock.dlBandwidth,destination.GetAsConfig ().sourceMasterInformationBlock.dlBandwidth, "dlBandwidth");
  NS_TEST_ASSERT_MSG_EQ (source.GetAsConfig ().sourceMasterInformationBlock.systemFrameNumber, destination.GetAsConfig ().sourceMasterInformationBlock.systemFrameNumber, "systemFrameNumber");
  NS_TEST_ASSERT_MSG_EQ (source.GetAsConfig ().sourceSystemInformationBlockType1.cellAccessRelatedInfo.plmnIdentityInfo.plmnIdentity, destination.GetAsConfig ().sourceSystemInformationBlockType1.cellAccessRelatedInfo.plmnIdentityInfo.plmnIdentity, "plmnIdentity");
  NS_TEST_ASSERT_MSG_EQ (source.GetAsConfig ().sourceSystemInformationBlockType1.cellAccessRelatedInfo.csgIndication, destination.GetAsConfig ().sourceSystemInformationBlockType1.cellAccessRelatedInfo.csgIndication, "csgIndication");
  NS_TEST_ASSERT_MSG_EQ (source.GetAsConfig ().sourceSystemInformationBlockType1.cellAccessRelatedInfo.cellIdentity, destination.GetAsConfig ().sourceSystemInformationBlockType1.cellAccessRelatedInfo.cellIdentity, "cellIdentity");
  NS_TEST_ASSERT_MSG_EQ (source.GetAsConfig ().sourceSystemInformationBlockType1.cellAccessRelatedInfo.csgIdentity, destination.GetAsConfig ().sourceSystemInformationBlockType1.cellAccessRelatedInfo.csgIdentity, "csgIdentity");
  NS_TEST_ASSERT_MSG_EQ (source.GetAsConfig ().sourceDlCarrierFreq, destination.GetAsConfig ().sourceDlCarrierFreq, "sourceDlCarrierFreq");
}

// --------------------------- CLASS RrcConnectionReestablishmentRequestTestCase -----------------------------
class RrcConnectionReestablishmentRequestTestCase : public RrcHeaderTestCase
{
public:
  RrcConnectionReestablishmentRequestTestCase ();
  virtual void DoRun (void);
};

RrcConnectionReestablishmentRequestTestCase::RrcConnectionReestablishmentRequestTestCase () : RrcHeaderTestCase ("Testing RrcConnectionReestablishmentRequestTestCase")
{
}

void
RrcConnectionReestablishmentRequestTestCase::DoRun (void)
{
  std::cout << "============= RrcConnectionReestablishmentRequestTestCase ===========" << std::endl;
  // add header
  Ptr<Packet> packet = Create<Packet> ();
  packet->Print (std::cout);
  std::cout << std::endl;

  LteRrcSap::RrcConnectionReestablishmentRequest msg;
  msg.ueIdentity.cRnti = 12;
  msg.ueIdentity.physCellId = 21;
  msg.reestablishmentCause = LteRrcSap::HANDOVER_FAILURE;

  RrcConnectionReestablishmentRequestHeader source;
  source.SetMessage (msg);

  std::cout << "--------- SOURCE INFO: -------" << std::endl;
  source.Print (std::cout);
  packet->AddHeader (source);
  std::cout << std::endl;

  // print serialized packet contents
  std::cout << "---- SERIALIZED PACKET CONTENTS: -------" << std::endl;
  std::cout << "Hex: ";
  TestUtils::printPacketContentsHex (GetPointer (packet));
  std::cout << "Bin: ";
  TestUtils::printPacketContentsBin (GetPointer (packet));

  // remove header
  RrcConnectionReestablishmentRequestHeader destination;
  packet->RemoveHeader (destination);
  std::cout << "--------- DESTINATION INFO: -------" << std::endl;
  destination.Print (std::cout);

  // Check that the destination and source headers contain the same values
  NS_TEST_ASSERT_MSG_EQ (source.GetUeIdentity ().cRnti, destination.GetUeIdentity ().cRnti, "cRnti");
  NS_TEST_ASSERT_MSG_EQ (source.GetUeIdentity ().physCellId, destination.GetUeIdentity ().physCellId, "physCellId");
  NS_TEST_ASSERT_MSG_EQ (source.GetReestablishmentCause (),destination.GetReestablishmentCause (), "ReestablishmentCause");
}

// --------------------------- CLASS RrcConnectionReestablishmentTestCase -----------------------------
class RrcConnectionReestablishmentTestCase : public RrcHeaderTestCase
{
public:
  RrcConnectionReestablishmentTestCase ();
  virtual void DoRun (void);
};

RrcConnectionReestablishmentTestCase::RrcConnectionReestablishmentTestCase () : RrcHeaderTestCase ("Testing RrcConnectionReestablishmentTestCase")
{
}

void
RrcConnectionReestablishmentTestCase::DoRun (void)
{
  std::cout << "============= RrcConnectionReestablishmentTestCase ===========" << std::endl;
  // add header
  Ptr<Packet> packet = Create<Packet> ();
  packet->Print (std::cout);
  std::cout << std::endl;

  LteRrcSap::RrcConnectionReestablishment msg;
  msg.rrcTransactionIdentifier = 2;
  msg.radioResourceConfigDedicated = CreateRadioResourceConfigDedicated ();

  RrcConnectionReestablishmentHeader source;
  source.SetMessage (msg);

  std::cout << "--------- SOURCE INFO: -------" << std::endl;
  source.Print (std::cout);
  packet->AddHeader (source);
  std::cout << std::endl;

  // print serialized packet contents
  std::cout << "---- SERIALIZED PACKET CONTENTS: -------" << std::endl;
  std::cout << "Hex: ";
  TestUtils::printPacketContentsHex (GetPointer (packet));
  std::cout << "Bin: ";
  TestUtils::printPacketContentsBin (GetPointer (packet));

  // remove header
  RrcConnectionReestablishmentHeader destination;
  packet->RemoveHeader (destination);
  std::cout << "--------- DESTINATION INFO: -------" << std::endl;
  destination.Print (std::cout);

  // Check that the destination and source headers contain the same values
  NS_TEST_ASSERT_MSG_EQ (source.GetRrcTransactionIdentifier (), destination.GetRrcTransactionIdentifier (), "rrcTransactionIdentifier");
  AssertEqualRadioResourceConfigDedicated (source.GetRadioResourceConfigDedicated (),destination.GetRadioResourceConfigDedicated ());
}

// --------------------------- CLASS RrcConnectionReestablishmentCompleteTestCase -----------------------------
class RrcConnectionReestablishmentCompleteTestCase : public RrcHeaderTestCase
{
public:
  RrcConnectionReestablishmentCompleteTestCase ();
  virtual void DoRun (void);
};

RrcConnectionReestablishmentCompleteTestCase::RrcConnectionReestablishmentCompleteTestCase () : RrcHeaderTestCase ("Testing RrcConnectionReestablishmentCompleteTestCase")
{
}

void
RrcConnectionReestablishmentCompleteTestCase::DoRun (void)
{
  std::cout << "============= RrcConnectionReestablishmentCompleteTestCase ===========" << std::endl;
  // add header
  Ptr<Packet> packet = Create<Packet> ();
  packet->Print (std::cout);
  std::cout << std::endl;

  LteRrcSap::RrcConnectionReestablishmentComplete msg;
  msg.rrcTransactionIdentifier = 3;

  RrcConnectionReestablishmentCompleteHeader source;
  source.SetMessage (msg);

  std::cout << "--------- SOURCE INFO: -------" << std::endl;
  source.Print (std::cout);
  packet->AddHeader (source);
  std::cout << std::endl;

  // print serialized packet contents
  std::cout << "---- SERIALIZED PACKET CONTENTS: -------" << std::endl;
  std::cout << "Hex: ";
  TestUtils::printPacketContentsHex (GetPointer (packet));
  std::cout << "Bin: ";
  TestUtils::printPacketContentsBin (GetPointer (packet));

  // remove header
  RrcConnectionReestablishmentCompleteHeader destination;
  packet->RemoveHeader (destination);
  std::cout << "--------- DESTINATION INFO: -------" << std::endl;
  destination.Print (std::cout);

  // Check that the destination and source headers contain the same values
  NS_TEST_ASSERT_MSG_EQ (source.GetRrcTransactionIdentifier (), destination.GetRrcTransactionIdentifier (), "rrcTransactionIdentifier");
}

// --------------------------- CLASS RrcConnectionRejectTestCase -----------------------------
class RrcConnectionRejectTestCase : public RrcHeaderTestCase
{
public:
  RrcConnectionRejectTestCase ();
  virtual void DoRun (void);
};

RrcConnectionRejectTestCase::RrcConnectionRejectTestCase () : RrcHeaderTestCase ("Testing RrcConnectionRejectTestCase")
{
}

void
RrcConnectionRejectTestCase::DoRun (void)
{
  std::cout << "============= RrcConnectionRejectTestCase ===========" << std::endl;
  // add header
  Ptr<Packet> packet = Create<Packet> ();
  packet->Print (std::cout);
  std::cout << std::endl;

  LteRrcSap::RrcConnectionReject msg;
  msg.waitTime = 2;

  RrcConnectionRejectHeader source;
  source.SetMessage (msg);

  std::cout << "--------- SOURCE INFO: -------" << std::endl;
  source.Print (std::cout);
  packet->AddHeader (source);
  std::cout << std::endl;

  // print serialized packet contents
  std::cout << "---- SERIALIZED PACKET CONTENTS: -------" << std::endl;
  std::cout << "Hex: ";
  TestUtils::printPacketContentsHex (GetPointer (packet));
  std::cout << "Bin: ";
  TestUtils::printPacketContentsBin (GetPointer (packet));

  // remove header
  RrcConnectionRejectHeader destination;
  packet->RemoveHeader (destination);
  std::cout << "--------- DESTINATION INFO: -------" << std::endl;
  destination.Print (std::cout);

  // Check that the destination and source headers contain the same values
  NS_TEST_ASSERT_MSG_EQ (source.GetMessage ().waitTime, destination.GetMessage ().waitTime, "Different waitTime!");
}

// --------------------------- CLASS Asn1EncodingSuite -----------------------------
class Asn1EncodingSuite : public TestSuite
{
public:
  Asn1EncodingSuite ();
};

Asn1EncodingSuite::Asn1EncodingSuite ()
  : TestSuite ("test-asn1-encoding", UNIT)
{
  Packet::EnablePrinting ();
  NS_LOG_FUNCTION (this);
  AddTestCase (new RrcConnectionRequestTestCase);
  AddTestCase (new RrcConnectionSetupTestCase);
  AddTestCase (new RrcConnectionSetupCompleteTestCase);
  AddTestCase (new RrcConnectionReconfigurationCompleteTestCase);
  AddTestCase (new RrcConnectionReconfigurationTestCase);
  AddTestCase (new HandoverPreparationInfoTestCase);
  AddTestCase (new RrcConnectionReestablishmentRequestTestCase);
  AddTestCase (new RrcConnectionReestablishmentTestCase);
  AddTestCase (new RrcConnectionReestablishmentCompleteTestCase);
  AddTestCase (new RrcConnectionRejectTestCase);
}

Asn1EncodingSuite asn1EncodingSuite;

} // namespace ns3

