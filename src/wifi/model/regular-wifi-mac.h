/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
 */

#ifndef REGULAR_WIFI_MAC_H
#define REGULAR_WIFI_MAC_H

#include "wifi-mac.h"
#include "qos-txop.h"
#include "ssid.h"

namespace ns3 {

class MacLow;
class MacRxMiddle;
class MacTxMiddle;
class ChannelAccessManager;
class ExtendedCapabilities;

/**
 * \brief base class for all MAC-level wifi objects.
 * \ingroup wifi
 *
 * This class encapsulates all the low-level MAC functionality and all the
 * high-level MAC functionality (association/disassociation state machines).
 *
 */
class RegularWifiMac : public WifiMac
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  RegularWifiMac ();
  virtual ~RegularWifiMac ();

  /**
   * \param slotTime the slot duration
   */
  void SetSlot (Time slotTime);
  /**
   * \param sifs the sifs duration
   */
  void SetSifs (Time sifs);
  /**
   * \param eifsNoDifs the duration of an EIFS minus DIFS.
   *
   * This value is used to calculate the EIFS depending
   * on AIFSN.
   */
  void SetEifsNoDifs (Time eifsNoDifs);
  /**
   * \param pifs the pifs duration.
   */
  void SetPifs (Time pifs);
  /**
   * \param rifs the rifs duration.
   */
  void SetRifs (Time rifs);
  /**
   * \param ctsTimeout the duration of a CTS timeout.
   */
  void SetCtsTimeout (Time ctsTimeout);
  /**
   * \param ackTimeout the duration of an ACK timeout.
   */
  void SetAckTimeout (Time ackTimeout);

  Time GetRifs (void) const;
  /**
   * \return the current PIFS duration.
   */
  Time GetPifs (void) const;
  /**
   * \return the current SIFS duration.
   */
  Time GetSifs (void) const;
  /**
   * \return the current slot duration.
   */
  Time GetSlot (void) const;
  /**
   * \return the current EIFS minus DIFS duration
   */
  Time GetEifsNoDifs (void) const;
  /**
   * \return the current CTS timeout duration.
   */
  Time GetCtsTimeout (void) const;
  /**
   * \return the current ACK timeout duration.
   */
  Time GetAckTimeout (void) const;

  /**
   * Enable or disable CTS-to-self feature.
   *
   * \param enable true if CTS-to-self is to be supported,
   *               false otherwise
   */
  void SetCtsToSelfSupported (bool enable);

  /**
   * Enable or disable short slot time feature.
   *
   * \param enable true if short slot time is to be supported,
   *               false otherwise
   */
  void SetShortSlotTimeSupported (bool enable);
  /**
   * \return whether the device supports short slot time capability.
   *
   * \return true if short slot time is supported,
   *         false otherwise.
   */
  bool GetShortSlotTimeSupported (void) const;
  /**
   * Enable or disable RIFS feature.
   *
   * \param enable true if RIFS is to be supported,
   *               false otherwise
   */
  void SetRifsSupported (bool enable);
  /**
   * \return whether the device supports RIFS capability.
   *
   * \return true if short RIFS is supported,
   *         false otherwise.
   */
  bool GetRifsSupported (void) const;

  /**
   * \return the MAC address associated to this MAC layer.
   */
  Mac48Address GetAddress (void) const;
  /**
   * \return the ssid which this MAC layer is going to try to stay in.
   */
  Ssid GetSsid (void) const;
  /**
   * \param address the current address of this MAC layer.
   */
  void SetAddress (Mac48Address address);
  /**
   * \param ssid the current ssid of this MAC layer.
   */
  void SetSsid (Ssid ssid);
  /**
   * \param bssid the BSSID of the network that this device belongs to.
   */
  void SetBssid (Mac48Address bssid);
  /**
   * \return the bssid of the network this device belongs to.
   */
  Mac48Address GetBssid (void) const;
  /**
   * \brief Sets the interface in promiscuous mode.
   *
   * Enables promiscuous mode on the interface. Note that any further
   * filtering on the incoming frame path may affect the overall
   * behavior.
   */
  void SetPromisc (void);

  /**
   * \param packet the packet to send.
   * \param to the address to which the packet should be sent.
   * \param from the address from which the packet should be sent.
   *
   * The packet should be enqueued in a tx queue, and should be
   * dequeued as soon as the channel access function determines that
   * access is granted to this MAC.  The extra parameter "from" allows
   * this device to operate in a bridged mode, forwarding received
   * frames without altering the source address.
   */
  virtual void Enqueue (Ptr<const Packet> packet, Mac48Address to, Mac48Address from);

