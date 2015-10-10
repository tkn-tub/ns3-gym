/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
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
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 *          Mirko Banchi <mk.banchi@gmail.com>
 */

#ifndef MGT_HEADERS_H
#define MGT_HEADERS_H

#include <stdint.h>

#include "ns3/header.h"
#include "status-code.h"
#include "capability-information.h"
#include "supported-rates.h"
#include "ssid.h"
#include "ht-capabilities.h"
#include "vht-capabilities.h"

namespace ns3 {

/**
 * \ingroup wifi
 * Implement the header for management frames of type association request.
 */
class MgtAssocRequestHeader : public Header
{
public:
  MgtAssocRequestHeader ();
  ~MgtAssocRequestHeader ();

  /**
   * Set the Service Set Identifier (SSID).
   *
   * \param ssid SSID
   */
  void SetSsid (Ssid ssid);
  /**
   * Set the supported rates.
   *
   * \param rates the supported rates
   */
  void SetSupportedRates (SupportedRates rates);
  /**
   * Set the listen interval.
   *
   * \param interval the listen interval
   */
  void SetListenInterval (uint16_t interval);
  /**
   * Set the Capability information.
   *
   * \param capabilities Capability information
   */
  void SetCapabilities (CapabilityInformation capabilities);
  /**
   * Set the HT capabilities.
   *
   * \param htcapabilities HT capabilities
   */
  void SetHtCapabilities (HtCapabilities htcapabilities);
  /**
   * Set the VHT capabilities.
   *
   * \param vhtcapabilities VHT capabilities
   */
  void SetVhtCapabilities (VhtCapabilities vhtcapabilities);
  /**
   * Return the Capability information.
   *
   * \return Capability information
   */
  CapabilityInformation GetCapabilities (void) const;
  /**
   * Return the HT capabilities.
   *
   * \return HT capabilities
   */
  HtCapabilities GetHtCapabilities (void) const;
  /**
   * Return the VHT capabilities.
   *
   * \return VHT capabilities
   */
  VhtCapabilities GetVhtCapabilities (void) const;
  /**
   * Return the Service Set Identifier (SSID).
   *
   * \return SSID
   */
  Ssid GetSsid (void) const;
  /**
   * Return the supported rates.
   *
   * \return the supported rates
   */
  SupportedRates GetSupportedRates (void) const;
  /**
   * Return the listen interval.
   *
   * \return the listen interval
   */
  uint16_t GetListenInterval (void) const;

  /**
   * Register this type.
   * \return The TypeId.
   */
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);


private:
  Ssid m_ssid;                        //!< Service Set ID (SSID)
  SupportedRates m_rates;             //!< List of supported rates
  CapabilityInformation m_capability; //!< Capability information
  HtCapabilities m_htCapability;      //!< HT capabilities
  VhtCapabilities m_vhtCapability;    //!< VHT capabilities
  uint16_t m_listenInterval;
};


/**
 * \ingroup wifi
 * Implement the header for management frames of type association response.
 */
class MgtAssocResponseHeader : public Header
{
public:
  MgtAssocResponseHeader ();
  ~MgtAssocResponseHeader ();

  /**
   * Return the status code.
   *
   * \return the status code
   */
  StatusCode GetStatusCode (void);
  /**
   * Return the supported rates.
   *
   * \return the supported rates
   */
  SupportedRates GetSupportedRates (void);
  /**
   * Return the Capability information.
   *
   * \return Capability information
   */
  CapabilityInformation GetCapabilities (void) const;
  /**
   * Set the Capability information.
   *
   * \param capabilities Capability information
   */
  void SetCapabilities (CapabilityInformation capabilities);
  /**
   * Return the HT capabilities.
   *
   * \return HT capabilities
   */
  HtCapabilities GetHtCapabilities (void) const;
  /**
    * Set the VHT capabilities.
    *
    * \param vhtcapabilities VHT capabilities
    */
  void SetVhtCapabilities (VhtCapabilities vhtcapabilities);
  /**
  * Return the VHT capabilities.
  *
  * \return VHT capabilities
  */
  VhtCapabilities GetVhtCapabilities (void) const;
  /**
   * Set the HT capabilities.
   *
   * \param htcapabilities HT capabilities
   */
  void SetHtCapabilities (HtCapabilities htcapabilities);
  /**
   * Set the supported rates.
   *
   * \param rates the supported rates
   */
  void SetSupportedRates (SupportedRates rates);
  /**
   * Set the status code.
   *
   * \param code the status code
   */
  void SetStatusCode (StatusCode code);

