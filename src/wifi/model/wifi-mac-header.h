/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006, 2009 INRIA
 * Copyright (c) 2009 MIRKO BANCHI
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 * Author: Mirko Banchi <mk.banchi@gmail.com>
 */
#ifndef WIFI_MAC_HEADER_H
#define WIFI_MAC_HEADER_H

#include "ns3/header.h"
#include "ns3/mac48-address.h"
#include "ns3/nstime.h"
#include <stdint.h>

namespace ns3 {

/**
 * Combination of valid MAC header type/subtype.
 */
enum WifiMacType
{
  WIFI_MAC_CTL_RTS = 0,
  WIFI_MAC_CTL_CTS,
  WIFI_MAC_CTL_ACK,
  WIFI_MAC_CTL_BACKREQ,
  WIFI_MAC_CTL_BACKRESP,
  WIFI_MAC_CTL_CTLWRAPPER,

  WIFI_MAC_MGT_BEACON,
  WIFI_MAC_MGT_ASSOCIATION_REQUEST,
  WIFI_MAC_MGT_ASSOCIATION_RESPONSE,
  WIFI_MAC_MGT_DISASSOCIATION,
  WIFI_MAC_MGT_REASSOCIATION_REQUEST,
  WIFI_MAC_MGT_REASSOCIATION_RESPONSE,
  WIFI_MAC_MGT_PROBE_REQUEST,
  WIFI_MAC_MGT_PROBE_RESPONSE,
  WIFI_MAC_MGT_AUTHENTICATION,
  WIFI_MAC_MGT_DEAUTHENTICATION,
  WIFI_MAC_MGT_ACTION,
  WIFI_MAC_MGT_ACTION_NO_ACK,
  WIFI_MAC_MGT_MULTIHOP_ACTION,

  WIFI_MAC_DATA,
  WIFI_MAC_DATA_CFACK,
  WIFI_MAC_DATA_CFPOLL,
  WIFI_MAC_DATA_CFACK_CFPOLL,
  WIFI_MAC_DATA_NULL,
  WIFI_MAC_DATA_NULL_CFACK,
  WIFI_MAC_DATA_NULL_CFPOLL,
  WIFI_MAC_DATA_NULL_CFACK_CFPOLL,
  WIFI_MAC_QOSDATA,
  WIFI_MAC_QOSDATA_CFACK,
  WIFI_MAC_QOSDATA_CFPOLL,
  WIFI_MAC_QOSDATA_CFACK_CFPOLL,
  WIFI_MAC_QOSDATA_NULL,
  WIFI_MAC_QOSDATA_NULL_CFPOLL,
  WIFI_MAC_QOSDATA_NULL_CFACK_CFPOLL,
};

/**
 * \ingroup wifi
 *
 * Implements the IEEE 802.11 MAC header
 */
class WifiMacHeader : public Header
{
public:
  /**
   * ACK policy for QoS frames.
   */
  enum QosAckPolicy
  {
    NORMAL_ACK = 0,
    NO_ACK = 1,
    NO_EXPLICIT_ACK = 2,
    BLOCK_ACK = 3,
  };

  /**
   * Address types.
   */
  enum AddressType
  {
    ADDR1,
    ADDR2,
    ADDR3,
    ADDR4
  };

  WifiMacHeader ();
  ~WifiMacHeader ();

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);