  virtual bool SupportsSendFrom (void) const;

  /**
   * \param packet the packet to send.
   * \param to the address to which the packet should be sent.
   *
   * The packet should be enqueued in a tx queue, and should be
   * dequeued as soon as the channel access function determines that
   * access is granted to this MAC.
   */
  virtual void Enqueue (Ptr<const Packet> packet, Mac48Address to) = 0;
  /**
   * \param phy the physical layer attached to this MAC.
   */
  virtual void SetWifiPhy (const Ptr<WifiPhy> phy);
  /**
   * \return the physical layer attached to this MAC.
   */
  Ptr<WifiPhy> GetWifiPhy (void) const;
  /**
   * removes attached WifiPhy device from this MAC.
   */
  void ResetWifiPhy (void);
  /**
   * \param stationManager the station manager attached to this MAC.
   */
  virtual void SetWifiRemoteStationManager (const Ptr<WifiRemoteStationManager> stationManager);
  /**
   * \return the station manager attached to this MAC.
   */
  Ptr<WifiRemoteStationManager> GetWifiRemoteStationManager (void) const;
  /**
   * Return the extended capabilities of the device.
   *
   * \return the extended capabilities that we support
   */
  ExtendedCapabilities GetExtendedCapabilities (void) const;
  /**
   * Return the HT capabilities of the device.
   *
   * \return the HT capabilities that we support
   */
  HtCapabilities GetHtCapabilities (void) const;
  /**
   * Return the VHT capabilities of the device.
   *
   * \return the VHT capabilities that we support
   */
  VhtCapabilities GetVhtCapabilities (void) const;
  /**
   * Return the HE capabilities of the device.
   *
   * \return the HE capabilities that we support
   */
  HeCapabilities GetHeCapabilities (void) const;

  /**
   * This type defines the callback of a higher layer that a
   * WifiMac(-derived) object invokes to pass a packet up the stack.
   *
   * \param packet the packet that has been received.
   * \param from the MAC address of the device that sent the packet.
   * \param to the MAC address of the device that the packet is destined for.
   */
  typedef Callback<void, Ptr<Packet>, Mac48Address, Mac48Address> ForwardUpCallback;
  /**
   * \param upCallback the callback to invoke when a packet must be
   * forwarded up the stack.
   */
  void SetForwardUpCallback (ForwardUpCallback upCallback);
  /**
   * \param linkUp the callback to invoke when the link becomes up.
   */
  void SetLinkUpCallback (Callback<void> linkUp);
  /**
   * \param linkDown the callback to invoke when the link becomes down.
   */
  void SetLinkDownCallback (Callback<void> linkDown);

  /* Next functions are not pure virtual so non Qos WifiMacs are not
   * forced to implement them.
   */
  void SetBasicBlockAckTimeout (Time blockAckTimeout);
  Time GetBasicBlockAckTimeout (void) const;
  void SetCompressedBlockAckTimeout (Time blockAckTimeout);
  Time GetCompressedBlockAckTimeout (void) const;


protected:
  virtual void DoInitialize ();
  virtual void DoDispose ();

  Ptr<MacRxMiddle> m_rxMiddle;  //!< RX middle (de-fragmentation etc.)
  Ptr<MacTxMiddle> m_txMiddle;  //!< TX middle (aggregation etc.)
  Ptr<MacLow> m_low;        //!< MacLow (RTS, CTS, DATA, ACK etc.)
  Ptr<ChannelAccessManager> m_channelAccessManager; //!< channel access manager
  Ptr<WifiPhy> m_phy;       //!< Wifi PHY

  Ptr<WifiRemoteStationManager> m_stationManager; //!< Remote station manager (rate control, RTS/CTS/fragmentation thresholds etc.)

  ForwardUpCallback m_forwardUp; //!< Callback to forward packet up the stack
  Callback<void> m_linkUp;       //!< Callback when a link is up
  Callback<void> m_linkDown;     //!< Callback when a link is down

  Ssid m_ssid; //!< Service Set ID (SSID)

  /** This holds a pointer to the TXOP instance for this WifiMac - used
  for transmission of frames to non-QoS peers. */
  Ptr<Txop> m_txop;

