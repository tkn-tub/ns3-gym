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

#ifndef LTE_ENB_RRC_H
#define LTE_ENB_RRC_H

#include <ns3/object.h>
#include <ns3/packet.h>
#include <ns3/lte-enb-cmac-sap.h>
#include <ns3/ff-mac-common.h>
#include <ns3/lte-pdcp-sap.h>

#include <map>

namespace ns3 {

class FfMacSchedSapProvider;
class LteMacSapProvider;
class LteRadioBearerInfo;
class LtePdcpSapUser;
class LtePdcpSapProvider;


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

  UeInfo (void);
  UeInfo (uint64_t imsi);
  virtual ~UeInfo (void);

  static TypeId GetTypeId (void);

  uint64_t GetImsi (void);

private:
  std::map <uint8_t, Ptr<LteRadioBearerInfo> > m_rbMap;
  uint8_t m_lastAllocatedId;
  uint64_t m_imsi;
};


/**
 *
 *
 */
class LteEnbRrc : public Object
{

  friend class EnbRrcMemberLteEnbCmacSapUser;
  friend class LtePdcpSpecificLtePdcpSapUser<LteEnbRrc>;

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
   * configure cell-specific parameters
   *
   * \param ulBandwidth the uplink bandwdith in number of RB
   * \param dlBandwidth the downlink bandwidth in number of RB
   */
  void ConfigureCell (uint8_t ulBandwidth,
                      uint8_t dlBandwidth);

  /**
   * Add a new UE to the cell
   *
   * \param imsi IMSI of the attaching UE
   * \return the C-RNTI of the newly added UE
   */
  uint16_t AddUe (uint64_t imsi);

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
   * Setup a new radio bearer for the given user
   *
   * \param rnti the RNTI of the user
   * \param bearer the characteristics of the bearer to be activated
   * \param rlcTypeId the TypeId identifying the type of RLC to be used for this bearer.
   *
   * \return the logical channel identifier of the radio bearer for the considered user
   */
  uint8_t SetupRadioBearer (uint16_t rnti, EpsBearer bearer, TypeId rlcTypeId);


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
  
  
private:

  void DoReceiveRrcPdu (LtePdcpSapUser::ReceiveRrcPduParameters params);
  void DoRrcConfigurationUpdateInd (LteUeConfig_t params);
  
  void DoNotifyLcConfigResult (uint16_t rnti, uint8_t lcid, bool success);
  LtePdcpSapProvider* GetLtePdcpSapProvider (uint16_t rnti, uint8_t lcid);

  // management of multiple UE info instances
  uint16_t CreateUeInfo (uint64_t imsi);
  Ptr<UeInfo> GetUeInfo (uint16_t rnti);
  void RemoveUeInfo (uint16_t rnti);

  Callback <void, Ptr<Packet> > m_forwardUpCallback;

  LteEnbCmacSapUser* m_cmacSapUser;
  LteEnbCmacSapProvider* m_cmacSapProvider;

  FfMacSchedSapProvider* m_ffMacSchedSapProvider;
  LteMacSapProvider* m_macSapProvider;
  LtePdcpSapUser* m_pdcpSapUser;

  bool m_configured;
  uint16_t m_lastAllocatedRnti;

  std::map<uint16_t, Ptr<UeInfo> > m_ueMap;
  
  uint8_t m_defaultTransmissionMode;

};


} // namespace ns3

#endif // LTE_ENB_RRC_H
