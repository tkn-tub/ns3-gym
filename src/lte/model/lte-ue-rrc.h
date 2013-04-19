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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 */

#ifndef LTE_UE_RRC_H
#define LTE_UE_RRC_H

#include <ns3/object.h>
#include <ns3/packet.h>
#include <ns3/lte-ue-cmac-sap.h>
#include <ns3/lte-pdcp-sap.h>
#include <ns3/lte-as-sap.h>
#include <ns3/lte-ue-cphy-sap.h>
#include <ns3/lte-rrc-sap.h>
#include <ns3/traced-callback.h>

#include <map>

namespace ns3 {

class LteRlc;
class LteMacSapProvider;
class LteUeCmacSapUser;
class LteUeCmacSapProvider;
class LteDataRadioBearerInfo;
class LteSignalingRadioBearerInfo;
class LteEnbRrc;

/**
 *
 *
 */
class LteUeRrc : public Object
{

  friend class UeMemberLteUeCmacSapUser;
  friend class UeRrcMemberLteEnbCmacSapUser;
  friend class LtePdcpSpecificLtePdcpSapUser<LteUeRrc>;
  friend class MemberLteAsSapProvider<LteUeRrc>;
  friend class MemberLteUeCphySapUser<LteUeRrc>;
  friend class MemberLteUeRrcSapProvider<LteUeRrc>;

public:

  /**
   * The states of the UE RRC entity
   * 
   */
  enum State
    {
      IDLE_CELL_SELECTION = 0,
      IDLE_WAIT_SYSTEM_INFO,
      IDLE_CAMPED_NORMALLY,
      IDLE_RANDOM_ACCESS,
      IDLE_CONNECTING,
      CONNECTED_NORMALLY,
      CONNECTED_REESTABLISHING,
      CONNECTED_HANDOVER,
      NUM_STATES
    };


  /**
   * create an RRC instance for use within an ue
   *
   */
  LteUeRrc ();


  /**
   * Destructor
   */
  virtual ~LteUeRrc ();


  // inherited from Object
private:
  virtual void DoInitialize (void);
  virtual void DoDispose (void);
public:
  static TypeId GetTypeId (void);


  /**
   * set the CPHY SAP this RRC should use to interact with the PHY
   *
   * \param s the CPHY SAP Provider
   */
  void SetLteUeCphySapProvider (LteUeCphySapProvider * s);

  /**
   *
   *
   * \return s the CPHY SAP User interface offered to the PHY by this RRC
   */
  LteUeCphySapUser* GetLteUeCphySapUser ();

  /**
   * set the CMAC SAP this RRC should interact with
   *
   * \param s the CMAC SAP Provider to be used by this RRC
   */
  void SetLteUeCmacSapProvider (LteUeCmacSapProvider * s);

  /**
   *
   *
   * \return s the CMAC SAP User interface offered to the MAC by this RRC
   */
  LteUeCmacSapUser* GetLteUeCmacSapUser ();


  /**
   * set the RRC SAP this RRC should interact with
   *
   * \param s the RRC SAP User to be used by this RRC
   */
  void SetLteUeRrcSapUser (LteUeRrcSapUser * s);

  /**
   *
   *
   * \return s the RRC SAP Provider interface offered to the MAC by this RRC
   */
  LteUeRrcSapProvider* GetLteUeRrcSapProvider ();

  /**
   * set the MAC SAP provider. The ue RRC does not use this
   * directly, but it needs to provide it to newly created RLC instances.
   *
   * \param s the MAC SAP provider that will be used by all
   * newly created RLC instances
   */
  void SetLteMacSapProvider (LteMacSapProvider* s);

  /** 
   * Set the AS SAP user to interact with the NAS entity
   * 
   * \param s the AS SAP user
   */
  void SetAsSapUser (LteAsSapUser* s);

  /** 
   * 
   * 
   * \return the AS SAP provider exported by this RRC
   */
  LteAsSapProvider* GetAsSapProvider ();

  /** 
   * 
   * \param imsi the unique UE identifier
   */
  void SetImsi (uint64_t imsi);

  /**
   *
   * \return imsi the unique UE identifier
   */
  uint64_t GetImsi (void);


  /**
   *
   * \return the C-RNTI of the user
   */
  uint16_t GetRnti () const;


  /**
   *
   * \return the CellId of the attached Enb
   */
  uint16_t GetCellId () const;


  /** 
   * \return the uplink bandwidth in RBs
   */
  uint8_t GetUlBandwidth () const;

  /** 
   * \return the downlink bandwidth in RBs
   */
  uint8_t GetDlBandwidth () const;

  /** 
   * \return the downlink carrier frequency (EARFCN)
   */
  uint16_t GetDlEarfcn () const;

  /** 
   * \return the uplink carrier frequency (EARFCN)
   */
  uint16_t GetUlEarfcn () const;

  /** 
   * 
   * \return the current state
   */
  State GetState ();

