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
 * Authors: Nicola Baldo <nbaldo@cttc.es>
 *          Marco Miozzo <mmiozzo@cttc.es>
 *          Manuel Requena <manuel.requena@cttc.es> 
 */

#ifndef LTE_ENB_RRC_H
#define LTE_ENB_RRC_H

#include <ns3/nstime.h>
#include <ns3/object.h>
#include <ns3/packet.h>
#include <ns3/lte-enb-cmac-sap.h>
#include <ns3/lte-mac-sap.h>
#include <ns3/ff-mac-sched-sap.h>
#include <ns3/lte-pdcp-sap.h>
#include <ns3/epc-x2-sap.h>
#include <ns3/epc-enb-s1-sap.h>
#include <ns3/lte-enb-cphy-sap.h>
#include <ns3/lte-rrc-sap.h>
#include <ns3/traced-callback.h>
#include <ns3/event-id.h>

#include <map>
#include <set>

namespace ns3 {

class LteRadioBearerInfo;
class LteSignalingRadioBearerInfo;
class LteDataRadioBearerInfo;
class EpcEnbS1SapUser;
class EpcEnbS1SapProvider;
class LteUeRrc;
class LteEnbRrc;

/**
 * Manages all the radio bearer information possessed by the ENB RRC for a single UE
 *
 */
class UeManager : public Object
{
  friend class LtePdcpSpecificLtePdcpSapUser<UeManager>;

public:


  /**
   * The state of the UeManager at the eNB RRC
   * 
   */
  enum State
    {
      INITIAL_RANDOM_ACCESS = 0,
      CONNECTION_SETUP,
      CONNECTION_REJECTED,
      CONNECTED_NORMALLY,
      CONNECTION_RECONFIGURATION,
      CONNECTION_REESTABLISHMENT,
      HANDOVER_PREPARATION,
      HANDOVER_JOINING,
      HANDOVER_PATH_SWITCH,
      HANDOVER_LEAVING,
      NUM_STATES
    };

  UeManager ();
 
  /** 
   * UeManager constructor
   * 
   * \param rrc pointer to the LteEnbRrc holding this UeManager
   * \param rnti RNTI of the UE
   * \param s initial state of the UeManager
   * 
   * \return 
   */
  UeManager (Ptr<LteEnbRrc> rrc, uint16_t rnti, State s);

  virtual ~UeManager (void);

  // inherited from Object
protected:
  virtual void DoInitialize ();
  virtual void DoDispose ();
public: 
  static TypeId GetTypeId (void);

  /** 
   * Set the identifiers of the source eNB for the case where a UE
   * joins the current eNB as part of a handover procedure 
   * 
   * \param sourceCellId 
   * \param sourceX2apId 
   */
  void SetSource (uint16_t sourceCellId, uint16_t sourceX2apId);

  /** 
   * Set the IMSI
   * 
   * \param imsi the IMSI
   */
  void SetImsi (uint64_t imsi);

  /** 
   * Setup a new data radio bearer, including both the configuration
   * within the eNB and the necessary RRC signaling with the UE
   * 
   * \param bearer the QoS characteristics of the bearer
   * \param bearerId the EPS bearer identifier
   * \param gtpTeid S1-bearer GTP tunnel endpoint identifier, see 36.423 9.2.1
   * \param transportLayerAddress  IP Address of the SGW, see 36.423 9.2.1
   * 
   */
  void SetupDataRadioBearer (EpsBearer bearer, uint8_t bearerId, uint32_t gtpTeid, Ipv4Address transportLayerAddress);

  /** 
   * Start all configured data radio bearers. It is safe to call this
   * method if any bearer had been already started previously.
   * 
   */
  void RecordDataRadioBearersToBeStarted ();
  
  /** 
   * Start the data radio bearers that have been previously recorded
   * to be started using RecordDataRadioBearersToBeStarted() 
   * 
   */
  void StartDataRadioBearers ();

  /**
   *
   * Release a given radio bearer
   *
   * \param drbid the data radio bearer id of the bearer to be released
   */
  void ReleaseDataRadioBearer (uint8_t drbid);