  /** This type defines a mapping between an Access Category index,
  and a pointer to the corresponding channel access function */
  typedef std::map<AcIndex, Ptr<QosTxop> > EdcaQueues;

  /** This is a map from Access Category index to the corresponding
  channel access function */
  EdcaQueues m_edca;

  /**
   * Accessor for the DCF object
   *
   * \return a smart pointer to Txop
   */
  Ptr<Txop> GetTxop (void) const;

  /**
   * Accessor for the AC_VO channel access function
   *
   * \return a smart pointer to QosTxop
   */
  Ptr<QosTxop> GetVOQueue (void) const;
  /**
   * Accessor for the AC_VI channel access function
   *
   * \return a smart pointer to QosTxop
   */
  Ptr<QosTxop> GetVIQueue (void) const;
  /**
   * Accessor for the AC_BE channel access function
   *
   * \return a smart pointer to QosTxop
   */
  Ptr<QosTxop> GetBEQueue (void) const;
  /**
   * Accessor for the AC_BK channel access function
   *
   * \return a smart pointer to QosTxop
   */
  Ptr<QosTxop> GetBKQueue (void) const;

  /**
   * \param standard the phy standard to be used
   *
   * This method is called by ns3::WifiMac::ConfigureStandard to
   * complete the configuration process for a requested phy standard.
   *
   * This method may be overridden by a derived class (e.g., in order
   * to apply DCF or EDCA parameters specific to the usage model it is
   * dealing with), in which case the reimplementation may choose to
   * deal with certain values in the WifiPhyStandard enumeration, and
   * chain up to this implementation to deal with the remainder.
   */
  void FinishConfigureStandard (WifiPhyStandard standard);

  /**
   * \param cwMin the minimum congestion window size
   * \param cwMax the maximum congestion window size
   *
   * This method is called to set the minimum and the maximum
   * contention window size.
   */
  void ConfigureContentionWindow (uint32_t cwMin, uint32_t cwMax);

  /**
   * This method is invoked by a subclass to specify what type of
   * station it is implementing. This is something that the channel
   * access functions (instantiated within this class as QosTxop's)
   * need to know.
   *
   * \param type the type of station.
   */
  void SetTypeOfStation (TypeOfStation type);

  /**
   * This method acts as the MacRxMiddle receive callback and is
   * invoked to notify us that a frame has been received. The
   * implementation is intended to capture logic that is going to be
   * common to all (or most) derived classes. Specifically, handling
   * of Block Ack management frames is dealt with here.
   *
   * This method will need, however, to be overridden by derived
   * classes so that they can perform their data handling before
   * invoking the base version.
   *
   * \param packet the packet that has been received.
   * \param hdr a pointer to the MAC header of the received frame.
   */
  virtual void Receive (Ptr<Packet> packet, const WifiMacHeader *hdr);
  /**
   * The packet we sent was successfully received by the receiver
   * (i.e. we received an ACK from the receiver).
   *
   * \param hdr the header of the packet that we successfully sent
   */
  virtual void TxOk (const WifiMacHeader &hdr);
  /**
   * The packet we sent was successfully received by the receiver
   * (i.e. we did not receive an ACK from the receiver).
   *
   * \param hdr the header of the packet that we failed to sent
   */
  virtual void TxFailed (const WifiMacHeader &hdr);

  /**
   * Forward the packet up to the device.
   *
   * \param packet the packet that we are forwarding up to the device
   * \param from the address of the source
   * \param to the address of the destination
   */
  void ForwardUp (Ptr<Packet> packet, Mac48Address from, Mac48Address to);

  /**
   * This method can be called to de-aggregate an A-MSDU and forward
   * the constituent packets up the stack.
   *
   * \param aggregatedPacket the Packet containing the A-MSDU.
   * \param hdr a pointer to the MAC header for \c aggregatedPacket.
   */
  virtual void DeaggregateAmsduAndForward (Ptr<Packet> aggregatedPacket,
                                           const WifiMacHeader *hdr);

  /**
   * This method can be called to accept a received ADDBA Request. An
   * ADDBA Response will be constructed and queued for transmission.
   *
   * \param reqHdr a pointer to the received ADDBA Request header.
   * \param originator the MAC address of the originator.
   */
  void SendAddBaResponse (const MgtAddBaRequestHeader *reqHdr,
                          Mac48Address originator);

