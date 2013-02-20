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
 * Authors: Nicola Baldo <nbaldo@cttc.es>
 *          Lluis Parcerisa <lparcerisa@cttc.cat>
 */


#ifndef LTE_RRC_SAP_H
#define LTE_RRC_SAP_H

#include <stdint.h>
#include <list>

#include <ns3/ptr.h>
#include <ns3/simulator.h>

namespace ns3 {

class LteRlcSapUser;
class LtePdcpSapUser;
class LteRlcSapProvider;
class LtePdcpSapProvider;
class Packet;  

/**
 * Class holding definition common to all Ue/Enb SAP
 * Users/Providers. See 3GPP TS 36.331 for reference. 
 * Note that only those values that are (expected to be) used by the
 * ns-3 model are mentioned here. The naming of the variables that are
 * defined here is the same of 36.331, except for removal of "-" and
 * conversion to CamelCase or ALL_CAPS where needed in order to follow
 * the ns-3 coding style. Due to the 1-to-1 mapping with TS 36.311, 
 * detailed doxygen documentation is omitted, so please refer to
 * 36.311 for the meaning of these data structures / fields.
 * 
 */
class LteRrcSap
{
public:

  virtual ~LteRrcSap ();

  // Information Elements
  
  struct PlmnIdentityInfo
  {
    uint32_t plmnIdentity;
  };

  struct CellAccessRelatedInfo
  {
    PlmnIdentityInfo plmnIdentityInfo;
    uint32_t cellIdentity;
    bool csgIndication;
    uint32_t csgIdentity;
  };
  
  struct FreqInfo
  {
    uint16_t ulCarrierFreq;
    uint8_t ulBandwidth;
  };

  struct RlcConfig
  {
    enum
      {
        AM, 
        UM_BI_DIRECTIONAL,
        UM_UNI_DIRECTIONAL_UL,
        UM_UNI_DIRECTIONAL_DL
      } choice;    
  };

  struct LogicalChannelConfig
  {
    uint8_t priority;
    uint16_t prioritizedBitRateKbps;
    uint16_t bucketSizeDurationMs;
    uint8_t logicalChannelGroup;
  };

  struct SoundingRsUlConfigCommon
  {
    enum {SETUP, RESET} type;
    uint8_t srsBandwidthConfig;
    uint8_t srsSubframeConfig;
  };

  struct SoundingRsUlConfigDedicated
  {
    enum {SETUP, RESET} type;
    uint8_t srsBandwidth;
    uint16_t srsConfigIndex;
  };

  struct AntennaInfoDedicated
  {
    uint8_t transmissionMode;
  };


  struct PhysicalConfigDedicated
  {
    bool haveSoundingRsUlConfigDedicated;
    SoundingRsUlConfigDedicated soundingRsUlConfigDedicated;
    bool haveAntennaInfoDedicated;
    AntennaInfoDedicated antennaInfo;
  };


  struct SrbToAddMod
  {
    uint8_t srbIdentity;   
    LogicalChannelConfig logicalChannelConfig;
  };
  
  struct DrbToAddMod
  {
    uint8_t epsBearerIdentity;
    uint8_t drbIdentity;
    RlcConfig rlcConfig;
    uint8_t logicalChannelIdentity;
    LogicalChannelConfig logicalChannelConfig;
  };

  struct PreambleInfo
  {
    uint8_t numberOfRaPreambles;
  };
  
  struct RaSupervisionInfo
  {
    uint8_t preambleTransMax;
    uint8_t raResponseWindowSize;
  };

  struct RachConfigCommon
  {
    PreambleInfo preambleInfo;
    RaSupervisionInfo raSupervisionInfo;
  };

  struct RadioResourceConfigCommon
  {    
    RachConfigCommon rachConfigCommon;
  };

  struct RadioResourceConfigCommonSib
  {    
    RachConfigCommon rachConfigCommon;
  };

  struct RadioResourceConfigDedicated
  {
    std::list<SrbToAddMod> srbToAddModList;
    std::list<DrbToAddMod> drbToAddModList;    
    std::list<uint8_t> drbToReleaseList;
    bool havePhysicalConfigDedicated;
    PhysicalConfigDedicated physicalConfigDedicated;
  };
  
  struct QuantityConfig
  {
    uint8_t filterCoefficientRSRP;
    uint8_t filterCoefficientRSRQ;
  };