  /** 
   * schedule an RRC Connection Reconfiguration procedure with the UE
   * 
   */
  void ScheduleRrcConnectionReconfiguration ();

  /** 
   * Start the handover preparation and send the handover request
   * 
   * \param cellId id of the target cell
   */
  void PrepareHandover (uint16_t cellId);

  /** 
   * take the necessary actions in response to the reception of an X2 HANDOVER REQUEST ACK message
   * 
   * \param params 
   */
  void RecvHandoverRequestAck (EpcX2SapUser::HandoverRequestAckParams params);
  
  /** 
   * 
   * \return the HandoverPreparationInfo sent by the source eNB to the
   * target eNB in the X2-based handover procedure
   */
  LteRrcSap::RadioResourceConfigDedicated GetRadioResourceConfigForHandoverPreparationInfo ();

  /** 
   * 
   * \return retrieve the data that the target eNB needs to send to the source
   * eNB as the Handover Command in the X2-based handover
   * procedure.
   *
   * \note mobility control info is not expected to be filled in
   * (shall be filled in by the caller). 
   */
  LteRrcSap::RrcConnectionReconfiguration GetRrcConnectionReconfigurationForHandover ();

  /** 
   * Send a data packet over the appropriate Data Radio Bearer
   * 
   * \param bid the corresponding EPS Bearer ID
   * \param p the packet
   */
  void SendData (uint8_t bid, Ptr<Packet> p);

  /** 
   * 
   * \return a list of ERAB-to-be-setup items to be put in a X2 HO REQ message
   */
  std::vector<EpcX2Sap::ErabToBeSetupItem> GetErabList ();

  /** 
   * send the UE CONTEXT RELEASE X2 message to the source eNB, thus
   * successfully terminating an X2 handover procedure 
   * 
   */
  void SendUeContextRelease ();

  /** 
   * Take the necessary actions in response to the reception of an X2 HO preparation failure message
   * 
   * \param cellId id of the target cell
   */
  void RecvHandoverPreparationFailure (uint16_t cellId);

  /** 
   * Take the necessary actions in response to the reception of an X2 SN STATUS TRANSFER message
   * 
   * \param params the SN STATUS
   */
  void RecvSnStatusTransfer (EpcX2SapUser::SnStatusTransferParams params);
  

  // methods forwarded from RRC SAP
  void CompleteSetupUe (LteEnbRrcSapProvider::CompleteSetupUeParameters params);
  void RecvRrcConnectionRequest (LteRrcSap::RrcConnectionRequest msg);
  void RecvRrcConnectionSetupCompleted (LteRrcSap::RrcConnectionSetupCompleted msg);
  void RecvRrcConnectionReconfigurationCompleted (LteRrcSap::RrcConnectionReconfigurationCompleted msg);
  void RecvRrcConnectionReestablishmentRequest (LteRrcSap::RrcConnectionReestablishmentRequest msg);
  void RecvRrcConnectionReestablishmentComplete (LteRrcSap::RrcConnectionReestablishmentComplete msg);
  void RecvMeasurementReport (LteRrcSap::MeasurementReport msg);


  // methods forwarded from CMAC SAP
  void CmacUeConfigUpdateInd (LteEnbCmacSapUser::UeConfig cmacParams);


  // methods forwarded from PDCP SAP
  void DoReceivePdcpSdu (LtePdcpSapUser::ReceivePdcpSduParameters params);
  
  /** 
   * 
   * \return the RNTI, i.e., an UE identifier that is unique within
   * the cell
   */
  uint16_t GetRnti (void);

  /** 
   *
   * \return the IMSI, i.e., a globally unique UE identifier
   */
  uint64_t GetImsi (void);

  /** 
   * 
   * \return the SRS Configuration Index
   */
  uint16_t GetSrsConfigurationIndex (void);  

  /** 
   * Set the SRS configuration index and do the necessary reconfiguration
   * 
   * \param srsConfIndex 
   */
  void SetSrsConfigurationIndex (uint16_t srsConfIndex);

  /** 
   * 
   * \return the current state
   */
  State GetState ();


private:

  /** 
   * Add a new LteDataRadioBearerInfo structure to the UeManager
   * 
   * \param radioBearerInfo 
   * 
   * \return the id of the newly added data radio bearer structure
   */
  uint8_t AddDataRadioBearerInfo (Ptr<LteDataRadioBearerInfo> radioBearerInfo);

  /** 
   * \param drbid the Data Radio Bearer id
   * 
   * \return the corresponding LteDataRadioBearerInfo
   */
  Ptr<LteDataRadioBearerInfo> GetDataRadioBearerInfo (uint8_t drbid);

  /** 
   * remove the LteDataRadioBearerInfo corresponding to a bearer being released
   *
   * \param drbid the Data Radio Bearer id
   */
  void RemoveDataRadioBearerInfo (uint8_t drbid);

  /** 
   * 
   * \return an RrcConnectionReconfiguration struct built based on the
   * current configuration
   */
  LteRrcSap::RrcConnectionReconfiguration BuildRrcConnectionReconfiguration ();
  
  /** 
   * 
   * \return a RadioResourceConfigDedicated struct built based on the
   * current configuration
   */
  LteRrcSap::RadioResourceConfigDedicated BuildRadioResourceConfigDedicated ();


  /** 
   * 
   * \return a newly allocated identifier for a new RRC transaction
   */
  uint8_t GetNewRrcTransactionIdentifier ();

  /** 
   * \param lcid a Logical Channel Identifier
   * 
   * \return the corresponding Data Radio Bearer Id
   */
  uint8_t Lcid2Drbid (uint8_t lcid);

  /** 
   * \param drbid a Data Radio Bearer Id
   * 
   * \return the corresponding  Logical Channel Identifier
   */
  uint8_t Drbid2Lcid (uint8_t drbid);

  /** 
   * \param lcid a  Logical Channel Identifier
   * 
   * \return the corresponding EPS Bearer Identifier
   */
  uint8_t Lcid2Bid (uint8_t lcid);

  /** 
   * \param bid  an EPS Bearer Identifier
   * 
   * \return the corresponding Logical Channel Identifier
   */
  uint8_t Bid2Lcid (uint8_t bid);

  /** 
   * \param drbid Data Radio Bearer Id
   * 
   * \return the corresponding EPS Bearer Identifier
   */
  uint8_t Drbid2Bid (uint8_t drbid);

  /** 
   * \param bid an EPS Bearer Identifier
   * 
   * \return the corresponding Data Radio Bearer Id
   */
  uint8_t Bid2Drbid (uint8_t bid);
  
  /** 
   * Switch the UeManager to the given state
   * 
   * \param s the given state
   */
  void SwitchToState (State s);


  std::map <uint8_t, Ptr<LteDataRadioBearerInfo> > m_drbMap;
  Ptr<LteSignalingRadioBearerInfo> m_srb0;
  Ptr<LteSignalingRadioBearerInfo> m_srb1;
  uint8_t m_lastAllocatedDrbid;
  uint16_t m_rnti;
  uint64_t m_imsi;
  uint8_t m_lastRrcTransactionIdentifier;
  LteRrcSap::PhysicalConfigDedicated m_physicalConfigDedicated;
  Ptr<LteEnbRrc> m_rrc;
  State m_state;
  LtePdcpSapUser* m_drbPdcpSapUser;
  bool m_pendingRrcConnectionReconfiguration;
  //             imsi      cellid    rnti      old    new
  TracedCallback<uint64_t, uint16_t, uint16_t, State, State> m_stateTransitionTrace;
  uint16_t m_sourceX2apId;
  uint16_t m_sourceCellId;
  uint16_t m_targetCellId;
  std::list<uint8_t> m_drbsToBeStarted;
  bool m_needTransmissionModeConfiguration;
  EventId m_connectionTimeout;
};


/**
 * \ingroup lte
 * 
 * The LTE Radio Resource Control entity at the eNB
 */
class LteEnbRrc : public Object
{

  friend class EnbRrcMemberLteEnbCmacSapUser;
  friend class MemberLteEnbRrcSapProvider<LteEnbRrc>;
  friend class MemberEpcEnbS1SapUser<LteEnbRrc>;
  friend class EpcX2SpecificEpcX2SapUser<LteEnbRrc>;
  friend class UeManager;

public:
  /**
   * create an RRC instance for use within an eNB
   *
   */
  LteEnbRrc ();

