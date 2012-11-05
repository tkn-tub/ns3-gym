/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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

#include "ns3/object.h"
#include "ns3/packet.h"
#include "ns3/lte-enb-cmac-sap.h"
#include "ns3/lte-mac-sap.h"
#include "ns3/ff-mac-sched-sap.h"
#include "ns3/lte-pdcp-sap.h"
#include "ns3/epc-x2-sap.h"
#include <ns3/epc-enb-s1-sap.h>
#include <ns3/lte-enb-cphy-sap.h>

#include <map>
#include <set>

namespace ns3 {

class LteRadioBearerInfo;
class EpcEnbS1SapUser;
class EpcEnbS1SapProvider;
class LteUeRrc;

/**
 * Manages all the radio bearer information possessed by the ENB RRC for a single UE
 *
 */
class UeInfo : public Object
{
public:
  /**
   *
   *
   * \param radioBearerInfo
   *
   * \return the allocated logical channel id; 0 is returned if it is not possible to allocate a channel id (e.g., id space exausted).
   */
  uint8_t AddRadioBearer (Ptr<LteRadioBearerInfo> radioBearerInfo);

  /**
   *
   *
   * \param uint8_t the logical channel id
   *
   * \return the LteRadioBearerInfo of the selected radio bearer
   */
  Ptr<LteRadioBearerInfo> GetRadioBearer (uint8_t lcid);


  /**
   * delete the entry of the given radio bearer
   *
   * \param lcid the logical channel id of the radio bearer
   */
  void RemoveRadioBearer (uint8_t lcid);

  /** 
   * 
   * \return a const iterator pointing to the beginning of the embedded map of radio bearers 
   */
  std::map <uint8_t, Ptr<LteRadioBearerInfo> >::const_iterator RadioBearerMapBegin ();


  /** 
   * 
   * \return a const iterator pointing to the end of the embedded map of radio bearers 
   */
  std::map <uint8_t, Ptr<LteRadioBearerInfo> >::const_iterator RadioBearerMapEnd ();

  UeInfo ();

  virtual ~UeInfo (void);

  static TypeId GetTypeId (void);

  uint64_t GetImsi (void);
  void SetImsi (uint64_t imsi);

  uint8_t GetTransmissionMode (void);  
  void SetTransmissionMode (uint8_t txMode);

  uint16_t GetSrsConfigurationIndex (void);  
  void SetSrsConfigurationIndex (uint16_t srsConfIndex);



private:
  std::map <uint8_t, Ptr<LteRadioBearerInfo> > m_rbMap;
  uint8_t m_lastAllocatedId;
  uint64_t m_imsi;
  uint16_t m_srsConfigurationIndex;
  uint8_t  m_transmissionMode;
};


/**
 * \ingroup lte
 * 
 * The LTE Radio Resource Control entity at the eNB
 */
class LteEnbRrc : public Object
{

  friend class EnbRrcMemberLteEnbCmacSapUser;
  friend class LtePdcpSpecificLtePdcpSapUser<LteEnbRrc>;
  friend class MemberEpcEnbS1SapUser<LteEnbRrc>;
  friend class EpcX2SpecificEpcX2SapUser<LteEnbRrc>;

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
  virtual void DoDispose (void);
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
   * set the FF MAC SCHED SAP provider. The eNB RRC does not use this
   * directly, but it needs to provide it to newly created RLC instances.
   *
   * \param s the FF MAC SCHED SAP provider that will be used by all
   * newly created RLC instances
   */
  void SetFfMacSchedSapProvider (FfMacSchedSapProvider* s);


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
   * configure cell-specific parameters
   *
   * \param ulBandwidth the uplink bandwdith in number of RB
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
   * receive RRC Connection Request from an UE
   * 
   * \param imsi the id of the UE
   * 
   * \return the RNTI identifying the UE in this cell
   */
  uint16_t DoRecvConnectionRequest (uint64_t imsi);

  /** 
   * receive RRC Connection Setup Completed from an UE
   * 
   * \param rnti the RNTI identifying the UE in this cell
   */
  void DoRecvConnectionSetupCompleted (uint16_t rnti);
  
  /** 
   * receive RRC Connection Reconfiguration Completed message from UE
   * 
   * \param rnti 
   * 
   */
  void DoRecvConnectionReconfigurationCompleted (uint16_t rnti);
  
  void SetCellId (uint16_t m_cellId);

  /**
   * remove a UE from the cell
   *
   * \param rnti the C-RNTI identiftying the user
   */
  void RemoveUe (uint16_t rnti);

  uint16_t GetLastAllocatedRnti () const;
  void SetLastAllocatedRnti (uint16_t lastAllocatedRnti);
  void SetUeMap (std::map<uint16_t,Ptr<UeInfo> > ueMap);
  std::map<uint16_t,Ptr<UeInfo> > GetUeMap (void) const;