  struct CellsToAddMod
  {
    uint8_t cellIndex;
    uint16_t physCellId;
    int8_t cellIndividualOffset;
  };
  
  struct PhysCellIdRange
  {
    uint16_t start;
    bool haveRange;
    uint16_t range;
  };

  struct BlackCellsToAddMod
  {
    uint8_t cellIndex;
    PhysCellIdRange physCellIdRange;
  };
  
  struct MeasObjectEutra
  {
    uint16_t carrierFreq;
    uint8_t allowedMeasBandwidth;
    bool presenceAntennaPort1;
    uint8_t neighCellConfig;
    int8_t offsetFreq;
    std::list<uint8_t> cellsToRemoveList;
    std::list<CellsToAddMod> cellsToAddModList;
    std::list<uint8_t> blackCellsToRemoveList;
    std::list<BlackCellsToAddMod> blackCellsToAddModList;
    bool haveCellForWhichToReportCGI;
    uint8_t cellForWhichToReportCGI;
  };
  
  struct ThresholdEutra
  {
    enum {thresholdRsrp, thresholdRsrq} choice;
    uint8_t range;
  };

  struct ReportConfigEutra
  {
    enum {event,periodical} triggerType;
    enum {eventA1,eventA2,eventA3,eventA4,eventA5} eventId;
    ThresholdEutra threshold1; // used for A1, A2, A4, A5
    ThresholdEutra threshold2; // used for A5
    bool reportOnLeave; // used for A3
    int8_t a3Offset; // used for A3
    uint8_t hysteresis;
    uint16_t timeToTrigger;
    enum {reportStrongestCells, reportCgi} purpose;
    enum {rsrp, rsrq} triggerQuantity;
    enum {sameAsTriggerQuantity, both} reportQuantity;
    uint8_t maxReportCells;
    enum {ms120, ms240, ms480, ms640, ms1024, ms2048, ms5120, ms10240, 
    min1, min6, min12, min30, min60, spare3, spare2, spare1} reportInterval;
    uint8_t reportAmount;
  };
  
  struct MeasObjectToAddMod
  {
    uint8_t measObjectId; 
    MeasObjectEutra measObjectEutra;
  };

  struct ReportConfigToAddMod
  {
    uint8_t reportConfigId;
    ReportConfigEutra reportConfigEutra;
  };

  struct MeasIdToAddMod
  {
    uint8_t measId;
    uint8_t measObjectId;
    uint8_t reportConfigId;
  };

  struct MeasGapConfig
  {
    enum {SETUP, RESET} type;
    enum {gp0, gp1} gapOffsetChoice;
    uint8_t gapOffsetValue;
  };
  
  struct MobilityStateParameters
  {
    uint8_t tEvaluation;
    uint8_t tHystNormal;
    uint8_t nCellChangeMedium;
    uint8_t nCellChangeHigh;
  };
  
  struct SpeedStateScaleFactors
  {
    // 25 = oDot25, 50 = oDot5, 75 = oDot75, 100 = lDot0
    uint8_t sfMedium;
    uint8_t sfHigh;
  };

  struct SpeedStatePars
  {
    enum {SETUP, RESET} type;
    MobilityStateParameters mobilityStateParameters;
    SpeedStateScaleFactors timeToTriggerSf;
  };

  struct MeasConfig
  {
    std::list<uint8_t> measObjectToRemoveList;
    std::list<MeasObjectToAddMod> measObjectToAddModList;
    std::list<uint8_t> reportConfigToRemoveList;
    std::list<ReportConfigToAddMod> reportConfigToAddModList;
    std::list<uint8_t> measIdToRemoveList;
    std::list<MeasIdToAddMod> measIdToAddModList;
    bool haveQuantityConfig;
    QuantityConfig quantityConfig;
    bool haveMeasGapConfig;
    MeasGapConfig measGapConfig;
    bool haveSmeasure;
    uint8_t sMeasure;
    bool haveSpeedStatePars;
    SpeedStatePars speedStatePars;
  };

  struct CarrierFreqEutra
  {
    uint16_t dlCarrierFreq;
    uint16_t ulCarrierFreq;
  };

  struct CarrierBandwidthEutra
  {
    uint8_t dlBandwidth;
    uint8_t ulBandwidth;
  };

  struct RachConfigDedicated 
  {
    uint8_t raPreambleIndex;
    uint8_t raPrachMaskIndex;
  };