  /**
   * Set Type/Subtype values for an association request header.
   */
  void SetAssocReq (void);
  /**
   * Set Type/Subtype values for an association response header.
   */
  void SetAssocResp (void);
  /**
   * Set Type/Subtype values for a probe request header.
   */
  void SetProbeReq (void);
  /**
   * Set Type/Subtype values for a probe response header.
   */
  void SetProbeResp (void);
  /**
   * Set Type/Subtype values for a beacon header.
   */
  void SetBeacon (void);
  /**
   * Set Type/Subtype values for a data packet with
   * no subtype equal to 0.
   */
  void SetTypeData (void);
  /**
   * Set Type/Subtype values for an action header.
   */
  void SetAction ();
  /**
   * Set Type/Subtype values for a Block Ack Request header.
   */
  void SetBlockAckReq (void);
  /**
   * Set Type/Subtype values for a Block Ack header.
   */
  void SetBlockAck (void);
  /**
   * Set Type/Subtype values for a multihop action header.
   */
  void SetMultihopAction ();
  /**
   * Set the From DS bit in the Frame Control field.
   */
  void SetDsFrom (void);
  /**
   * Un-set the From DS bit in the Frame Control field.
   */
  void SetDsNotFrom (void);
  /**
   * Set the To DS bit in the Frame Control field.
   */
  void SetDsTo (void);
  /**
   * Un-set the To DS bit in the Frame Control field.
   */
  void SetDsNotTo (void);
  /**
   * Fill the Address 1 field with the given address.
   *
   * \param address the address to be used in the Address 1 field
   */
  void SetAddr1 (Mac48Address address);
  /**
   * Fill the Address 2 field with the given address.
   *
   * \param address the address to be used in the Address 2 field
   */
  void SetAddr2 (Mac48Address address);
  /**
   * Fill the Address 3 field with the given address.
   *
   * \param address the address to be used in the Address 3 field
   */
  void SetAddr3 (Mac48Address address);
  /**
   * Fill the Address 4 field with the given address.
   *
   * \param address the address to be used in the Address 4 field
   */
  void SetAddr4 (Mac48Address address);
  /**
   * Set Type/Subtype values with the correct values depending
   * on the given type.
   *
   * \param type the WifiMacType for the header
   */
  void SetType (enum WifiMacType type);
  /**
   * Set the Duration/ID field with the given raw uint16_t value.
   *
   * \param duration the raw duration in uint16_t
   */
  void SetRawDuration (uint16_t duration);
  /**
   * Set the Duration/ID field with the given duration (Time object).
   * The method converts the given time to microseconds.
   *
   * \param duration the duration (Time object)
   */
  void SetDuration (Time duration);
  /**
   * Set the Duration/ID field with the given ID.
   *
   * \param id the ID
   */
  void SetId (uint16_t id);
  /**
   * Set the sequence number of the header.
   *
   * \param seq the given sequence number
   */
  void SetSequenceNumber (uint16_t seq);
  /**
   * Set the fragment number of the header.
   *
   * \param frag the given fragment number
   */
  void SetFragmentNumber (uint8_t frag);
  /**
   * Un-set the More Fragment bit in the Frame Control Field
   */
  void SetNoMoreFragments (void);
  /**
   * Set the More Fragment bit in the Frame Control field
   */
  void SetMoreFragments (void);
  /**
   * Set the Retry bit in the Frame Control field.
   */
  void SetRetry (void);
  /**
   * Un-set the Retry bit in the Frame Control field.
   */
  void SetNoRetry (void);
  /**
   * Set the TID for the QoS header.
   *
   * \param tid the TID for the QoS header
   */
  void SetQosTid (uint8_t tid);
  /**
   * Set the end of service period (EOSP) bit in the QoS control field.
   */
  void SetQosEosp ();
  /**
   * Un-set the end of service period (EOSP) bit in the QoS control field.
   */
  void SetQosNoEosp ();
  /**
   * Set the QoS ACK policy in the QoS control field.
   *
   * \param policy
   */
  void SetQosAckPolicy (enum QosAckPolicy policy);
  /**
   * Set the QoS ACK policy in the QoS control field to normal ACK.
   */
  void SetQosNormalAck (void);
  /**
   * Set the QoS ACK policy in the QoS control field to block ACK.
   */
  void SetQosBlockAck (void);
  /**
   * Set the QoS ACK policy in the QoS control field to no ACK.
   */
  void SetQosNoAck (void);
  /**
   * Set that A-MSDU is present.
   */
  void SetQosAmsdu (void);
  /**
   * Set that A-MSDU is not present.
   */
  void SetQosNoAmsdu (void);
  /**
   * Set TXOP limit in the QoS control field.
   *
   * \param txop
   */
  void SetQosTxopLimit (uint8_t txop);
  /**
   * Set the Mesh Control Present flag for the QoS header.
   */
  void SetQosMeshControlPresent ();
  /**
   * Clear the Mesh Control Present flag for the QoS header.
   */
  void SetQosNoMeshControlPresent ();
  /**
   * Set order bit in the frame control field.
   */
  void SetOrder (void);
  /**
   * Unset order bit in the frame control field.
   */
  void SetNoOrder (void);

