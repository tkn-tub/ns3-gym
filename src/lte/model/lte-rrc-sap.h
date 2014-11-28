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
 * \ingroup lte
 *
 * \brief Class holding definition common to all UE/eNodeB SAP Users/Providers.
 *
 * See 3GPP TS 36.331 for reference.
 *
 * Note that only those values that are (expected to be) used by the
 * ns-3 model are mentioned here. The naming of the variables that are
 * defined here is the same of 36.331, except for removal of "-" and
 * conversion to CamelCase or ALL_CAPS where needed in order to follow
 * the ns-3 coding style. Due to the 1-to-1 mapping with TS 36.331,
 * detailed doxygen documentation is omitted, so please refer to
 * 36.331 for the meaning of these data structures / fields.
 *
 */
class LteRrcSap
{
public:
  virtual ~LteRrcSap ();

  // Constraint values

  static const uint8_t MaxReportCells = 255;

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

  struct CellSelectionInfo
  {
    int8_t qRxLevMin; ///< INTEGER (-70..-22), actual value = IE value * 2 [dBm].
    int8_t qQualMin; ///< INTEGER (-34..-3), actual value = IE value [dB].
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
    enum
    {
      SETUP, RESET
    } type;
    uint8_t srsBandwidthConfig;
    uint8_t srsSubframeConfig;
  };

  struct SoundingRsUlConfigDedicated
  {
    enum
    {
      SETUP, RESET
    } type;
    uint8_t srsBandwidth;
    uint16_t srsConfigIndex;
  };

  struct AntennaInfoDedicated
  {
    uint8_t transmissionMode;
  };

  struct PdschConfigCommon
  {
	int8_t referenceSignalPower;  // INTEGER (-60..50),
    int8_t pb;                    // INTEGER (0..3),
  };

  struct PdschConfigDedicated
  {
    /*
     * P_A values, TS 36.331 6.3.2 PDSCH-Config
     * ENUMERATED { dB-6, dB-4dot77, dB-3, dB-1dot77, dB0, dB1, dB2, dB3 }
     */
    enum
    {
      dB_6,
      dB_4dot77,
      dB_3,
      dB_1dot77,
      dB0,
      dB1,
      dB2,
      dB3
    };
    uint8_t pa;
  };

  static double ConvertPdschConfigDedicated2Double (PdschConfigDedicated pdschConfigDedicated)
  {
    double pa = 0;
    switch (pdschConfigDedicated.pa)
      {
      case PdschConfigDedicated::dB_6:
        pa = -6;
        break;
      case PdschConfigDedicated::dB_4dot77:
        pa = -4.77;
        break;
      case PdschConfigDedicated::dB_3:
        pa = -3;
        break;
      case PdschConfigDedicated::dB_1dot77:
        pa = -1.77;
        break;
      case PdschConfigDedicated::dB0:
        pa = 0;
        break;
      case PdschConfigDedicated::dB1:
        pa = 1;
        break;
      case PdschConfigDedicated::dB2:
        pa = 2;
        break;
      case PdschConfigDedicated::dB3:
        pa = 3;
        break;
      default:
        break;
      }
    return pa;
  }

  struct PhysicalConfigDedicated
  {
    bool haveSoundingRsUlConfigDedicated;
    SoundingRsUlConfigDedicated soundingRsUlConfigDedicated;
    bool haveAntennaInfoDedicated;
    AntennaInfoDedicated antennaInfo;
    bool havePdschConfigDedicated;
    PdschConfigDedicated pdschConfigDedicated;
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
    PdschConfigCommon pdschConfigCommon;
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

  /**
   * \brief Threshold for event evaluation.
   *
   * For RSRP-based threshold, the actual value is (value - 140) dBm. While for
   * RSRQ-based threshold, the actual value is (value - 40) / 2 dB. This is in
   * accordance with section 9.1.4 and 9.1.7 of 3GPP TS 36.133.
   *
   * \sa ns3::EutranMeasurementMapping
   */
  struct ThresholdEutra
  {
    enum
    {
      THRESHOLD_RSRP, ///< RSRP is used for the threshold.
      THRESHOLD_RSRQ ///< RSRQ is used for the threshold.
    } choice;
    uint8_t range; ///< Value range used in RSRP/RSRQ threshold.
  };

  /// Specifies criteria for triggering of an E-UTRA measurement reporting event.
  struct ReportConfigEutra
  {
    enum
    {
      EVENT,
      PERIODICAL
    } triggerType;