  struct MobilityControlInfo
  {
    uint16_t targetPhysCellId;
    bool haveCarrierFreq;
    CarrierFreqEutra carrierFreq;    
    bool haveCarrierBandwidth;
    CarrierBandwidthEutra carrierBandwidth;   
    uint16_t newUeIdentity;
    RadioResourceConfigCommon radioResourceConfigCommon;
    bool haveRachConfigDedicated;
    RachConfigDedicated rachConfigDedicated;
  };

  struct ReestabUeIdentity
  {
    uint16_t cRnti;
    uint16_t physCellId;    
  };

  enum ReestablishmentCause 
  { 
    RECONFIGURATION_FAILURE,
    HANDOVER_FAILURE,
    OTHER_FAILURE
  };

  struct MasterInformationBlock
  {
    uint8_t dlBandwidth;
    uint8_t systemFrameNumber;
  };
  
  struct SystemInformationBlockType1
  {
    CellAccessRelatedInfo cellAccessRelatedInfo;
  };

  struct SystemInformationBlockType2
  {
    RadioResourceConfigCommonSib radioResourceConfigCommon;
    FreqInfo freqInfo;    
  };

  struct SystemInformation
  {
    bool haveSib2;
    SystemInformationBlockType2 sib2;
  };

  struct AsConfig
  {
    MeasConfig sourceMeasConfig;
    RadioResourceConfigDedicated sourceRadioResourceConfig;
    uint16_t sourceUeIdentity;      
    MasterInformationBlock sourceMasterInformationBlock;
    SystemInformationBlockType1 sourceSystemInformationBlockType1;
    SystemInformationBlockType2 sourceSystemInformationBlockType2;
    uint16_t sourceDlCarrierFreq;
  };

  struct CgiInfo
  {
    uint32_t plmnIdentity;
    uint32_t cellIdentity;
    uint16_t trackingAreaCode;
    std::list<uint32_t> plmnIdentityList;
  };

  struct MeasResultEutra
  {
    uint16_t physCellId;
    bool haveCgiInfo;
    CgiInfo cgiInfo;
    bool haveRsrpResult;
    uint8_t rsrpResult;
    bool haveRsrqResult;
    uint8_t rsrqResult;
  };

  struct MeasResults
  {
    uint8_t measId;
    uint8_t rsrpResult;
    uint8_t rsrqResult;
    bool haveMeasResultNeighCells;
    std::list<MeasResultEutra> measResultListEutra;
  };

  // Messages

  struct RrcConnectionRequest
  {
    uint64_t ueIdentity;
  };

  struct RrcConnectionSetup
  {
    uint8_t rrcTransactionIdentifier;
    RadioResourceConfigDedicated radioResourceConfigDedicated;
  };
   
  struct RrcConnectionSetupCompleted
  {
    uint8_t rrcTransactionIdentifier;
  };

  struct RrcConnectionReconfiguration
  {
    uint8_t rrcTransactionIdentifier;
    bool haveMeasConfig;
    MeasConfig measConfig;
    bool haveMobilityControlInfo;
    MobilityControlInfo mobilityControlInfo;
    bool haveRadioResourceConfigDedicated;
    RadioResourceConfigDedicated radioResourceConfigDedicated;
  };
    
  struct RrcConnectionReconfigurationCompleted
  {
    uint8_t rrcTransactionIdentifier;
  };
    
      
  struct RrcConnectionReestablishmentRequest
  {
    ReestabUeIdentity ueIdentity;
    ReestablishmentCause reestablishmentCause;
  };
    
  struct RrcConnectionReestablishment
  {
    uint8_t rrcTransactionIdentifier;
    RadioResourceConfigDedicated radioResourceConfigDedicated;
  };
      
  struct RrcConnectionReestablishmentComplete
  {
    uint8_t rrcTransactionIdentifier;
  };

  struct RrcConnectionReestablishmentReject
  {
  };

  struct RrcConnectionRelease
  {
    uint8_t rrcTransactionIdentifier;
  };

  struct RrcConnectionReject
  {
    uint8_t waitTime;
  };

  struct HandoverPreparationInfo
  {
    AsConfig asConfig;
  };
  
  struct MeasurementReport
  {
    MeasResults measResults;
  };
};



/**
 * Service Access Point (SAP) used by the UE RRC to send messages to
 * the eNB. Each method defined in this class correspond to the
 * transmission of a message that is defined in section 6.2.2 of TS
 * 36.331. 
 */
class LteUeRrcSapUser : public LteRrcSap
{
public:

  struct SetupParameters 
  {
    LteRlcSapProvider* srb0SapProvider;
    LtePdcpSapProvider* srb1SapProvider;       
  };

  virtual void Setup (SetupParameters params) = 0;
  virtual void SendRrcConnectionRequest (RrcConnectionRequest msg) = 0;
  virtual void SendRrcConnectionSetupCompleted (RrcConnectionSetupCompleted msg) = 0;
  virtual void SendRrcConnectionReconfigurationCompleted (RrcConnectionReconfigurationCompleted msg) = 0;
  virtual void SendRrcConnectionReestablishmentRequest (RrcConnectionReestablishmentRequest msg) = 0;
  virtual void SendRrcConnectionReestablishmentComplete (RrcConnectionReestablishmentComplete msg) = 0;

};


/**
 * Service Access Point (SAP) used to let the UE RRC receive a message
 * from the eNB RRC. Each method defined in this class correspond to
 * the reception of a message that is defined in section 6.2.2 of TS
 * 36.331. 
 */
class LteUeRrcSapProvider : public LteRrcSap
{
public:

  struct CompleteSetupParameters 
  {
    LteRlcSapUser* srb0SapUser;
    LtePdcpSapUser* srb1SapUser;       
  };

  virtual void CompleteSetup (CompleteSetupParameters params) = 0;
  virtual void RecvMasterInformationBlock (MasterInformationBlock msg) = 0;
  virtual void RecvSystemInformationBlockType1 (SystemInformationBlockType1 msg) = 0;
  virtual void RecvSystemInformation (SystemInformation msg) = 0;
  virtual void RecvRrcConnectionSetup (RrcConnectionSetup msg) = 0;
  virtual void RecvRrcConnectionReconfiguration (RrcConnectionReconfiguration msg) = 0;
  virtual void RecvRrcConnectionReestablishment (RrcConnectionReestablishment msg) = 0;
  virtual void RecvRrcConnectionReestablishmentReject (RrcConnectionReestablishmentReject msg) = 0;
  virtual void RecvRrcConnectionRelease (RrcConnectionRelease msg) = 0;
  virtual void RecvRrcConnectionReject (RrcConnectionReject msg) = 0;

};


/**
 * Service Access Point (SAP) used by the eNB RRC to send messages to
 * the UE RC.  Each method defined in this class correspond to
 * the transmission of a message that is defined in section 6.2.2 of TS
 * 36.331. 
 */
class LteEnbRrcSapUser : public LteRrcSap
{
public:

  struct SetupUeParameters 
  {
    LteRlcSapProvider* srb0SapProvider;
    LtePdcpSapProvider* srb1SapProvider;       
  };

  virtual void SetupUe (uint16_t rnti, SetupUeParameters params) = 0;
  virtual void RemoveUe (uint16_t rnti) = 0;
  virtual void SendSystemInformationBlockType1 (SystemInformationBlockType1 msg) = 0;
  virtual void SendSystemInformation (SystemInformation msg) = 0;
  virtual void SendRrcConnectionSetup (uint16_t rnti, RrcConnectionSetup msg) = 0;
  virtual void SendRrcConnectionReconfiguration (uint16_t rnti, RrcConnectionReconfiguration msg) = 0;
  virtual void SendRrcConnectionReestablishment (uint16_t rnti, RrcConnectionReestablishment msg) = 0;
  virtual void SendRrcConnectionReestablishmentReject (uint16_t rnti, RrcConnectionReestablishmentReject msg) = 0;
  virtual void SendRrcConnectionRelease (uint16_t rnti, RrcConnectionRelease msg) = 0;
  virtual void SendRrcConnectionReject (uint16_t rnti, RrcConnectionReject msg) = 0;
  virtual Ptr<Packet> EncodeHandoverPreparationInformation (HandoverPreparationInfo msg) = 0;
  virtual HandoverPreparationInfo DecodeHandoverPreparationInformation (Ptr<Packet> p) = 0;
  virtual Ptr<Packet> EncodeHandoverCommand (RrcConnectionReconfiguration msg) = 0;
  virtual RrcConnectionReconfiguration DecodeHandoverCommand (Ptr<Packet> p) = 0;

};


/**
 * Service Access Point (SAP) used to let the eNB RRC receive a
 * message from a UE RRC.  Each method defined in this class correspond to
 * the reception of a message that is defined in section 6.2.2 of TS
 * 36.331. 
 */
class LteEnbRrcSapProvider : public LteRrcSap
{
public:
   