  /**
   * Destructor
   */
  virtual ~LteEnbRrc ();


  // inherited from Object
protected:
  virtual void DoDispose (void);
public:
  static TypeId GetTypeId (void);


  /**
   * Set the X2 SAP this RRC should interact with
   * \param s the X2 SAP Provider to be used by this RRC entity
   */
  void SetEpcX2SapProvider (EpcX2SapProvider* s);

  /** 
   * Get the X2 SAP offered by this RRC
   * \return s the X2 SAP User interface offered to the X2 entity by this RRC entity
   */
  EpcX2SapUser* GetEpcX2SapUser ();


  /**
   * set the CMAC SAP this RRC should interact with
   *
   * \param s the CMAC SAP Provider to be used by this RRC
   */
  void SetLteEnbCmacSapProvider (LteEnbCmacSapProvider * s);

  /** 
   * Get the CMAC SAP offered by this RRC
   * \return s the CMAC SAP User interface offered to the MAC by this RRC
   */
  LteEnbCmacSapUser* GetLteEnbCmacSapUser ();


  /**
   * set the RRC SAP this RRC should interact with
   *
   * \param s the RRC SAP User to be used by this RRC
   */
  void SetLteEnbRrcSapUser (LteEnbRrcSapUser * s);

  /**
   *
   *
   * \return s the RRC SAP Provider interface offered to the MAC by this RRC
   */
  LteEnbRrcSapProvider* GetLteEnbRrcSapProvider ();

  /**
   * set the MAC SAP provider. The eNB RRC does not use this
   * directly, but it needs to provide it to newly created RLC instances.
   *
   * \param s the MAC SAP provider that will be used by all
   * newly created RLC instances
   */
  void SetLteMacSapProvider (LteMacSapProvider* s);


  /** 
   * Set the S1 SAP Provider
   * 
   * \param s the S1 SAP Provider
   */
  void SetS1SapProvider (EpcEnbS1SapProvider * s);

  /** 
   * 
   * \return the S1 SAP user
   */
  EpcEnbS1SapUser* GetS1SapUser ();


  /**
   * set the CPHY SAP this RRC should use to interact with the PHY
   *
   * \param s the CPHY SAP Provider
   */
  void SetLteEnbCphySapProvider (LteEnbCphySapProvider * s);

  /**
   *
   *
   * \return s the CPHY SAP User interface offered to the PHY by this RRC
   */
  LteEnbCphySapUser* GetLteEnbCphySapUser ();

  /** 
   * 
   * 
   * \param rnti the identifier of an UE
   * 
   * \return the corresponding UeManager instance
   */
  Ptr<UeManager> GetUeManager (uint16_t rnti);

  /**
   * configure cell-specific parameters
   *
   * \param ulBandwidth the uplink bandwidth in number of RB
   * \param dlBandwidth the downlink bandwidth in number of RB
   * \param ulEarfcn the UL EARFCN
   * \param dlEarfcn the DL EARFCN
   * \param cellId the ID of the cell
   */
  void ConfigureCell (uint8_t ulBandwidth,
                      uint8_t dlBandwidth,
                      uint16_t ulEarfcn, 
                      uint16_t dlEarfcn,
                      uint16_t cellId);
  
  /** 
   * set the cell id of this eNB
   * 
   * \param m_cellId 
   */
  void SetCellId (uint16_t m_cellId);

  /** 
   * Enqueue an IP data packet on the proper bearer for downlink
   * transmission. Normally expected to be called by the NetDevice
   * forwarding a packet coming from the EpcEnbApplication 
   * 
   * \param p the packet
   * 
   * \return true if successful, false if an error occurred
   */
  bool SendData (Ptr<Packet> p);

  /** 
   * set the callback used to forward data packets up the stack
   * 
   * \param void 
   * \param cb 
   */
  void SetForwardUpCallback (Callback <void, Ptr<Packet> > cb);