  /** 
   * 
   * \param bearer the specification of an EPS bearer
   * 
   * \return the type of RLC that is to be created for the given EPS bearer
   */
  TypeId GetRlcType (EpsBearer bearer);


  /**
   *
   * Release the given radio bearer
   *
   * \param rnti the C-RNTI  of the user owning the bearer
   * \param lcId the logical channel id of the bearer to be released
   */
  void ReleaseRadioBearer (uint16_t rnti, uint8_t lcId);

  
  /** 
   * Enqueue an IP packet on the proper bearer for downlink transmission
   * 
   * \param p the packet
   * 
   * \return true if successful, false if an error occurred
   */
  bool Send (Ptr<Packet> p);

  /** 
   * set the callback used to forward data packets up the stack
   * 
   * \param void 
   * \param cb 
   */
  void SetForwardUpCallback (Callback <void, Ptr<Packet> > cb);

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

  /**
   * Allocate a new RNTI for a new UE. This is done in the following cases:
   *   * T-C-RNTI allocation upon contention-based MAC Random Access procedure
   *   * target cell RNTI allocation upon handover
   *
   * \return the newly allocated RNTI
   */
  uint16_t AddUe ();



  /**
   * Setup a new radio bearer for the given user
   *
   * \param rnti the RNTI of the user
   * \param bearer the characteristics of the bearer to be activated
   * \param teid the S1-U Tunnel Endpoint Identifier for this bearer
   *
   * \return the logical channel identifier of the radio bearer for the considered user
   */
  uint8_t SetupRadioBearer (uint16_t rnti, EpsBearer bearer, uint32_t teid);


  void DoRecvHandoverRequest (EpcX2SapUser::HandoverRequestParams params);
  void DoRecvHandoverRequestAck (EpcX2SapUser::HandoverRequestAckParams params);


  LtePdcpSapProvider* GetLtePdcpSapProvider (uint16_t rnti, uint8_t lcid);

  // PDCP SAP methods
  void DoReceiveRrcPdu (LtePdcpSapUser::ReceiveRrcPduParameters params);

  // CMAC SAP methods
  void DoRrcConfigurationUpdateInd (LteEnbCmacSapUser::UeConfig params);
  void DoNotifyLcConfigResult (uint16_t rnti, uint8_t lcid, bool success);

  // S1 SAP methods
  void DoDataRadioBearerSetupRequest (EpcEnbS1SapUser::DataRadioBearerSetupRequestParameters params);

  void PropagateRrcConnectionReconfiguration (LteUeConfig_t ueConfig);


  // management of multiple UE info instances
  uint16_t CreateUeInfo ();
  Ptr<UeInfo> GetUeInfo (uint16_t rnti);
  void RemoveUeInfo (uint16_t rnti);
  uint16_t GetNewSrsConfigurationIndex (void);
  void RemoveSrsConfigurationIndex (uint16_t srcCi);

  // methods used to talk to UE RRC directly in absence of real RRC protocol
  Ptr<LteUeRrc> GetUeRrcByImsi (uint64_t imsi);
  Ptr<LteUeRrc> GetUeRrcByRnti (uint16_t rnti);

  Callback <void, Ptr<Packet> > m_forwardUpCallback;

  EpcX2SapUser* m_x2SapUser;
  EpcX2SapProvider* m_x2SapProvider;

  LteEnbCmacSapUser* m_cmacSapUser;
  LteEnbCmacSapProvider* m_cmacSapProvider;

  FfMacSchedSapProvider* m_ffMacSchedSapProvider;
  LteMacSapProvider* m_macSapProvider;
  LtePdcpSapUser* m_pdcpSapUser;

  EpcEnbS1SapProvider* m_s1SapProvider;
  EpcEnbS1SapUser* m_s1SapUser;

  LteEnbCphySapUser* m_cphySapUser;
  LteEnbCphySapProvider* m_cphySapProvider;

  bool m_configured;
  uint16_t m_cellId;
  uint16_t m_lastAllocatedRnti;

  std::map<uint16_t, Ptr<UeInfo> > m_ueMap;  
  
  uint8_t m_defaultTransmissionMode;

  enum LteEpsBearerToRlcMapping_t m_epsBearerToRlcMapping;
  
  // SRS related attributes
  uint16_t m_srsCurrentPeriodicityId;
  std::set<uint16_t> m_ueSrsConfigurationIndexSet;
  uint16_t m_lastAllocatedConfigurationIndex;
  bool m_reconfigureUes;

};


} // namespace ns3

#endif // LTE_ENB_RRC_H