  struct CompleteSetupUeParameters 
  {
    LteRlcSapUser* srb0SapUser;
    LtePdcpSapUser* srb1SapUser;       
  };

  virtual void CompleteSetupUe (uint16_t rnti, CompleteSetupUeParameters params) = 0;
  virtual void RecvRrcConnectionRequest (uint16_t rnti, RrcConnectionRequest msg) = 0;
  virtual void RecvRrcConnectionSetupCompleted (uint16_t rnti, RrcConnectionSetupCompleted msg) = 0;
  virtual void RecvRrcConnectionReconfigurationCompleted (uint16_t rnti, RrcConnectionReconfigurationCompleted msg) = 0;
  virtual void RecvRrcConnectionReestablishmentRequest (uint16_t rnti, RrcConnectionReestablishmentRequest msg) = 0;
  virtual void RecvRrcConnectionReestablishmentComplete (uint16_t rnti, RrcConnectionReestablishmentComplete msg) = 0;
  virtual void RecvMeasurementReport (uint16_t rnti, MeasurementReport msg) = 0;

};






////////////////////////////////////
//   templates
////////////////////////////////////


/**
 * Template for the implementation of the LteUeRrcSapUser as a member
 * of an owner class of type C to which all methods are forwarded
 * 
 */
template <class C>
class MemberLteUeRrcSapUser : public LteUeRrcSapUser
{
public:
  MemberLteUeRrcSapUser (C* owner);

  // inherited from LteUeRrcSapUser
  virtual void Setup (SetupParameters params);
  virtual void SendRrcConnectionRequest (RrcConnectionRequest msg);
  virtual void SendRrcConnectionSetupCompleted (RrcConnectionSetupCompleted msg);
  virtual void SendRrcConnectionReconfigurationCompleted (RrcConnectionReconfigurationCompleted msg);
  virtual void SendRrcConnectionReestablishmentRequest (RrcConnectionReestablishmentRequest msg);
  virtual void SendRrcConnectionReestablishmentComplete (RrcConnectionReestablishmentComplete msg);

private:
  MemberLteUeRrcSapUser ();
  C* m_owner;
};

template <class C>
MemberLteUeRrcSapUser<C>::MemberLteUeRrcSapUser (C* owner)
  : m_owner (owner)
{
}

template <class C>
MemberLteUeRrcSapUser<C>::MemberLteUeRrcSapUser ()
{
}

template <class C>
void 
MemberLteUeRrcSapUser<C>::Setup (SetupParameters params)
{
  m_owner->DoSetup (params);
}

template <class C>
void 
MemberLteUeRrcSapUser<C>::SendRrcConnectionRequest (RrcConnectionRequest msg)
{
  m_owner->DoSendRrcConnectionRequest (msg);
}

template <class C>
void 
MemberLteUeRrcSapUser<C>::SendRrcConnectionSetupCompleted (RrcConnectionSetupCompleted msg)
{
  m_owner->DoSendRrcConnectionSetupCompleted (msg);
}

template <class C>
void 
MemberLteUeRrcSapUser<C>::SendRrcConnectionReconfigurationCompleted (RrcConnectionReconfigurationCompleted msg) 
{
  m_owner->DoSendRrcConnectionReconfigurationCompleted (msg) ;
}

template <class C>
void 
MemberLteUeRrcSapUser<C>::SendRrcConnectionReestablishmentRequest (RrcConnectionReestablishmentRequest msg)
{
  m_owner->DoSendRrcConnectionReestablishmentRequest (msg);
}

template <class C>
void 
MemberLteUeRrcSapUser<C>::SendRrcConnectionReestablishmentComplete (RrcConnectionReestablishmentComplete msg)
{
  m_owner->DoSendRrcConnectionReestablishmentComplete (msg);
}

/**
 * Template for the implementation of the LteUeRrcSapProvider as a member
 * of an owner class of type C to which all methods are forwarded
 * 
 */
template <class C>
class MemberLteUeRrcSapProvider : public LteUeRrcSapProvider
{
public:
  MemberLteUeRrcSapProvider (C* owner);