  /**
   * Register this type.
   * \return The TypeId.
   */
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);

private:
  SupportedRates m_rates; //!< List of supported rates
  CapabilityInformation m_capability; //!< Capability information
  StatusCode m_code; //!< Status code
  uint16_t m_aid;
  HtCapabilities m_htCapability; //!< HT capabilities
  VhtCapabilities m_vhtCapability; //!< VHT capabilities
};


/**
 * \ingroup wifi
 * Implement the header for management frames of type probe request.
 */
class MgtProbeRequestHeader : public Header
{
public:
  ~MgtProbeRequestHeader ();

  /**
   * Set the Service Set Identifier (SSID).
   *
   * \param ssid SSID
   */
  void SetSsid (Ssid ssid);
  /**
   * Set the supported rates.
   *
   * \param rates the supported rates
   */
  void SetSupportedRates (SupportedRates rates);
  /**
   * Return the Service Set Identifier (SSID).
   *
   * \return SSID
   */
  Ssid GetSsid (void) const;
  /**
   * Return the supported rates.
   *
   * \return the supported rates
   */
  SupportedRates GetSupportedRates (void) const;
  /**
   * Return the HT capabilities.
   *
   * \return HT capabilities
   */
  HtCapabilities GetHtCapabilities (void) const;
  /**
   * Set the VHT capabilities.
   *
   * \param vhtcapabilities VHT capabilities
   */
  void SetVhtCapabilities (VhtCapabilities vhtcapabilities);
  /**
  * Return the VHT capabilities.
  *
  * \return VHT capabilities
  */
  VhtCapabilities GetVhtCapabilities (void) const;

  /**
   * Set the HT capabilities.
   *
   * \param htcapabilities HT capabilities
   */
  void SetHtCapabilities (HtCapabilities htcapabilities);

  /**
   * Register this type.
   * \return The TypeId.
   */
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);


private:
  Ssid m_ssid;                   //!< Service Set ID (SSID)
  SupportedRates m_rates;        //!< List of supported rates
  HtCapabilities m_htCapability; //!< HT capabilities
  VhtCapabilities m_vhtCapability; //!< VHT capabilities
};


/**
 * \ingroup wifi
 * Implement the header for management frames of type probe response.
 */
class MgtProbeResponseHeader : public Header
{
public:
  MgtProbeResponseHeader ();
  ~MgtProbeResponseHeader ();

  /**
   * Return the Service Set Identifier (SSID).
   *
   * \return SSID
   */
  Ssid GetSsid (void) const;
  /**
   * Return the beacon interval in microseconds unit.
   *
   * \return beacon interval in microseconds unit
   */
  uint64_t GetBeaconIntervalUs (void) const;
  /**
   * Return the supported rates.
   *
   * \return the supported rates
   */
  SupportedRates GetSupportedRates (void) const;
  /**
   * Return the Capability information.
   *
   * \return Capability information
   */
  CapabilityInformation GetCapabilities (void) const;
  /**
   * Return the HT capabilities.
   *
   * \return HT capabilities
   */
  HtCapabilities GetHtCapabilities (void) const;
  /**
  * Return the VHT capabilities.
  *
  * \return VHT capabilities
  */
  VhtCapabilities GetVhtCapabilities (void) const;
  /**
   * Set the Capability information.
   *
   * \param capabilities Capability information
   */
  void SetCapabilities (CapabilityInformation capabilities);
  /**
   * Set the HT capabilities.
   *
   * \param htcapabilities HT capabilities
   */
  void SetHtCapabilities (HtCapabilities htcapabilities);
  /**
   * Set the VHT capabilities.
   *
   * \param vhtcapabilities VHT capabilities
   */
  void SetVhtCapabilities (VhtCapabilities vhtcapabilities);
  /**
   * Set the Service Set Identifier (SSID).
   *
   * \param ssid SSID
   */
  void SetSsid (Ssid ssid);
  /**
   * Set the beacon interval in microseconds unit.
   *
   * \param us beacon interval in microseconds unit
   */
  void SetBeaconIntervalUs (uint64_t us);
  /**
   * Set the supported rates.
   *
   * \param rates the supported rates
   */
  void SetSupportedRates (SupportedRates rates);
  /**
   * Return the time stamp.
   *
   * \return time stamp
   */
  uint64_t GetTimestamp ();

