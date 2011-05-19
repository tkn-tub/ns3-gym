/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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

#ifndef WIFI_PHY_H
#define WIFI_PHY_H

#include <stdint.h>
#include "ns3/callback.h"
#include "ns3/packet.h"
#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/ptr.h"
#include "wifi-mode.h"
#include "wifi-preamble.h"
#include "wifi-phy-standard.h"
#include "ns3/traced-callback.h"


namespace ns3 {

class WifiChannel;
class NetDevice;

/**
 * \brief receive notifications about phy events.
 */
class WifiPhyListener
{
public:
  virtual ~WifiPhyListener ();

  /**
   * \param duration the expected duration of the packet reception.
   *
   * We have received the first bit of a packet. We decided
   * that we could synchronize on this packet. It does not mean
   * we will be able to successfully receive completely the
   * whole packet. It means that we will report a BUSY status until
   * one of the following happens:
   *   - NotifyRxEndOk
   *   - NotifyExEndError
   *   - NotifyTxStart
   */
  virtual void NotifyRxStart (Time duration) = 0;
  /**
   * We have received the last bit of a packet for which
   * NotifyRxStart was invoked first and, the packet has
   * been successfully received.
   */
  virtual void NotifyRxEndOk (void) = 0;
  /**
   * We have received the last bit of a packet for which
   * NotifyRxStart was invoked first and, the packet has
   * _not_ been successfully received.
   */
  virtual void NotifyRxEndError (void) = 0;
  /**
   * \param duration the expected transmission duration.
   *
   * We are about to send the first bit of the packet.
   * We do not send any event to notify the end of
   * transmission. Listeners should assume that the
   * channel implicitely reverts to the idle state
   * unless they have received a cca busy report.
   */
  virtual void NotifyTxStart (Time duration) = 0;

  /**
   * \param duration the expected busy duration.
   *
   * This method does not really report a real state
   * change as opposed to the other methods in this class.
   * It merely reports that, unless the medium is reported
   * busy through NotifyTxStart or NotifyRxStart/End,
   * it will be busy as defined by the currently selected
   * CCA mode.
   *
   * Typical client code which wants to have a clear picture
   * of the CCA state will need to keep track of the time at
   * which the last NotifyCcaBusyStart method is called and
   * what duration it reported.
   */
  virtual void NotifyMaybeCcaBusyStart (Time duration) = 0;
  /**
   * \param duration the expected channel switching duration.
   *
   * We do not send any event to notify the end of
   * channel switching. Listeners should assume that the
   * channel implicitely reverts to the idle or busy states.
   */
  virtual void NotifySwitchingStart (Time duration) = 0;
};


/**
 * \brief 802.11 PHY layer model
 * \ingroup wifi
 *
 */
class WifiPhy : public Object
{
public:
  /**
   * The state of the PHY layer.
   */
  enum State
  {
    /**
     * The PHY layer is IDLE.
     */
    IDLE,
    /**
     * The PHY layer has sense the medium busy through the CCA mechanism
     */
    CCA_BUSY,
    /**
     * The PHY layer is sending a packet.
     */
    TX,
    /**
     * The PHY layer is receiving a packet.
     */
    RX,
    /**
     * The PHY layer is switching to other channel.
     */
    SWITCHING
  };

  /**
   * arg1: packet received successfully
   * arg2: snr of packet
   * arg3: mode of packet
   * arg4: type of preamble used for packet.
   */
  typedef Callback<void,Ptr<Packet>, double, WifiMode, enum WifiPreamble> RxOkCallback;
  /**
   * arg1: packet received unsuccessfully
   * arg2: snr of packet
   */
  typedef Callback<void,Ptr<const Packet>, double> RxErrorCallback;

  static TypeId GetTypeId (void);

  WifiPhy ();
  virtual ~WifiPhy ();

  virtual double GetTxPowerStart (void) const = 0;
  virtual double GetTxPowerEnd (void) const = 0;
  /**
   * \returns the number of tx power levels available for this PHY.
   */
  virtual uint32_t GetNTxPower (void) const = 0;

  /**
   * \param callback the callback to invoke
   *        upon successful packet reception.
   */
  virtual void SetReceiveOkCallback (RxOkCallback callback) = 0;
  /**
   * \param callback the callback to invoke
   *        upon erroneous packet reception.
   */
  virtual void SetReceiveErrorCallback (RxErrorCallback callback) = 0;