  // methods inherited from LteUeRrcSapProvider go here  
  virtual void CompleteSetup (CompleteSetupParameters params);
  virtual void RecvMasterInformationBlock (MasterInformationBlock msg);
  virtual void RecvSystemInformationBlockType1 (SystemInformationBlockType1 msg);
  virtual void RecvSystemInformation (SystemInformation msg);
  virtual void RecvRrcConnectionSetup (RrcConnectionSetup msg);
  virtual void RecvRrcConnectionReconfiguration (RrcConnectionReconfiguration msg);
  virtual void RecvRrcConnectionReestablishment (RrcConnectionReestablishment msg);
  virtual void RecvRrcConnectionReestablishmentReject (RrcConnectionReestablishmentReject msg);
  virtual void RecvRrcConnectionRelease (RrcConnectionRelease msg);
  virtual void RecvRrcConnectionReject (RrcConnectionReject msg);

private:
  MemberLteUeRrcSapProvider ();
  C* m_owner;
};

template <class C>
MemberLteUeRrcSapProvider<C>::MemberLteUeRrcSapProvider (C* owner)
  : m_owner (owner)
{
}

template <class C>
MemberLteUeRrcSapProvider<C>::MemberLteUeRrcSapProvider ()
{
}

template <class C>
void 
MemberLteUeRrcSapProvider<C>::CompleteSetup (CompleteSetupParameters params)
{
  m_owner->DoCompleteSetup (params);
}

template <class C>
void 
MemberLteUeRrcSapProvider<C>::RecvMasterInformationBlock (MasterInformationBlock msg)
{
  Simulator::ScheduleNow (&C::DoRecvMasterInformationBlock, m_owner, msg);
}

template <class C>
void 
MemberLteUeRrcSapProvider<C>::RecvSystemInformationBlockType1 (SystemInformationBlockType1 msg)
{
  Simulator::ScheduleNow (&C::DoRecvSystemInformationBlockType1, m_owner, msg);
}

template <class C>
void 
MemberLteUeRrcSapProvider<C>::RecvSystemInformation (SystemInformation msg)
{
  Simulator::ScheduleNow (&C::DoRecvSystemInformation, m_owner, msg);
}

template <class C>
void 
MemberLteUeRrcSapProvider<C>::RecvRrcConnectionSetup (RrcConnectionSetup msg)
{
  Simulator::ScheduleNow (&C::DoRecvRrcConnectionSetup, m_owner, msg);
}

template <class C>
void 
MemberLteUeRrcSapProvider<C>::RecvRrcConnectionReconfiguration (RrcConnectionReconfiguration msg)
{
  Simulator::ScheduleNow (&C::DoRecvRrcConnectionReconfiguration, m_owner, msg);
}

template <class C>
void 
MemberLteUeRrcSapProvider<C>::RecvRrcConnectionReestablishment (RrcConnectionReestablishment msg)
{
  Simulator::ScheduleNow (&C::DoRecvRrcConnectionReestablishment, m_owner, msg);
}

template <class C>
void 
MemberLteUeRrcSapProvider<C>::RecvRrcConnectionReestablishmentReject (RrcConnectionReestablishmentReject msg)
{
  Simulator::ScheduleNow (&C::DoRecvRrcConnectionReestablishmentReject, m_owner, msg);
}

template <class C>
void 
MemberLteUeRrcSapProvider<C>::RecvRrcConnectionRelease (RrcConnectionRelease msg)
{
  Simulator::ScheduleNow (&C::DoRecvRrcConnectionRelease, m_owner, msg);
}

template <class C>
void 
MemberLteUeRrcSapProvider<C>::RecvRrcConnectionReject (RrcConnectionReject msg)
{
  Simulator::ScheduleNow (&C::DoRecvRrcConnectionReject, m_owner, msg);
}


/**
 * Template for the implementation of the LteEnbRrcSapUser as a member
 * of an owner class of type C to which all methods are forwarded
 * 
 */
template <class C>
class MemberLteEnbRrcSapUser : public LteEnbRrcSapUser
{
public:
  MemberLteEnbRrcSapUser (C* owner);

  // inherited from LteEnbRrcSapUser