  /**
   * Register this type.
   * \return The TypeId.
   */
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);

private:
  uint64_t m_timestamp;               //!< Timestamp
  Ssid m_ssid;                        //!< Service set ID (SSID)
  uint64_t m_beaconInterval;          //!< Beacon interval
  SupportedRates m_rates;             //!< List of supported rates
  CapabilityInformation m_capability; //!< Capability information
  HtCapabilities m_htCapability;      //!< HT capabilities
  VhtCapabilities m_vhtCapability;    //!< VHT capabilities
};


/**
 * \ingroup wifi
 * Implement the header for management frames of type beacon.
 */
class MgtBeaconHeader : public MgtProbeResponseHeader
{
public:
  /** Register this type. */
  /**
   * Register this type.
   * \return The TypeId.
   */
  static TypeId GetTypeId (void);
};


/****************************
*     Action frames
*****************************/

/**
 * \ingroup wifi
 *
 * See IEEE 802.11 chapter 7.3.1.11
 * Header format: | category: 1 | action value: 1 |
 *
 */
class WifiActionHeader : public Header
{
public:
  WifiActionHeader ();
  ~WifiActionHeader ();

  /*
   * Compatible with table 8-38 IEEE 802.11, Part11, (Year 2012)
   * Category values - see 802.11-2012 Table 8-38
   */

  enum CategoryValue //table 8-38 staring from IEEE 802.11, Part11, (Year 2012)
  {
    BLOCK_ACK = 3,
    MESH = 13,                  //Category: Mesh
    MULTIHOP = 14,              //not used so far
    SELF_PROTECTED = 15,        //Category: Self Protected
    //Since vendor specific action has no stationary Action value,the parse process is not here.
    //Refer to vendor-specific-action in wave module.
    VENDOR_SPECIFIC_ACTION = 127,
  };

  enum SelfProtectedActionValue //Category: 15 (Self Protected)
  {
    PEER_LINK_OPEN = 1,         //Mesh Peering Open
    PEER_LINK_CONFIRM = 2,      //Mesh Peering Confirm
    PEER_LINK_CLOSE = 3,        //Mesh Peering Close
    GROUP_KEY_INFORM = 4,       //Mesh Group Key Inform
    GROUP_KEY_ACK = 5,          //Mesh Group Key Acknowledge
  };

  enum MultihopActionValue
  {
    PROXY_UPDATE = 0,                   //not used so far
    PROXY_UPDATE_CONFIRMATION = 1,      //not used so far
  };

  enum MeshActionValue
  {
    LINK_METRIC_REPORT = 0,               //Action Value:0 in Category 13: Mesh
    PATH_SELECTION = 1,                   //Action Value:1 in Category 13: Mesh
    PORTAL_ANNOUNCEMENT = 2,              //Action Value:2 in Category 13: Mesh
    CONGESTION_CONTROL_NOTIFICATION = 3,  //Action Value:3 in Category 13: Mesh
    MDA_SETUP_REQUEST = 4,                //Action Value:4 in Category 13: Mesh MCCA-Setup-Request (not used so far)
    MDA_SETUP_REPLY = 5,                  //Action Value:5 in Category 13: Mesh MCCA-Setup-Reply (not used so far)
    MDAOP_ADVERTISMENT_REQUEST = 6,       //Action Value:6 in Category 13: Mesh MCCA-Advertisement-Request (not used so far)
    MDAOP_ADVERTISMENTS = 7,              //Action Value:7 in Category 13: Mesh (not used so far)
    MDAOP_SET_TEARDOWN = 8,               //Action Value:8 in Category 13: Mesh (not used so far)
    TBTT_ADJUSTMENT_REQUEST = 9,          //Action Value:9 in Category 13: Mesh (not used so far)
    TBTT_ADJUSTMENT_RESPONSE = 10,        //Action Value:10 in Category 13: Mesh (not used so far)
  };

