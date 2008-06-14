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


namespace ns3 {

class WifiChannel;

/**
 * \brief receive notifications about phy events.
 */
class WifiPhyListener {
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
};


/**
 * \brief 802.11 PHY layer model
 *
 */
class WifiPhy : public Object
{
public:
  /**
   * The state of the PHY layer.
   */
  enum State {
    /**
     * The PHY layer is synchronized upon a packet.
     */
    SYNC,
    /**
     * The PHY layer is sending a packet.
     */
    TX,
    /**
     * The PHY layer has sense the medium busy through
     * the CCA mechanism
     */
    CCA_BUSY,
    /**
     * The PHY layer is IDLE.
     */
    IDLE
  };
  /**
   * arg1: packet received successfully
   * arg2: snr of packet
   * arg3: mode of packet
   * arg4: type of preamble used for packet.
   */
  typedef Callback<void,Ptr<Packet>, double, WifiMode, enum WifiPreamble> SyncOkCallback;
  /**
   * arg1: packet received unsuccessfully
   * arg2: snr of packet
   */
  typedef Callback<void,Ptr<const Packet>, double> SyncErrorCallback;

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
  virtual void SetReceiveOkCallback (SyncOkCallback callback) = 0;
  /**
   * \param callback the callback to invoke
   *        upon erronous packet reception.
   */
  virtual void SetReceiveErrorCallback (SyncErrorCallback callback) = 0;

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
   * \returns true of the current state of the PHY layer is WifiPhy::CCA_BUSY, false otherwise.
   */
  virtual bool IsStateCcaBusy (void) = 0;
  /**
   * \returns true of the current state of the PHY layer is WifiPhy::IDLE, false otherwise.
   */
  virtual bool IsStateIdle (void) = 0;
  /**
   * \returns true of the current state of the PHY layer is not WifiPhy::IDLE, false otherwise.
   */
  virtual bool IsStateBusy (void) = 0;
  /**
   * \returns true of the current state of the PHY layer is WifiPhy::SYNC, false otherwise.
   */
  virtual bool IsStateSync (void) = 0;
  /**
   * \returns true of the current state of the PHY layer is WifiPhy::TX, false otherwise.
   */
  virtual bool IsStateTx (void) = 0;
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
   * \returns the total amount of time this PHY will stay busy for
   *          the transmission of these bytes.
   */
  virtual Time CalculateTxDuration (uint32_t size, WifiMode payloadMode, enum WifiPreamble preamble) const = 0;
  
  /**
   * \returns the number of transmission modes supported by this PHY.
   */
  virtual uint32_t GetNModes (void) const = 0;
  /**
   * \param mode index in array of supported modes
   * \returns the mode whose index is specified.
   */
  virtual WifiMode GetMode (uint32_t mode) const = 0;
  /* return snr: W/W */
  /**
   * \param txMode the transmission mode
   * \param ber the probability of bit error rate
   * \returns the minimum snr which is required to achieve
   *          the requested ber for the specified transmission mode.
   */
  virtual double CalculateSnr (WifiMode txMode, double ber) const = 0;


  virtual Ptr<WifiChannel> GetChannel (void) const = 0;

  static WifiMode g_6mba;
  static WifiMode g_9mba;
  static WifiMode g_12mba;
  static WifiMode g_18mba;
  static WifiMode g_24mba;
  static WifiMode g_36mba;
  static WifiMode g_48mba;
  static WifiMode g_54mba;
};

} // namespace ns3


#endif /* WIFI_PHY_H */