  virtual void SetupUe (uint16_t rnti, SetupUeParameters params);
  virtual void RemoveUe (uint16_t rnti);
  virtual void SendMasterInformationBlock (MasterInformationBlock msg);
  virtual void SendSystemInformationBlockType1 (SystemInformationBlockType1 msg);
  virtual void SendSystemInformation (SystemInformation msg);
  virtual void SendRrcConnectionSetup (uint16_t rnti, RrcConnectionSetup msg);
  virtual void SendRrcConnectionReconfiguration (uint16_t rnti, RrcConnectionReconfiguration msg);
  virtual void SendRrcConnectionReestablishment (uint16_t rnti, RrcConnectionReestablishment msg);
  virtual void SendRrcConnectionReestablishmentReject (uint16_t rnti, RrcConnectionReestablishmentReject msg);
  virtual void SendRrcConnectionRelease (uint16_t rnti, RrcConnectionRelease msg);
  virtual void SendRrcConnectionReject (uint16_t rnti, RrcConnectionReject msg);
  virtual Ptr<Packet> EncodeHandoverPreparationInformation (HandoverPreparationInfo msg);
  virtual HandoverPreparationInfo DecodeHandoverPreparationInformation (Ptr<Packet> p);
  virtual Ptr<Packet> EncodeHandoverCommand (RrcConnectionReconfiguration msg);
  virtual RrcConnectionReconfiguration DecodeHandoverCommand (Ptr<Packet> p);

private:
  MemberLteEnbRrcSapUser ();
  C* m_owner;
};

template <class C>
MemberLteEnbRrcSapUser<C>::MemberLteEnbRrcSapUser (C* owner)
  : m_owner (owner)
{
}

template <class C>
MemberLteEnbRrcSapUser<C>::MemberLteEnbRrcSapUser ()
{
}

template <class C>
void 
MemberLteEnbRrcSapUser<C>::SetupUe (uint16_t rnti, SetupUeParameters params)
{
  m_owner->DoSetupUe (rnti, params);
}

template <class C>
void 
MemberLteEnbRrcSapUser<C>::RemoveUe (uint16_t rnti)
{
  m_owner->DoRemoveUe (rnti);
}

template <class C>
void 
MemberLteEnbRrcSapUser<C>::SendMasterInformationBlock (MasterInformationBlock msg)
{
  m_owner->DoSendMasterInformationBlock (msg);
}

template <class C>
void 
MemberLteEnbRrcSapUser<C>::SendSystemInformationBlockType1 (SystemInformationBlockType1 msg)
{
  m_owner->DoSendSystemInformationBlockType1 (msg);
}

template <class C>
void 
MemberLteEnbRrcSapUser<C>::SendSystemInformation (SystemInformation msg)
{
  m_owner->DoSendSystemInformation (msg);
}

template <class C>
void 
MemberLteEnbRrcSapUser<C>::SendRrcConnectionSetup (uint16_t rnti, RrcConnectionSetup msg) 
{
  m_owner->DoSendRrcConnectionSetup (rnti, msg) ;
}

template <class C>
void 
MemberLteEnbRrcSapUser<C>::SendRrcConnectionReconfiguration (uint16_t rnti, RrcConnectionReconfiguration msg)
{
  m_owner->DoSendRrcConnectionReconfiguration (rnti, msg);
}

template <class C>
void 
MemberLteEnbRrcSapUser<C>::SendRrcConnectionReestablishment (uint16_t rnti, RrcConnectionReestablishment msg)
{
  m_owner->DoSendRrcConnectionReestablishment (rnti, msg);
}

template <class C>
void 
MemberLteEnbRrcSapUser<C>::SendRrcConnectionReestablishmentReject (uint16_t rnti, RrcConnectionReestablishmentReject msg)
{
  m_owner->DoSendRrcConnectionReestablishmentReject (rnti, msg);
}

template <class C>
void 
MemberLteEnbRrcSapUser<C>::SendRrcConnectionRelease (uint16_t rnti, RrcConnectionRelease msg)
{
  m_owner->DoSendRrcConnectionRelease (rnti, msg);
}

template <class C>
void 
MemberLteEnbRrcSapUser<C>::SendRrcConnectionReject (uint16_t rnti, RrcConnectionReject msg)
{
  m_owner->DoSendRrcConnectionReject (rnti, msg);
}

template <class C>
Ptr<Packet> 
MemberLteEnbRrcSapUser<C>::EncodeHandoverPreparationInformation (HandoverPreparationInfo msg)
{
  return m_owner->DoEncodeHandoverPreparationInformation (msg);
}

template <class C>
LteRrcSap::HandoverPreparationInfo 
MemberLteEnbRrcSapUser<C>::DecodeHandoverPreparationInformation (Ptr<Packet> p)
{
  return m_owner->DoDecodeHandoverPreparationInformation (p);
}


template <class C>
Ptr<Packet>  
MemberLteEnbRrcSapUser<C>::EncodeHandoverCommand (RrcConnectionReconfiguration msg)
{
  return m_owner->DoEncodeHandoverCommand (msg);
}

template <class C>
LteRrcSap::RrcConnectionReconfiguration  
MemberLteEnbRrcSapUser<C>::DecodeHandoverCommand (Ptr<Packet> p)
{
  return m_owner->DoDecodeHandoverCommand (p);
}

/**
 * Template for the implementation of the LteEnbRrcSapProvider as a member
 * of an owner class of type C to which all methods are forwarded
 * 
 */
template <class C>
class MemberLteEnbRrcSapProvider : public LteEnbRrcSapProvider
{
public:
  MemberLteEnbRrcSapProvider (C* owner);