  /**
   * Block ACK action field values
   * See 802.11 Table 8-202
   */
  enum BlockAckActionValue
  {
    BLOCK_ACK_ADDBA_REQUEST = 0,
    BLOCK_ACK_ADDBA_RESPONSE = 1,
    BLOCK_ACK_DELBA = 2
  };


  /**
   * typedef for union of different ActionValues
   */
  typedef union
  {
    enum MeshActionValue meshAction;
    enum MultihopActionValue multihopAction;
    enum SelfProtectedActionValue selfProtectedAction;
    enum BlockAckActionValue blockAck;
  } ActionValue;
  /**
   * Set action for this Action header.
   *
   * \param type category
   * \param action action
   */
  void   SetAction (enum CategoryValue type,ActionValue action);

  /**
   * Return the category value.
   *
   * \return CategoryValue
   */
  CategoryValue GetCategory ();
  /**
   * Return the action value.
   *
   * \return ActionValue
   */
  ActionValue GetAction ();

  /**
   * Register this type.
   * \return The TypeId.
   */
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId () const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize () const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);


private:
  std::string CategoryValueToString (CategoryValue value) const;
  std::string SelfProtectedActionValueToString (SelfProtectedActionValue value) const;
  uint8_t m_category; //!< Category of the action
  uint8_t m_actionValue; //!< Action value
};


/**
 * \ingroup wifi
 * Implement the header for management frames of type add block ack request.
 */
class MgtAddBaRequestHeader : public Header
{
public:
  MgtAddBaRequestHeader ();

  /**
   * Register this type.
   * \return The TypeId.
   */
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);

  /**
   * Enable delayed Block ACK.
   */
  void SetDelayedBlockAck ();
  /**
   * Enable immediate Block ACK
   */
  void SetImmediateBlockAck ();
  /**
   * Set Traffic ID (TID).
   *
   * \param tid traffic ID
   */
  void SetTid (uint8_t tid);
  /**
   * Set timeout.
   *
   * \param timeout timeout
   */
  void SetTimeout (uint16_t timeout);
  /**
   * Set buffer size.
   *
   * \param size buffer size
   */
  void SetBufferSize (uint16_t size);
  /**
   * Set the starting sequence number.
   *
   * \param seq the starting sequence number
   */
  void SetStartingSequence (uint16_t seq);
  /**
   * Enable or disable A-MSDU support.
   *
   * \param supported enable or disable A-MSDU support
   */
  void SetAmsduSupport (bool supported);

  /**
   * Return the starting sequence number.
   *
   * \return the starting sequence number
   */
  uint16_t GetStartingSequence (void) const;
  /**
   * Return the Traffic ID (TID).
   *
   * \return TID
   */
  uint8_t GetTid (void) const;
  /**
   * Return whether the Block ACK policy is immediate Block ACK.
   *
   * \return true if immediate Block ACK is being used, false otherwise
   */
  bool IsImmediateBlockAck (void) const;
  /**
   * Return the timeout.
   *
   * \return timeout
   */
  uint16_t GetTimeout (void) const;
  /**
   * Return the buffer size.
   *
   * \return the buffer size.
   */
  uint16_t GetBufferSize (void) const;
  /**
   * Return whether A-MSDU capability is supported.
   *
   * \return true is A-MSDU is supported, false otherwise
   */
  bool IsAmsduSupported (void) const;

private:
  /**
   * Return the raw parameter set.
   *
   * \return the raw parameter set
   */
  uint16_t GetParameterSet (void) const;
  /**
   * Set the parameter set from the given raw value.
   *
   * \param params raw parameter set value
   */
  void SetParameterSet (uint16_t params);
  /**
   * Return the raw sequence control.
   *
   * \return the raw sequence control
   */
  uint16_t GetStartingSequenceControl (void) const;
  /**
   * Set sequence control with the given raw value.
   *
   * \param seqControl
   */
  void SetStartingSequenceControl (uint16_t seqControl);

  uint8_t m_dialogToken;   /* Not used for now */
  uint8_t m_amsduSupport;  //!< Flag if A-MSDU is supported
  uint8_t m_policy;        //!< Block ACK policy
  uint8_t m_tid;           //!< Traffic ID
  uint16_t m_bufferSize;   //!< Buffer size
  uint16_t m_timeoutValue; //!< Timeout
  uint16_t m_startingSeq;  //!< Starting sequence number
};