    enum
    {
      EVENT_A1, ///< Event A1: Serving becomes better than absolute threshold.
      EVENT_A2, ///< Event A2: Serving becomes worse than absolute threshold.
      EVENT_A3, ///< Event A3: Neighbour becomes amount of offset better than PCell.
      EVENT_A4, ///< Event A4: Neighbour becomes better than absolute threshold.
      EVENT_A5 ///< Event A5: PCell becomes worse than absolute `threshold1` AND Neighbour becomes better than another absolute `threshold2`.

    } eventId; ///< Choice of E-UTRA event triggered reporting criteria.

    ThresholdEutra threshold1; ///< Threshold for event A1, A2, A4, and A5.
    ThresholdEutra threshold2; ///< Threshold for event A5.

    /// Indicates whether or not the UE shall initiate the measurement reporting procedure when the leaving condition is met for a cell in `cellsTriggeredList`, as specified in 5.5.4.1 of 3GPP TS 36.331.
    bool reportOnLeave;

    /// Offset value for Event A3. An integer between -30 and 30. The actual value is (value * 0.5) dB.
    int8_t a3Offset;

    /// Parameter used within the entry and leave condition of an event triggered reporting condition. The actual value is (value * 0.5) dB.
    uint8_t hysteresis;

    /// Time during which specific criteria for the event needs to be met in order to trigger a measurement report.
    uint16_t timeToTrigger;

    enum
    {
      REPORT_STRONGEST_CELLS,
      REPORT_CGI
    } purpose;

    enum
    {
      RSRP, ///< Reference Signal Received Power
      RSRQ ///< Reference Signal Received Quality
    } triggerQuantity; ///< The quantities used to evaluate the triggering condition for the event, see 3GPP TS 36.214.

    enum
    {
      SAME_AS_TRIGGER_QUANTITY,
      BOTH ///< Both the RSRP and RSRQ quantities are to be included in the measurement report.
    } reportQuantity; ///< The quantities to be included in the measurement report, always assumed to be BOTH.

    /// Maximum number of cells, excluding the serving cell, to be included in the measurement report.
    uint8_t maxReportCells;

    enum
    {
      MS120,
      MS240,
      MS480,
      MS640,
      MS1024,
      MS2048,
      MS5120,
      MS10240,
      MIN1,
      MIN6,
      MIN12,
      MIN30,
      MIN60,
      SPARE3,
      SPARE2,
      SPARE1
    } reportInterval; ///< Indicates the interval between periodical reports.

    /// Number of measurement reports applicable, always assumed to be infinite.
    uint8_t reportAmount;

    ReportConfigEutra ();

  }; // end of struct ReportConfigEutra

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
    enum
    {
      SETUP, RESET
    } type;
    enum
    {
      GP0, GP1
    } gapOffsetChoice;
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
    enum
    {
      SETUP,
      RESET
    } type;
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
    CellSelectionInfo cellSelectionInfo;
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
 * \brief Part of the RRC protocol. This Service Access Point (SAP) is used by
 *        the UE RRC to send messages to the eNB. Each method defined in this
 *        class corresponds to the transmission of a message that is defined in
 *        Section 6.2.2 of TS 36.331.
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

  /**
   * \brief Send an _RRCConnectionRequest message to the serving eNodeB
   *        during an RRC connection establishment procedure
   *        (Section 5.3.3 of TS 36.331).
   * \param msg the message
   */
  virtual void SendRrcConnectionRequest (RrcConnectionRequest msg) = 0;

  /**
   * \brief Send an _RRCConnectionSetupComplete_ message to the serving eNodeB
   *        during an RRC connection establishment procedure
   *        (Section 5.3.3 of TS 36.331).
   * \param msg the message
   */
  virtual void SendRrcConnectionSetupCompleted (RrcConnectionSetupCompleted msg) = 0;

  /**
   * \brief Send an _RRCConnectionReconfigurationComplete_ message to the serving eNodeB
   *        during an RRC connection reconfiguration procedure
   *        (Section 5.3.5 of TS 36.331).
   * \param msg the message
   */
  virtual void SendRrcConnectionReconfigurationCompleted (RrcConnectionReconfigurationCompleted msg) = 0;

  /**
   * \brief Send an _RRCConnectionReestablishmentRequest_ message to the serving eNodeB
   *        during an RRC connection re-establishment procedure
   *        (Section 5.3.7 of TS 36.331).
   * \param msg the message
   */
  virtual void SendRrcConnectionReestablishmentRequest (RrcConnectionReestablishmentRequest msg) = 0;

  /**
   * \brief Send an _RRCConnectionReestablishmentComplete_ message to the serving eNodeB
   *        during an RRC connection re-establishment procedure
   *        (Section 5.3.7 of TS 36.331).
   * \param msg the message
   */
  virtual void SendRrcConnectionReestablishmentComplete (RrcConnectionReestablishmentComplete msg) = 0;