  /**
   * \param packet the packet to send
   * \param mode the transmission mode to use to send this packet
   * \param preamble the type of preamble to use to send this packet.
   * \param txPowerLevel a power level to use to send this packet. The real
   *        transmission power is calculated as txPowerMin + txPowerLevel * (txPowerMax - txPowerMin) / nTxLevels
   */
  virtual void SendPacket (Ptr<const Packet> packet, WifiMode mode, enum WifiPreamble preamble, uint8_t txPowerLevel) = 0;

  /**
   * \param listener the new listener
   *
   * Add the input listener to the list of objects to be notified of
   * PHY-level events.
   */
  virtual void RegisterListener (WifiPhyListener *listener) = 0;

  /**
   * \returns true of the current state of the PHY layer is WifiPhy::IDLE, false otherwise.
   */
  virtual bool IsStateIdle (void) = 0;
  /**
   * \returns true of the current state of the PHY layer is WifiPhy::CCA_BUSY, false otherwise.
   */
  virtual bool IsStateCcaBusy (void) = 0;
  /**
   * \returns true of the current state of the PHY layer is not WifiPhy::IDLE, false otherwise.
   */
  virtual bool IsStateBusy (void) = 0;
  /**
   * \returns true of the current state of the PHY layer is WifiPhy::RX, false otherwise.
   */
  virtual bool IsStateRx (void) = 0;
  /**
   * \returns true of the current state of the PHY layer is WifiPhy::TX, false otherwise.
   */
  virtual bool IsStateTx (void) = 0;
  /**
   * \returns true of the current state of the PHY layer is WifiPhy::SWITCHING, false otherwise.
   */
  virtual bool IsStateSwitching (void) = 0;
  /**
   * \returns the amount of time since the current state has started.
   */
  virtual Time GetStateDuration (void) = 0;
  /**
   * \returns the predicted delay until this PHY can become WifiPhy::IDLE.
   *
   * The PHY will never become WifiPhy::IDLE _before_ the delay returned by
   * this method but it could become really idle later.
   */
  virtual Time GetDelayUntilIdle (void) = 0;

  virtual Time GetLastRxStartTime (void) const = 0;

  /**
   * \param size the number of bytes in the packet to send
   * \param payloadMode the transmission mode to use for this packet
   * \param preamble the type of preamble to use for this packet.
   * \return the total amount of time this PHY will stay busy for
   *          the transmission of these bytes.
   */
  static Time CalculateTxDuration (uint32_t size, WifiMode payloadMode, enum WifiPreamble preamble);

  /** 
   * \param payloadMode the WifiMode use for the transmission of the payload
   * \param preamble the type of preamble
   * 
   * \return the WifiMode used for the transmission of the PLCP header 
   */
  static WifiMode GetPlcpHeaderMode (WifiMode payloadMode, WifiPreamble preamble);

  /** 
   * 
   * 
   * \param payloadMode the WifiMode use for the transmission of the payload
   * \param preamble the type of preamble 
   * 
   * \return the duration of the PLCP header in microseconds
   */
  static uint32_t GetPlcpHeaderDurationMicroSeconds (WifiMode payloadMode, WifiPreamble preamble);

  /** 
   * \param payloadMode the WifiMode use for the transmission of the payload
   * \param preamble the type of preamble 
   * 
   * \return the duration of the PLCP preamble in microseconds
   */
  static uint32_t GetPlcpPreambleDurationMicroSeconds (WifiMode payloadMode, WifiPreamble preamble);

  /** 
   * \param payloadMode the WifiMode use for the transmission of the payload
   * \param preamble the type of preamble 
   * 
   * \return the duration of the payload in microseconds
   */
  static uint32_t GetPayloadDurationMicroSeconds (uint32_t size, WifiMode payloadMode);