/**
 * \ingroup wifi
 * Implement the header for management frames of type add block ack response.
 */
class MgtAddBaResponseHeader : public Header
{
public:
  MgtAddBaResponseHeader ();

  /**
   * Register this type.
   * \return The TypeId.
   */
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);

  /**
   * Enable delayed Block ACK.
   */
  void SetDelayedBlockAck ();
  /**
   * Enable immediate Block ACK
   */
  void SetImmediateBlockAck ();
  /**
   * Set Traffic ID (TID).
   *
   * \param tid traffic ID
   */
  void SetTid (uint8_t tid);
  /**
   * Set timeout.
   *
   * \param timeout timeout
   */
  void SetTimeout (uint16_t timeout);
  /**
   * Set buffer size.
   *
   * \param size buffer size
   */
  void SetBufferSize (uint16_t size);
  /**
   * Set the status code.
   *
   * \param code the status code
   */
  void SetStatusCode (StatusCode code);
  /**
   * Enable or disable A-MSDU support.
   *
   * \param supported enable or disable A-MSDU support
   */
  void SetAmsduSupport (bool supported);

  /**
   * Return the status code.
   *
   * \return the status code
   */
  StatusCode GetStatusCode (void) const;
  /**
   * Return the Traffic ID (TID).
   *
   * \return TID
   */
  uint8_t GetTid (void) const;
  /**
   * Return whether the Block ACK policy is immediate Block ACK.
   *
   * \return true if immediate Block ACK is being used, false otherwise
   */
  bool IsImmediateBlockAck (void) const;
  /**
   * Return the timeout.
   *
   * \return timeout
   */
  uint16_t GetTimeout (void) const;
  /**
   * Return the buffer size.
   *
   * \return the buffer size.
   */
  uint16_t GetBufferSize (void) const;
  /**
   * Return whether A-MSDU capability is supported.
   *
   * \return true is A-MSDU is supported, false otherwise
   */
  bool IsAmsduSupported (void) const;


private:
  /**
   * Return the raw parameter set.
   *
   * \return the raw parameter set
   */
  uint16_t GetParameterSet (void) const;
  /**
   * Set the parameter set from the given raw value.
   *
   * \param params raw parameter set value
   */
  void SetParameterSet (uint16_t params);

  uint8_t m_dialogToken;   /* Not used for now */
  StatusCode m_code;       //!< Status code
  uint8_t m_amsduSupport;  //!< Flag if A-MSDU is supported
  uint8_t m_policy;        //!< Block ACK policy
  uint8_t m_tid;           //!< Traffic ID
  uint16_t m_bufferSize;   //!< Buffer size
  uint16_t m_timeoutValue; //!< Timeout
};


/**
 * \ingroup wifi
 * Implement the header for management frames of type del block ack.
 */
class MgtDelBaHeader : public Header
{
public:
  MgtDelBaHeader ();

  /**
   * Register this type.
   * \return The TypeId.
   */
  static TypeId GetTypeId (void);
  // Inherited
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);

  /**
   * Check if the initiator bit in the DELBA is setted.
   *
   * \return true if the initiator bit in the DELBA is setted,
   *         false otherwise
   */
  bool IsByOriginator (void) const;
  /**
   * Return the Traffic ID (TID).
   *
   * \return TID
   */
  uint8_t GetTid (void) const;
  /**
   * Set Traffic ID (TID).
   *
   * \param tid traffic ID
   */
  void SetTid (uint8_t);
  /**
   * Set the initiator bit in the DELBA.
   */
  void SetByOriginator (void);
  /**
   * Un-set the initiator bit in the DELBA.
   */
  void SetByRecipient (void);

private:
  /**
   * Return the raw parameter set.
   *
   * \return the raw parameter set
   */
  uint16_t GetParameterSet (void) const;
  /**
   * Set the parameter set from the given raw value.
   *
   * \param params raw parameter set value
   */
  void SetParameterSet (uint16_t params);

  uint16_t m_initiator;
  uint16_t m_tid; //!< Traffic ID
  uint16_t m_reasonCode; /* Not used for now. Always set to 1: "Unspecified reason" */
};

} //namespace ns3

#endif /* MGT_HEADERS_H */