  // methods inherited from LteEnbRrcSapProvider go here  
  
  virtual void CompleteSetupUe (uint16_t rnti, CompleteSetupUeParameters params);
  virtual void RecvRrcConnectionRequest (uint16_t rnti, RrcConnectionRequest msg);
  virtual void RecvRrcConnectionSetupCompleted (uint16_t rnti, RrcConnectionSetupCompleted msg);
  virtual void RecvRrcConnectionReconfigurationCompleted (uint16_t rnti, RrcConnectionReconfigurationCompleted msg);
  virtual void RecvRrcConnectionReestablishmentRequest (uint16_t rnti, RrcConnectionReestablishmentRequest msg);
  virtual void RecvRrcConnectionReestablishmentComplete (uint16_t rnti, RrcConnectionReestablishmentComplete msg);
  virtual void RecvMeasurementReport (uint16_t rnti, MeasurementReport msg);

private:
  MemberLteEnbRrcSapProvider ();
  C* m_owner;
};

template <class C>
MemberLteEnbRrcSapProvider<C>::MemberLteEnbRrcSapProvider (C* owner)
  : m_owner (owner)
{
}

template <class C>
MemberLteEnbRrcSapProvider<C>::MemberLteEnbRrcSapProvider ()
{
}

template <class C>
void 
MemberLteEnbRrcSapProvider<C>::CompleteSetupUe (uint16_t rnti, CompleteSetupUeParameters params)
{
  m_owner->DoCompleteSetupUe (rnti, params);
}

template <class C>
void 
MemberLteEnbRrcSapProvider<C>::RecvRrcConnectionRequest (uint16_t rnti, RrcConnectionRequest msg)
{
  Simulator::ScheduleNow (&C::DoRecvRrcConnectionRequest, m_owner, rnti, msg);
}

template <class C>
void 
MemberLteEnbRrcSapProvider<C>::RecvRrcConnectionSetupCompleted (uint16_t rnti, RrcConnectionSetupCompleted msg)
{
  Simulator::ScheduleNow (&C::DoRecvRrcConnectionSetupCompleted, m_owner, rnti, msg);
}

template <class C>
void 
MemberLteEnbRrcSapProvider<C>::RecvRrcConnectionReconfigurationCompleted (uint16_t rnti, RrcConnectionReconfigurationCompleted msg)
{
  Simulator::ScheduleNow (&C::DoRecvRrcConnectionReconfigurationCompleted, m_owner, rnti, msg);
}

template <class C>
void 
MemberLteEnbRrcSapProvider<C>::RecvRrcConnectionReestablishmentRequest (uint16_t rnti, RrcConnectionReestablishmentRequest msg)
{
  Simulator::ScheduleNow (&C::DoRecvRrcConnectionReestablishmentRequest, m_owner, rnti, msg);
}

template <class C>
void 
MemberLteEnbRrcSapProvider<C>::RecvRrcConnectionReestablishmentComplete (uint16_t rnti, RrcConnectionReestablishmentComplete msg)
{
  Simulator::ScheduleNow (&C::DoRecvRrcConnectionReestablishmentComplete, m_owner, rnti, msg);
}

template <class C>
void 
MemberLteEnbRrcSapProvider<C>::RecvMeasurementReport (uint16_t rnti, MeasurementReport msg)
{
  Simulator::ScheduleNow (&C::DoRecvMeasurementReport, m_owner, rnti, msg);
}














} // namespace ns3


#endif // LTE_RRC_SAP_H