  /**
   * \brief Send a _MeasurementReport_ message to the serving eNodeB
   *        during a measurement reporting procedure
   *        (Section 5.5.5 of TS 36.331).
   * \param msg the message
   */
  virtual void SendMeasurementReport (MeasurementReport msg) = 0;

};


/**
 * \brief Part of the RRC protocol. This Service Access Point (SAP) is used to
 *        let the UE RRC receive a message from the eNB RRC. Each method defined
 *        in this class corresponds to the reception of a message that is
 *        defined in Section 6.2.2 of TS 36.331.
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

  /**
   * \brief Receive a _SystemInformation_ message from the serving eNodeB
   *        during a system information acquisition procedure
   *        (Section 5.2.2 of TS 36.331).
   * \param msg the message
   */
  virtual void RecvSystemInformation (SystemInformation msg) = 0;

  /**
   * \brief Receive an _RRCConnectionSetup_ message from the serving eNodeB
   *        during an RRC connection establishment procedure
   *        (Section 5.3.3 of TS 36.331).
   * \param msg the message
   */
  virtual void RecvRrcConnectionSetup (RrcConnectionSetup msg) = 0;

  /**
   * \brief Receive an _RRCConnectionReconfiguration_ message from the serving eNodeB
   *        during an RRC connection reconfiguration procedure
   *        (Section 5.3.5 of TS 36.331).
   * \param msg the message
   */
  virtual void RecvRrcConnectionReconfiguration (RrcConnectionReconfiguration msg) = 0;

  /**
   * \brief Receive an _RRCConnectionReestablishment_ message from the serving eNodeB
   *        during an RRC connection re-establishment procedure
   *        (Section 5.3.7 of TS 36.331).
   * \param msg the message
   */
  virtual void RecvRrcConnectionReestablishment (RrcConnectionReestablishment msg) = 0;

  /**
   * \brief Receive an _RRCConnectionReestablishmentReject_ message from the serving eNodeB
   *        during an RRC connection re-establishment procedure
   *        (Section 5.3.7 of TS 36.331).
   * \param msg the message
   */
  virtual void RecvRrcConnectionReestablishmentReject (RrcConnectionReestablishmentReject msg) = 0;

  /**
   * \brief Receive an _RRCConnectionRelease_ message from the serving eNodeB
   *        during an RRC connection release procedure
   *        (Section 5.3.8 of TS 36.331).
   * \param msg the message
   */
  virtual void RecvRrcConnectionRelease (RrcConnectionRelease msg) = 0;

  /**
   * \brief Receive an _RRCConnectionReject_ message from the serving eNodeB
   *        during an RRC connection establishment procedure
   *        (Section 5.3.3 of TS 36.331).
   * \param msg the message
   */
  virtual void RecvRrcConnectionReject (RrcConnectionReject msg) = 0;

};


/**
 * \brief Part of the RRC protocol. This Service Access Point (SAP) is used by
 *        the eNB RRC to send messages to the UE RRC.  Each method defined in
 *        this class corresponds to the transmission of a message that is
 *        defined in Section 6.2.2 of TS 36.331.
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

  /**
   * \brief Send a _SystemInformation_ message to all attached UEs
   *        during a system information acquisition procedure
   *        (Section 5.2.2 of TS 36.331).
   * \param msg the message
   */
  virtual void SendSystemInformation (SystemInformation msg) = 0;

  /**
   * \brief Send an _RRCConnectionSetup_ message to a UE
   *        during an RRC connection establishment procedure
   *        (Section 5.3.3 of TS 36.331).
   * \param rnti the RNTI of the destination UE
   * \param msg the message
   */
  virtual void SendRrcConnectionSetup (uint16_t rnti, RrcConnectionSetup msg) = 0;

  /**
   * \brief Send an _RRCConnectionReconfiguration_ message to a UE
   *        during an RRC connection reconfiguration procedure
   *        (Section 5.3.5 of TS 36.331).
   * \param rnti the RNTI of the destination UE
   * \param msg the message
   */
  virtual void SendRrcConnectionReconfiguration (uint16_t rnti, RrcConnectionReconfiguration msg) = 0;

  /**
   * \brief Send an _RRCConnectionReestablishment_ message to a UE
   *        during an RRC connection re-establishment procedure
   *        (Section 5.3.7 of TS 36.331).
   * \param rnti the RNTI of the destination UE
   * \param msg the message
   */
  virtual void SendRrcConnectionReestablishment (uint16_t rnti, RrcConnectionReestablishment msg) = 0;

  /**
   * \brief Send an _RRCConnectionReestablishmentReject_ message to a UE
   *        during an RRC connection re-establishment procedure
   *        (Section 5.3.7 of TS 36.331).
   * \param rnti the RNTI of the destination UE
   * \param msg the message
   */
  virtual void SendRrcConnectionReestablishmentReject (uint16_t rnti, RrcConnectionReestablishmentReject msg) = 0;

  /**
   * \brief Send an _RRCConnectionRelease_ message to a UE
   *        during an RRC connection release procedure
   *        (Section 5.3.8 of TS 36.331).
   * \param rnti the RNTI of the destination UE
   * \param msg the message
   */
  virtual void SendRrcConnectionRelease (uint16_t rnti, RrcConnectionRelease msg) = 0;

  /**
   * \brief Send an _RRCConnectionReject_ message to a UE
   *        during an RRC connection establishment procedure
   *        (Section 5.3.3 of TS 36.331).
   * \param rnti the RNTI of the destination UE
   * \param msg the message
   */
  virtual void SendRrcConnectionReject (uint16_t rnti, RrcConnectionReject msg) = 0;

  virtual Ptr<Packet> EncodeHandoverPreparationInformation (HandoverPreparationInfo msg) = 0;
  virtual HandoverPreparationInfo DecodeHandoverPreparationInformation (Ptr<Packet> p) = 0;
  virtual Ptr<Packet> EncodeHandoverCommand (RrcConnectionReconfiguration msg) = 0;
  virtual RrcConnectionReconfiguration DecodeHandoverCommand (Ptr<Packet> p) = 0;

};