  /**
   * Return the address in the Address 1 field.
   *
   * \return the address in the Address 1 field
   */
  Mac48Address GetAddr1 (void) const;
  /**
   * Return the address in the Address 2 field.
   *
   * \return the address in the Address 2 field
   */
  Mac48Address GetAddr2 (void) const;
  /**
   * Return the address in the Address 3 field.
   *
   * \return the address in the Address 3 field
   */
  Mac48Address GetAddr3 (void) const;
  /**
   * Return the address in the Address 4 field.
   *
   * \return the address in the Address 4 field
   */
  Mac48Address GetAddr4 (void) const;
  /**
   * Return the type (enum WifiMacType)
   *
   * \return the type (enum WifiMacType)
   */
  enum WifiMacType GetType (void) const;
  /**
   * \return true if From DS bit is set, false otherwise
   */
  bool IsFromDs (void) const;
  /**
   * \return true if To DS bit is set, false otherwise
   */
  bool IsToDs (void) const;
  /**
   * Return true if the Type is DATA.  The method does
   * not check the Subtype field. (e.g. the header may be
   * DATA with QoS)
   *
   * \return true if Type is DATA, false otherwise
   */
  bool IsData (void) const;
  /**
   * Return true if the Type is DATA and Subtype is one of the
   * possible values for QoS DATA.
   *
   * \return true if Type is QoS DATA, false otherwise
   */
  bool IsQosData (void) const;
  /**
   * Return true if the Type is Control.
   *
   * \return true if Type is Control, false otherwise
   */
  bool IsCtl (void) const;
  /**
   * Return true if the Type is Management.
   *
   * \return true if Type is Management, false otherwise
   */
  bool IsMgt (void) const;
  /**
   * Return true if the Type/Subtype is one of the possible CF-Poll headers.
   *
   * \return true if the Type/Subtype is one of the possible CF-Poll headers, false otherwise
   */
  bool IsCfpoll (void) const;
  /**
   * Return true if the header is a RTS header.
   *
   * \return true if the header is a RTS header, false otherwise
   */
  bool IsRts (void) const;
  /**
   * Return true if the header is a CTS header.
   *
   * \return true if the header is a CTS header, false otherwise
   */
  bool IsCts (void) const;
  /**
   * Return true if the header is an ACK header.
   *
   * \return true if the header is an ACK header, false otherwise
   */
  bool IsAck (void) const;
  /**
   * Return true if the header is a Block ACK Request header.
   *
   * \return true if the header is a Block ACK Request header, false otherwise
   */
  bool IsBlockAckReq (void) const;
  /**
   * Return true if the header is a Block ACK header.
   *
   * \return true if the header is a Block ACK header, false otherwise
   */
  bool IsBlockAck (void) const;
  /**
   * Return true if the header is an Association Request header.
   *
   * \return true if the header is an Association Request header, false otherwise
   */
  bool IsAssocReq (void) const;
  /**
   * Return true if the header is an Association Response header.
   *
   * \return true if the header is an Association Response header, false otherwise
   */
  bool IsAssocResp (void) const;
  /**
   * Return true if the header is a Reassociation Request header.
   *
   * \return true if the header is a Reassociation Request header, false otherwise
   */
  bool IsReassocReq (void) const;
  /**
   * Return true if the header is a Reassociation Response header.
   *
   * \return true if the header is a Reassociation Response header, false otherwise
   */
  bool IsReassocResp (void) const;
  /**
   * Return true if the header is a Probe Request header.
   *
   * \return true if the header is a Probe Request header, false otherwise
   */
  bool IsProbeReq (void) const;
  /**
   * Return true if the header is a Probe Response header.
   *
   * \return true if the header is a Probe Response header, false otherwise
   */
  bool IsProbeResp (void) const;
  /**
   * Return true if the header is a Beacon header.
   *
   * \return true if the header is a Beacon header, false otherwise
   */
  bool IsBeacon (void) const;
  /**
   * Return true if the header is a Disassociation header.
   *
   * \return true if the header is a Disassociation header, false otherwise
   */
  bool IsDisassociation (void) const;
  /**
   * Return true if the header is an Authentication header.
   *
   * \return true if the header is an Authentication header, false otherwise
   */
  bool IsAuthentication (void) const;
  /**
   * Return true if the header is a Deauthentication header.
   *
   * \return true if the header is a Deauthentication header, false otherwise
   */
  bool IsDeauthentication (void) const;
  /**
   * Return true if the header is an Action header.
   *
   * \return true if the header is an Action header, false otherwise
   */
  bool IsAction () const;
  /**
   * Check if the header is a Multihop action header.
   *
   * \return true if the header is a Multihop action header,
   *         false otherwise
   */
  bool IsMultihopAction () const;
  /**
   * Return the raw duration from the Duration/ID field.
   *
   * \return the raw duration from the Duration/ID field
   */
  uint16_t GetRawDuration (void) const;
  /**
   * Return the duration from the Duration/ID field (Time object).
   *
   * \return the duration from the Duration/ID field (Time object)
   */
  Time GetDuration (void) const;
  /**
   * Return the raw Sequence Control field.
   *
   * \return the raw Sequence Control field
   */
  uint16_t GetSequenceControl (void) const;
  /**
   * Return the sequence number of the header.
   *
   * \return the sequence number of the header
   */
  uint16_t GetSequenceNumber (void) const;
  /**
   * Return the fragment number of the header.
   *
   * \return the fragment number of the header
   */
  uint16_t GetFragmentNumber (void) const;
  /**
   * Return if the Retry bit is set.
   *
   * \return true if the Retry bit is set, false otherwise
   */
  bool IsRetry (void) const;
  /**
   * Return if the More Fragment bit is set.
   *
   * \return true if the More Fragment bit is set, false otherwise
   */
  bool IsMoreFragments (void) const;
  /**
   * Return if the QoS ACK policy is Block ACK.
   *
   * \return true if the QoS ACK policy is Block ACK, false otherwise
   */
  bool IsQosBlockAck (void) const;
  /**
   * Return if the QoS ACK policy is No ACK.
   *
   * \return true if the QoS ACK policy is No ACK, false otherwise
   */
  bool IsQosNoAck (void) const;
  /**
   * Return if the QoS ACK policy is Normal ACK.
   *
   * \return true if the QoS ACK policy is No ACK, false otherwise
   */
  bool IsQosAck (void) const;
  /**
   * Return if the end of service period (EOSP) is set.
   *
   * \return true if the end of service period (EOSP) is set, false otherwise
   */
  bool IsQosEosp (void) const;
  /**
   * Check if the A-MSDU present bit is set in the QoS control field.
   *
   * \return true if the A-MSDU present bit is set,
   *         false otherwise
   */
  bool IsQosAmsdu (void) const;
  /**
   * Return the Traffic ID of a QoS header.
   *
   * \return the Traffic ID of a QoS header
   */
  uint8_t GetQosTid (void) const;
  /**
   * Return the QoS ACK Policy of a QoS header.
   *
   * \return the QoS ACK Policy of a QoS header
   */
  enum QosAckPolicy GetQosAckPolicy (void) const;
  /**
   * Return the TXOP limit.
   *
   * \return the TXOP limit
   */
  uint8_t GetQosTxopLimit (void) const;
  /**
   * Return the size of the WifiMacHeader in octets.
   * GetSerializedSize calls this function.
   *
   * \return the size of the WifiMacHeader in octets
   */
  uint32_t GetSize (void) const;
  /**
   * Return a string corresponds to the header type.
   *
   * \returns a string corresponds to the header type.
   */
  const char * GetTypeString (void) const;