  /** 
   * Method triggered when a UE is expected to get connected but does
   * not do so in a reasonable time
   * 
   * \param rnti the T-C-RNTI whose timeout expired
   */
  void ConnectionTimeout (uint16_t rnti);

  /** 
   * Send a HandoverRequest through the X2 SAP interface
   * 
   * This method will trigger a handover which is started by the RRC
   * by sending a handover request to the target eNB over the X2
   * interface 
   *
   * \param imsi the id of the UE to be handed over 
   * \param cellId the id of the target eNB
   */
  void SendHandoverRequest (uint16_t rnti, uint16_t cellId);

  /**
   * Identifies how EPS Bearer parameters are mapped to different RLC types
   * 
   */
  enum LteEpsBearerToRlcMapping_t {RLC_SM_ALWAYS = 1,
                                   RLC_UM_ALWAYS = 2,
                                   RLC_AM_ALWAYS = 3,
                                   PER_BASED = 4};

private:


  // methods forwarded from RRC SAP
  
  void DoCompleteSetupUe (uint16_t rnti, LteEnbRrcSapProvider::CompleteSetupUeParameters params);
  void DoRecvRrcConnectionRequest (uint16_t rnti, LteRrcSap::RrcConnectionRequest msg);
  void DoRecvRrcConnectionSetupCompleted (uint16_t rnti, LteRrcSap::RrcConnectionSetupCompleted msg);
  void DoRecvRrcConnectionReconfigurationCompleted (uint16_t rnti, LteRrcSap::RrcConnectionReconfigurationCompleted msg);
  void DoRecvRrcConnectionReestablishmentRequest (uint16_t rnti, LteRrcSap::RrcConnectionReestablishmentRequest msg);
  void DoRecvRrcConnectionReestablishmentComplete (uint16_t rnti, LteRrcSap::RrcConnectionReestablishmentComplete msg);
  void DoRecvMeasurementReport (uint16_t rnti, LteRrcSap::MeasurementReport msg);


  // S1 SAP methods
  void DoDataRadioBearerSetupRequest (EpcEnbS1SapUser::DataRadioBearerSetupRequestParameters params);
  void DoPathSwitchRequestAcknowledge (EpcEnbS1SapUser::PathSwitchRequestAcknowledgeParameters params);       
  // X2 SAP methods
  void DoRecvHandoverRequest (EpcX2SapUser::HandoverRequestParams params);
  void DoRecvHandoverRequestAck (EpcX2SapUser::HandoverRequestAckParams params);
  void DoRecvHandoverPreparationFailure (EpcX2SapUser::HandoverPreparationFailureParams params);
  void DoRecvSnStatusTransfer (EpcX2SapUser::SnStatusTransferParams params);
  void DoRecvUeContextRelease (EpcX2SapUser::UeContextReleaseParams params);
  void DoRecvLoadInformation (EpcX2SapUser::LoadInformationParams params);
  void DoRecvResourceStatusUpdate (EpcX2SapUser::ResourceStatusUpdateParams params);
  void DoRecvUeData (EpcX2SapUser::UeDataParams params);


  // CMAC SAP methods
  uint16_t DoAllocateTemporaryCellRnti ();
  void DoNotifyLcConfigResult (uint16_t rnti, uint8_t lcid, bool success);
  void DoRrcConfigurationUpdateInd (LteEnbCmacSapUser::UeConfig params);
  

  // Internal methods

  /**
   * Allocate a new RNTI for a new UE. This is done in the following cases:
   *   * T-C-RNTI allocation upon contention-based MAC Random Access procedure
   *   * target cell RNTI allocation upon handover
   *
   * \param state the initial state of the UeManager
   *
   * \return the newly allocated RNTI
   */
  uint16_t AddUe (UeManager::State state);

  /**
   * remove a UE from the cell
   *
   * \param rnti the C-RNTI identiftying the user
   */
  void RemoveUe (uint16_t rnti);


  /** 
   * 
   * \param bearer the specification of an EPS bearer
   * 
   * \return the type of RLC that is to be created for the given EPS bearer
   */
  TypeId GetRlcType (EpsBearer bearer);


public:

  /** 
   * 
   * \param p the SRS periodicity in num TTIs
   */
  void SetSrsPeriodicity (uint32_t p);

  /** 
   *
   * \return the current SRS periodicity
   */
  uint32_t GetSrsPeriodicity () const;

private:

  /** 
   * Allocate a new SRS configuration index for a new UE. 
   *
   * \note this method can have the side effect of updating the SRS
   * configuration index of all UEs
   * 
   * \return the newly allocated SRS configuration index
   */
  uint16_t GetNewSrsConfigurationIndex (void);

  /** 
   * remove a previously allocated SRS configuration index
   *
   * \note this method can have the side effect of updating the SRS
   * configuration index of all UEs
   * 
   * \param srcCi the indext to be removed
   */
  void RemoveSrsConfigurationIndex (uint16_t srcCi);

 

  /** 
   * 
   * \param bearer the characteristics of the bearer
   * 
   * \return the Logical Channel Group in a bearer with these
   * characteristics is put. Used for MAC Buffer Status Reporting purposes. 
   */
  uint8_t GetLogicalChannelGroup (EpsBearer bearer);

  /** 
   * 
   * \param bearer the characteristics of the bearer
   * 
   * \return the priority level of a bearer with these
   * characteristics is put. Used for the part of UL MAC Scheduling
   * carried out by the UE
   */
  uint8_t GetLogicalChannelPriority (EpsBearer bearer);


  /** 
   * method used to periodically send System Information
   * 
   */
  void SendSystemInformation ();

  Callback <void, Ptr<Packet> > m_forwardUpCallback;

  EpcX2SapUser* m_x2SapUser;
  EpcX2SapProvider* m_x2SapProvider;

  LteEnbCmacSapUser* m_cmacSapUser;
  LteEnbCmacSapProvider* m_cmacSapProvider;

  LteEnbRrcSapUser* m_rrcSapUser;
  LteEnbRrcSapProvider* m_rrcSapProvider;

  LteMacSapProvider* m_macSapProvider;

  EpcEnbS1SapProvider* m_s1SapProvider;
  EpcEnbS1SapUser* m_s1SapUser;

  LteEnbCphySapUser* m_cphySapUser;
  LteEnbCphySapProvider* m_cphySapProvider;

  bool m_configured;
  uint16_t m_cellId;
  uint16_t m_dlEarfcn;
  uint16_t m_ulEarfcn;
  uint16_t m_dlBandwidth;
  uint16_t m_ulBandwidth;
  uint16_t m_lastAllocatedRnti;

  std::map<uint16_t, Ptr<UeManager> > m_ueMap;  

  struct X2uTeidInfo
  {
    uint16_t rnti;
    uint8_t drbid;
  };
    
  //       TEID      RNTI, DRBID
  std::map<uint32_t, X2uTeidInfo> m_x2uTeidInfoMap; 
  
  uint8_t m_defaultTransmissionMode;

  enum LteEpsBearerToRlcMapping_t m_epsBearerToRlcMapping;

  Time m_systemInformationPeriodicity;
  
  // SRS related attributes
  uint16_t m_srsCurrentPeriodicityId;
  std::set<uint16_t> m_ueSrsConfigurationIndexSet;
  uint16_t m_lastAllocatedConfigurationIndex;
  bool m_reconfigureUes;

  bool m_admitHandoverRequest;
  bool m_admitRrcConnectionRequest;

  //             cellid    rnti   
  TracedCallback<uint16_t, uint16_t> m_newUeContextTrace;
  //             imsi      cellid    rnti   
  TracedCallback<uint64_t, uint16_t, uint16_t> m_connectionEstablishedTrace;
  //             imsi      cellid    rnti   
  TracedCallback<uint64_t, uint16_t, uint16_t> m_connectionReconfigurationTrace;
  //             imsi      cellid    rnti     targetCellId
  TracedCallback<uint64_t, uint16_t, uint16_t, uint16_t> m_handoverStartTrace;
  //             imsi      cellid    rnti    
  TracedCallback<uint64_t, uint16_t, uint16_t> m_handoverEndOkTrace;

};


} // namespace ns3

#endif // LTE_ENB_RRC_H