/**
 * \brief Part of the RRC protocol. This Service Access Point (SAP) is used to
 *        let the eNB RRC receive a message from a UE RRC.  Each method defined
 *        in this class corresponds to the reception of a message that is
 *        defined in Section 6.2.2 of TS 36.331.
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

  /**
   * \brief Receive an _RRCConnectionRequest_ message from a UE
   *        during an RRC connection establishment procedure
   *        (Section 5.3.3 of TS 36.331).
   * \param rnti the RNTI of UE which sent the message
   * \param msg the message
   */
  virtual void RecvRrcConnectionRequest (uint16_t rnti,
                                         RrcConnectionRequest msg) = 0;

  /**
   * \brief Receive an _RRCConnectionSetupComplete_ message from a UE
   *        during an RRC connection establishment procedure
   *        (Section 5.3.3 of TS 36.331).
   * \param rnti the RNTI of UE which sent the message
   * \param msg the message
   */
  virtual void RecvRrcConnectionSetupCompleted (uint16_t rnti,
                                                RrcConnectionSetupCompleted msg) = 0;

  /**
   * \brief Receive an _RRCConnectionReconfigurationComplete_ message from a UE
   *        during an RRC connection reconfiguration procedure
   *        (Section 5.3.5 of TS 36.331).
   * \param rnti the RNTI of UE which sent the message
   * \param msg the message
   */
  virtual void RecvRrcConnectionReconfigurationCompleted (uint16_t rnti,
                                                          RrcConnectionReconfigurationCompleted msg) = 0;

  /**
   * \brief Receive an _RRCConnectionReestablishmentRequest_ message from a UE
   *        during an RRC connection re-establishment procedure
   *        (Section 5.3.7 of TS 36.331).
   * \param rnti the RNTI of UE which sent the message
   * \param msg the message
   */
  virtual void RecvRrcConnectionReestablishmentRequest (uint16_t rnti,
                                                        RrcConnectionReestablishmentRequest msg) = 0;

  /**
   * \brief Receive an _RRCConnectionReestablishmentComplete_ message from a UE
   *        during an RRC connection re-establishment procedure
   *        (Section 5.3.7 of TS 36.331).
   * \param rnti the RNTI of UE which sent the message
   * \param msg the message
   */
  virtual void RecvRrcConnectionReestablishmentComplete (uint16_t rnti,
                                                         RrcConnectionReestablishmentComplete msg) = 0;

  /**
   * \brief Receive a _MeasurementReport_ message from a UE
   *        during a measurement reporting procedure
   *        (Section 5.5.5 of TS 36.331).
   * \param rnti the RNTI of UE which sent the message
   * \param msg the message
   */
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
  virtual void SendMeasurementReport (MeasurementReport msg);

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
  m_owner->DoSendRrcConnectionReconfigurationCompleted (msg);
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

template <class C>
void
MemberLteUeRrcSapUser<C>::SendMeasurementReport (MeasurementReport msg)
{
  m_owner->DoSendMeasurementReport (msg);
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
MemberLteEnbRrcSapUser<C>::SendSystemInformation (SystemInformation msg)
{
  m_owner->DoSendSystemInformation (msg);
}

template <class C>
void
MemberLteEnbRrcSapUser<C>::SendRrcConnectionSetup (uint16_t rnti, RrcConnectionSetup msg)
{
  m_owner->DoSendRrcConnectionSetup (rnti, msg);
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