  /** 
   * 
   * 
   * \param val true if RLC SM is to be used, false if RLC UM/AM are to be used
   */
  void SetUseRlcSm (bool val);


private:


  // PDCP SAP methods
  void DoReceivePdcpSdu (LtePdcpSapUser::ReceivePdcpSduParameters params);

  // CMAC SAP methods
  void DoSetTemporaryCellRnti (uint16_t rnti);
  void DoNotifyRandomAccessSuccessful ();
  void DoNotifyRandomAccessFailed ();
 
  // LTE AS SAP methods
  void DoForceCampedOnEnb (uint16_t cellId, uint16_t earfcn);
  void DoConnect ();
  void DoSendData (Ptr<Packet> packet, uint8_t bid);
  void DoDisconnect ();

  // CPHY SAP methods
  void DoRecvMasterInformationBlock (LteRrcSap::MasterInformationBlock msg);

  // RRC SAP methods
  void DoCompleteSetup (LteUeRrcSapProvider::CompleteSetupParameters params);
  void DoRecvSystemInformationBlockType1 (LteRrcSap::SystemInformationBlockType1 msg);
  void DoRecvSystemInformation (LteRrcSap::SystemInformation msg);
  void DoRecvRrcConnectionSetup (LteRrcSap::RrcConnectionSetup msg);
  void DoRecvRrcConnectionReconfiguration (LteRrcSap::RrcConnectionReconfiguration msg);
  void DoRecvRrcConnectionReestablishment (LteRrcSap::RrcConnectionReestablishment msg);
  void DoRecvRrcConnectionReestablishmentReject (LteRrcSap::RrcConnectionReestablishmentReject msg);
  void DoRecvRrcConnectionRelease (LteRrcSap::RrcConnectionRelease msg);
  void DoRecvRrcConnectionReject (LteRrcSap::RrcConnectionReject msg);

 
  // internal methods
  void ApplyRadioResourceConfigDedicated (LteRrcSap::RadioResourceConfigDedicated rrcd);
  void StartConnection ();
  void LeaveConnectedMode ();
  void DisposeOldSrb1 ();
  uint8_t Bid2Drbid (uint8_t bid);
  void SwitchToState (State s);

  std::map<uint8_t, uint8_t> m_bid2DrbidMap;

  LteUeCphySapUser* m_cphySapUser;
  LteUeCphySapProvider* m_cphySapProvider;

  LteUeCmacSapUser* m_cmacSapUser;
  LteUeCmacSapProvider* m_cmacSapProvider;

  LteUeRrcSapUser* m_rrcSapUser;
  LteUeRrcSapProvider* m_rrcSapProvider;

  LteMacSapProvider* m_macSapProvider;
  LtePdcpSapUser* m_drbPdcpSapUser;
  
  LteAsSapProvider* m_asSapProvider;
  LteAsSapUser* m_asSapUser;

  State m_state;

  uint64_t m_imsi;
  uint16_t m_rnti;
  uint16_t m_cellId;

  Ptr<LteSignalingRadioBearerInfo> m_srb0;
  Ptr<LteSignalingRadioBearerInfo> m_srb1;
  Ptr<LteSignalingRadioBearerInfo> m_srb1Old;
  std::map <uint8_t, Ptr<LteDataRadioBearerInfo> > m_drbMap;
  
  bool m_useRlcSm;

  uint8_t m_lastRrcTransactionIdentifier;

  uint8_t m_dlBandwidth; /**< downlink bandwidth in RBs */
  uint8_t m_ulBandwidth; /**< uplink bandwidth in RBs */

  uint16_t m_dlEarfcn;  /**< downlink carrier frequency */
  uint16_t m_ulEarfcn;  /**< uplink carrier frequency */

  //             imsi      cellid    rnti   
  TracedCallback<uint64_t, uint16_t, uint16_t, State, State> m_stateTransitionTrace;
  //             imsi      cellid    rnti   
  TracedCallback<uint64_t, uint16_t, uint16_t> m_randomAccessSuccessfulTrace;
  //             imsi      cellid    rnti   
  TracedCallback<uint64_t, uint16_t, uint16_t> m_connectionEstablishedTrace;
  //             imsi      cellid    rnti   
  TracedCallback<uint64_t, uint16_t, uint16_t> m_connectionReconfigurationTrace;
  //             imsi      cellid    rnti     targetCellId
  TracedCallback<uint64_t, uint16_t, uint16_t, uint16_t> m_handoverStartTrace;
  //             imsi      cellid    rnti    
  TracedCallback<uint64_t, uint16_t, uint16_t> m_handoverEndOkTrace;

  bool m_connectionPending; /**< true if a connection request by upper layers is pending */
  bool m_receivedMib; /**< true if MIB was received for the current cell  */
  bool m_receivedSib2; /**< true if SIB2 was received for the current cell  */

};


} // namespace ns3

#endif // LTE_UE_RRC_H