  /**
   * Enable or disable QoS support for the device.
   *
   * \param enable whether QoS is supported
   */
  virtual void SetQosSupported (bool enable);
  /**
   * Return whether the device supports QoS.
   *
   * \return true if QoS is supported, false otherwise
   */
  bool GetQosSupported () const;

  /**
   * Enable or disable HT support for the device.
   *
   * \param enable whether HT is supported
   */
  void SetHtSupported (bool enable);
  /**
   * Return whether the device supports HT.
   *
   * \return true if HT is supported, false otherwise
   */
  bool GetHtSupported () const;

  /**
   * Enable or disable VHT support for the device.
   *
   * \param enable whether VHT is supported
   */
  void SetVhtSupported (bool enable);
  /**
   * Return whether the device supports VHT.
   *
   * \return true if VHT is supported, false otherwise
   */
  bool GetVhtSupported () const;

  /**
   * Enable or disable ERP support for the device.
   *
   * \param enable whether ERP is supported
   */
  void SetErpSupported (bool enable);
  /**
   * Return whether the device supports ERP.
   *
   * \return true if ERP is supported, false otherwise
   */
  bool GetErpSupported () const;

  /**
   * Enable or disable DSSS support for the device.
   *
   * \param enable whether DSSS is supported
   */
  void SetDsssSupported (bool enable);
  /**
   * Return whether the device supports DSSS.
   *
   * \return true if DSSS is supported, false otherwise
   */
  bool GetDsssSupported () const;

  /**
   * Enable or disable HE support for the device.
   *
   * \param enable whether HE is supported
   */
  void SetHeSupported (bool enable);
  /**
   * Return whether the device supports HE.
   *
   * \return true if HE is supported, false otherwise
   */
  bool GetHeSupported () const;


private:
  /// type conversion operator
  RegularWifiMac (const RegularWifiMac &);
  /**
   * assignment operator
   *
   * \param mac the RegularWifiMac to assign
   * \returns the assigned value
   */
  RegularWifiMac & operator= (const RegularWifiMac & mac);

  /**
   * This method is a private utility invoked to configure the channel
   * access function for the specified Access Category.
   *
   * \param ac the Access Category of the queue to initialise.
   */
  void SetupEdcaQueue (AcIndex ac);

  /**
   * Set the maximum A-MSDU size for AC_VO.
   *
   * \param size the maximum A-MSDU size for AC_VO.
   */
  void SetVoMaxAmsduSize (uint16_t size);
  /**
   * Set the maximum A-MSDU size for AC_VI.
   *
   * \param size the maximum A-MSDU size for AC_VI.
   */
  void SetViMaxAmsduSize (uint16_t size);
  /**
   * Set the maximum A-MSDU size for AC_BE.
   *
   * \param size the maximum A-MSDU size for AC_BE.
   */
  void SetBeMaxAmsduSize (uint16_t size);
  /**
   * Set the maximum A-MSDU size for AC_BK.
   *
   * \param size the maximum A-MSDU size for AC_BK.
   */
  void SetBkMaxAmsduSize (uint16_t size);

  /**
   * Set the maximum A-MPDU size for AC_VO.
   *
   * \param size the maximum A-MPDU size for AC_VO.
   */
  void SetVoMaxAmpduSize (uint16_t size);
  /**
   * Set the maximum A-MPDU size for AC_VI.
   *
   * \param size the maximum A-MPDU size for AC_VI.
   */
  void SetViMaxAmpduSize (uint16_t size);
  /**
   * Set the maximum A-MPDU size for AC_BE.
   *
   * \param size the maximum A-MPDU size for AC_BE.
   */
  void SetBeMaxAmpduSize (uint16_t size);
  /**
   * Set the maximum A-MPDU size for AC_BK.
   *
   * \param size the maximum A-MPDU size for AC_BK.
   */
  void SetBkMaxAmpduSize (uint16_t size);

  /**
   * Set the Block ACK threshold for AC_VO.
   *
   * \param threshold the Block ACK threshold for AC_VO.
   */
  void SetVoBlockAckThreshold (uint8_t threshold);
  /**
   * Set the Block ACK threshold for AC_VI.
   *
   * \param threshold the Block ACK threshold for AC_VI.
   */
  void SetViBlockAckThreshold (uint8_t threshold);
  /**
   * Set the Block ACK threshold for AC_BE.
   *
   * \param threshold the Block ACK threshold for AC_BE.
   */
  void SetBeBlockAckThreshold (uint8_t threshold);
  /**
   * Set the Block ACK threshold for AC_BK.
   *
   * \param threshold the Block ACK threshold for AC_BK.
   */
  void SetBkBlockAckThreshold (uint8_t threshold);

