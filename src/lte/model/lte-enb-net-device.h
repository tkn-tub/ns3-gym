/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 TELEMATICS LAB, DEE - Politecnico di Bari
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
 * Author: Giuseppe Piro  <g.piro@poliba.it>
 * Author: Marco Miozzo <marco.miozzo@cttc.es> : Update to FF API Architecture
 */

#ifndef LTE_ENB_NET_DEVICE_H
#define LTE_ENB_NET_DEVICE_H

#include "ns3/lte-net-device.h"
#include "ns3/event-id.h"
#include "ns3/mac48-address.h"
#include "ns3/traced-callback.h"
#include "ns3/nstime.h"
#include "ns3/lte-phy.h"
#include <vector>

namespace ns3 {

class Packet;
class PacketBurst;
class Node;
class LtePhy;
class LteEnbPhy;
class LteEnbMac;
class LteEnbRrc;
class FfMacScheduler;
class LteHandoverAlgorithm;
class LteAnr;


/**
 * \ingroup lte
 *
 * The eNodeB device implementation
 */
class LteEnbNetDevice : public LteNetDevice
{
public:
  static TypeId GetTypeId (void);

  LteEnbNetDevice ();

  virtual ~LteEnbNetDevice (void);
  virtual void DoDispose (void);

  // inherited from NetDevice
  virtual bool Send (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber);

  /**
   * \return a pointer to the MAC 
   */
  Ptr<LteEnbMac> GetMac (void) const;

  /**
   * \return a pointer to the physical layer.
   */
  Ptr<LteEnbPhy> GetPhy (void) const;

  /** 
   * \return a pointer to the Radio Resource Control instance of the eNB
   */
  Ptr<LteEnbRrc> GetRrc () const;

  /** 
   * \return the Cell Identifier of this eNB
   */
  uint16_t GetCellId () const;

  /** 
   * \return the uplink bandwidth in RBs
   */
  uint8_t GetUlBandwidth () const;

  /** 
   * \param bw the uplink bandwidth in RBs
   */
  void SetUlBandwidth (uint8_t bw);

  /** 
   * \return the downlink bandwidth in RBs
   */
  uint8_t GetDlBandwidth () const;

  /** 
   * \param bw the downlink bandwidth in RBs
   */
  void SetDlBandwidth (uint8_t bw);

  /** 
   * \return the downlink carrier frequency (EARFCN)
   */
  uint16_t GetDlEarfcn () const;

  /** 
   * \param earfcn the downlink carrier frequency (EARFCN)
   */
  void SetDlEarfcn (uint16_t earfcn);

  /** 
   * \return the uplink carrier frequency (EARFCN)
   */
  uint16_t GetUlEarfcn () const;

  /** 
   * \param earfcn the uplink carrier frequency (EARFCN)
   */
  void SetUlEarfcn (uint16_t earfcn);

  /**
   * \brief Returns the CSG ID of the eNodeB.
   * \return the Closed Subscriber Group identity
   * \sa LteEnbNetDevice::SetCsgId
   */
  uint32_t GetCsgId () const;

  /**
   * \brief Associate the eNodeB device with a particular CSG.
   * \param csgId the intended Closed Subscriber Group identity
   *
   * CSG identity is a number identifying a Closed Subscriber Group which the
   * cell belongs to. eNodeB is associated with a single CSG identity.
   *
   * The same CSG identity can also be associated to several UEs, which is
   * equivalent as enlisting these UEs as the members of this particular CSG.
   *
   * \sa LteEnbNetDevice::SetCsgIndication
   */
  void SetCsgId (uint32_t csgId);

  /**
   * \brief Returns the CSG indication flag of the eNodeB.
   * \return the CSG indication flag
   * \sa LteEnbNetDevice::SetCsgIndication
   */
  bool GetCsgIndication () const;

  /**
   * \brief Enable or disable the CSG indication flag.
   * \param csgIndication if TRUE, only CSG members are allowed to access this
   *                      cell
   *
   * When the CSG indication field is set to TRUE, only UEs which are members of
   * the CSG (i.e. same CSG ID) can gain access to the eNodeB, therefore
   * enforcing closed access mode. Otherwise, the eNodeB operates as a non-CSG
   * cell and implements open access mode.
   *
   * \note This restriction only applies to initial cell selection and
   *       EPC-enabled simulation.
   *
   * \sa LteEnbNetDevice::SetCsgIndication
   */
  void SetCsgIndication (bool csgIndication);

protected:
  // inherited from Object
  virtual void DoInitialize (void);


private:
  bool m_isConstructed;
  bool m_isConfigured;

  /**
   * \brief Propagate attributes and configuration to sub-modules.
   *
   * Several attributes (e.g., the bandwidth) are exported as the attributes of
   * the LteEnbNetDevice from a user perspective, but are actually used also in
   * other sub-modules (the RRC, the PHY, the scheduler, etc.). This method
   * takes care of updating the configuration of all these sub-modules so that
   * their copy of attribute values are in sync with the one in
   * the LteEnbNetDevice.
   */
  void UpdateConfig ();

  Ptr<LteEnbMac> m_mac;

  Ptr<LteEnbPhy> m_phy;

  Ptr<LteEnbRrc> m_rrc;

  Ptr<FfMacScheduler> m_scheduler;

  Ptr<LteHandoverAlgorithm> m_handoverAlgorithm;

  Ptr<LteAnr> m_anr;

  uint16_t m_cellId; /**< Cell Identifer. Part of the CGI, see TS 29.274, section 8.21.1  */

  uint8_t m_dlBandwidth; /**< downlink bandwidth in RBs */
  uint8_t m_ulBandwidth; /**< uplink bandwidth in RBs */

  uint16_t m_dlEarfcn;  /**< downlink carrier frequency */
  uint16_t m_ulEarfcn;  /**< uplink carrier frequency */

  uint16_t m_csgId;
  bool m_csgIndication;

}; // end of class LteEnbNetDevice

} // namespace ns3

#endif /* LTE_ENB_NET_DEVICE_H */
