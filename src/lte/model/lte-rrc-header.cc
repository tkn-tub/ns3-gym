/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
#include "ns3/lte-rrc-header.h"

#include <stdio.h>
#include <sstream>
#include <math.h> // For log()

#define MAX_DRB 11 // According to section 6.4 3GPP TS 36.331
#define MAX_EARFCN 65535
#define MAX_RAT_CAPABILITIES 8
#define MAX_SI_MESSAGE 32
#define MAX_SIB 32

#define MAX_REPORT_CONFIG_ID 32
#define MAX_OBJECT_ID 32
#define MAX_MEAS_ID 32
#define MAX_CELL_MEAS 32
#define MAX_CELL_REPORT 8

NS_LOG_COMPONENT_DEFINE ("RrcHeader");

namespace ns3 {

//////////////////// RrcAsn1Header class ///////////////////////////////
RrcAsn1Header::RrcAsn1Header ()
{
}

int
RrcAsn1Header::GetMessageType ()
{
  return m_messageType;
}

void
RrcAsn1Header::SerializeDrbToAddModList (std::list<LteRrcSap::DrbToAddMod> drbToAddModList) const
{
  // Serialize DRB-ToAddModList sequence-of
  SerializeSequenceOf (drbToAddModList.size (),MAX_DRB,1);

  // Serialize the elements in the sequence-of list
  std::list<LteRrcSap::DrbToAddMod>::iterator it = drbToAddModList.begin ();
  for (; it != drbToAddModList.end (); it++)
    {
      // Serialize DRB-ToAddMod sequence
      // 5 otional fields. Extension marker is present.
      std::bitset<5> drbToAddModListOptionalFieldsPresent = std::bitset<5> ();
      drbToAddModListOptionalFieldsPresent.set (4,1); // eps-BearerIdentity present
      drbToAddModListOptionalFieldsPresent.set (3,0); // pdcp-Config not present
      drbToAddModListOptionalFieldsPresent.set (2,1); // rlc-Config present
      drbToAddModListOptionalFieldsPresent.set (1,1); // logicalChannelIdentity present
      drbToAddModListOptionalFieldsPresent.set (0,1); // logicalChannelConfig present
      SerializeSequence<5> (drbToAddModListOptionalFieldsPresent,true);

      // Serialize eps-BearerIdentity::=INTEGER (0..15)
      SerializeInteger (it->epsBearerIdentity,0,15);

      // Serialize drb-Identity ::= INTEGER (1..32)
      SerializeInteger (it->drbIdentity,1,32);

      switch (it->rlcConfig.choice)
        {
        case LteRrcSap::RlcConfig::UM_BI_DIRECTIONAL:
          // Serialize rlc-Config choice
          SerializeChoice (4,1);

          // Serialize UL-UM-RLC
          SerializeSequence<0> (std::bitset<0> (),false);
          SerializeEnum (2,0);  // sn-FieldLength

          // Serialize DL-UM-RLC
          SerializeSequence<0> (std::bitset<0> (),false);
          SerializeEnum (2,0);  // sn-FieldLength
          SerializeEnum (32,0);  // t-Reordering
          break;

        case LteRrcSap::RlcConfig::UM_UNI_DIRECTIONAL_UL:
          // Serialize rlc-Config choice
          SerializeChoice (4,2);

          // Serialize UL-UM-RLC
          SerializeSequence<0> (std::bitset<0> (),false);
          SerializeEnum (2,0);  // sn-FieldLength
          break;

        case LteRrcSap::RlcConfig::UM_UNI_DIRECTIONAL_DL:
          // Serialize rlc-Config choice
          SerializeChoice (4,3);

          // Serialize DL-UM-RLC
          SerializeSequence<0> (std::bitset<0> (),false);
          SerializeEnum (2,0);  // sn-FieldLength
          SerializeEnum (32,0);  // t-Reordering
          break;

        case LteRrcSap::RlcConfig::AM:
        default:
          // Serialize rlc-Config choice
          SerializeChoice (4,0);

          // Serialize UL-AM-RLC
          SerializeSequence<0> (std::bitset<0> (),false);
          SerializeEnum (64,0);  // t-PollRetransmit
          SerializeEnum (8,0);   // pollPDU
          SerializeEnum (16,0);  // pollByte
          SerializeEnum (8,0);   // maxRetxThreshold

          // Serialize DL-AM-RLC
          SerializeSequence<0> (std::bitset<0> (),false);
          SerializeEnum (32,0);  // t-Reordering
          SerializeEnum (64,0);  // t-StatusProhibit
          break;
        }

      // Serialize logicalChannelIdentity ::=INTEGER (3..10)
      SerializeInteger (it->logicalChannelIdentity,3,10);

      // Serialize logicalChannelConfig
      SerializeLogicalChannelConfig (it->logicalChannelConfig);
    }
}

void
RrcAsn1Header::SerializeSrbToAddModList (std::list<LteRrcSap::SrbToAddMod> srbToAddModList) const
{
  // Serialize SRB-ToAddModList ::= SEQUENCE (SIZE (1..2)) OF SRB-ToAddMod
  SerializeSequenceOf (srbToAddModList.size (),2,1);

  // Serialize the elements in the sequence-of list
  std::list<LteRrcSap::SrbToAddMod>::iterator it = srbToAddModList.begin ();
  for (; it != srbToAddModList.end (); it++)
    {
      // Serialize SRB-ToAddMod sequence
      // 2 otional fields. Extension marker is present.
      std::bitset<2> srbToAddModListOptionalFieldsPresent = std::bitset<2> ();
      srbToAddModListOptionalFieldsPresent.set (1,0); // rlc-Config not present
      srbToAddModListOptionalFieldsPresent.set (0,1); // logicalChannelConfig present
      SerializeSequence<2> (srbToAddModListOptionalFieldsPresent,true);

      // Serialize srb-Identity ::= INTEGER (1..2)
      SerializeInteger (it->srbIdentity,1,2);

      // Serialize logicalChannelConfig choice
      // 2 options, selected option 0 (var "explicitValue", of type LogicalChannelConfig)
      SerializeChoice (2,0);

      // Serialize LogicalChannelConfig 
      SerializeLogicalChannelConfig (it->logicalChannelConfig);
    }
}

void
RrcAsn1Header::SerializeLogicalChannelConfig (LteRrcSap::LogicalChannelConfig logicalChannelConfig) const
{
  // Serialize LogicalChannelConfig sequence
  // 1 optional field, which is present. No extension marker.
  SerializeSequence<1> (std::bitset<1> (1),false);

  // Serialize ul-SpecificParameters sequence
  // no optional/default fields. No extension marker.
  SerializeSequence<0> (std::bitset<0> (),false);

  // Serialize priority ::= INTEGER (1..16)
  SerializeInteger (logicalChannelConfig.priority,1,16);

  // Serialize prioritisedBitRate
  int prioritizedBitRate;
  switch (logicalChannelConfig.prioritizedBitRateKbps)
    {
    case 0:
      prioritizedBitRate = 0;
      break;
    case 8:
      prioritizedBitRate = 1;
      break;
    case 16:
      prioritizedBitRate = 2;
      break;
    case 32:
      prioritizedBitRate = 3;
      break;
    case 64:
      prioritizedBitRate = 4;
      break;
    case 128:
      prioritizedBitRate = 5;
      break;
    case 256:
      prioritizedBitRate = 6;
      break;
    default:
      prioritizedBitRate = 7;          // Infinity
    }
  SerializeEnum (16,prioritizedBitRate);

  // Serialize bucketSizeDuration
  int bucketSizeDuration;
  switch (logicalChannelConfig.bucketSizeDurationMs)
    {
    case 50:
      bucketSizeDuration = 0;
      break;
    case 100:
      bucketSizeDuration = 1;
      break;
    case 150:
      bucketSizeDuration = 2;
      break;
    case 300:
      bucketSizeDuration = 3;
      break;
    case 500:
      bucketSizeDuration = 4;
      break;
    case 1000:
      bucketSizeDuration = 5;
      break;
    default:
      bucketSizeDuration = 5;
    }
  SerializeEnum (8,bucketSizeDuration);

  // Serialize logicalChannelGroup ::= INTEGER (0..3)
  SerializeInteger (logicalChannelConfig.logicalChannelGroup,0,3);
}

void
RrcAsn1Header::SerializePhysicalConfigDedicated (LteRrcSap::PhysicalConfigDedicated physicalConfigDedicated) const
{
  // Serialize PhysicalConfigDedicated Sequence
  std::bitset<10> optionalFieldsPhysicalConfigDedicated;
  optionalFieldsPhysicalConfigDedicated.set (9,0);  // pdsch-ConfigDedicated not present
  optionalFieldsPhysicalConfigDedicated.set (8,0);  // pucch-ConfigDedicated not present
  optionalFieldsPhysicalConfigDedicated.set (7,0);  // pusch-ConfigDedicated not present
  optionalFieldsPhysicalConfigDedicated.set (6,0);  // uplinkPowerControlDedicated not present
  optionalFieldsPhysicalConfigDedicated.set (5,0);  // tpc-PDCCH-ConfigPUCCH not present
  optionalFieldsPhysicalConfigDedicated.set (4,0);  // tpc-PDCCH-ConfigPUSCH not present
  optionalFieldsPhysicalConfigDedicated.set (3,0);  // cqi-ReportConfig not present
  optionalFieldsPhysicalConfigDedicated.set (2,physicalConfigDedicated.haveSoundingRsUlConfigDedicated);  // soundingRS-UL-ConfigDedicated
  optionalFieldsPhysicalConfigDedicated.set (1,physicalConfigDedicated.haveAntennaInfoDedicated);  // antennaInfo
  optionalFieldsPhysicalConfigDedicated.set (0,0);  // schedulingRequestConfig not present
  SerializeSequence<10> (optionalFieldsPhysicalConfigDedicated,true);

  if (physicalConfigDedicated.haveSoundingRsUlConfigDedicated)
    {
      // Serialize SoundingRS-UL-ConfigDedicated choice:
      switch (physicalConfigDedicated.soundingRsUlConfigDedicated.type)
        {
        case LteRrcSap::SoundingRsUlConfigDedicated::RESET:
          SerializeChoice (2,0);
          SerializeNull ();
          break;

        case LteRrcSap::SoundingRsUlConfigDedicated::SETUP:
        default:
          // 2 options, selected: 1 (setup)
          SerializeChoice (2,1);

          // Serialize setup sequence
          // 0 optional / default fields, no extension marker.
          SerializeSequence<0> (std::bitset<0> (),false);

          // Serialize srs-Bandwidth
          SerializeEnum (4,physicalConfigDedicated.soundingRsUlConfigDedicated.srsBandwidth);

          // Serialize  srs-HoppingBandwidth
          SerializeEnum (4,0);

          // Serialize freqDomainPosition
          SerializeInteger (0,0,23);

          // Serialize duration
          SerializeBoolean (false);

          // Serialize srs-ConfigIndex
          SerializeInteger (physicalConfigDedicated.soundingRsUlConfigDedicated.srsConfigIndex,0,1023);

          // Serialize transmissionComb
          SerializeInteger (0,0,1);

          // Serialize cyclicShift
          SerializeEnum (8,0);

          break;
        }
    }

  if (physicalConfigDedicated.haveAntennaInfoDedicated)
    {
      // Serialize antennaInfo choice
      // 2 options. Selected: 0 ("explicitValue" of type "AntennaInfoDedicated")
      SerializeChoice (2,0);

      // Serialize AntennaInfoDedicated sequence
      // 1 optional parameter, not present. No extension marker.
      SerializeSequence<1> (std::bitset<1> (0),false);

      // Serialize transmissionMode
      SerializeEnum (8,physicalConfigDedicated.antennaInfo.transmissionMode);

      // Serialize ue-TransmitAntennaSelection choice
      SerializeChoice (2,0);

      // Serialize release
      SerializeNull ();
    }
}

void
RrcAsn1Header::SerializeRadioResourceConfigDedicated (LteRrcSap::RadioResourceConfigDedicated radioResourceConfigDedicated) const
{
  bool isSrbToAddModListPresent = !radioResourceConfigDedicated.srbToAddModList.empty ();
  bool isDrbToAddModListPresent = !radioResourceConfigDedicated.drbToAddModList.empty ();
  bool isDrbToReleaseListPresent = !radioResourceConfigDedicated.drbToReleaseList.empty ();

  // 6 optional fields. Extension marker is present.
  std::bitset<6> optionalFieldsPresent = std::bitset<6> ();
  optionalFieldsPresent.set (5,isSrbToAddModListPresent);  // srb-ToAddModList present
  optionalFieldsPresent.set (4,isDrbToAddModListPresent);  // drb-ToAddModList present
  optionalFieldsPresent.set (3,isDrbToReleaseListPresent);  // drb-ToReleaseList present
  optionalFieldsPresent.set (2,0);  // mac-MainConfig not present
  optionalFieldsPresent.set (1,0);  // sps-Config not present
  optionalFieldsPresent.set (0,(radioResourceConfigDedicated.havePhysicalConfigDedicated) ? 1 : 0);
  SerializeSequence<6> (optionalFieldsPresent,true);

  // Serialize srbToAddModList
  if (isSrbToAddModListPresent)
    {
      SerializeSrbToAddModList (radioResourceConfigDedicated.srbToAddModList);
    }

  // Serialize drbToAddModList
  if (isDrbToAddModListPresent)
    {
      SerializeDrbToAddModList (radioResourceConfigDedicated.drbToAddModList);
    }

  // Serialize drbToReleaseList
  if (isDrbToReleaseListPresent)
    {
      SerializeSequenceOf (radioResourceConfigDedicated.drbToReleaseList.size (),MAX_DRB,1);
      std::list<uint8_t>::iterator it = radioResourceConfigDedicated.drbToReleaseList.begin ();
      for (; it != radioResourceConfigDedicated.drbToReleaseList.end (); it++)
        {
          // DRB-Identity ::= INTEGER (1..32)
          SerializeInteger (*it,1,32);
        }
    }

  if (radioResourceConfigDedicated.havePhysicalConfigDedicated)
    {
      SerializePhysicalConfigDedicated (radioResourceConfigDedicated.physicalConfigDedicated);
    }
}

void
RrcAsn1Header::SerializeSystemInformationBlockType1 (LteRrcSap::SystemInformationBlockType1 systemInformationBlockType1) const
{
  // 3 optional fields, no extension marker.
  std::bitset<3> sysInfoBlk1Opts;
  sysInfoBlk1Opts.set (2,0); // p-Max absent
  sysInfoBlk1Opts.set (1,0); // tdd-Config absent
  sysInfoBlk1Opts.set (0,0); // nonCriticalExtension absent
  SerializeSequence (sysInfoBlk1Opts,false);

  // Serialize cellAccessRelatedInfo
  // 1 optional field (csgIdentity) which is present, no extension marker.
  SerializeSequence (std::bitset<1> (1),false);

  // Serialize plmn-IdentityList
  SerializeSequenceOf (1,6,1);

  // PLMN-IdentityInfo 
  SerializeSequence (std::bitset<0> (),false);

  SerializePlmnIdentity (systemInformationBlockType1.cellAccessRelatedInfo.plmnIdentityInfo.plmnIdentity);

  // Serialize trackingAreaCode
  SerializeBitstring (std::bitset<16> (0));
  // Serialize cellIdentity
  SerializeBitstring (std::bitset<28> (systemInformationBlockType1.cellAccessRelatedInfo.cellIdentity));
  // Serialize cellBarred
  SerializeEnum (2,0);
  // Serialize intraFreqReselection
  SerializeEnum (2,0);
  // Serialize csg-Indication
  SerializeBoolean (systemInformationBlockType1.cellAccessRelatedInfo.csgIndication);
  // Serialize csg-Identity
  SerializeBitstring (std::bitset<27> (systemInformationBlockType1.cellAccessRelatedInfo.csgIdentity));

  // Serialize cellSelectionInfo
  SerializeSequence (std::bitset<1> (0),false);
  // Serialize q-RxLevMin
  SerializeInteger (-50,-70,-22);

  // Serialize freqBandIndicator
  SerializeInteger (1,1,64);

  // Serialize schedulingInfoList
  SerializeSequenceOf (1,MAX_SI_MESSAGE,1);
  // SchedulingInfo
  SerializeSequence (std::bitset<0> (),false);
  // si-Periodicity
  SerializeEnum (7,0);
  // sib-MappingInfo
  SerializeSequenceOf (0,MAX_SIB - 1,0);

  // Serialize si-WindowLength
  SerializeEnum (7,0);

  // Serialize systemInfoValueTag
  SerializeInteger (0,0,31);
}

void
RrcAsn1Header::SerializeRadioResourceConfigCommonSIB () const
{
  SerializeSequence (std::bitset<0> (0),true);
  // rach-ConfigCommon
  SerializeSequence (std::bitset<0> (0),true);
  SerializeSequence (std::bitset<1> (0),false); // preambleInfo
  SerializeEnum (16,0); // numberOfRA-Preambles
  SerializeSequence (std::bitset<0> (0),false); // powerRampingParameters
  SerializeEnum (4,0); // powerRampingStep
  SerializeEnum (16,0); // preambleInitialReceivedTargetPower
  SerializeSequence (std::bitset<0> (0),false); // ra-SupervisionInfo
  SerializeEnum (11,0); // preambleTransMax
  SerializeEnum (8,0); // ra-ResponseWindowSize
  SerializeEnum (8,0); // mac-ContentionResolutionTimer
  SerializeInteger (1,1,8); // maxHARQ-Msg3Tx
  // bcch-Config 
  SerializeSequence (std::bitset<0> (0),false);
  SerializeEnum (4,0); // modificationPeriodCoeff
  // pcch-Config 
  SerializeSequence (std::bitset<0> (0),false);
  SerializeEnum (4,0); // defaultPagingCycle
  SerializeEnum (8,0); // nB
  // prach-Config 
  SerializeSequence (std::bitset<1> (0),false);
  SerializeInteger (0,0,1023); // rootSequenceIndex
  // pdsch-ConfigCommon 
  SerializeSequence (std::bitset<0> (0),false);
  SerializeInteger (0,-60,50); // referenceSignalPower
  SerializeInteger (0,0,3); // p-b
  // pusch-ConfigCommon 
  SerializeSequence (std::bitset<0> (0),false);
  SerializeSequence (std::bitset<0> (0),false); // pusch-ConfigBasic
  SerializeInteger (1,1,4); // n-SB
  SerializeEnum (2,0); // hoppingMode
  SerializeInteger (0,0,98); // pusch-HoppingOffset
  SerializeBoolean (false); // enable64QAM
  SerializeSequence (std::bitset<0> (0),false); // UL-ReferenceSignalsPUSCH
  SerializeBoolean (false); // groupHoppingEnabled
  SerializeInteger (0,0,29); // groupAssignmentPUSCH
  SerializeBoolean (false); // sequenceHoppingEnabled
  SerializeInteger (0,0,7); // cyclicShift
  // pucch-ConfigCommon 
  SerializeSequence (std::bitset<0> (0),false);
  SerializeEnum (3,0); // deltaPUCCH-Shift
  SerializeInteger (0,0,98); // nRB-CQI
  SerializeInteger (0,0,7); // nCS-AN
  SerializeInteger (0,0,2047); // n1PUCCH-AN
  // soundingRS-UL-ConfigCommon 
  SerializeChoice (2,0);
  SerializeNull (); // release
  // uplinkPowerControlCommon 
  SerializeSequence (std::bitset<0> (0),false);
  SerializeInteger (0,-126,24); // p0-NominalPUSCH
  SerializeEnum (8,0); // alpha
  SerializeInteger (-50,-127,-96); // p0-NominalPUCCH
  SerializeSequence (std::bitset<0> (0),false); // deltaFList-PUCCH
  SerializeEnum (3,0); // deltaF-PUCCH-Format1
  SerializeEnum (3,0); // deltaF-PUCCH-Format1b
  SerializeEnum (4,0); // deltaF-PUCCH-Format2
  SerializeEnum (3,0); // deltaF-PUCCH-Format2a
  SerializeEnum (3,0); // deltaF-PUCCH-Format2b
  SerializeInteger (0,-1,6);
  // ul-CyclicPrefixLength 
  SerializeEnum (2,0);
}

void
RrcAsn1Header::SerializeSystemInformationBlockType2 () const
{
  SerializeSequence (std::bitset<2> (0),true);

  // RadioResourceConfigCommonSIB
  SerializeRadioResourceConfigCommonSIB ();

  // ue-TimersAndConstants
  SerializeSequence (std::bitset<0> (0),true);
  SerializeEnum (8,0); // t300
  SerializeEnum (8,0); // t301
  SerializeEnum (7,0); // t310
  SerializeEnum (8,0); // n310
  SerializeEnum (7,0); // t311
  SerializeEnum (8,0); // n311
  // freqInfo
  SerializeSequence (std::bitset<2> (0),false);
  SerializeInteger (29,1,32); // additionalSpectrumEmission
  // timeAlignmentTimerCommon
  SerializeEnum (8,0);
}

void
RrcAsn1Header::SerializeMeasResults (LteRrcSap::MeasResults measResults) const
{
  // Watchdog: if list has 0 elements, set boolean to false
  if(measResults.measResultListEutra.empty())
  {
    measResults.haveMeasResultNeighCells = false;
  }

  // Serialize MeasResults sequence, 1 optional value, extension marker present
  SerializeSequence(std::bitset<1>(measResults.haveMeasResultNeighCells),true);

  // Serialize measId
  SerializeInteger(measResults.measId,1,MAX_MEAS_ID);
  
  // Serialize measResultServCell sequence
  SerializeSequence(std::bitset<0>(0),false);

  // Serialize rsrpResult
  SerializeInteger(measResults.rsrpResult,0,97);
  
  // Serialize rsrqResult
  SerializeInteger(measResults.rsrqResult,0,34);
  
  if(measResults.haveMeasResultNeighCells)
  {
    // Serialize Choice = 0 (MeasResultListEUTRA)
    SerializeChoice (4,0);
    
    // Serialize measResultNeighCells
    SerializeSequenceOf(measResults.measResultListEutra.size(),MAX_CELL_REPORT,1);
    
    // serialize MeasResultEutra elements in the list
    std::list<LteRrcSap::MeasResultEutra>::iterator it;
    for (it = measResults.measResultListEutra.begin(); it != measResults.measResultListEutra.end(); it++)
    {
      SerializeSequence (std::bitset<1> (it->haveCgiInfo),false);
      
      // Serialize PhysCellId
      SerializeInteger (it->physCellId, 0, 503);
      
      // Serialize CgiInfo
      if(it->haveCgiInfo)
      {
        SerializeSequence (std::bitset<1>(it->cgiInfo.plmnIdentityList.size()),false);

        // Serialize cellGlobalId
        SerializeSequence (std::bitset<0>(0),false);
        SerializePlmnIdentity(it->cgiInfo.plmnIdentity);
        SerializeBitstring (std::bitset<28>(it->cgiInfo.cellIdentity));
        
        // Serialize trackingAreaCode
        SerializeBitstring (std::bitset<16>(it->cgiInfo.trackingAreaCode));
        
        // Serialize plmn-IdentityList
        if(!it->cgiInfo.plmnIdentityList.empty())
        {
          SerializeSequenceOf(it->cgiInfo.plmnIdentityList.size(),5,1);
          std::list<uint32_t>::iterator it2;
          for (it2 = it->cgiInfo.plmnIdentityList.begin(); it2 != it->cgiInfo.plmnIdentityList.end(); it2++)
          {
            SerializePlmnIdentity(*it2);
          }
        }
      }
      
      // Serialize measResult
      std::bitset<2> measResultFieldsPresent;
      measResultFieldsPresent[1] = it->haveRsrpResult;
      measResultFieldsPresent[0] = it->haveRsrqResult;
      SerializeSequence(measResultFieldsPresent,true);
      
      if(it->haveRsrpResult)
      {
        SerializeInteger (it->rsrpResult,0,97);
      }
      
      if(it->haveRsrqResult)
      {
        SerializeInteger (it->rsrqResult,0,34);
      }
    }
  } 
}

void
RrcAsn1Header::SerializePlmnIdentity (uint32_t plmnId) const
{
  // plmn-Identity sequence, mcc is optional, no extension marker
  SerializeSequence (std::bitset<1> (0), false);
  
  // Serialize mnc
  int nDig = (plmnId > 99) ? 3 : 2;

  SerializeSequenceOf (nDig,3,2);
  for (int i = nDig - 1; i >= 0; i--)
    {
      int n = floor (plmnId / pow (10,i));
      SerializeInteger (n,0,9);
      plmnId -= n * pow (10,i);
    }
    
  // cellReservedForOperatorUse 
  SerializeEnum (2,0);
}

Buffer::Iterator
RrcAsn1Header::DeserializeRadioResourceConfigDedicated (LteRrcSap::RadioResourceConfigDedicated *radioResourceConfigDedicated, Buffer::Iterator bIterator)
{
  // Deserialize RadioResourceConfigDedicated sequence
  std::bitset<6> optionalFieldsPresent = std::bitset<6> ();
  bIterator = DeserializeSequence (&optionalFieldsPresent,true,bIterator);

  if (optionalFieldsPresent[5])
    {
      // Deserialize srb-ToAddModList
      bIterator = DeserializeSrbToAddModList (&(radioResourceConfigDedicated->srbToAddModList),bIterator);
    }

  if (optionalFieldsPresent[4])
    {
      // Deserialize drb-ToAddModList
      bIterator = DeserializeDrbToAddModList (&(radioResourceConfigDedicated->drbToAddModList),bIterator);
    }

  if (optionalFieldsPresent[3])
    {
      // Deserialize drb-ToReleaseList
      int n;
      int val;
      bIterator = DeserializeSequenceOf (&n,MAX_DRB,1,bIterator);
      for (int i = 0; i < n; i++)
        {
          bIterator = DeserializeInteger (&val,1,32,bIterator);
          radioResourceConfigDedicated->drbToReleaseList.push_back (val);
        }
    }

  if (optionalFieldsPresent[2])
    {
      // Deserialize mac-MainConfig
      // ...
    }

  if (optionalFieldsPresent[1])
    {
      // Deserialize sps-Config
      // ...
    }

  radioResourceConfigDedicated->havePhysicalConfigDedicated = optionalFieldsPresent[0];
  if (optionalFieldsPresent[0])
    {
      // Deserialize physicalConfigDedicated
      bIterator = DeserializePhysicalConfigDedicated (&radioResourceConfigDedicated->physicalConfigDedicated,bIterator);
    }

  return bIterator;
}

Buffer::Iterator
RrcAsn1Header::DeserializeSrbToAddModList (std::list<LteRrcSap::SrbToAddMod> *srbToAddModList, Buffer::Iterator bIterator)
{
  int numElems;
  bIterator = DeserializeSequenceOf (&numElems,2,1,bIterator);

  srbToAddModList->clear ();

  // Deserialize SRB-ToAddMod elements
  for (int i = 0; i < numElems; i++)
    {
      LteRrcSap::SrbToAddMod srbToAddMod;
      // Deserialize SRB-ToAddMod sequence
      // 2 optional fields, extension marker present
      std::bitset<2> optionalFields;
      bIterator = DeserializeSequence (&optionalFields,true,bIterator);

      // Deserialize srbIdentity
      int n;
      bIterator = DeserializeInteger (&n,1,2,bIterator);
      srbToAddMod.srbIdentity = n;

      if (optionalFields[1])
        {
          // Deserialize rlcConfig choice
          // ...
        }

      if (optionalFields[0])
        {
          // Deserialize logicalChannelConfig choice
          int sel;
          bIterator = DeserializeChoice (2,&sel,bIterator);

          // Deserialize logicalChannelConfig defaultValue
          if (sel == 1)
            {
              bIterator = DeserializeNull (bIterator);
            }

          // Deserialize logicalChannelConfig explicitValue
          else if (sel == 0)
            {
              bIterator = DeserializeLogicalChannelConfig (&srbToAddMod.logicalChannelConfig,bIterator);
            }
        }
      srbToAddModList->insert (srbToAddModList->end (),srbToAddMod);
    }

  return bIterator;
}

Buffer::Iterator
RrcAsn1Header::DeserializeDrbToAddModList (std::list<LteRrcSap::DrbToAddMod> *drbToAddModList, Buffer::Iterator bIterator)
{
  int n;
  int val;
  bIterator = DeserializeSequenceOf (&n,MAX_DRB,1,bIterator);

  drbToAddModList->clear ();

  for (int i = 0; i < n; i++)
    {
      LteRrcSap::DrbToAddMod drbToAddMod;

      std::bitset<5> optionalFields;
      bIterator = DeserializeSequence (&optionalFields,true,bIterator);

      if (optionalFields[4])
        {
          // Deserialize epsBearerIdentity
          bIterator = DeserializeInteger (&val,0,15,bIterator);
          drbToAddMod.epsBearerIdentity = val;
        }

      bIterator = DeserializeInteger (&val,1,32,bIterator);
      drbToAddMod.drbIdentity = val;

      if (optionalFields[3])
        {
          // Deserialize pdcp-Config
          // ...
        }

      if (optionalFields[2])
        {
          // Deserialize RLC-Config
          int chosen;
          bIterator = DeserializeChoice (4,&chosen,bIterator);

          int sel;
          std::bitset<0> bitset0;
          switch (chosen)
            {
            case 0:
              drbToAddMod.rlcConfig.choice = LteRrcSap::RlcConfig::AM;

              // Deserialize UL-AM-RLC
              bIterator = DeserializeSequence<0> (&bitset0,false, bIterator);
              bIterator = DeserializeEnum (64,&sel, bIterator); // t-PollRetransmit
              bIterator = DeserializeEnum (8,&sel, bIterator); // pollPDU
              bIterator = DeserializeEnum (16,&sel, bIterator); // pollByte
              bIterator = DeserializeEnum (8,&sel, bIterator); // maxRetxThreshold

              // Deserialize DL-AM-RLC
              bIterator = DeserializeSequence<0> (&bitset0,false, bIterator);
              bIterator = DeserializeEnum (32,&sel, bIterator); // t-Reordering
              bIterator = DeserializeEnum (64,&sel, bIterator); // t-StatusProhibit
              break;

            case 1:
              drbToAddMod.rlcConfig.choice = LteRrcSap::RlcConfig::UM_BI_DIRECTIONAL;

              // Deserialize UL-UM-RLC
              bIterator = DeserializeSequence<0> (&bitset0,false, bIterator);
              bIterator = DeserializeEnum (2,&sel, bIterator); // sn-FieldLength

              // Deserialize DL-UM-RLC
              bIterator = DeserializeSequence<0> (&bitset0,false, bIterator);
              bIterator = DeserializeEnum (2,&sel, bIterator); // sn-FieldLength
              bIterator = DeserializeEnum (32,&sel, bIterator); // t-Reordering
              break;

            case 2:
              drbToAddMod.rlcConfig.choice = LteRrcSap::RlcConfig::UM_UNI_DIRECTIONAL_UL;

              // Deserialize UL-UM-RLC
              bIterator = DeserializeSequence<0> (&bitset0,false, bIterator);
              bIterator = DeserializeEnum (2,&sel, bIterator); // sn-FieldLength
              break;

            case 3:
              drbToAddMod.rlcConfig.choice = LteRrcSap::RlcConfig::UM_UNI_DIRECTIONAL_DL;

              // Deserialize DL-UM-RLC
              bIterator = DeserializeSequence<0> (&bitset0,false, bIterator);
              bIterator = DeserializeEnum (2,&sel, bIterator); // sn-FieldLength
              bIterator = DeserializeEnum (32,&sel, bIterator); // t-Reordering
              break;
            }

        }

      if (optionalFields[1])
        {
          bIterator = DeserializeInteger (&val,3,10,bIterator);
          drbToAddMod.logicalChannelIdentity = val;
        }

      if (optionalFields[0])
        {
          bIterator = DeserializeLogicalChannelConfig (&drbToAddMod.logicalChannelConfig,bIterator);
        }

      drbToAddModList->insert (drbToAddModList->end (),drbToAddMod);
    }
  return bIterator;
}

Buffer::Iterator
RrcAsn1Header::DeserializeLogicalChannelConfig (LteRrcSap::LogicalChannelConfig *logicalChannelConfig, Buffer::Iterator bIterator)
{
  int n;

  // Deserialize LogicalChannelConfig sequence
  // 1 optional field, no extension marker.
  std::bitset<1> bitset1;
  bIterator = DeserializeSequence (&bitset1,false,bIterator);

  if (bitset1[0])
    {
      // Deserialize ul-SpecificParameters sequence
      std::bitset<0> bitset0;
      bIterator = DeserializeSequence (&bitset0,false,bIterator);

      // Deserialize priority
      bIterator = DeserializeInteger (&n,1,16,bIterator);
      logicalChannelConfig->priority = n;

      // Deserialize prioritisedBitRate
      bIterator = DeserializeEnum (16,&n,bIterator);
      uint16_t prioritizedBitRateKbps;

      switch (n)
        {
        case 0:
          prioritizedBitRateKbps = 0;
          break;
        case 1:
          prioritizedBitRateKbps = 8;
          break;
        case 2:
          prioritizedBitRateKbps = 16;
          break;
        case 3:
          prioritizedBitRateKbps = 32;
          break;
        case 4:
          prioritizedBitRateKbps = 64;
          break;
        case 5:
          prioritizedBitRateKbps = 128;
          break;
        case 6:
          prioritizedBitRateKbps = 256;
          break;
        case 7:
          prioritizedBitRateKbps = 10000;
          break;
        }
      logicalChannelConfig->prioritizedBitRateKbps = prioritizedBitRateKbps;

      // Deserialize bucketSizeDuration
      bIterator = DeserializeEnum (8,&n,bIterator);
      uint16_t bucketSizeDurationMs;
      switch (n)
        {
        case 0:
          bucketSizeDurationMs = 50;
          break;
        case 1:
          bucketSizeDurationMs = 100;
          break;
        case 2:
          bucketSizeDurationMs = 150;
          break;
        case 3:
          bucketSizeDurationMs = 300;
          break;
        case 4:
          bucketSizeDurationMs = 500;
          break;
        case 5:
          bucketSizeDurationMs = 1000;
          break;
        }
      logicalChannelConfig->bucketSizeDurationMs = bucketSizeDurationMs;

      // Deserialize logicalChannelGroup
      bIterator = DeserializeInteger (&n,0,3,bIterator);
      logicalChannelConfig->logicalChannelGroup = n;
    }
  return bIterator;
}

Buffer::Iterator
RrcAsn1Header::DeserializePhysicalConfigDedicated (LteRrcSap::PhysicalConfigDedicated *physicalConfigDedicated, Buffer::Iterator bIterator)
{
  std::bitset<10> optionalFieldPresent;
  bIterator = DeserializeSequence (&optionalFieldPresent,true,bIterator);

  if (optionalFieldPresent[9])
    {
      // Deserialize pdsch-ConfigDedicated
      // ...
    }
  if (optionalFieldPresent[8])
    {
      // Deserialize pucch-ConfigDedicated
      // ...
    }
  if (optionalFieldPresent[7])
    {
      // Deserialize pusch-ConfigDedicated
      // ...
    }
  if (optionalFieldPresent[6])
    {
      // Deserialize uplinkPowerControlDedicated
      // ...
    }
  if (optionalFieldPresent[5])
    {
      // Deserialize tpc-PDCCH-ConfigPUCCH
      // ...
    }
  if (optionalFieldPresent[4])
    {
      // Deserialize tpc-PDCCH-ConfigPUSCH
      // ...
    }
  if (optionalFieldPresent[3])
    {
      // Deserialize cqi-ReportConfig
      // ...
    }
  physicalConfigDedicated->haveSoundingRsUlConfigDedicated = optionalFieldPresent[2];
  if (optionalFieldPresent[2])
    {
      // Deserialize soundingRS-UL-ConfigDedicated
      int sel;
      bIterator = DeserializeChoice (2,&sel,bIterator);

      if (sel == 0)
        {
          physicalConfigDedicated->soundingRsUlConfigDedicated.type = LteRrcSap::SoundingRsUlConfigDedicated::RESET;

          bIterator = DeserializeNull (bIterator);
        }

      else if (sel == 1)
        {
          physicalConfigDedicated->soundingRsUlConfigDedicated.type = LteRrcSap::SoundingRsUlConfigDedicated::SETUP;

          std::bitset<0> bitset0;
          bIterator = DeserializeSequence (&bitset0,false,bIterator);

          int slct;

          // Deserialize srs-Bandwidth
          bIterator = DeserializeEnum (4,&slct,bIterator);
          physicalConfigDedicated->soundingRsUlConfigDedicated.srsBandwidth = slct;

          // Deserialize srs-HoppingBandwidth
          bIterator = DeserializeEnum (4,&slct,bIterator);

          // Deserialize freqDomainPosition
          bIterator = DeserializeInteger (&slct,0,23,bIterator);

          // Deserialize duration
          bool duration;
          bIterator = DeserializeBoolean (&duration,bIterator);

          // Deserialize srs-ConfigIndex
          bIterator = DeserializeInteger (&slct,0,1023,bIterator);
          physicalConfigDedicated->soundingRsUlConfigDedicated.srsConfigIndex = slct;

          // Deserialize transmissionComb
          bIterator = DeserializeInteger (&slct,0,1,bIterator);

          // Deserialize cyclicShift
          bIterator = DeserializeEnum (8,&slct,bIterator);
        }
    }
  physicalConfigDedicated->haveAntennaInfoDedicated = optionalFieldPresent[1];
  if (optionalFieldPresent[1])
    {
      // Deserialize antennaInfo
      int sel;
      bIterator = DeserializeChoice (2,&sel,bIterator);
      if (sel == 1)
        {
          bIterator = DeserializeNull (bIterator);
        }
      else if (sel == 0)
        {
          std::bitset<1> codebookSubsetRestrictionPresent;
          bIterator = DeserializeSequence (&codebookSubsetRestrictionPresent,false,bIterator);

          int txmode;
          bIterator = DeserializeEnum (8,&txmode,bIterator);
          physicalConfigDedicated->antennaInfo.transmissionMode = txmode;

          if (codebookSubsetRestrictionPresent[0])
            {
              // Deserialize codebookSubsetRestriction
              // ...
            }

          int txantennaselchosen;
          bIterator = DeserializeChoice (2,&txantennaselchosen,bIterator);
          if (txantennaselchosen == 0)
            {
              // Deserialize ue-TransmitAntennaSelection release
              bIterator = DeserializeNull (bIterator);
            }
          else if (txantennaselchosen == 1)
            {
              // Deserialize ue-TransmitAntennaSelection setup
              // ...
            }
        }
    }
  if (optionalFieldPresent[0])
    {
      // Deserialize schedulingRequestConfig
      // ...
    }
  return bIterator;
}

void
RrcAsn1Header::Print (std::ostream &os, LteRrcSap::RadioResourceConfigDedicated radioResourceConfigDedicated) const
{
  os << "   srbToAddModList: " << std::endl;
  std::list<LteRrcSap::SrbToAddMod>::iterator it = radioResourceConfigDedicated.srbToAddModList.begin ();
  for (; it != radioResourceConfigDedicated.srbToAddModList.end (); it++)
    {
      os << "      srbIdentity: " << (int)it->srbIdentity << std::endl;
      os << "      logicalChannelConfig: " << std::endl;
      os << "         priority: " <<  (int)it->logicalChannelConfig.priority << std::endl;
      os << "         prioritizedBitRateKbps: " <<  (int)it->logicalChannelConfig.prioritizedBitRateKbps << std::endl;
      os << "         bucketSizeDurationMs: " <<  (int)it->logicalChannelConfig.bucketSizeDurationMs << std::endl;
      os << "         logicalChannelGroup: " <<  (int)it->logicalChannelConfig.logicalChannelGroup << std::endl;
    }
  os << std::endl;

  os << "   drbToAddModList: " << std::endl;
  std::list<LteRrcSap::DrbToAddMod>::iterator it2 = radioResourceConfigDedicated.drbToAddModList.begin ();
  for (; it2 != radioResourceConfigDedicated.drbToAddModList.end (); it2++)
    {
      os << "      epsBearerIdentity: " << (int)it2->epsBearerIdentity << std::endl;
      os << "      drbIdentity: " << (int)it2->drbIdentity << std::endl;
      os << "      rlcConfig: " << it2->rlcConfig.choice << std::endl;
      os << "      logicalChannelIdentity: " << (int)it2->logicalChannelIdentity << std::endl;
      os << "      logicalChannelConfig: " << std::endl;
      os << "         priority: " <<  (int)it2->logicalChannelConfig.priority << std::endl;
      os << "         prioritizedBitRateKbps: " <<  (int)it2->logicalChannelConfig.prioritizedBitRateKbps << std::endl;
      os << "         bucketSizeDurationMs: " <<  (int)it2->logicalChannelConfig.bucketSizeDurationMs << std::endl;
      os << "         logicalChannelGroup: " <<  (int)it2->logicalChannelConfig.logicalChannelGroup << std::endl;
    }
  os << std::endl;

  os << "   drbToReleaseList: ";
  std::list<uint8_t>::iterator it3 = radioResourceConfigDedicated.drbToReleaseList.begin ();
  for (; it3 != radioResourceConfigDedicated.drbToReleaseList.end (); it3++)
    {
      os << (int)*it3 << ", ";
    }
  os << std::endl;

  os << "   havePhysicalConfigDedicated: " << radioResourceConfigDedicated.havePhysicalConfigDedicated << std::endl;

  if (radioResourceConfigDedicated.havePhysicalConfigDedicated)
    {
      os << "   physicalConfigDedicated: " << std::endl;

      os << "      haveSoundingRsUlConfigDedicated: " << radioResourceConfigDedicated.physicalConfigDedicated.haveSoundingRsUlConfigDedicated << std::endl;
      if (radioResourceConfigDedicated.physicalConfigDedicated.haveSoundingRsUlConfigDedicated)
        {
          os << "      soundingRsUlConfigDedicated: " << std::endl;
          os << "         type: " << radioResourceConfigDedicated.physicalConfigDedicated.soundingRsUlConfigDedicated.type << std::endl;
          os << "         srsBandwidth: " << (int)radioResourceConfigDedicated.physicalConfigDedicated.soundingRsUlConfigDedicated.srsBandwidth << std::endl;
          os << "         srsConfigIndex: " << (int)radioResourceConfigDedicated.physicalConfigDedicated.soundingRsUlConfigDedicated.srsConfigIndex << std::endl;
        }

      os << "      haveAntennaInfoDedicated: " << radioResourceConfigDedicated.physicalConfigDedicated.haveAntennaInfoDedicated << std::endl;
      if (radioResourceConfigDedicated.physicalConfigDedicated.haveAntennaInfoDedicated)
        {
          os << "      antennaInfo Tx mode: " << (int)radioResourceConfigDedicated.physicalConfigDedicated.antennaInfo.transmissionMode << std::endl;
        }
    }
}

Buffer::Iterator
RrcAsn1Header::DeserializeSystemInformationBlockType1 (LteRrcSap::SystemInformationBlockType1 *systemInformationBlockType1, Buffer::Iterator bIterator)
{
  std::bitset<0> bitset0;
  int n;

  std::bitset<3> sysInfoBlkT1Opts;
  bIterator = DeserializeSequence (&sysInfoBlkT1Opts,false,bIterator);

  // Deserialize cellAccessRelatedInfo
  std::bitset<1> cellAccessRelatedInfoOpts;
  bIterator = DeserializeSequence (&cellAccessRelatedInfoOpts,false,bIterator);

  // Deserialize plmn-IdentityList
  int numPlmnIdentityInfoElements;
  bIterator = DeserializeSequenceOf (&numPlmnIdentityInfoElements,6,1,bIterator);
  for (int i = 0; i < numPlmnIdentityInfoElements; i++)
    {
      bIterator = DeserializeSequence (&bitset0,false,bIterator);

      // plmn-Identity
      bIterator = DeserializePlmnIdentity(&systemInformationBlockType1->cellAccessRelatedInfo.plmnIdentityInfo.plmnIdentity,bIterator);
    }

  // Deserialize trackingAreaCode
  std::bitset<16> trackingAreaCode;
  bIterator = DeserializeBitstring (&trackingAreaCode,bIterator);

  // Deserialize cellIdentity
  std::bitset<28> cellIdentity;
  bIterator = DeserializeBitstring (&cellIdentity,bIterator);
  systemInformationBlockType1->cellAccessRelatedInfo.cellIdentity = cellIdentity.to_ulong ();

  // Deserialize cellBarred
  bIterator = DeserializeEnum (2,&n,bIterator);

  // Deserialize intraFreqReselection
  bIterator = DeserializeEnum (2,&n,bIterator);

  // Deserialize csg-Indication
  bIterator = DeserializeBoolean (&systemInformationBlockType1->cellAccessRelatedInfo.csgIndication,bIterator);

  if (cellAccessRelatedInfoOpts[0])
    {
      // Deserialize csg-Identity
      std::bitset<27> csgIdentity;
      bIterator = DeserializeBitstring (&csgIdentity,bIterator);
      systemInformationBlockType1->cellAccessRelatedInfo.csgIdentity = csgIdentity.to_ulong ();
    }

  // Deserialize cellSelectionInfo
  std::bitset<1> qRxLevMinOffsetPresent;
  bIterator = DeserializeSequence (&qRxLevMinOffsetPresent,false,bIterator);
  bIterator = DeserializeInteger (&n,-70,-22,bIterator); //q-RxLevMin
  if (qRxLevMinOffsetPresent[0])
    {
      // Deserialize qRxLevMinOffset
      // ...
    }

  if (sysInfoBlkT1Opts[2])
    {
      // Deserialize p-Max
      // ...
    }

  // freqBandIndicator
  bIterator = DeserializeInteger (&n,1,64,bIterator);

  // schedulingInfoList
  int numSchedulingInfo;
  bIterator = DeserializeSequenceOf (&numSchedulingInfo,MAX_SI_MESSAGE,1,bIterator);
  for (int i = 0; i < numSchedulingInfo; i++)
    {
      bIterator = DeserializeSequence (&bitset0,false,bIterator);
      bIterator = DeserializeEnum (7,&n,bIterator); // si-Periodicity
      int numSibType;
      bIterator = DeserializeSequenceOf (&numSibType,MAX_SIB - 1,0, bIterator); // sib-MappingInfo
      for (int j = 0; j < numSibType; j++)
        {
          bIterator = DeserializeEnum (16,&n,bIterator); // SIB-Type
        }
    }

  if (sysInfoBlkT1Opts[1])
    {
      // tdd-Config
      // ...
    }

  // si-WindowLength
  bIterator = DeserializeEnum (7,&n,bIterator);

  // systemInfoValueTag
  bIterator = DeserializeInteger (&n,0,31,bIterator);

  if (sysInfoBlkT1Opts[0])
    {
      // Deserialize nonCriticalExtension
      // ...
    }
  return bIterator;
}

Buffer::Iterator
RrcAsn1Header::DeserializeSystemInformationBlockType2 (Buffer::Iterator bIterator)
{
  std::bitset<0> bitset0;
  int n;

  std::bitset<2> sysInfoBlkT2Opts;
  bIterator = DeserializeSequence (&sysInfoBlkT2Opts,true,bIterator);
  if (sysInfoBlkT2Opts[1])
    {
      // Deserialize ac-BarringInfo
      // ...
    }

  // Deserialize radioResourceConfigCommon
  bIterator = DeserializeRadioResourceConfigCommonSib (bIterator);

  // Deserialize ue-TimersAndConstants
  bIterator = DeserializeSequence (&bitset0,true,bIterator);
  bIterator = DeserializeEnum (8,&n,bIterator); // t300
  bIterator = DeserializeEnum (8,&n,bIterator); // t301
  bIterator = DeserializeEnum (7,&n,bIterator); // t310
  bIterator = DeserializeEnum (8,&n,bIterator); // n310
  bIterator = DeserializeEnum (7,&n,bIterator); // t311
  bIterator = DeserializeEnum (8,&n,bIterator); // n311

  // Deserialize freqInfo
  std::bitset<2> freqInfoOpts;
  bIterator = DeserializeSequence (&freqInfoOpts,false,bIterator);
  if (freqInfoOpts[1])
    {
      // Deserialize ul-CarrierFreq
      // ...
    }
  if (freqInfoOpts[0])
    {
      // Deserialize ul-Bandwidth
      // ...
    }
  bIterator = DeserializeInteger (&n,1,32,bIterator); // additionalSpectrumEmission

  if (sysInfoBlkT2Opts[0])
    {
      // Deserialize mbsfn-SubframeConfigList
      // ...
    }

  // Deserialize timeAlignmentTimerCommon
  bIterator = DeserializeEnum (8,&n,bIterator);

  return bIterator;
}


Buffer::Iterator
RrcAsn1Header::DeserializeRadioResourceConfigCommon (Buffer::Iterator bIterator)
{
  std::bitset<0> bitset0;
  int n;

  std::bitset<9> rrCfgCommOptions;
  bIterator = DeserializeSequence (&rrCfgCommOptions,true,bIterator);

  // rach-ConfigCommon
  if (rrCfgCommOptions[8])
    {
      // ...
    }

  // prach-Config
  std::bitset<1> prachConfigInfoPresent;
  bIterator = DeserializeSequence (&prachConfigInfoPresent,false,bIterator);

  // prach-Config -> rootSequenceIndex
  bIterator = DeserializeInteger (&n,0,1023,bIterator);

  // prach-Config -> prach-ConfigInfo
  if (prachConfigInfoPresent[0])
    {
      // ...
    }

  // pdsch-ConfigCommon
  if (rrCfgCommOptions[7])
    {
      // ...
    }

  // pusch-ConfigCommon
  bIterator = DeserializeSequence (&bitset0,false,bIterator);

  // pusch-ConfigCommon -> pusch-ConfigBasic
  bIterator = DeserializeSequence (&bitset0,false,bIterator);

  // pusch-ConfigCommon -> pusch-ConfigBasic -> n-SB
  bIterator = DeserializeInteger (&n,1,4,bIterator);

  // pusch-ConfigCommon -> pusch-ConfigBasic -> hoppingMode
  bIterator = DeserializeEnum (2,&n,bIterator);

  // pusch-ConfigCommon -> pusch-ConfigBasic -> pusch-HoppingOffset
  bIterator = DeserializeInteger (&n,0,98,bIterator);

  // pusch-ConfigCommon -> pusch-ConfigBasic -> enable64QAM
  bool enable64QAM;
  bIterator = DeserializeBoolean (&enable64QAM,bIterator);

  // ul-ReferenceSignalsPUSCH
  bIterator = DeserializeSequence (&bitset0,false,bIterator);

  // groupHoppingEnabled
  bool dummyBool;
  bIterator = DeserializeBoolean (&dummyBool,bIterator);

  // groupAssignmentPUSCH
  bIterator = DeserializeInteger (&n,0,29,bIterator);

  // sequenceHoppingEnabled
  bIterator = DeserializeBoolean (&dummyBool,bIterator);

  // cyclicShift
  bIterator = DeserializeInteger (&n,0,7,bIterator);

  // phich-Config
  if (rrCfgCommOptions[6])
    {
      // ...
    }

  // pucch-ConfigCommon
  if (rrCfgCommOptions[5])
    {
      // ...
    }

  // soundingRS-UL-ConfigCommon
  if (rrCfgCommOptions[4])
    {
      // ...
    }

  // uplinkPowerControlCommon
  if (rrCfgCommOptions[3])
    {
      // ...
    }

  // antennaInfoCommon
  if (rrCfgCommOptions[2])
    {
      // ...
    }

  // p-Max
  if (rrCfgCommOptions[1])
    {
      // ...
    }

  // tdd-Config
  if (rrCfgCommOptions[0])
    {
      // ...
    }

  // ul-CyclicPrefixLength
  bIterator = DeserializeEnum (2,&n,bIterator);

  return bIterator;
}

Buffer::Iterator
RrcAsn1Header::DeserializeRadioResourceConfigCommonSib (Buffer::Iterator bIterator)
{
  std::bitset<0> bitset0;
  int n;

  bIterator = DeserializeSequence (&bitset0,true,bIterator);
  // rach-ConfigCommon
  bIterator = DeserializeSequence (&bitset0,true,bIterator);
  // preambleInfo
  std::bitset<1> preamblesGroupAConfigPresent;
  bIterator = DeserializeSequence (&preamblesGroupAConfigPresent,false,bIterator);
  bIterator = DeserializeEnum (16,&n,bIterator); // numberOfRA-Preambles

  if (preamblesGroupAConfigPresent[0])
    {
      // Deserialize preamblesGroupAConfig
      // ...
    }
  // powerRampingParameters
  bIterator = DeserializeSequence (&bitset0,false,bIterator);
  bIterator = DeserializeEnum (4,&n,bIterator); // powerRampingStep
  bIterator = DeserializeEnum (16,&n,bIterator); // preambleInitialReceivedTargetPower
  // ra-SupervisionInfo
  bIterator = DeserializeSequence (&bitset0,false,bIterator);
  bIterator = DeserializeEnum (11,&n,bIterator); // preambleTransMax
  bIterator = DeserializeEnum (8,&n,bIterator); // ra-ResponseWindowSize
  bIterator = DeserializeEnum (8,&n,bIterator); // mac-ContentionResolutionTimer

  bIterator = DeserializeInteger (&n,1,8,bIterator); //maxHARQ-Msg3Tx

  // bcch-Config 
  bIterator = DeserializeSequence (&bitset0,false,bIterator);
  bIterator = DeserializeEnum (4,&n,bIterator); // modificationPeriodCoeff

  // pcch-Config 
  bIterator = DeserializeSequence (&bitset0,false,bIterator);
  bIterator = DeserializeEnum (4,&n,bIterator); // defaultPagingCycle
  bIterator = DeserializeEnum (8,&n,bIterator); // nB

  // prach-Config 
  std::bitset<1> prachConfigInfoPresent;
  bIterator = DeserializeSequence (&prachConfigInfoPresent,false,bIterator);
  // prach-Config -> rootSequenceIndex
  bIterator = DeserializeInteger (&n,0,1023,bIterator);
  // prach-Config -> prach-ConfigInfo
  if (prachConfigInfoPresent[0])
    {
      // ...
    }

  // pdsch-ConfigCommon 
  bIterator = DeserializeSequence (&bitset0,false,bIterator);
  bIterator = DeserializeInteger (&n,-60,50,bIterator); // referenceSignalPower
  bIterator = DeserializeInteger (&n,0,3,bIterator); // p-b

  // pusch-ConfigCommon
  bIterator = DeserializeSequence (&bitset0,false,bIterator);

  // pusch-ConfigCommon -> pusch-ConfigBasic
  bIterator = DeserializeSequence (&bitset0,false,bIterator);

  // pusch-ConfigCommon -> pusch-ConfigBasic -> n-SB
  bIterator = DeserializeInteger (&n,1,4,bIterator);

  // pusch-ConfigCommon -> pusch-ConfigBasic -> hoppingMode
  bIterator = DeserializeEnum (2,&n,bIterator);

  // pusch-ConfigCommon -> pusch-ConfigBasic -> pusch-HoppingOffset
  bIterator = DeserializeInteger (&n,0,98,bIterator);

  // pusch-ConfigCommon -> pusch-ConfigBasic -> enable64QAM
  bool dummyBoolean;
  bIterator = DeserializeBoolean (&dummyBoolean,bIterator);

  // ul-ReferenceSignalsPUSCH 
  bIterator = DeserializeSequence (&bitset0,false,bIterator);

  // groupHoppingEnabled 
  bIterator = DeserializeBoolean (&dummyBoolean,bIterator);

  // groupAssignmentPUSCH 
  bIterator = DeserializeInteger (&n,0,29,bIterator);

  // sequenceHoppingEnabled 
  bIterator = DeserializeBoolean (&dummyBoolean,bIterator);

  // cyclicShift 
  bIterator = DeserializeInteger (&n,0,7,bIterator);

  // pucch-ConfigCommon 
  bIterator = DeserializeEnum (3,&n,bIterator); // deltaPUCCH-Shift 
  bIterator = DeserializeInteger (&n,0,98,bIterator); // nRB-CQI 
  bIterator = DeserializeInteger (&n,0,7,bIterator); // nCS-AN 
  bIterator = DeserializeInteger (&n,0,2047,bIterator); // n1PUCCH-AN 

  // soundingRS-UL-ConfigCommon
  int choice;
  bIterator = DeserializeChoice (2,&choice,bIterator);
  if (choice == 0)
    {
      bIterator = DeserializeNull (bIterator); // release
    }
  if (choice == 1)
    {
      // setup
      // ...
    }

  // uplinkPowerControlCommon 
  bIterator = DeserializeSequence (&bitset0,false,bIterator);
  bIterator = DeserializeInteger (&n,-126,24,bIterator); // p0-NominalPUSCH
  bIterator = DeserializeEnum (8,&n,bIterator); // alpha 
  bIterator = DeserializeInteger (&n,-127,-96,bIterator); // p0-NominalPUCCH 
  //deltaFList-PUCCH 
  bIterator = DeserializeSequence (&bitset0,false,bIterator);
  bIterator = DeserializeEnum (3,&n,bIterator); // deltaF-PUCCH-Format1 
  bIterator = DeserializeEnum (3,&n,bIterator); // deltaF-PUCCH-Format1b 
  bIterator = DeserializeEnum (4,&n,bIterator); // deltaF-PUCCH-Format2 
  bIterator = DeserializeEnum (3,&n,bIterator); // deltaF-PUCCH-Format2a 
  bIterator = DeserializeEnum (3,&n,bIterator); // deltaF-PUCCH-Format2b
  bIterator = DeserializeInteger (&n,-1,6,bIterator); // deltaPreambleMsg3 
 
  // ul-CyclicPrefixLength
  bIterator = DeserializeEnum (2,&n,bIterator);

  return bIterator;
}

Buffer::Iterator
RrcAsn1Header::DeserializeMeasResults (LteRrcSap::MeasResults *measResults, Buffer::Iterator bIterator)
{
  int n;
  std::bitset<0> b0;
  std::bitset<1> measResultNeighCellsPresent;
  bIterator = DeserializeSequence(&measResultNeighCellsPresent,true,bIterator);

  // Deserialize measId
  bIterator = DeserializeInteger (&n, 1, MAX_MEAS_ID, bIterator);
  measResults->measId = n;

  // Deserialize measResultServCell
  bIterator = DeserializeSequence(&b0,false,bIterator);

  // Deserialize rsrpResult
  bIterator = DeserializeInteger (&n, 0, 97, bIterator);
  measResults->rsrpResult = n;

  // Deserialize rsrqResult
  bIterator = DeserializeInteger (&n, 0, 34, bIterator);
  measResults->rsrqResult = n;

  measResults->haveMeasResultNeighCells = measResultNeighCellsPresent[0];
  if( measResults->haveMeasResultNeighCells)
  {
    int measResultNeighCellsChoice;

    // Deserialize measResultNeighCells
    bIterator = DeserializeChoice (4,&measResultNeighCellsChoice,bIterator);

    if(measResultNeighCellsChoice == 0)
    {
      // Deserialize measResultListEUTRA
      int numElems;
      bIterator = DeserializeSequenceOf (&numElems,MAX_CELL_REPORT,1,bIterator);
      
      for(int i =0; i<numElems; i++)
      {
        LteRrcSap::MeasResultEutra measResultEutra;
        
        std::bitset<1> isCgiInfoPresent;
        bIterator = DeserializeSequence(&isCgiInfoPresent,false,bIterator);
        
        // PhysCellId
        bIterator = DeserializeInteger(&n,0,503,bIterator);
        measResultEutra.physCellId = n;
        
        measResultEutra.haveCgiInfo = isCgiInfoPresent[0];
        if(isCgiInfoPresent[0])
        {
          std::bitset<1> havePlmnIdentityList;
          bIterator = DeserializeSequence(&havePlmnIdentityList,false,bIterator);
          
          // Deserialize cellGlobalId
          bIterator = DeserializeSequence (&b0,false,bIterator);
          
          // Deserialize plmn-Identity
          bIterator = DeserializePlmnIdentity(&measResultEutra.cgiInfo.plmnIdentity,bIterator);
          
          // Deserialize CellIdentity
          std::bitset<28> cellId;
          bIterator = DeserializeBitstring(&cellId,bIterator);
          measResultEutra.cgiInfo.cellIdentity = cellId.to_ulong();
          
          // Deserialize trackingAreaCode
          std::bitset<16> trArCo;
          bIterator = DeserializeBitstring(&trArCo,bIterator);
          measResultEutra.cgiInfo.trackingAreaCode = trArCo.to_ulong();
          
          // Deserialize plmn-IdentityList
          if(havePlmnIdentityList[0])
          {
            int numPlmnElems;
            bIterator = DeserializeSequenceOf (&numPlmnElems, 5, 1, bIterator);
            
            for( int j=0; j<numPlmnElems; j++)
            {
              uint32_t plmnId;
              bIterator = DeserializePlmnIdentity(&plmnId,bIterator);
              measResultEutra.cgiInfo.plmnIdentityList.push_back(plmnId);
            }
          }
        }
        
        // Deserialize measResult
        std::bitset<2> measResultOpts;
        bIterator = DeserializeSequence(&measResultOpts, true, bIterator);
        
        measResultEutra.haveRsrpResult = measResultOpts[1];
        if(measResultOpts[1])
        {
          // Deserialize rsrpResult
          bIterator = DeserializeInteger(&n,0,97,bIterator);
          measResultEutra.rsrpResult = n;
        }
        
        measResultEutra.haveRsrqResult = measResultOpts[0];
        if(measResultOpts[0])
        {
          // Deserialize rsrqResult
          bIterator = DeserializeInteger(&n,0,34,bIterator);
          measResultEutra.rsrqResult = n;
        }

        measResults->measResultListEutra.push_back(measResultEutra);
      }
    }
    
    if(measResultNeighCellsChoice == 1)
    {    
      // Deserialize measResultListUTRA
      // ...
    }
    
    if(measResultNeighCellsChoice == 2)
    {
      // Deserialize measResultListGERAN
      // ...
    }
    if(measResultNeighCellsChoice == 3)
    {
      // Deserialize measResultsCDMA2000
      // ...
    }
  }

  return bIterator;
}

Buffer::Iterator
RrcAsn1Header::DeserializePlmnIdentity (uint32_t *plmnId, Buffer::Iterator bIterator)
{
  int n;
  std::bitset<1> isMccPresent;
  bIterator = DeserializeSequence (&isMccPresent,false,bIterator);
  
  if (isMccPresent[0])
    {
      // Deserialize mcc
      // ...
    }

  // Deserialize mnc
  int mncDigits;
  int mnc = 0;
  bIterator = DeserializeSequenceOf (&mncDigits,3,2,bIterator);

  for (int j = mncDigits - 1; j >= 0; j--)
    {
      bIterator = DeserializeInteger (&n,0,9,bIterator);
      mnc += n * pow (10,j);
    }

  *plmnId = mnc;

  // cellReservedForOperatorUse
  bIterator = DeserializeEnum (2,&n,bIterator);
  return bIterator;
}
  
//////////////////// RrcConnectionRequest class ////////////////////////

// Constructor
RrcConnectionRequestHeader::RrcConnectionRequestHeader ()
{
  m_mmec = std::bitset<8> (0ul);
  m_mTmsi = std::bitset<32> (0ul);
  m_establishmentCause = MO_SIGNALLING;
  m_spare = std::bitset<1> (0ul);
}

void
RrcConnectionRequestHeader::Print (std::ostream &os) const
{
  os << "MMEC:" << m_mmec << std::endl;
  os << "MTMSI:" << m_mTmsi << std::endl;
  os << "EstablishmentCause:" << m_establishmentCause << std::endl;
  os << "Spare: " << m_spare << std::endl;
}

void
RrcConnectionRequestHeader::PreSerialize () const
{
  m_serializationResult = Buffer ();

  SerializeUlCcchMessage (1);

  // Serialize RRCConnectionRequest sequence:
  // no default or optional fields. Extension marker not present.
  SerializeSequence<0> (std::bitset<0> (),false);

  // Serialize criticalExtensions choice:
  // 2 options, selected: 0 (option: rrcConnectionRequest-r8)
  SerializeChoice (2,0);

  // Serialize RRCConnectionRequest-r8-IEs sequence:
  // no default or optional fields. Extension marker not present.
  SerializeSequence<0> (std::bitset<0> (),false);

  // Serialize InitialUE-Identity choice:
  // 2 options, selected: 0 (option: s-TMSI)
  SerializeChoice (2,0);

  // Serialize S-TMSI sequence:
  // no default or optional fields. Extension marker not present.
  SerializeSequence<0> (std::bitset<0> (),false);

  // Serialize mmec : MMEC ::= BIT STRING (SIZE (8))
  SerializeBitstring<8> (m_mmec);

  // Serialize m-TMSI ::= BIT STRING (SIZE (32))
  SerializeBitstring<32> (m_mTmsi);

  // Serialize establishmentCause : EstablishmentCause ::= ENUMERATED
  SerializeEnum (8,m_establishmentCause);

  // Serialize spare : BIT STRING (SIZE (1))
  SerializeBitstring<1> (std::bitset<1> ());

  // Finish serialization
  FinalizeSerialization ();
}

uint32_t
RrcConnectionRequestHeader::Deserialize (Buffer::Iterator bIterator)
{
  std::bitset<1> dummy;
  std::bitset<0> optionalOrDefaultMask;
  int selectedOption;

  bIterator = DeserializeUlCcchMessage (bIterator);

  // Deserialize RCConnectionRequest sequence
  bIterator = DeserializeSequence (&optionalOrDefaultMask,false,bIterator);

  // Deserialize criticalExtensions choice:
  bIterator = DeserializeChoice (2,&selectedOption,bIterator);

  // Deserialize RRCConnectionRequest-r8-IEs sequence
  bIterator = DeserializeSequence (&optionalOrDefaultMask,false,bIterator);

  // Deserialize InitialUE-Identity choice
  bIterator = DeserializeChoice (2,&selectedOption,bIterator);

  // Deserialize S-TMSI sequence
  bIterator = DeserializeSequence (&optionalOrDefaultMask,false,bIterator);

  // Deserialize mmec
  bIterator = DeserializeBitstring (&m_mmec,bIterator);

  // Deserialize m-TMSI
  bIterator = DeserializeBitstring (&m_mTmsi,bIterator);

  // Deserialize establishmentCause
  bIterator = DeserializeEnum (8,&selectedOption,bIterator);

  // Deserialize spare
  bIterator = DeserializeBitstring (&dummy,bIterator);

  return GetSerializedSize ();
}

void
RrcConnectionRequestHeader::SetMessage (RrcConnectionRequest msg)
{
  m_mTmsi = std::bitset<32> ((uint32_t)msg.ueIdentity);
  m_mmec = std::bitset<8> ((uint32_t)(msg.ueIdentity >> 32));
  m_isDataSerialized = false;
}

LteRrcSap::RrcConnectionRequest
RrcConnectionRequestHeader::GetMessage () const
{
  RrcConnectionRequest msg;
  msg.ueIdentity = (((uint64_t) m_mmec.to_ulong ()) << 32) | (m_mTmsi.to_ulong ());

  return msg;
}

std::bitset<8>
RrcConnectionRequestHeader::getMmec () const
{
  return m_mmec;
}

std::bitset<32>
RrcConnectionRequestHeader::getMtmsi () const
{
  return m_mTmsi;
}


//////////////////// RrcConnectionSetup class ////////////////////////
RrcConnectionSetupHeader::RrcConnectionSetupHeader ()
{
}
 
void
RrcConnectionSetupHeader::Print (std::ostream &os) const
{
  os << "rrcTransactionIdentifier: " << (int)rrcTransactionIdentifier << std::endl;
  os << "radioResourceConfigDedicated:" << std::endl;
  RrcAsn1Header::Print (os,radioResourceConfigDedicated);
}

void
RrcConnectionSetupHeader::PreSerialize () const
{
  m_serializationResult = Buffer ();

  SerializeDlCcchMessage (3);

  SerializeInteger (15,0,15);

  // Serialize RRCConnectionSetup sequence:
  // no default or optional fields. Extension marker not present.
  SerializeSequence (std::bitset<0> (),false);

  // Serialize rrc-TransactionIdentifier ::=INTEGER (0..3)
  SerializeInteger (rrcTransactionIdentifier,0,3);

  // Serialize criticalExtensions choice:
  // 2 options, selected: 0 (option: c1)
  SerializeChoice (2,0);

  // Serialize c1 choice:
  // 8 options, selected: 0 (option: rrcConnectionSetup-r8)
  SerializeChoice (8,0);

  // Serialize rrcConnectionSetup-r8 sequence
  // 1 optional fields (not present). Extension marker not present.
  SerializeSequence (std::bitset<1> (0),false);

  // Serialize RadioResourceConfigDedicated sequence
  SerializeRadioResourceConfigDedicated (radioResourceConfigDedicated);

  // Serialize nonCriticalExtension sequence
  // 2 optional fields, none present. No extension marker.
  SerializeSequence (std::bitset<2> (0),false);

  // Finish serialization
  FinalizeSerialization ();
}


uint32_t
RrcConnectionSetupHeader::Deserialize (Buffer::Iterator bIterator)
{
  int n;

  std::bitset<0> bitset0;
  std::bitset<1> bitset1;
  std::bitset<2> bitset2;

  bIterator = DeserializeDlCcchMessage (bIterator);

  bIterator = DeserializeInteger (&n,0,15,bIterator);

  // Deserialize RRCConnectionSetup sequence
  bIterator = DeserializeSequence (&bitset0,false,bIterator);

  // Deserialize rrc-TransactionIdentifier ::=INTEGER (0..3)
  bIterator = DeserializeInteger (&n,0,3,bIterator);
  rrcTransactionIdentifier = n;

  // Deserialize criticalExtensions choice
  int criticalExtensionChoice;
  bIterator = DeserializeChoice (2,&criticalExtensionChoice,bIterator);
  if (criticalExtensionChoice == 1)
    {
      // Deserialize criticalExtensionsFuture
      bIterator = DeserializeSequence (&bitset0,false,bIterator);
    }
  else if (criticalExtensionChoice == 0)
    {
      // Deserialize c1
      int c1;
      bIterator = DeserializeChoice (8,&c1,bIterator);

      if (c1 > 0)
        {
          // Deserialize spareX , X:=7..1
          bIterator = DeserializeNull (bIterator);
        }
      else if (c1 == 0)
        {
          // Deserialize rrcConnectionSetup-r8
          // 1 optional fields, no extension marker.
          bIterator = DeserializeSequence (&bitset1,false,bIterator);

          // Deserialize radioResourceConfigDedicated
          bIterator = DeserializeRadioResourceConfigDedicated (&radioResourceConfigDedicated,bIterator);

          if (bitset1[0])
            {
              // Deserialize nonCriticalExtension
              // 2 optional fields, no extension marker.
              bIterator = DeserializeSequence (&bitset2,false,bIterator);

              // Deserialization of lateR8NonCriticalExtension and nonCriticalExtension
              // ...
            }
        }
    }
  return GetSerializedSize ();
}

void
RrcConnectionSetupHeader::SetMessage (RrcConnectionSetup msg)
{
  rrcTransactionIdentifier = msg.rrcTransactionIdentifier;
  radioResourceConfigDedicated = msg.radioResourceConfigDedicated;
  m_isDataSerialized = false;
}

LteRrcSap::RrcConnectionSetup
RrcConnectionSetupHeader::GetMessage () const
{
  RrcConnectionSetup msg;
  msg.rrcTransactionIdentifier = rrcTransactionIdentifier;
  msg.radioResourceConfigDedicated = radioResourceConfigDedicated; 
  return msg;
}

uint8_t
RrcConnectionSetupHeader::GetRrcTransactionIdentifier () const
{
  return rrcTransactionIdentifier;
}

bool
RrcConnectionSetupHeader::HavePhysicalConfigDedicated () const
{
  return radioResourceConfigDedicated.havePhysicalConfigDedicated;
}

std::list<LteRrcSap::SrbToAddMod>
RrcConnectionSetupHeader::GetSrbToAddModList () const
{
  return radioResourceConfigDedicated.srbToAddModList;
}

std::list<LteRrcSap::DrbToAddMod>
RrcConnectionSetupHeader::GetDrbToAddModList () const
{
  return radioResourceConfigDedicated.drbToAddModList;
} 

std::list<uint8_t>
RrcConnectionSetupHeader::GetDrbToReleaseList () const
{
  return radioResourceConfigDedicated.drbToReleaseList;
}

LteRrcSap::PhysicalConfigDedicated
RrcConnectionSetupHeader::GetPhysicalConfigDedicated () const
{
  return radioResourceConfigDedicated.physicalConfigDedicated;
}

LteRrcSap::RadioResourceConfigDedicated
RrcConnectionSetupHeader::GetRadioResourceConfigDedicated () const
{
  return radioResourceConfigDedicated;
}

//////////////////// RrcConnectionSetupCompleteHeader class ////////////////////////

RrcConnectionSetupCompleteHeader::RrcConnectionSetupCompleteHeader ()
{
}

void
RrcConnectionSetupCompleteHeader::PreSerialize () const
{
  m_serializationResult = Buffer ();

  // Serialize DCCH message
  SerializeUlDcchMessage (4);

  // Serialize RRCConnectionSetupComplete sequence:
  // no default or optional fields. Extension marker not present.
  SerializeSequence<0> (std::bitset<0> (),false);

  // Serialize rrc-TransactionIdentifier
  SerializeInteger (m_rrcTransactionIdentifier,0,3);

  // Serialize criticalExtensions choice
  // 2 options, selected 0 (c1)
  SerializeChoice (2,0);

  // Choose spare3 NULL
  SerializeChoice (4,1);

  // Serialize spare3 NULL
  SerializeNull ();

  // Finish serialization
  FinalizeSerialization ();
}

uint32_t
RrcConnectionSetupCompleteHeader::Deserialize (Buffer::Iterator bIterator)
{
  std::bitset<0> bitset0;

  bIterator = DeserializeUlDcchMessage (bIterator);

  bIterator = DeserializeSequence (&bitset0,false,bIterator);

  int n;
  bIterator = DeserializeInteger (&n,0,3,bIterator);
  m_rrcTransactionIdentifier = n;

  bIterator = DeserializeChoice (2,&n,bIterator);

  if (n == 1)
    {
      // Deserialize criticalExtensionsFuture
      bIterator = DeserializeSequence (&bitset0,false,bIterator);
    }
  else if (n == 0)
    {
      // Deserialize c1
      int c1Chosen;
      bIterator = DeserializeChoice (4,&c1Chosen,bIterator);

      if (c1Chosen == 0)
        {
          // Deserialize rrcConnectionSetupComplete-r8
          // ...
        }
      else
        {
          bIterator = DeserializeNull (bIterator);
        }
    }

  return GetSerializedSize ();
}

void
RrcConnectionSetupCompleteHeader::Print (std::ostream &os) const
{
  os << "rrcTransactionIdentifier: " << (int) m_rrcTransactionIdentifier << std::endl;
}

void
RrcConnectionSetupCompleteHeader::SetMessage (RrcConnectionSetupCompleted msg)
{
  m_rrcTransactionIdentifier = msg.rrcTransactionIdentifier;
  m_isDataSerialized = false;
}

uint8_t
RrcConnectionSetupCompleteHeader::GetRrcTransactionIdentifier () const
{
  return m_rrcTransactionIdentifier;
}

LteRrcSap::RrcConnectionSetupCompleted
RrcConnectionSetupCompleteHeader::GetMessage () const
{
  LteRrcSap::RrcConnectionSetupCompleted msg;
  msg.rrcTransactionIdentifier = m_rrcTransactionIdentifier;
  return msg;
}

//////////////////// RrcConnectionReconfigurationCompleteHeader class ////////////////////////

RrcConnectionReconfigurationCompleteHeader::RrcConnectionReconfigurationCompleteHeader ()
{
}

void
RrcConnectionReconfigurationCompleteHeader::PreSerialize () const
{
  m_serializationResult = Buffer ();

  // Serialize DCCH message
  SerializeUlDcchMessage (2);

  // Serialize RRCConnectionSetupComplete sequence:
  // no default or optional fields. Extension marker not present.
  SerializeSequence<0> (std::bitset<0> (),false);

  // Serialize rrc-TransactionIdentifier
  SerializeInteger (m_rrcTransactionIdentifier,0,3);

  // Serialize criticalExtensions choice
  // 2 options, selected 1 (criticalExtensionsFuture)
  SerializeChoice (2,1);

  // Choose criticalExtensionsFuture
  SerializeSequence<0> (std::bitset<0> (),false);

  // Finish serialization
  FinalizeSerialization ();
}

uint32_t
RrcConnectionReconfigurationCompleteHeader::Deserialize (Buffer::Iterator bIterator)
{
  std::bitset<0> bitset0;
  int n;

  bIterator = DeserializeUlDcchMessage (bIterator);
  bIterator = DeserializeSequence (&bitset0,false,bIterator);

  bIterator = DeserializeInteger (&n,0,3,bIterator);
  m_rrcTransactionIdentifier = n;

  bIterator = DeserializeChoice (2,&n,bIterator);

  if (n == 1)
    {
      // Deserialize criticalExtensionsFuture
      bIterator = DeserializeSequence (&bitset0,false,bIterator);
    }
  else if (n == 0)
    {
      // Deserialize rrcConnectionReconfigurationComplete-r8
      // ...
    }

  return GetSerializedSize ();
}

void
RrcConnectionReconfigurationCompleteHeader::Print (std::ostream &os) const
{
  os << "rrcTransactionIdentifier: " << (int) m_rrcTransactionIdentifier << std::endl;
}

void
RrcConnectionReconfigurationCompleteHeader::SetMessage (RrcConnectionReconfigurationCompleted msg)
{
  m_rrcTransactionIdentifier = msg.rrcTransactionIdentifier;
  m_isDataSerialized = false;
}

LteRrcSap::RrcConnectionReconfigurationCompleted
RrcConnectionReconfigurationCompleteHeader::GetMessage () const
{
  RrcConnectionReconfigurationCompleted msg;
  msg.rrcTransactionIdentifier = m_rrcTransactionIdentifier;
  return msg;
}

uint8_t
RrcConnectionReconfigurationCompleteHeader::GetRrcTransactionIdentifier () const
{
  return m_rrcTransactionIdentifier;
}

//////////////////// RrcConnectionReconfigurationHeader class ////////////////////////

RrcConnectionReconfigurationHeader::RrcConnectionReconfigurationHeader ()
{
}

void
RrcConnectionReconfigurationHeader::PreSerialize () const
{
  m_serializationResult = Buffer ();

  SerializeDlDcchMessage (4);

  // Serialize RRCConnectionSetupComplete sequence:
  // no default or optional fields. Extension marker not present.
  SerializeSequence<0> (std::bitset<0> (),false);

  // Serialize rrc-TransactionIdentifier
  SerializeInteger (m_rrcTransactionIdentifier,0,3);

  // Serialize criticalExtensions choice
  // 2 options, selected 0 (c1)
  SerializeChoice (2,0);

  // Serialize c1 choice
  // 8 options, selected 0 (rrcConnectionReconfiguration-r8)
  SerializeChoice (8,0);

  // Serialize RRCConnectionReconfiguration-r8-IEs sequence:
  // 6 optional fields. Extension marker not present.
  std::bitset<6> options;
  options.set (5,m_haveMeasConfig);
  options.set (4,m_haveMobilityControlInfo);
  options.set (3,0); // No dedicatedInfoNASList
  options.set (2,m_haveRadioResourceConfigDedicated);
  options.set (1,0); // No securityConfigHO
  options.set (0,0); // No nonCriticalExtension
  SerializeSequence<6> (options,false);


  if (m_haveMeasConfig)
    {
      // Serialize MeasConfig sequence
      // 11 optional fields, extension marker present
      std::bitset<11> measConfigOptional (0);
      SerializeSequence (measConfigOptional,true);
    }

  if (m_haveMobilityControlInfo)
    {
      // Serialize MobilityControlInfo

      // 4 optional fields, extension marker present.
      std::bitset<4> mobCtrlIntoOptional;
      mobCtrlIntoOptional.set (3,m_mobilityControlInfo.haveCarrierFreq);
      mobCtrlIntoOptional.set (2,m_mobilityControlInfo.haveCarrierBandwidth);
      mobCtrlIntoOptional.set (1,0); // No additionalSpectrumEmission
      mobCtrlIntoOptional.set (0,m_mobilityControlInfo.haveRachConfigDedicated);
      SerializeSequence (mobCtrlIntoOptional,true);

      // Serialize targetPhysCellId
      SerializeInteger (m_mobilityControlInfo.targetPhysCellId,0,503);

      if (m_mobilityControlInfo.haveCarrierFreq)
        {
          SerializeSequence (std::bitset<1> (1),false);
          SerializeInteger (m_mobilityControlInfo.carrierFreq.dlCarrierFreq,0,MAX_EARFCN);
          SerializeInteger (m_mobilityControlInfo.carrierFreq.ulCarrierFreq,0,MAX_EARFCN);
        }

      if (m_mobilityControlInfo.haveCarrierBandwidth)
        {
          SerializeSequence (std::bitset<1> (1),false);

          // Serialize dl-Bandwidth
          switch (m_mobilityControlInfo.carrierBandwidth.dlBandwidth)
            {
            case 6:
              SerializeEnum (16,0);
              break;
            case 15:
              SerializeEnum (16,1);
              break;
            case 25:
              SerializeEnum (16,2);
              break;
            case 50:
              SerializeEnum (16,3);
              break;
            case 75:
              SerializeEnum (16,4);
              break;
            case 100:
              SerializeEnum (16,5);
              break;
            default:
              SerializeEnum (16,6);
            }

          // Serialize ul-Bandwidth
          switch (m_mobilityControlInfo.carrierBandwidth.ulBandwidth)
            {
            case 6:
              SerializeEnum (16,0);
              break;
            case 15:
              SerializeEnum (16,1);
              break;
            case 25:
              SerializeEnum (16,2);
              break;
            case 50:
              SerializeEnum (16,3);
              break;
            case 75:
              SerializeEnum (16,4);
              break;
            case 100:
              SerializeEnum (16,5);
              break;
            default:
              SerializeEnum (16,6);
            }
        }

      // Serialize t304
      SerializeEnum (8,0);

      // Serialize newUE-Identitiy
      SerializeBitstring (std::bitset<16> (m_mobilityControlInfo.newUeIdentity));

      // Serialize radioResourceConfigCommon
      // 9 optional fields, none present. Extension marker yes.
      SerializeSequence (std::bitset<9> (0),true);

      // Serialize PRACH-Config
      // 1 optional, 0 extension marker.
      SerializeSequence (std::bitset<1> (0),false);

      // Serialize PRACH-Config rootSequenceIndex
      SerializeInteger (0,0,1023);

      // Serialize PUSCH-ConfigCommon
      SerializeSequence (std::bitset<0> (),false);

      // Serialize pusch-ConfigBasic
      SerializeSequence (std::bitset<0> (),false);
      SerializeInteger (1,1,4);
      SerializeEnum (2,0);
      SerializeInteger (0,0,98);
      SerializeBoolean (false);

      // Serialize UL-ReferenceSignalsPUSCH
      SerializeSequence (std::bitset<0> (),false);
      SerializeBoolean (false);
      SerializeInteger (0,0,29);
      SerializeBoolean (false);
      SerializeInteger (4,0,7);

      // Serialize UL-CyclicPrefixLength
      SerializeEnum (2,0);

      if (m_mobilityControlInfo.haveRachConfigDedicated)
        {
          SerializeSequence (std::bitset<0> (),false);
          SerializeInteger (m_mobilityControlInfo.rachConfigDedicated.raPreambleIndex,0,63);
          SerializeInteger (m_mobilityControlInfo.rachConfigDedicated.raPrachMaskIndex,0,15);
        }
    }

  if (m_haveRadioResourceConfigDedicated)
    {
      // Serialize RadioResourceConfigDedicated
      SerializeRadioResourceConfigDedicated (m_radioResourceConfigDedicated);
    }

  // Finish serialization
  FinalizeSerialization ();
}

uint32_t
RrcConnectionReconfigurationHeader::Deserialize (Buffer::Iterator bIterator)
{
  std::bitset<0> bitset0;

  bIterator = DeserializeDlDcchMessage (bIterator);

  // RRCConnectionReconfiguration sequence
  bIterator = DeserializeSequence (&bitset0,false,bIterator);

  // rrc-TransactionIdentifier
  int n;
  bIterator = DeserializeInteger (&n,0,3,bIterator);
  m_rrcTransactionIdentifier = n;

  // criticalExtensions
  int sel;
  bIterator = DeserializeChoice (2,&sel,bIterator);
  if (sel == 1)
    {
      // criticalExtensionsFuture
      bIterator = DeserializeSequence (&bitset0,false,bIterator);
    }
  else if (sel == 0)
    {
      // c1
      int c1Chosen;
      bIterator = DeserializeChoice (8,&c1Chosen,bIterator);
      if (c1Chosen > 0)
        {
          bIterator = DeserializeNull (bIterator);
        }
      else if (c1Chosen == 0)
        {
          // rrcConnectionReconfiguration-r8
          std::bitset<6> rrcConnRecOpts;
          bIterator = DeserializeSequence (&rrcConnRecOpts,false,bIterator);

          m_haveMeasConfig = rrcConnRecOpts[5];
          if (m_haveMeasConfig)
            {
              std::bitset<11> bitset11;

              // measConfig
              bIterator = DeserializeSequence (&bitset11,true,bIterator);

              if (bitset11[10])
                {
                  // measObjectToRemoveList
                }
              if (bitset11[9])
                {
                  // measObjectToAddModList
                }
              if (bitset11[8])
                {
                  // reportConfigToRemoveList
                }
              if (bitset11[7])
                {
                  // reportConfigToAddModList
                }
              if (bitset11[6])
                {
                  // measIdToRemoveList
                }
              if (bitset11[5])
                {
                  // measIdToAddModList
                }
              if (bitset11[4])
                {
                  // quantityConfig
                }
              if (bitset11[3])
                {
                  // measGapConfig
                }
              if (bitset11[2])
                {
                  // s-Measure
                }
              if (bitset11[1])
                {
                  // preRegistrationInfoHRPD
                }
              if (bitset11[0])
                {
                  // speedStatePars
                }
            }

          m_haveMobilityControlInfo = rrcConnRecOpts[4];
          if (m_haveMobilityControlInfo)
            {
              // mobilityControlInfo
              std::bitset<4> mobCtrlOpts;
              bIterator = DeserializeSequence (&mobCtrlOpts,true,bIterator);

              // PhysCellId
              bIterator = DeserializeInteger (&n,0,503,bIterator);
              m_mobilityControlInfo.targetPhysCellId = n;

              // carrierFreq
              m_mobilityControlInfo.haveCarrierFreq = mobCtrlOpts[3];
              if (m_mobilityControlInfo.haveCarrierFreq)
                {
                  std::bitset<1> ulCarrierFreqPresent;
                  bIterator = DeserializeSequence (&ulCarrierFreqPresent,false,bIterator);

                  bIterator = DeserializeInteger (&n,0,MAX_EARFCN,bIterator);
                  m_mobilityControlInfo.carrierFreq.dlCarrierFreq = n;

                  if (ulCarrierFreqPresent[0])
                    {
                      bIterator = DeserializeInteger (&n,0,MAX_EARFCN,bIterator);
                      m_mobilityControlInfo.carrierFreq.ulCarrierFreq = n;
                    }
                }

              // carrierBandwidth
              m_mobilityControlInfo.haveCarrierBandwidth = mobCtrlOpts[2];
              if (m_mobilityControlInfo.haveCarrierBandwidth)
                {
                  std::bitset<1> ulBandwidthPresent;
                  bIterator = DeserializeSequence (&ulBandwidthPresent,false,bIterator);

                  bIterator = DeserializeEnum (16,&n,bIterator);
                  switch (n)
                    {
                    case 0:
                      m_mobilityControlInfo.carrierBandwidth.dlBandwidth = 6;
                      break;
                    case 1:
                      m_mobilityControlInfo.carrierBandwidth.dlBandwidth = 15;
                      break;
                    case 2:
                      m_mobilityControlInfo.carrierBandwidth.dlBandwidth = 25;
                      break;
                    case 3:
                      m_mobilityControlInfo.carrierBandwidth.dlBandwidth = 50;
                      break;
                    case 4:
                      m_mobilityControlInfo.carrierBandwidth.dlBandwidth = 75;
                      break;
                    case 5:
                      m_mobilityControlInfo.carrierBandwidth.dlBandwidth = 100;
                      break;
                    case 6:
                      m_mobilityControlInfo.carrierBandwidth.dlBandwidth = 0;
                      break;
                    }

                  if (ulBandwidthPresent[0])
                    {
                      bIterator = DeserializeEnum (16,&n,bIterator);
                      switch (n)
                        {
                        case 0:
                          m_mobilityControlInfo.carrierBandwidth.ulBandwidth = 6;
                          break;
                        case 1:
                          m_mobilityControlInfo.carrierBandwidth.ulBandwidth = 15;
                          break;
                        case 2:
                          m_mobilityControlInfo.carrierBandwidth.ulBandwidth = 25;
                          break;
                        case 3:
                          m_mobilityControlInfo.carrierBandwidth.ulBandwidth = 50;
                          break;
                        case 4:
                          m_mobilityControlInfo.carrierBandwidth.ulBandwidth = 75;
                          break;
                        case 5:
                          m_mobilityControlInfo.carrierBandwidth.ulBandwidth = 100;
                          break;
                        case 6:
                          m_mobilityControlInfo.carrierBandwidth.ulBandwidth = 0;
                          break;
                        }
                    }
                }

              // additionalSpectrumEmission
              if (mobCtrlOpts[1])
                {
                  // ...
                }

              // t304
              bIterator = DeserializeEnum (8,&n,bIterator);

              // newUE-Identity
              std::bitset<16> cRnti;
              bIterator = DeserializeBitstring (&cRnti, bIterator);
              m_mobilityControlInfo.newUeIdentity = cRnti.to_ulong ();

              // radioResourceConfigCommon
              bIterator = DeserializeRadioResourceConfigCommon (bIterator);

              m_mobilityControlInfo.haveRachConfigDedicated = mobCtrlOpts[0];
              if (m_mobilityControlInfo.haveRachConfigDedicated)
                {
                  bIterator = DeserializeSequence (&bitset0, false, bIterator);
                  bIterator = DeserializeInteger (&n,0,63, bIterator);
                  m_mobilityControlInfo.rachConfigDedicated.raPreambleIndex = n;
                  bIterator = DeserializeInteger (&n,0,15, bIterator);
                  m_mobilityControlInfo.rachConfigDedicated.raPrachMaskIndex = n;
                }
            }

          // dedicatedInfoNASList
          if (rrcConnRecOpts[3])
            {
              // ...
            }

          // radioResourceConfigDedicated
          m_haveRadioResourceConfigDedicated = rrcConnRecOpts[2];
          if (m_haveRadioResourceConfigDedicated)
            {
              bIterator = DeserializeRadioResourceConfigDedicated (&m_radioResourceConfigDedicated,bIterator);
            }

          // securityConfigHO
          if (rrcConnRecOpts[1])
            {
              // ...
            }

          // nonCriticalExtension
          if (rrcConnRecOpts[0])
            {
              // ...
            }
        }
    }

  return GetSerializedSize ();
}

void
RrcConnectionReconfigurationHeader::Print (std::ostream &os) const
{
  os << "rrcTransactionIdentifier: " << (int) m_rrcTransactionIdentifier << std::endl;
  os << "haveMeasConfig: " << m_haveMeasConfig << std::endl;
  os << "haveMobilityControlInfo: " << m_haveMobilityControlInfo << std::endl;
  if (m_haveMobilityControlInfo)
    {
      os << "targetPhysCellId: " << (int)m_mobilityControlInfo.targetPhysCellId << std::endl;
      os << "haveCarrierFreq: " << m_mobilityControlInfo.haveCarrierFreq << std::endl;
      if (m_mobilityControlInfo.haveCarrierFreq)
        {
          os << "  carrierFreq.dlCarrierFreq: " << (int) m_mobilityControlInfo.carrierFreq.dlCarrierFreq << std::endl;
          os << "  carrierFreq.dlCarrierFreq: " << (int) m_mobilityControlInfo.carrierFreq.ulCarrierFreq << std::endl;
        }
      os << "haveCarrierBandwidth: " << m_mobilityControlInfo.haveCarrierBandwidth << std::endl;
      if (m_mobilityControlInfo.haveCarrierBandwidth)
        {
          os << "  carrierBandwidth.dlBandwidth: " << (int) m_mobilityControlInfo.carrierBandwidth.dlBandwidth << std::endl;
          os << "  carrierBandwidth.ulBandwidth: " << (int) m_mobilityControlInfo.carrierBandwidth.ulBandwidth << std::endl;
        }
      os << "newUeIdentity: " << (int) m_mobilityControlInfo.newUeIdentity << std::endl;
      os << "haveRachConfigDedicated: " << m_mobilityControlInfo.haveRachConfigDedicated << std::endl;
      if (m_mobilityControlInfo.haveRachConfigDedicated)
        {
          os << "raPreambleIndex: " << (int) m_mobilityControlInfo.rachConfigDedicated.raPreambleIndex << std::endl;
          os << "raPrachMaskIndex: "  << (int) m_mobilityControlInfo.rachConfigDedicated.raPrachMaskIndex << std::endl;
        }
    }
  os << "haveRadioResourceConfigDedicated: " << m_haveRadioResourceConfigDedicated << std::endl;
  if (m_haveRadioResourceConfigDedicated)
    {
      RrcAsn1Header::Print (os,m_radioResourceConfigDedicated);
    }
}

void
RrcConnectionReconfigurationHeader::SetMessage (RrcConnectionReconfiguration msg)
{
  m_rrcTransactionIdentifier = msg.rrcTransactionIdentifier;
  m_haveMeasConfig = msg.haveMeasConfig;
  m_measConfig = msg.measConfig;
  m_haveMobilityControlInfo = msg.haveMobilityControlInfo;
  m_mobilityControlInfo = msg.mobilityControlInfo;
  m_haveRadioResourceConfigDedicated = msg.haveRadioResourceConfigDedicated;
  m_radioResourceConfigDedicated = msg.radioResourceConfigDedicated;

  m_isDataSerialized = false;
}

LteRrcSap::RrcConnectionReconfiguration
RrcConnectionReconfigurationHeader::GetMessage () const
{
  RrcConnectionReconfiguration msg;

  msg.rrcTransactionIdentifier = m_rrcTransactionIdentifier;
  msg.haveMeasConfig = m_haveMeasConfig;
  msg.measConfig = m_measConfig;
  msg.haveMobilityControlInfo = m_haveMobilityControlInfo;
  msg.mobilityControlInfo = m_mobilityControlInfo;
  msg.haveRadioResourceConfigDedicated = m_haveRadioResourceConfigDedicated;
  msg.radioResourceConfigDedicated = m_radioResourceConfigDedicated;

  return msg;
}

uint8_t
RrcConnectionReconfigurationHeader::GetRrcTransactionIdentifier () const
{
  return m_rrcTransactionIdentifier;
}

bool
RrcConnectionReconfigurationHeader::GetHaveMeasConfig ()
{
  return m_haveMeasConfig;
}

LteRrcSap::MeasConfig
RrcConnectionReconfigurationHeader::GetMeasConfig ()
{
  return m_measConfig;
}

bool
RrcConnectionReconfigurationHeader::GetHaveMobilityControlInfo ()
{
  return m_haveMobilityControlInfo;
}

LteRrcSap::MobilityControlInfo
RrcConnectionReconfigurationHeader::GetMobilityControlInfo ()
{
  return m_mobilityControlInfo;
}

bool
RrcConnectionReconfigurationHeader::GetHaveRadioResourceConfigDedicated ()
{
  return m_haveRadioResourceConfigDedicated;
}

LteRrcSap::RadioResourceConfigDedicated
RrcConnectionReconfigurationHeader::GetRadioResourceConfigDedicated ()
{
  return m_radioResourceConfigDedicated;
}

bool
RrcConnectionReconfigurationHeader::HavePhysicalConfigDedicated () const
{
  return m_radioResourceConfigDedicated.havePhysicalConfigDedicated;
}

std::list<LteRrcSap::SrbToAddMod> 
RrcConnectionReconfigurationHeader::GetSrbToAddModList () const
{
  return m_radioResourceConfigDedicated.srbToAddModList;
}

std::list<LteRrcSap::DrbToAddMod> 
RrcConnectionReconfigurationHeader::GetDrbToAddModList () const
{
  return m_radioResourceConfigDedicated.drbToAddModList;
}

std::list<uint8_t> 
RrcConnectionReconfigurationHeader::GetDrbToReleaseList () const
{
  return m_radioResourceConfigDedicated.drbToReleaseList;
}

LteRrcSap::PhysicalConfigDedicated 
RrcConnectionReconfigurationHeader::GetPhysicalConfigDedicated () const
{
  return m_radioResourceConfigDedicated.physicalConfigDedicated;
}


//////////////////// HandoverPreparationInfoHeader class ////////////////////////

HandoverPreparationInfoHeader::HandoverPreparationInfoHeader ()
{
}

void
HandoverPreparationInfoHeader::PreSerialize () const
{
  m_serializationResult = Buffer ();

  // Serialize HandoverPreparationInformation sequence:
  // no default or optional fields. Extension marker not present.
  SerializeSequence<0> (std::bitset<0> (),false);

  // Serialize criticalExtensions choice
  // 2 options, selected 0 (c1)
  SerializeChoice (2,0);

  // Serialize c1 choice
  // 8 options, selected 0 (handoverPreparationInformation-r8)
  SerializeChoice (8,0);

  // Serialize HandoverPreparationInformation-r8-IEs sequence
  // 4 optional fields, no extension marker.
  std::bitset<4> handoverPrepInfoOpts;
  handoverPrepInfoOpts.set (3,1); // as-Config present
  handoverPrepInfoOpts.set (2,0); // rrm-Config not present
  handoverPrepInfoOpts.set (1,0); // as-Context not present
  handoverPrepInfoOpts.set (0,0); // nonCriticalExtension not present
  SerializeSequence (handoverPrepInfoOpts,false);

  // Serialize ue-RadioAccessCapabilityInfo
  SerializeSequenceOf (0,MAX_RAT_CAPABILITIES,0);

  // Serialize as-Config
  SerializeSequence (std::bitset<0> (),true);

  // Serialize sourceMeasConfig
  // 11 optional fields, extension marker present
  std::bitset<11> measConfigOptional (0);
  SerializeSequence (measConfigOptional,true);

  // Serialize sourceRadioResourceConfig
  SerializeRadioResourceConfigDedicated (m_asConfig.sourceRadioResourceConfig);

  // Serialize sourceSecurityAlgorithmConfig
  SerializeSequence (std::bitset<0> (),false);
  // cipheringAlgorithm
  SerializeEnum (8,0);
  // integrityProtAlgorithm
  SerializeEnum (8,0);

  // Serialize sourceUE-Identity
  SerializeBitstring (std::bitset<16> (m_asConfig.sourceUeIdentity));

  // Serialize sourceMasterInformationBlock
  SerializeSequence (std::bitset<0> (),false);
  SerializeEnum (6,m_asConfig.sourceMasterInformationBlock.dlBandwidth); // dl-Bandwidth
  SerializeSequence (std::bitset<0> (),false); // phich-Config sequence
  SerializeEnum (2,0); // phich-Duration
  SerializeEnum (4,0); // phich-Resource
  SerializeBitstring (std::bitset<8> (m_asConfig.sourceMasterInformationBlock.systemFrameNumber)); // systemFrameNumber
  SerializeBitstring (std::bitset<10> (321)); // spare

  // Serialize sourceSystemInformationBlockType1 sequence
  SerializeSystemInformationBlockType1 (m_asConfig.sourceSystemInformationBlockType1);

  // Serialize sourceSystemInformationBlockType2
  SerializeSystemInformationBlockType2 ();

  // Serialize AntennaInfoCommon 
  SerializeSequence (std::bitset<0> (0),false);
  SerializeEnum (4,0); // antennaPortsCount 

  // Serialize sourceDlCarrierFreq
  SerializeInteger (m_asConfig.sourceDlCarrierFreq,0,MAX_EARFCN);

  // Finish serialization
  FinalizeSerialization ();
}

uint32_t
HandoverPreparationInfoHeader::Deserialize (Buffer::Iterator bIterator)
{
  std::bitset<0> bitset0;
  int n;

  // Deserialize HandoverPreparationInformation sequence
  // 0 optional fields, no extension marker
  bIterator = DeserializeSequence (&bitset0,false,bIterator);

  // Deserialize criticalExtensions choice
  int criticalExtensionsChosen;
  bIterator = DeserializeChoice (2,&criticalExtensionsChosen,bIterator);

  if (criticalExtensionsChosen == 1)
    {
      // Deserialize criticalExtensionsFuture
      bIterator = DeserializeSequence (&bitset0,false,bIterator);
    }
  else if (criticalExtensionsChosen == 0)
    {
      // Deserialize c1 choice
      int c1Chosen;
      bIterator = DeserializeChoice (8,&c1Chosen,bIterator);
      if (c1Chosen > 0)
        {
          bIterator = DeserializeNull (bIterator);
        }
      else if (c1Chosen == 0)
        {
          // Deserialize handoverPreparationInformation-r8
          std::bitset<4> handoverPrepInfoOpts;
          bIterator = DeserializeSequence (&handoverPrepInfoOpts,false,bIterator);

          // Deserialize ue-RadioAccessCapabilityInfo
          bIterator = DeserializeSequenceOf (&n,MAX_RAT_CAPABILITIES,0,bIterator);
          for (int i = 0; i < n; i++)
            {
              // Deserialize UE-CapabilityRAT-Container
              // ...
            }

          if (handoverPrepInfoOpts[3])
            {
              // Deserialize as-Config sequence
              bIterator = DeserializeSequence (&bitset0,true,bIterator);

              // Deserialize sourceMeasConfig
              std::bitset<11> bitset11;
              bIterator = DeserializeSequence (&bitset11,true,bIterator);

              if (bitset11[10]) //measObjectToRemoveList
                {
                  // ...
                }
              if (bitset11[9]) //measObjectToAddModList
                {
                  // ...
                }
              if (bitset11[8]) //reportConfigToRemoveList
                {
                  // ...
                }
              if (bitset11[7]) //reportConfigToAddModList
                {
                  // ...
                }
              if (bitset11[6]) //measIdToRemoveList
                {
                  // ...
                }
              if (bitset11[5]) //measIdToAddModList
                {
                  // ...
                }
              if (bitset11[4]) //quantityConfig
                {
                  // ...
                }
              if (bitset11[3]) //measGapConfig
                {
                  // ...
                }
              if (bitset11[2]) //s-Measure
                {
                  // ...
                }
              if (bitset11[1]) //preRegistrationInfoHRPD
                {
                  // ...
                }
              if (bitset11[0]) //speedStatePars
                {
                  // ...
                }

              // Deserialize sourceRadioResourceConfig
              bIterator = DeserializeRadioResourceConfigDedicated (&m_asConfig.sourceRadioResourceConfig,bIterator);

              // Deserialize sourceSecurityAlgorithmConfig
              bIterator = DeserializeSequence (&bitset0,false,bIterator);
              bIterator = DeserializeEnum (8,&n,bIterator); // cipheringAlgorithm
              bIterator = DeserializeEnum (8,&n,bIterator); // integrityProtAlgorithm

              // Deserialize sourceUE-Identity
              std::bitset<16> cRnti;
              bIterator = DeserializeBitstring (&cRnti,bIterator);
              m_asConfig.sourceUeIdentity = cRnti.to_ulong ();

              // Deserialize sourceMasterInformationBlock
              bIterator = DeserializeSequence (&bitset0,false,bIterator);
              bIterator = DeserializeEnum (6,&n,bIterator); // dl-Bandwidth
              m_asConfig.sourceMasterInformationBlock.dlBandwidth = n;

              // phich-Config
              bIterator = DeserializeSequence (&bitset0,false,bIterator);
              bIterator = DeserializeEnum (2,&n,bIterator); // phich-Duration
              bIterator = DeserializeEnum (4,&n,bIterator); // phich-Resource

              // systemFrameNumber
              std::bitset<8> systemFrameNumber;
              bIterator = DeserializeBitstring (&systemFrameNumber,bIterator);
              m_asConfig.sourceMasterInformationBlock.systemFrameNumber = systemFrameNumber.to_ulong ();
              // spare
              std::bitset<10> spare;
              bIterator = DeserializeBitstring (&spare,bIterator);

              // Deserialize sourceSystemInformationBlockType1
              bIterator = DeserializeSystemInformationBlockType1 (&m_asConfig.sourceSystemInformationBlockType1,bIterator);

              // Deserialize sourceSystemInformationBlockType2
              bIterator = DeserializeSystemInformationBlockType2 (bIterator);

              // Deserialize antennaInfoCommon
              bIterator = DeserializeSequence (&bitset0,false,bIterator);
              bIterator = DeserializeEnum (4,&n,bIterator); // antennaPortsCount

              // Deserialize sourceDl-CarrierFreq
              bIterator = DeserializeInteger (&n,0,MAX_EARFCN,bIterator);
              m_asConfig.sourceDlCarrierFreq = n;
            }
          if (handoverPrepInfoOpts[2])
            {
              // Deserialize rrm-Config
              // ...
            }
          if (handoverPrepInfoOpts[1])
            {
              // Deserialize as-Context
              // ...
            }
          if (handoverPrepInfoOpts[0])
            {
              // Deserialize nonCriticalExtension
              // ...
            }
        }
    }

  return GetSerializedSize ();
}

void
HandoverPreparationInfoHeader::Print (std::ostream &os) const
{
  RrcAsn1Header::Print (os,m_asConfig.sourceRadioResourceConfig);
  os << "sourceUeIdentity: " << m_asConfig.sourceUeIdentity << std::endl;
  os << "dlBandwidth: " << (int)m_asConfig.sourceMasterInformationBlock.dlBandwidth << std::endl;
  os << "systemFrameNumber: " << (int)m_asConfig.sourceMasterInformationBlock.systemFrameNumber << std::endl;
  os << "plmnIdentityInfo.plmnIdentity: " << (int) m_asConfig.sourceSystemInformationBlockType1.cellAccessRelatedInfo.plmnIdentityInfo.plmnIdentity << std::endl;
  os << "cellAccessRelatedInfo.cellIdentity " << (int)m_asConfig.sourceSystemInformationBlockType1.cellAccessRelatedInfo.cellIdentity << std::endl;
  os << "cellAccessRelatedInfo.csgIndication: " << m_asConfig.sourceSystemInformationBlockType1.cellAccessRelatedInfo.csgIndication << std::endl;
  os << "cellAccessRelatedInfo.csgIdentity: " << (int)m_asConfig.sourceSystemInformationBlockType1.cellAccessRelatedInfo.csgIdentity << std::endl;
  os << "sourceDlCarrierFreq: " << m_asConfig.sourceDlCarrierFreq << std::endl;
}

void
HandoverPreparationInfoHeader::SetMessage (HandoverPreparationInfo msg)
{
  m_asConfig = msg.asConfig;
  m_isDataSerialized = false;
}

LteRrcSap::HandoverPreparationInfo
HandoverPreparationInfoHeader::GetMessage () const
{
  HandoverPreparationInfo msg;
  msg.asConfig = m_asConfig;

  return msg;
}

LteRrcSap::AsConfig
HandoverPreparationInfoHeader::GetAsConfig () const
{
  return m_asConfig;
}

//////////////////// RrcConnectionReestablishmentRequestHeader class ////////////////////////

RrcConnectionReestablishmentRequestHeader::RrcConnectionReestablishmentRequestHeader ()
{
}

void
RrcConnectionReestablishmentRequestHeader::PreSerialize () const
{
  m_serializationResult = Buffer ();

  SerializeUlCcchMessage (0);

  // Serialize RrcConnectionReestablishmentReques sequence:
  // no default or optional fields. Extension marker not present.
  SerializeSequence<0> (std::bitset<0> (),false);

  // Serialize criticalExtensions choice
  // chosen: rrcConnectionReestablishmentRequest-r8
  SerializeChoice (2,0);

  // Serialize RRCConnectionReestablishmentRequest-r8-IEs sequence
  // no default or optional fields. Extension marker not present.
  SerializeSequence<0> (std::bitset<0> (),false);

  // Serialize ue-Identity
  SerializeSequence<0> (std::bitset<0> (),false);
  // Serialize c-RNTI
  SerializeBitstring (std::bitset<16> (m_ueIdentity.cRnti));
  // Serialize physCellId
  SerializeInteger (m_ueIdentity.physCellId,0,503);
  // Serialize shortMAC-I
  SerializeBitstring (std::bitset<16> (0));

  // Serialize ReestablishmentCause
  switch (m_reestablishmentCause)
    {
    case RECONFIGURATION_FAILURE:
      SerializeEnum (4,0);
      break;
    case HANDOVER_FAILURE:
      SerializeEnum (4,1);
      break;
    case OTHER_FAILURE:
      SerializeEnum (4,2);
      break;
    default:
      SerializeEnum (4,3);
    }

  // Serialize spare
  SerializeBitstring<2> (std::bitset<2> (0));

  // Finish serialization
  FinalizeSerialization ();
}

uint32_t
RrcConnectionReestablishmentRequestHeader::Deserialize (Buffer::Iterator bIterator)
{
  std::bitset<0> bitset0;
  int n;

  bIterator = DeserializeUlCcchMessage (bIterator);

  // Deserialize RrcConnectionReestablishmentRequest sequence
  // 0 optional fields, no extension marker
  bIterator = DeserializeSequence (&bitset0,false,bIterator);

  // Deserialize criticalExtensions choice
  bIterator = DeserializeChoice (2,&n,bIterator);
  if ( n == 1)
    {
      // Deserialize criticalExtensionsFuture
      bIterator = DeserializeSequence (&bitset0,false,bIterator);
    }
  else if ( n == 0)
    {
      // Deserialize RRCConnectionReestablishmentRequest-r8-IEs
      bIterator = DeserializeSequence (&bitset0,false,bIterator);

      // Deserialize ReestabUE-Identity sequence
      bIterator = DeserializeSequence (&bitset0,false,bIterator);

      // Deserialize c-RNTI
      std::bitset<16> cRnti;
      bIterator = DeserializeBitstring (&cRnti,bIterator);
      m_ueIdentity.cRnti = cRnti.to_ulong ();

      // Deserialize physCellId
      int physCellId;
      bIterator = DeserializeInteger (&physCellId,0,503,bIterator);
      m_ueIdentity.physCellId = physCellId;

      // Deserialize shortMAC-I
      std::bitset<16> shortMacI;
      bIterator = DeserializeBitstring (&shortMacI,bIterator);

      // Deserialize ReestablishmentCause
      int reestCs;
      bIterator = DeserializeEnum (4,&reestCs,bIterator);
      switch (reestCs)
        {
        case 0:
          m_reestablishmentCause = RECONFIGURATION_FAILURE;
          break;
        case 1:
          m_reestablishmentCause = HANDOVER_FAILURE;
          break;
        case 2:
          m_reestablishmentCause = OTHER_FAILURE;
          break;
        case 3:
          break;
        }

      // Deserialize spare
      std::bitset<2> spare;
      bIterator = DeserializeBitstring (&spare,bIterator);
    }

  return GetSerializedSize ();
}

void
RrcConnectionReestablishmentRequestHeader::Print (std::ostream &os) const
{
  os << "ueIdentity.cRnti: " << (int)m_ueIdentity.cRnti << std::endl;
  os << "ueIdentity.physCellId: " << (int)m_ueIdentity.physCellId << std::endl;
  os << "m_reestablishmentCause: " << m_reestablishmentCause << std::endl;
}

void
RrcConnectionReestablishmentRequestHeader::SetMessage (RrcConnectionReestablishmentRequest msg)
{
  m_ueIdentity = msg.ueIdentity;
  m_reestablishmentCause = msg.reestablishmentCause;
  m_isDataSerialized = false;
}

LteRrcSap::RrcConnectionReestablishmentRequest
RrcConnectionReestablishmentRequestHeader::GetMessage () const
{
  RrcConnectionReestablishmentRequest msg;
  msg.ueIdentity = m_ueIdentity;
  msg.reestablishmentCause = m_reestablishmentCause;

  return msg;
}

LteRrcSap::ReestabUeIdentity
RrcConnectionReestablishmentRequestHeader::GetUeIdentity () const
{
  return m_ueIdentity;
}

LteRrcSap::ReestablishmentCause
RrcConnectionReestablishmentRequestHeader::GetReestablishmentCause () const
{
  return m_reestablishmentCause;
}

//////////////////// RrcConnectionReestablishmentHeader class ////////////////////////

RrcConnectionReestablishmentHeader::RrcConnectionReestablishmentHeader ()
{
}

void
RrcConnectionReestablishmentHeader::PreSerialize () const
{
  m_serializationResult = Buffer ();

  SerializeDlCcchMessage (0);

  // Serialize RrcConnectionReestablishment sequence:
  // no default or optional fields. Extension marker not present.
  SerializeSequence (std::bitset<0> (),false);

  // Serialize rrc-TransactionIdentifier
  SerializeInteger (m_rrcTransactionIdentifier,0,3);

  // Serialize criticalExtensions choice
  SerializeChoice (2,0);

  // Serialize c1 choice
  SerializeChoice (8,0);

  // Serialize RRCConnectionReestablishment-r8-IEs sequence
  // 1 optional field, no extension marker
  SerializeSequence (std::bitset<1> (0),false);

  // Serialize radioResourceConfigDedicated
  SerializeRadioResourceConfigDedicated (m_radioResourceConfigDedicated);

  // Serialize nextHopChainingCount
  SerializeInteger (0,0,7);

  // Finish serialization
  FinalizeSerialization ();
}

uint32_t
RrcConnectionReestablishmentHeader::Deserialize (Buffer::Iterator bIterator)
{
  std::bitset<0> bitset0;
  int n;

  bIterator = DeserializeDlCcchMessage (bIterator);

  // Deserialize RrcConnectionReestablishment sequence
  // 0 optional fields, no extension marker
  bIterator = DeserializeSequence (&bitset0,false,bIterator);

  // Deserialize rrc-TransactionIdentifier
  bIterator = DeserializeInteger (&n,0,3,bIterator);
  m_rrcTransactionIdentifier = n;

  // Deserialize criticalExtensions choice
  int criticalExtensionsChoice;
  bIterator = DeserializeChoice (2,&criticalExtensionsChoice,bIterator);
  if (criticalExtensionsChoice == 1)
    {
      // Deserialize criticalExtensionsFuture
      bIterator = DeserializeSequence (&bitset0,false,bIterator);
    }
  else if (criticalExtensionsChoice == 0)
    {
      // Deserialize c1
      int c1;
      bIterator = DeserializeChoice (8,&c1,bIterator);
      if (c1 > 0)
        {
          bIterator = DeserializeNull (bIterator);
        }
      else if (c1 == 0)
        {
          // Deserialize rrcConnectionReestablishment-r8
          // 1 optional field
          std::bitset<1> nonCriticalExtensionPresent;
          bIterator = DeserializeSequence (&nonCriticalExtensionPresent,false,bIterator);

          // Deserialize RadioResourceConfigDedicated
          bIterator = DeserializeRadioResourceConfigDedicated (&m_radioResourceConfigDedicated,bIterator);

          // Deserialize nextHopChainingCount
          bIterator = DeserializeInteger (&n,0,7,bIterator);
        }
    }

  return GetSerializedSize ();
}

void
RrcConnectionReestablishmentHeader::Print (std::ostream &os) const
{
  os << "rrcTransactionIdentifier: " << (int)m_rrcTransactionIdentifier << std::endl;
  os << "RadioResourceConfigDedicated: " << std::endl;
  RrcAsn1Header::Print (os,m_radioResourceConfigDedicated);
}

void
RrcConnectionReestablishmentHeader::SetMessage (RrcConnectionReestablishment msg)
{
  m_rrcTransactionIdentifier = msg.rrcTransactionIdentifier;
  m_radioResourceConfigDedicated = msg.radioResourceConfigDedicated;
  m_isDataSerialized = false;
}

LteRrcSap::RrcConnectionReestablishment
RrcConnectionReestablishmentHeader::GetMessage () const
{
  RrcConnectionReestablishment msg;
  msg.rrcTransactionIdentifier = m_rrcTransactionIdentifier;
  msg.radioResourceConfigDedicated = m_radioResourceConfigDedicated;
  return msg;
}

uint8_t
RrcConnectionReestablishmentHeader::GetRrcTransactionIdentifier () const
{
  return m_rrcTransactionIdentifier;
}

LteRrcSap::RadioResourceConfigDedicated
RrcConnectionReestablishmentHeader::GetRadioResourceConfigDedicated () const
{
  return m_radioResourceConfigDedicated;
}

//////////////////// RrcConnectionReestablishmentCompleteHeader class ////////////////////////

RrcConnectionReestablishmentCompleteHeader::RrcConnectionReestablishmentCompleteHeader ()
{
}

void
RrcConnectionReestablishmentCompleteHeader::PreSerialize () const
{
  m_serializationResult = Buffer ();

  // Serialize DCCH message
  SerializeUlDcchMessage (3);

  // Serialize RrcConnectionReestablishmentComplete sequence:
  // no default or optional fields. Extension marker not present.
  SerializeSequence (std::bitset<0> (),false);

  // Serialize rrc-TransactionIdentifier
  SerializeInteger (m_rrcTransactionIdentifier,0,3);

  // Serialize criticalExtensions choice
  SerializeChoice (2,0);

  // Serialize rrcConnectionReestablishmentComplete-r8 sequence
  // 1 optional field (not present), no extension marker.
  SerializeSequence (std::bitset<1> (0),false);

  // Finish serialization
  FinalizeSerialization ();
}

uint32_t
RrcConnectionReestablishmentCompleteHeader::Deserialize (Buffer::Iterator bIterator)
{
  std::bitset<0> bitset0;
  int n;

  bIterator = DeserializeUlDcchMessage (bIterator);

  // Deserialize RrcConnectionReestablishmentComplete sequence
  // 0 optional fields, no extension marker
  bIterator = DeserializeSequence (&bitset0,false,bIterator);

  // Deserialize rrc-TransactionIdentifier
  bIterator = DeserializeInteger (&n,0,3,bIterator);
  m_rrcTransactionIdentifier = n;

  // Deserialize criticalExtensions choice
  int criticalExtensionsChoice;
  bIterator = DeserializeChoice (2,&criticalExtensionsChoice,bIterator);
  if (criticalExtensionsChoice == 1)
    {
      // Deserialize criticalExtensionsFuture
      bIterator = DeserializeSequence (&bitset0,false,bIterator);
    }
  else if (criticalExtensionsChoice == 0)
    {
      // Deserialize rrcConnectionReestablishmentComplete-r8
      std::bitset<1> opts;
      bIterator = DeserializeSequence (&opts,false,bIterator);
      if (opts[0])
        {
          // Deserialize RRCConnectionReestablishmentComplete-v920-IEs
          // ...
        }
    }

  return GetSerializedSize ();
}

void
RrcConnectionReestablishmentCompleteHeader::Print (std::ostream &os) const
{
  os << "rrcTransactionIdentifier: " << (int)m_rrcTransactionIdentifier << std::endl;
}

void
RrcConnectionReestablishmentCompleteHeader::SetMessage (RrcConnectionReestablishmentComplete msg)
{
  m_rrcTransactionIdentifier = msg.rrcTransactionIdentifier;
  m_isDataSerialized = false;
}

LteRrcSap::RrcConnectionReestablishmentComplete
RrcConnectionReestablishmentCompleteHeader::GetMessage () const
{
  RrcConnectionReestablishmentComplete msg;
  msg.rrcTransactionIdentifier = m_rrcTransactionIdentifier;
  return msg;
}

uint8_t
RrcConnectionReestablishmentCompleteHeader::GetRrcTransactionIdentifier () const
{
  return m_rrcTransactionIdentifier;
}

//////////////////// RrcConnectionReestablishmentRejectHeader class ////////////////////////

RrcConnectionReestablishmentRejectHeader::RrcConnectionReestablishmentRejectHeader ()
{
}

void
RrcConnectionReestablishmentRejectHeader::PreSerialize () const
{
  m_serializationResult = Buffer ();

  // Serialize CCCH message
  SerializeDlCcchMessage (1);

  // Serialize RrcConnectionReestablishmentReject sequence:
  // no default or optional fields. Extension marker not present.
  SerializeSequence (std::bitset<0> (),false);

  // Serialize criticalExtensions choice
  SerializeChoice (2,0);

  // Serialize RRCConnectionReestablishmentReject-r8-IEs sequence
  // 1 optional field (not present), no extension marker.
  SerializeSequence (std::bitset<1> (0),false);

  // Finish serialization
  FinalizeSerialization ();
}

uint32_t
RrcConnectionReestablishmentRejectHeader::Deserialize (Buffer::Iterator bIterator)
{
  std::bitset<0> bitset0;

  bIterator = DeserializeDlCcchMessage (bIterator);

  // Deserialize RrcConnectionReestablishmentReject sequence
  // 0 optional fields, no extension marker
  bIterator = DeserializeSequence (&bitset0,false,bIterator);

  // Deserialize criticalExtensions choice
  int criticalExtensionsChoice;
  bIterator = DeserializeChoice (2,&criticalExtensionsChoice,bIterator);
  if (criticalExtensionsChoice == 1)
    {
      // Deserialize criticalExtensionsFuture
      bIterator = DeserializeSequence (&bitset0,false,bIterator);
    }
  else if (criticalExtensionsChoice == 0)
    {
      // Deserialize rrcConnectionReestablishmentReject-r8
      std::bitset<1> opts;
      bIterator = DeserializeSequence (&opts,false,bIterator);
      if (opts[0])
        {
          // Deserialize RRCConnectionReestablishmentReject-v8a0-IEs
          // ...
        }
    }

  return GetSerializedSize ();
}

void
RrcConnectionReestablishmentRejectHeader::Print (std::ostream &os) const
{
}

void
RrcConnectionReestablishmentRejectHeader::SetMessage (RrcConnectionReestablishmentReject msg)
{
  m_rrcConnectionReestablishmentReject = msg;
  m_isDataSerialized = false;
}

LteRrcSap::RrcConnectionReestablishmentReject
RrcConnectionReestablishmentRejectHeader::GetMessage () const
{
  return m_rrcConnectionReestablishmentReject;
}

//////////////////// RrcConnectionReleaseHeader class ////////////////////////

RrcConnectionReleaseHeader::RrcConnectionReleaseHeader ()
{
}

void
RrcConnectionReleaseHeader::PreSerialize () const
{
  m_serializationResult = Buffer ();

  // Serialize DCCH message
  SerializeDlDcchMessage (5);

  // Serialize RrcConnectionRelease sequence:
  // no default or optional fields. Extension marker not present.
  SerializeSequence (std::bitset<0> (),false);

  // Serialize rrc-TransactionIdentifier
  SerializeInteger (m_rrcConnectionRelease.rrcTransactionIdentifier,0,3);

  // Serialize criticalExtensions choice
  SerializeChoice (2,0);

  // Serialize c1 choice
  SerializeChoice (4,0);

  // Serialize RRCConnectionRelease-r8-IEs sequence
  // 3 optional field (not present), no extension marker.
  SerializeSequence (std::bitset<3> (0),false);

  // Serialize ReleaseCause
  SerializeEnum (4,1);

  // Finish serialization
  FinalizeSerialization ();
}

uint32_t
RrcConnectionReleaseHeader::Deserialize (Buffer::Iterator bIterator)
{
  std::bitset<0> bitset0;
  int n;

  bIterator = DeserializeDlDcchMessage (bIterator);

  // Deserialize RrcConnectionRelease sequence
  // 0 optional fields, no extension marker
  bIterator = DeserializeSequence (&bitset0,false,bIterator);

  // Deserialize rrc-TransactionIdentifier
  bIterator = DeserializeInteger (&n,0,3,bIterator);
  m_rrcConnectionRelease.rrcTransactionIdentifier = n;

  // Deserialize criticalExtensions choice
  int criticalExtensionsChoice;
  bIterator = DeserializeChoice (2,&criticalExtensionsChoice,bIterator);
  if (criticalExtensionsChoice == 1)
    {
      // Deserialize criticalExtensionsFuture
      bIterator = DeserializeSequence (&bitset0,false,bIterator);
    }
  else if (criticalExtensionsChoice == 0)
    {
      // Deserialize c1
      int c1Choice;
      bIterator = DeserializeChoice (4,&c1Choice,bIterator);

      if (c1Choice == 0)
        {
          // Deserialize RRCConnectionRelease-r8-IEs
          std::bitset<3> opts;
          bIterator = DeserializeSequence (&opts,false,bIterator);

          // Deserialize releaseCause
          bIterator = DeserializeEnum (4,&n,bIterator);

          if (opts[2])
            {
              // Deserialize redirectedCarrierInfo
              // ...
            }
          if (opts[1])
            {
              // Deserialize idleModeMobilityControlInfo
              // ...
            }
          if (opts[0])
            {
              // Deserialize nonCriticalExtension
              // ...
            }
        }

      else
        {
          bIterator = DeserializeNull (bIterator);
        }
    }

  return GetSerializedSize ();
}

void
RrcConnectionReleaseHeader::Print (std::ostream &os) const
{
}

void
RrcConnectionReleaseHeader::SetMessage (RrcConnectionRelease msg)
{
  m_rrcConnectionRelease = msg;
  m_isDataSerialized = false;
}

LteRrcSap::RrcConnectionRelease
RrcConnectionReleaseHeader::GetMessage () const
{
  return m_rrcConnectionRelease;
}

//////////////////// RrcConnectionRejectHeader class ////////////////////////

RrcConnectionRejectHeader::RrcConnectionRejectHeader ()
{
}

void
RrcConnectionRejectHeader::PreSerialize () const
{
  m_serializationResult = Buffer ();

  // Serialize CCCH message
  SerializeDlCcchMessage (2);

  // Serialize RrcConnectionReject sequence:
  // no default or optional fields. Extension marker not present.
  SerializeSequence (std::bitset<0> (),false);

  // Serialize criticalExtensions choice
  SerializeChoice (2,0);

  // Serialize c1 choice
  SerializeChoice (4,0);

  // Serialize rrcConnectionReject-r8 sequence
  // 1 optional field (not present), no extension marker.
  SerializeSequence (std::bitset<1> (0),false);

  // Serialize waitTime
  SerializeInteger (m_rrcConnectionReject.waitTime, 1, 16);

  // Finish serialization
  FinalizeSerialization ();
}

uint32_t
RrcConnectionRejectHeader::Deserialize (Buffer::Iterator bIterator)
{
  std::bitset<0> bitset0;
  int n;

  bIterator = DeserializeDlCcchMessage (bIterator);

  // Deserialize RrcConnectionReject sequence
  // 0 optional fields, no extension marker
  bIterator = DeserializeSequence (&bitset0,false,bIterator);

  // Deserialize criticalExtensions choice
  int criticalExtensionsChoice;
  bIterator = DeserializeChoice (2,&criticalExtensionsChoice,bIterator);
  if (criticalExtensionsChoice == 1)
    {
      // Deserialize criticalExtensionsFuture
      bIterator = DeserializeSequence (&bitset0,false,bIterator);
    }
  else if (criticalExtensionsChoice == 0)
    {
      // Deserialize c1 choice
      int c1Choice;
      bIterator = DeserializeChoice (4,&c1Choice,bIterator);

      if (c1Choice > 0)
        {
          bIterator = DeserializeNull (bIterator);
        }
      else if (c1Choice == 0)
        {
          // Deserialize rrcConnectionReject-r8
          std::bitset<1> opts;
          bIterator = DeserializeSequence (&opts,false,bIterator);

          bIterator = DeserializeInteger (&n,1,16,bIterator);
          m_rrcConnectionReject.waitTime = n;

          if (opts[0])
            {
              // Deserialize RRCConnectionReject-v8a0-IEs
              // ...
            }
        }
    }

  return GetSerializedSize ();
}

void
RrcConnectionRejectHeader::Print (std::ostream &os) const
{
  os << "wait time: " << (int)m_rrcConnectionReject.waitTime << std::endl;
}

void
RrcConnectionRejectHeader::SetMessage (RrcConnectionReject msg)
{
  m_rrcConnectionReject = msg;
  m_isDataSerialized = false;
}

LteRrcSap::RrcConnectionReject
RrcConnectionRejectHeader::GetMessage () const
{
  return m_rrcConnectionReject;
}

//////////////////// MeasurementReportHeader class ////////////////////////

MeasurementReportHeader::MeasurementReportHeader ()
{
}

void
MeasurementReportHeader::PreSerialize () const
{
  m_serializationResult = Buffer ();

  // Serialize DCCH message
  SerializeUlDcchMessage (1);

  // Serialize MeasurementReport sequence:
  // no default or optional fields. Extension marker not present.
  SerializeSequence<0> (std::bitset<0> (),false);

  // Serialize criticalExtensions choice:
  // c1 chosen
  SerializeChoice (2,0);
  
  // Serialize c1 choice
  // measurementReport-r8 chosen
  SerializeChoice (8,0);

  // Serialize MeasurementReport-r8-IEs sequence:
  // 1 optional fields, not present. Extension marker not present.
  SerializeSequence<1> (std::bitset<1> (0),false);

  // Serialize measResults
  SerializeMeasResults (m_measurementReport.measResults);
  
  // Finish serialization
  FinalizeSerialization ();
}

uint32_t
MeasurementReportHeader::Deserialize (Buffer::Iterator bIterator)
{
  std::bitset<0> bitset0;

  bIterator = DeserializeSequence (&bitset0,false,bIterator);

  bIterator = DeserializeUlDcchMessage (bIterator);

  int criticalExtensionsChoice;
  bIterator = DeserializeChoice (2,&criticalExtensionsChoice,bIterator);

  if (criticalExtensionsChoice == 1)
    {
      // Deserialize criticalExtensionsFuture
      bIterator = DeserializeSequence (&bitset0,false,bIterator);
    }
  else if (criticalExtensionsChoice == 0)
    {
      // Deserialize c1
      int c1Choice;
      bIterator = DeserializeChoice (8,&c1Choice,bIterator);
      
      if(c1Choice>0)
      {
        bIterator = DeserializeNull(bIterator);
      }
      else
      {
        // Deserialize measurementReport-r8
        std::bitset<1> isNonCriticalExtensionPresent;
        bIterator = DeserializeSequence(&isNonCriticalExtensionPresent,false,bIterator);
        
        // Deserialize measResults
        bIterator = DeserializeMeasResults (&m_measurementReport.measResults, bIterator);
        
        if(isNonCriticalExtensionPresent[0])
        {
          // Deserialize nonCriticalExtension MeasurementReport-v8a0-IEs
          // ...
        }

      }
    }

  return GetSerializedSize ();
}

void
MeasurementReportHeader::Print (std::ostream &os) const
{
  os << "measId = " << (int)m_measurementReport.measResults.measId << std::endl;
  os << "rsrpResult = " << (int)m_measurementReport.measResults.rsrpResult << std::endl;
  os << "rsrqResult = " << (int)m_measurementReport.measResults.rsrqResult << std::endl;
  os << "haveMeasResultNeighCells = " << (int)m_measurementReport.measResults.haveMeasResultNeighCells << std::endl;
  
  if(m_measurementReport.measResults.haveMeasResultNeighCells)
  {
    std::list<LteRrcSap::MeasResultEutra> measResultListEutra = m_measurementReport.measResults.measResultListEutra;
    std::list<LteRrcSap::MeasResultEutra>::iterator it = measResultListEutra.begin();
    for ( ; it != measResultListEutra.end(); it++)
    {
      os << "   physCellId =" << (int) it->physCellId << std::endl;
      os << "   haveCgiInfo =" << it->haveCgiInfo << std::endl;
      if (it->haveCgiInfo)
      {
         os << "      plmnIdentity = " << (int) it->cgiInfo.plmnIdentity << std::endl;
         os << "      cellIdentity = " << (int) it->cgiInfo.cellIdentity << std::endl;
         os << "      trackingAreaCode = " << (int) it->cgiInfo.trackingAreaCode << std::endl;
         os << "      havePlmnIdentityList = " << !it->cgiInfo.plmnIdentityList.empty() << std::endl;
         if(!it->cgiInfo.plmnIdentityList.empty())
         {
           for(std::list<uint32_t>::iterator it2 = it->cgiInfo.plmnIdentityList.begin(); it2 != it->cgiInfo.plmnIdentityList.begin(); it2++)
           {
             os << "         plmnId : " << *it2 << std::endl;
           }
         }
      }

      os << "   haveRsrpResult =" << it->haveRsrpResult << std::endl;
      if (it->haveRsrpResult)
      {
        os << "   rsrpResult =" << (int) it->rsrpResult << std::endl;
      }

      os << "   haveRsrqResult =" << it->haveRsrqResult << std::endl;
      if (it->haveRsrqResult)
      {
        os << "   rsrqResult =" << (int) it->rsrqResult << std::endl;
      }

    }
  }
}

void
MeasurementReportHeader::SetMessage (MeasurementReport msg)
{
  m_measurementReport = msg;
  m_isDataSerialized = false;
}

LteRrcSap::MeasurementReport
MeasurementReportHeader::GetMessage () const
{
  MeasurementReport msg;
  msg = m_measurementReport;
  return msg;
}

///////////////////  RrcUlDcchMessage //////////////////////////////////
uint32_t
RrcUlDcchMessage::Deserialize (Buffer::Iterator bIterator)
{
  DeserializeUlDcchMessage (bIterator);
  return 1;
}

void
RrcUlDcchMessage::Print (std::ostream &os) const
{
  std::cout << "UL DCCH MSG TYPE: " << m_messageType << std::endl;
}

void
RrcUlDcchMessage::PreSerialize () const
{
  SerializeUlDcchMessage (m_messageType);
}

Buffer::Iterator
RrcUlDcchMessage::DeserializeUlDcchMessage (Buffer::Iterator bIterator)
{
  std::bitset<0> bitset0;
  int n;

  bIterator = DeserializeSequence (&bitset0,false,bIterator);
  bIterator = DeserializeChoice (2,&n,bIterator);
  if (n == 1)
    {
      // Deserialize messageClassExtension
      bIterator = DeserializeSequence (&bitset0,false,bIterator);
      m_messageType = -1;
    }
  else if (n == 0)
    {
      // Deserialize c1
      bIterator = DeserializeChoice (16,&m_messageType,bIterator);
    }

  return bIterator;
}

void
RrcUlDcchMessage::SerializeUlDcchMessage (int messageType) const
{
  SerializeSequence (std::bitset<0> (),false);
  // Choose c1
  SerializeChoice (2,0);
  // Choose message type
  SerializeChoice (16,messageType);
}

///////////////////  RrcDlDcchMessage //////////////////////////////////
uint32_t
RrcDlDcchMessage::Deserialize (Buffer::Iterator bIterator)
{
  DeserializeDlDcchMessage (bIterator);
  return 1;
}

void
RrcDlDcchMessage::Print (std::ostream &os) const
{
  std::cout << "DL DCCH MSG TYPE: " << m_messageType << std::endl;
}

void
RrcDlDcchMessage::PreSerialize () const
{
  SerializeDlDcchMessage (m_messageType);
}

Buffer::Iterator
RrcDlDcchMessage::DeserializeDlDcchMessage (Buffer::Iterator bIterator)
{
  std::bitset<0> bitset0;
  int n;

  bIterator = DeserializeSequence (&bitset0,false,bIterator);
  bIterator = DeserializeChoice (2,&n,bIterator);
  if (n == 1)
    {
      // Deserialize messageClassExtension
      bIterator = DeserializeSequence (&bitset0,false,bIterator);
      m_messageType = -1;
    }
  else if (n == 0)
    {
      // Deserialize c1
      bIterator = DeserializeChoice (16,&m_messageType,bIterator);
    }

  return bIterator;
}

void
RrcDlDcchMessage::SerializeDlDcchMessage (int messageType) const
{
  SerializeSequence (std::bitset<0> (),false);
  // Choose c1
  SerializeChoice (2,0);
  // Choose message type
  SerializeChoice (16,messageType);
}

///////////////////  RrcUlCcchMessage //////////////////////////////////
uint32_t
RrcUlCcchMessage::Deserialize (Buffer::Iterator bIterator)
{
  DeserializeUlCcchMessage (bIterator);
  return 1;
}

void
RrcUlCcchMessage::Print (std::ostream &os) const
{
  std::cout << "UL CCCH MSG TYPE: " << m_messageType << std::endl;
}

void
RrcUlCcchMessage::PreSerialize () const
{
  SerializeUlCcchMessage (m_messageType);
}

Buffer::Iterator
RrcUlCcchMessage::DeserializeUlCcchMessage (Buffer::Iterator bIterator)
{
  std::bitset<0> bitset0;
  int n;

  bIterator = DeserializeSequence (&bitset0,false,bIterator);
  bIterator = DeserializeChoice (2,&n,bIterator);
  if (n == 1)
    {
      // Deserialize messageClassExtension
      bIterator = DeserializeSequence (&bitset0,false,bIterator);
      m_messageType = -1;
    }
  else if (n == 0)
    {
      // Deserialize c1
      bIterator = DeserializeChoice (2,&m_messageType,bIterator);
    }

  return bIterator;
}

void
RrcUlCcchMessage::SerializeUlCcchMessage (int messageType) const
{
  SerializeSequence (std::bitset<0> (),false);
  // Choose c1
  SerializeChoice (2,0);
  // Choose message type
  SerializeChoice (2,messageType);
}

///////////////////  RrcDlCcchMessage //////////////////////////////////
uint32_t
RrcDlCcchMessage::Deserialize (Buffer::Iterator bIterator)
{
  DeserializeDlCcchMessage (bIterator);
  return 1;
}

void
RrcDlCcchMessage::Print (std::ostream &os) const
{
  std::cout << "DL CCCH MSG TYPE: " << m_messageType << std::endl;
}

void
RrcDlCcchMessage::PreSerialize () const
{
  SerializeDlCcchMessage (m_messageType);
}

Buffer::Iterator
RrcDlCcchMessage::DeserializeDlCcchMessage (Buffer::Iterator bIterator)
{
  std::bitset<0> bitset0;
  int n;

  bIterator = DeserializeSequence (&bitset0,false,bIterator);
  bIterator = DeserializeChoice (2,&n,bIterator);
  if (n == 1)
    {
      // Deserialize messageClassExtension
      bIterator = DeserializeSequence (&bitset0,false,bIterator);
      m_messageType = -1;
    }
  else if (n == 0)
    {
      // Deserialize c1
      bIterator = DeserializeChoice (4,&m_messageType,bIterator);
    }

  return bIterator;
}

void
RrcDlCcchMessage::SerializeDlCcchMessage (int messageType) const
{
  SerializeSequence (std::bitset<0> (),false);
  // Choose c1
  SerializeChoice (2,0);
  // Choose message type
  SerializeChoice (4,messageType);
}

} // namespace ns3