  /**
   * The WifiPhy::GetNModes() and WifiPhy::GetMode() methods are used
   * (e.g., by a WifiRemoteStationManager) to determine the set of
   * transmission/reception modes that this WifiPhy(-derived class)
   * can support - a set of WifiMode objects which we call the
   * DeviceRateSet, and which is stored as WifiPhy::m_deviceRateSet.
   *
   * It is important to note that the DeviceRateSet is a superset (not
   * necessarily proper) of the OperationalRateSet (which is
   * logically, if not actually, a property of the associated
   * WifiRemoteStationManager), which itself is a superset (again, not
   * necessarily proper) of the BSSBasicRateSet.
   *
   * \returns the number of transmission modes supported by this PHY.
   *
   * \sa WifiPhy::GetMode()
   */
  virtual uint32_t GetNModes (void) const = 0;
  /**
   * The WifiPhy::GetNModes() and WifiPhy::GetMode() methods are used
   * (e.g., by a WifiRemoteStationManager) to determine the set of
   * transmission/reception modes that this WifiPhy(-derived class)
   * can support - a set of WifiMode objects which we call the
   * DeviceRateSet, and which is stored as WifiPhy::m_deviceRateSet.
   *
   * It is important to note that the DeviceRateSet is a superset (not
   * necessarily proper) of the OperationalRateSet (which is
   * logically, if not actually, a property of the associated
   * WifiRemoteStationManager), which itself is a superset (again, not
   * necessarily proper) of the BSSBasicRateSet.
   *
   * \param mode index in array of supported modes
   * \returns the mode whose index is specified.
   *
   * \sa WifiPhy::GetNModes()
   */
  virtual WifiMode GetMode (uint32_t mode) const = 0;
  /**
   * \param txMode the transmission mode
   * \param ber the probability of bit error rate
   * \returns the minimum snr which is required to achieve
   *          the requested ber for the specified transmission mode. (W/W)
   */
  virtual double CalculateSnr (WifiMode txMode, double ber) const = 0;

  /**
   * \brief Set channel number.
   *
   * Channel center frequency = Channel starting frequency + 5 MHz * (nch - 1)
   *
   * where Starting channel frequency is standard-dependent, see SetStandard()
   * as defined in IEEE 802.11-2007 17.3.8.3.2.
   */
  virtual void SetChannelNumber (uint16_t id) = 0;
  /// Return current channel number, see SetChannelNumber()
  virtual uint16_t GetChannelNumber () const = 0;

  virtual void ConfigureStandard (enum WifiPhyStandard standard) = 0;

  virtual Ptr<WifiChannel> GetChannel (void) const = 0;

  static WifiMode GetDsssRate1Mbps ();
  static WifiMode GetDsssRate2Mbps ();
  static WifiMode GetDsssRate5_5Mbps ();
  static WifiMode GetDsssRate11Mbps ();
  static WifiMode GetErpOfdmRate6Mbps ();
  static WifiMode GetErpOfdmRate9Mbps ();
  static WifiMode GetErpOfdmRate12Mbps ();
  static WifiMode GetErpOfdmRate18Mbps ();
  static WifiMode GetErpOfdmRate24Mbps ();
  static WifiMode GetErpOfdmRate36Mbps ();
  static WifiMode GetErpOfdmRate48Mbps ();
  static WifiMode GetErpOfdmRate54Mbps ();
  static WifiMode GetOfdmRate6Mbps ();
  static WifiMode GetOfdmRate9Mbps ();
  static WifiMode GetOfdmRate12Mbps ();
  static WifiMode GetOfdmRate18Mbps ();
  static WifiMode GetOfdmRate24Mbps ();
  static WifiMode GetOfdmRate36Mbps ();
  static WifiMode GetOfdmRate48Mbps ();
  static WifiMode GetOfdmRate54Mbps ();
  static WifiMode GetOfdmRate3MbpsBW10MHz ();
  static WifiMode GetOfdmRate4_5MbpsBW10MHz ();
  static WifiMode GetOfdmRate6MbpsBW10MHz ();
  static WifiMode GetOfdmRate9MbpsBW10MHz ();
  static WifiMode GetOfdmRate12MbpsBW10MHz ();
  static WifiMode GetOfdmRate18MbpsBW10MHz ();
  static WifiMode GetOfdmRate24MbpsBW10MHz ();
  static WifiMode GetOfdmRate27MbpsBW10MHz ();
  static WifiMode GetOfdmRate1_5MbpsBW5MHz ();
  static WifiMode GetOfdmRate2_25MbpsBW5MHz ();
  static WifiMode GetOfdmRate3MbpsBW5MHz ();
  static WifiMode GetOfdmRate4_5MbpsBW5MHz ();
  static WifiMode GetOfdmRate6MbpsBW5MHz ();
  static WifiMode GetOfdmRate9MbpsBW5MHz ();
  static WifiMode GetOfdmRate12MbpsBW5MHz ();
  static WifiMode GetOfdmRate13_5MbpsBW5MHz ();


  /**
   * Public method used to fire a PhyTxBegin trace.  Implemented for encapsulation
   * purposes.
   */
  void NotifyTxBegin (Ptr<const Packet> packet);