  /**
   * TracedCallback signature for WifiMacHeader
   *
   * \param [in] header The header
   */
  typedef void (* TracedCallback)(const WifiMacHeader &header);


private:
  /**
   * Return the raw Frame Control field.
   *
   * \return the raw Frame Control field
   */
  uint16_t GetFrameControl (void) const;
  /**
   * Return the raw QoS Control field.
   *
   * \return the raw QoS Control field
   */
  uint16_t GetQosControl (void) const;
  /**
   * Set the Frame Control field with the given raw value.
   *
   * \param control the raw Frame Control field value
   */
  void SetFrameControl (uint16_t control);
  /**
   * Set the Sequence Control field with the given raw value.
   *
   * \param seq the raw Sequence Control field value
   */
  void SetSequenceControl (uint16_t seq);
  /**
   * Set the QoS Control field with the given raw value.
   *
   * \param qos the raw QoS Control field value
   */
  void SetQosControl (uint16_t qos);
  /**
   * Print the Frame Control field to the output stream.
   *
   * \param os the output stream to print to
   */
  void PrintFrameControl (std::ostream &os) const;

  uint8_t m_ctrlType;
  uint8_t m_ctrlSubtype;
  uint8_t m_ctrlToDs;
  uint8_t m_ctrlFromDs;
  uint8_t m_ctrlMoreFrag;
  uint8_t m_ctrlRetry;
  uint8_t m_ctrlMoreData;
  uint8_t m_ctrlWep;
  uint8_t m_ctrlOrder;
  uint16_t m_duration;
  Mac48Address m_addr1;
  Mac48Address m_addr2;
  Mac48Address m_addr3;
  uint8_t m_seqFrag;
  uint16_t m_seqSeq;
  Mac48Address m_addr4;
  uint8_t m_qosTid;
  uint8_t m_qosEosp;
  uint8_t m_qosAckPolicy;
  uint8_t m_amsduPresent;
  uint16_t m_qosStuff;
};

} //namespace ns3

#endif /* WIFI_MAC_HEADER_H */
