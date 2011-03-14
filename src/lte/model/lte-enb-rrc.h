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

#include <map>

namespace ns3 {

class FfMacSchedSapProvider;
class LteMacSapProvider;
class UeInfo;

/**
 *
 *
 */
class LteEnbRrc : public Object
{

  friend class EnbRrcMemberLteEnbCmacSapUser;

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
   *
   *
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
   *
   * \return the C-RNTI of the newly added UE
   */
  uint16_t AddUe ();

  /**
   * remove a UE from the cell
   *
   * \param rnti the C-RNTI identiftying the user
   */
  void RemoveUe (uint16_t rnti);


  /**
   * Setup a new radio bearer for the given user
   *
   * \param rnti the RNTI of the user
   * \param bearer the characteristics of the bearer to be activated
   *
   * \return the logical channel identifier of the radio bearer for the considered user
   */
  uint8_t SetupRadioBearer (uint16_t rnti, EpsBearer bearer);


  /**
   *
   * Release the given radio bearer
   *
   * \param rnti the C-RNTI  of the user owning the bearer
   * \param lcId the logical channel id of the bearer to be released
   */
  void ReleaseRadioBearer (uint16_t rnti, uint8_t lcId);

private:
  void DoNotifyLcConfigResult (uint16_t rnti, uint8_t lcid, bool success);

  // management of multiple UE info instances
  uint16_t CreateUeInfo ();
  Ptr<UeInfo> GetUeInfo (uint16_t rnti);
  void RemoveUeInfo (uint16_t rnti);

  LteEnbCmacSapUser* m_cmacSapUser;
  LteEnbCmacSapProvider* m_cmacSapProvider;

  FfMacSchedSapProvider* m_ffMacSchedSapProvider;
  LteMacSapProvider* m_macSapProvider;

  bool m_configured;
  uint16_t m_lastAllocatedRnti;

  std::map<uint16_t, Ptr<UeInfo> > m_ueMap;

};


} // namespace ns3

#endif // LTE_ENB_RRC_H