  /**
   * Public method used to fire a PhyTxEnd trace.  Implemented for encapsulation
   * purposes.
   */
  void NotifyTxEnd (Ptr<const Packet> packet);

  /**
   * Public method used to fire a PhyTxDrop trace.  Implemented for encapsulation
   * purposes.
   */
  void NotifyTxDrop (Ptr<const Packet> packet);

  /**
   * Public method used to fire a PhyRxBegin trace.  Implemented for encapsulation
   * purposes.
   */
  void NotifyRxBegin (Ptr<const Packet> packet);

  /**
   * Public method used to fire a PhyRxEnd trace.  Implemented for encapsulation
   * purposes.
   */
  void NotifyRxEnd (Ptr<const Packet> packet);

  /**
   * Public method used to fire a PhyRxDrop trace.  Implemented for encapsulation
   * purposes.
   */
  void NotifyRxDrop (Ptr<const Packet> packet);

  /**
   *
   * Public method used to fire a PromiscSniffer trace for a wifi packet being received.  Implemented for encapsulation
   * purposes.
   *
   * @param packet the packet being received
   * @param channelFreqMhz the frequency in MHz at which the packet is
   * received. Note that in real devices this is normally the
   * frequency to which  the receiver is tuned, and this can be
   * different than the frequency at which the packet was originally
   * transmitted. This is because it is possible to have the receiver
   * tuned on a given channel and still to be able to receive packets
   * on a nearby channel.
   * @param channelNumber the channel on which the packet is received
   * @param rate the PHY data rate in units of 500kbps (i.e., the same
   * units used both for the radiotap and for the prism header)
   * @param isShortPreamble true if short preamble is used, false otherwise
   * @param signalDbm signal power in dBm
   * @param noiseDbm  noise power in dBm
   */
  void NotifyPromiscSniffRx (Ptr<const Packet> packet, uint16_t channelFreqMhz, uint16_t channelNumber, uint32_t rate, bool isShortPreamble,
                             double signalDbm, double noiseDbm);

  /**
   *
   * Public method used to fire a PromiscSniffer trace for a wifi packet being transmitted.  Implemented for encapsulation
   * purposes.
   *
   * @param packet the packet being transmitted
   * @param channelFreqMhz the frequency in MHz at which the packet is
   * transmitted.
   * @param channelNumber the channel on which the packet is transmitted
   * @param rate the PHY data rate in units of 500kbps (i.e., the same
   * units used both for the radiotap and for the prism header)
   * @param isShortPreamble true if short preamble is used, false otherwise
   */
  void NotifyPromiscSniffTx (Ptr<const Packet> packet, uint16_t channelFreqMhz, uint16_t channelNumber, uint32_t rate, bool isShortPreamble);


private:
  /**
   * The trace source fired when a packet begins the transmission process on
   * the medium.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_phyTxBeginTrace;

  /**
   * The trace source fired when a packet ends the transmission process on
   * the medium.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_phyTxEndTrace;

  /**
   * The trace source fired when the phy layer drops a packet as it tries
   * to transmit it.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_phyTxDropTrace;

  /**
   * The trace source fired when a packet begins the reception process from
   * the medium.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_phyRxBeginTrace;

  /**
   * The trace source fired when a packet ends the reception process from
   * the medium.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_phyRxEndTrace;

  /**
   * The trace source fired when the phy layer drops a packet it has received.
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet> > m_phyRxDropTrace;

  /**
   * A trace source that emulates a wifi device in monitor mode
   * sniffing a packet being received.
   *
   * As a reference with the real world, firing this trace
   * corresponds in the madwifi driver to calling the function
   * ieee80211_input_monitor()
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet>, uint16_t, uint16_t, uint32_t, bool, double, double> m_phyPromiscSniffRxTrace;

  /**
   * A trace source that emulates a wifi device in monitor mode
   * sniffing a packet being transmitted.
   *
   * As a reference with the real world, firing this trace
   * corresponds in the madwifi driver to calling the function
   * ieee80211_input_monitor()
   *
   * \see class CallBackTraceSource
   */
  TracedCallback<Ptr<const Packet>, uint16_t, uint16_t, uint32_t, bool> m_phyPromiscSniffTxTrace;

};

/**
 * \param os          output stream
 * \param state       wifi state to stringify
 */
std::ostream& operator<< (std::ostream& os, enum WifiPhy::State state);

} // namespace ns3

#endif /* WIFI_PHY_H */