  /**
   * Set VO block ack inactivity timeout.
   *
   * \param timeout the VO block ack inactivity timeout.
   */
  void SetVoBlockAckInactivityTimeout (uint16_t timeout);
  /**
   * Set VI block ack inactivity timeout.
   *
   * \param timeout the VI block ack inactivity timeout.
   */
  void SetViBlockAckInactivityTimeout (uint16_t timeout);
  /**
   * Set BE block ack inactivity timeout.
   *
   * \param timeout the BE block ack inactivity timeout.
   */
  void SetBeBlockAckInactivityTimeout (uint16_t timeout);
  /**
   * Set BK block ack inactivity timeout.
   *
   * \param timeout the BK block ack inactivity timeout.
   */
  void SetBkBlockAckInactivityTimeout (uint16_t timeout);

  /**
   * This Boolean is set \c true iff this WifiMac is to model
   * 802.11e/WMM style Quality of Service. It is exposed through the
   * attribute system.
   *
   * At the moment, this flag is the sole selection between QoS and
   * non-QoS operation for the STA (whether IBSS, AP, or
   * non-AP). Ultimately, we will want a QoS-enabled STA to be able to
   * fall back to non-QoS operation with a non-QoS peer. This'll
   * require further intelligence - i.e., per-association QoS
   * state. Having a big switch seems like a good intermediate stage,
   * however.
   */
  bool m_qosSupported;
  /**
    * This Boolean is set \c true iff this WifiMac is to model
    * 802.11n. It is exposed through the attribute system.
    *
    * At the moment, this flag is the sole selection between HT and
    * non-HT operation for the STA (whether IBSS, AP, or
    * non-AP). Ultimately, we will want a HT-enabled STA to be able to
    * fall back to non-HT operation with a non-HT peer. This'll
    * require further intelligence - i.e., per-association HT
    * state. Having a big switch seems like a good intermediate stage,
    * however.
    */
  bool m_htSupported;
  /**
   * This Boolean is set \c true iff this WifiMac is to model
   * 802.11ac. It is exposed through the attribute system.
   */
  bool m_vhtSupported;
  /**
   * This Boolean is set \c true iff this WifiMac is to model
   * 802.11g. It is exposed through the attribute system.
   */
  bool m_erpSupported;
  /**
   * This Boolean is set \c true iff this WifiMac is to model
   * 802.11b. It is exposed through the attribute system.
   */
  bool m_dsssSupported;
  /**
   * This Boolean is set \c true iff this WifiMac is to model
   * 802.11ax. It is exposed through the attribute system.
   */
  bool m_heSupported;

  /// Configure aggregation function
  void ConfigureAggregation (void);
  /// Enable aggregation function
  void EnableAggregation (void);
  /// Disable aggregation function
  void DisableAggregation (void);

  uint16_t m_voMaxAmsduSize; ///< maximum A-MSDU size for AC_VO
  uint16_t m_viMaxAmsduSize; ///< maximum A-MSDU size for AC_VI
  uint16_t m_beMaxAmsduSize; ///< maximum A-MSDU size for AC_BE
  uint16_t m_bkMaxAmsduSize; ///< maximum A-MSDU size for AC_BK

  uint16_t m_voMaxAmpduSize; ///< maximum A-MPDU size for AC_VO
  uint16_t m_viMaxAmpduSize; ///< maximum A-MPDU size for AC_VI
  uint16_t m_beMaxAmpduSize; ///< maximum A-MPDU size for AC_BE
  uint16_t m_bkMaxAmpduSize; ///< maximum A-MPDU size for AC_BK

  TracedCallback<const WifiMacHeader &> m_txOkCallback; ///< transmit OK callback
  TracedCallback<const WifiMacHeader &> m_txErrCallback; ///< transmit error callback

  bool m_shortSlotTimeSupported; ///< flag whether short slot time is supported
  bool m_rifsSupported; ///< flag whether RIFS is supported
};

} //namespace ns3

#endif /* REGULAR_WIFI_MAC_H */
