/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
#include "wifi-tx-vector.h"

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
   *   - NotifyRxEndError
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

  /**
   * Return the minimum available transmission power level (dBm).
   *
   * \return the minimum available transmission power level in dBm
   */
  virtual double GetTxPowerStart (void) const = 0;
  /**
   * Return the maximum available transmission power level (dBm).
   *
   * \return the maximum available transmission power level in dBm
   */
  virtual double GetTxPowerEnd (void) const = 0;
  /**
   * \return the number of tx power levels available for this PHY.
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
   * \param txvector the txvector that has tx parameters as txPowerLevel a power level to use to send this packet. The real
   *        transmission power is calculated as txPowerMin + txPowerLevel * (txPowerMax - txPowerMin) / nTxLevels
   */
  virtual void SendPacket (Ptr<const Packet> packet, WifiMode mode, enum WifiPreamble preamble, WifiTxVector txvector) = 0;

  /**
   * \param listener the new listener
   *
   * Add the input listener to the list of objects to be notified of
   * PHY-level events.
   */
  virtual void RegisterListener (WifiPhyListener *listener) = 0;

  /**
   * \return true of the current state of the PHY layer is WifiPhy::IDLE, false otherwise.
   */
  virtual bool IsStateIdle (void) = 0;
  /**
   * \return true of the current state of the PHY layer is WifiPhy::CCA_BUSY, false otherwise.
   */
  virtual bool IsStateCcaBusy (void) = 0;
  /**
   * \return true of the current state of the PHY layer is not WifiPhy::IDLE, false otherwise.
   */
  virtual bool IsStateBusy (void) = 0;
  /**
   * \return true of the current state of the PHY layer is WifiPhy::RX, false otherwise.
   */
  virtual bool IsStateRx (void) = 0;
  /**
   * \return true of the current state of the PHY layer is WifiPhy::TX, false otherwise.
   */
  virtual bool IsStateTx (void) = 0;
  /**
   * \return true of the current state of the PHY layer is WifiPhy::SWITCHING, false otherwise.
   */
  virtual bool IsStateSwitching (void) = 0;
  /**
   * \return the amount of time since the current state has started.
   */
  virtual Time GetStateDuration (void) = 0;
  /**
   * \return the predicted delay until this PHY can become WifiPhy::IDLE.
   *
   * The PHY will never become WifiPhy::IDLE _before_ the delay returned by
   * this method but it could become really idle later.
   */
  virtual Time GetDelayUntilIdle (void) = 0;

  /**
   * Return the start time of the last received packet.
   *
   * \return the start time of the last received packet
   */
  virtual Time GetLastRxStartTime (void) const = 0;

  /**
   * \param size the number of bytes in the packet to send
   * \param txvector the transmission parameters used for this packet
   * \param preamble the type of preamble to use for this packet.
   * \return the total amount of time this PHY will stay busy for
   *          the transmission of these bytes.
   */
  static Time CalculateTxDuration (uint32_t size, WifiTxVector txvector, enum WifiPreamble preamble);

/** 
   * \param payloadMode the WifiMode use for the transmission of the payload
   * \param preamble the type of preamble
   * \param txvector the transmission parameters used for this packet

   * \return the training symbol duration
   */
  static uint32_t GetPlcpHtTrainingSymbolDurationMicroSeconds (WifiMode payloadMode, WifiPreamble preamble, WifiTxVector txvector);
/** 
   * \param payloadMode the WifiMode use for the transmission of the payload
   * \param preamble the type of preamble
   * 
   * \return the WifiMode used for the transmission of the HT-SIG in Mixed Format and greenfield format PLCP header 
   */
  static WifiMode GetMFPlcpHeaderMode (WifiMode payloadMode, WifiPreamble preamble);
/** 
   * \param payloadMode the WifiMode use for the transmission of the payload
   * \param preamble the type of preamble
   * 
   * \return the duration of the GT-SIG in Mixed Format and greenfield format PLCP header 
   */
  static uint32_t GetPlcpHtSigHeaderDurationMicroSeconds (WifiMode payloadMode, WifiPreamble preamble);


  /** 
   * \param payloadMode the WifiMode use for the transmission of the payload
   * \param preamble the type of preamble
   * 
   * \return the WifiMode used for the transmission of the PLCP header 
   */
  static WifiMode GetPlcpHeaderMode (WifiMode payloadMode, WifiPreamble preamble);

  /**
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
   * \param size the number of bytes in the packet to send
   * \param txvector the transmission parameters used for this packet
   * 
   * \return the duration of the payload in microseconds
   */
  static double GetPayloadDurationMicroSeconds (uint32_t size, WifiTxVector txvector);

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
   * \return the number of transmission modes supported by this PHY.
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
   * \return the mode whose index is specified.
   *
   * \sa WifiPhy::GetNModes()
   */
  virtual WifiMode GetMode (uint32_t mode) const = 0;
  /**
   * Check if the given WifiMode is supported by the PHY.
   *
   * \param mode the wifi mode to check
   * \return true if the given mode is supported,
   *         false otherwise
   */
  virtual bool IsModeSupported (WifiMode mode) const = 0;
  /**
   * \param txMode the transmission mode
   * \param ber the probability of bit error rate
   * \return the minimum snr which is required to achieve
   *          the requested ber for the specified transmission mode. (W/W)
   */
  virtual double CalculateSnr (WifiMode txMode, double ber) const = 0;
   /**
   * The WifiPhy::NBssMembershipSelectors() method is used
   * (e.g., by a WifiRemoteStationManager) to determine the set of
   * transmission/reception modes that this WifiPhy(-derived class)
   * can support - a set of WifiMode objects which we call the
   * BssMemebershipSelectorSet, and which is stored as WifiPhy::m_bssMembershipSelectorSet.
   *
   * This was introduced with 11n
   *
   * \return the memebership selector whose index is specified.
   */
  virtual uint32_t GetNBssMembershipSelectors (void) const=0;

   /**
   * The WifiPhy::BssMembershipSelector() method is used
   * (e.g., by a WifiRemoteStationManager) to determine the set of
   * transmission/reception modes that this WifiPhy(-derived class)
   * can support - a set of WifiMode objects which we call the
   * BssMemebershipSelectorSet, and which is stored as WifiPhy::m_bssMembershipSelectorSet.
   *
   * This was introduced with 11n
   *
   * \param selector index in array of supported memeberships
   * \return the memebership selector whose index is specified.
   */
  virtual uint32_t GetBssMembershipSelector (uint32_t selector) const=0;
  /**
   * The WifiPhy::GetMembershipSelectorModes() method is used
   * (e.g., by a WifiRemoteStationManager) to determine the set of
   * transmission/reception modes that this WifiPhy(-derived class)
   * can support - a set of WifiMode objects which we call the
   * BssMemebershipSelectorSet, and which is stored as WifiPhy::m_bssMembershipSelectorSet.
   *
   * This was introduced with 11n
   *
   * \param selector index in array of supported memeberships
   * \return a WifiModeList that contains the WifiModes associrated with the selected index.
   *
   * \sa WifiPhy::GetMembershipSelectorModes()
   */
  virtual WifiModeList GetMembershipSelectorModes(uint32_t selector)=0;
  /**
   * The WifiPhy::GetNMcs() method is used
   * (e.g., by a WifiRemoteStationManager) to determine the set of
   * transmission/reception MCS indexes that this WifiPhy(-derived class)
   * can support - a set of Mcs indexes which we call the
   * DeviceMcsSet, and which is stored as WifiPhy::m_deviceMcsSet.
   *
   * This was introduced with 11n
   *
   * \return the Mcs index whose index is specified.
   */
  virtual uint8_t GetNMcs (void) const=0;
  /**
   * The WifiPhy::GetMcs() method is used
   * (e.g., by a WifiRemoteStationManager) to determine the set of
   * transmission/reception MCS indexes that this WifiPhy(-derived class)
   * can support - a set of Mcs indexes which we call the
   * DeviceMcsSet, and which is stored as WifiPhy::m_deviceMcsSet.
   *
   * This was introduced with 11n
   *
   * \param mcs index in array of supported Mcs
   * \return the Mcs index whose index is specified.
   */
  virtual uint8_t GetMcs (uint8_t mcs) const=0;

  /**
  * For a given WifiMode finds the corresponding MCS value and returns it 
  * as defined in the IEEE 802.11n standard 
  *
  * \param mode the WifiMode
  * \return the MCS number that corresponds to the given WifiMode
  */
  virtual uint32_t WifiModeToMcs (WifiMode mode)=0;
 /**
  * For a given MCS finds the corresponding WifiMode and returns it 
  * as defined in the IEEE 802.11n standard. 
  * 
  * \param mcs the MCS number 
  * \return the WifiMode that corresponds to the given mcs number
  */
  virtual WifiMode McsToWifiMode (uint8_t mcs)=0;

  
  /**
   * \brief Set channel number.
   *
   * Channel center frequency = Channel starting frequency + 5 MHz * (nch - 1)
   *
   * where Starting channel frequency is standard-dependent, see SetStandard()
   * as defined in (Section 18.3.8.4.2 "Channel numbering"; IEEE Std 802.11-2012).
   *
   * \param id the channel number
   */
  virtual void SetChannelNumber (uint16_t id) = 0;
  /**  
   * Return current channel number.
   *
   * \return the current channel number
   */
  virtual uint16_t GetChannelNumber () const = 0;

  /**
   * Configure the PHY-level parameters for different Wi-Fi standard.
   *
   * \param standard the Wi-Fi standard
   */
  virtual void ConfigureStandard (enum WifiPhyStandard standard) = 0;

  /**
   * Return the WifiChannel this WifiPhy is connected to.
   *
   * \return the WifiChannel this WifiPhy is connected to
   */
  virtual Ptr<WifiChannel> GetChannel (void) const = 0;

  /**
   * Return a WifiMode for DSSS at 1Mbps.
   *
   * \return a WifiMode for DSSS at 1Mbps
   */
  static WifiMode GetDsssRate1Mbps ();
  /**
   * Return a WifiMode for DSSS at 2Mbps.
   *
   * \return a WifiMode for DSSS at 2Mbps
   */
  static WifiMode GetDsssRate2Mbps ();
  /**
   * Return a WifiMode for DSSS at 5.5Mbps.
   *
   * \return a WifiMode for DSSS at 5.5Mbps
   */
  static WifiMode GetDsssRate5_5Mbps ();
  /**
   * Return a WifiMode for DSSS at 11Mbps.
   *
   * \return a WifiMode for DSSS at 11Mbps
   */
  static WifiMode GetDsssRate11Mbps ();
  /**
   * Return a WifiMode for ERP-ODFM at 6Mbps.
   *
   * \return a WifiMode for ERP-OFDM at 6Mbps
   */
  static WifiMode GetErpOfdmRate6Mbps ();
  /**
   * Return a WifiMode for ERP-ODFM at 9Mbps.
   *
   * \return a WifiMode for ERP-OFDM at 9Mbps
   */
  static WifiMode GetErpOfdmRate9Mbps ();
  /**
   * Return a WifiMode for ERP-ODFM at 12Mbps.
   *
   * \return a WifiMode for ERP-OFDM at 12Mbps
   */
  static WifiMode GetErpOfdmRate12Mbps ();
  /**
   * Return a WifiMode for ERP-ODFM at 18Mbps.
   *
   * \return a WifiMode for ERP-OFDM at 18Mbps
   */
  static WifiMode GetErpOfdmRate18Mbps ();
  /**
   * Return a WifiMode for ERP-ODFM at 24Mbps.
   *
   * \return a WifiMode for ERP-OFDM at 24Mbps
   */
  static WifiMode GetErpOfdmRate24Mbps ();
  /**
   * Return a WifiMode for ERP-ODFM at 36Mbps.
   *
   * \return a WifiMode for ERP-OFDM at 36Mbps
   */
  static WifiMode GetErpOfdmRate36Mbps ();
  /**
   * Return a WifiMode for ERP-ODFM at 48Mbps.
   *
   * \return a WifiMode for ERP-OFDM at 48Mbps
   */
  static WifiMode GetErpOfdmRate48Mbps ();
  /**
   * Return a WifiMode for ERP-ODFM at 54Mbps.
   *
   * \return a WifiMode for ERP-OFDM at 54Mbps
   */
  static WifiMode GetErpOfdmRate54Mbps ();
  /**
   * Return a WifiMode for ODFM at 6Mbps.
   *
   * \return a WifiMode for OFDM at 6Mbps
   */
  static WifiMode GetOfdmRate6Mbps ();
  /**
   * Return a WifiMode for ODFM at 9Mbps.
   *
   * \return a WifiMode for OFDM at 9Mbps
   */
  static WifiMode GetOfdmRate9Mbps ();
  /**
   * Return a WifiMode for ODFM at 12Mbps.
   *
   * \return a WifiMode for OFDM at 12Mbps
   */
  static WifiMode GetOfdmRate12Mbps ();
  /**
   * Return a WifiMode for ODFM at 18Mbps.
   *
   * \return a WifiMode for OFDM at 18Mbps
   */
  static WifiMode GetOfdmRate18Mbps ();
  /**
   * Return a WifiMode for ODFM at 24Mbps.
   *
   * \return a WifiMode for OFDM at 24Mbps
   */
  static WifiMode GetOfdmRate24Mbps ();
  /**
   * Return a WifiMode for ODFM at 36Mbps.
   *
   * \return a WifiMode for OFDM at 36Mbps
   */
  static WifiMode GetOfdmRate36Mbps ();
  /**
   * Return a WifiMode for ODFM at 48Mbps.
   *
   * \return a WifiMode for OFDM at 48Mbps
   */
  static WifiMode GetOfdmRate48Mbps ();
  /**
   * Return a WifiMode for ODFM at 54Mbps.
   *
   * \return a WifiMode for OFDM at 54Mbps
   */
  static WifiMode GetOfdmRate54Mbps ();
  /**
   * Return a WifiMode for ODFM at 3Mbps with 10MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 3Mbps with 10MHz channel spacing
   */
  static WifiMode GetOfdmRate3MbpsBW10MHz ();
  /**
   * Return a WifiMode for ODFM at 4.5Mbps with 10MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 4.5Mbps with 10MHz channel spacing
   */
  static WifiMode GetOfdmRate4_5MbpsBW10MHz ();
  /**
   * Return a WifiMode for ODFM at 6Mbps with 10MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 6Mbps with 10MHz channel spacing
   */
  static WifiMode GetOfdmRate6MbpsBW10MHz ();
  /**
   * Return a WifiMode for ODFM at 9Mbps with 10MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 9Mbps with 10MHz channel spacing
   */
  static WifiMode GetOfdmRate9MbpsBW10MHz ();
  /**
   * Return a WifiMode for ODFM at 12Mbps with 10MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 12Mbps with 10MHz channel spacing
   */
  static WifiMode GetOfdmRate12MbpsBW10MHz ();
  /**
   * Return a WifiMode for ODFM at 18Mbps with 10MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 18Mbps with 10MHz channel spacing
   */
  static WifiMode GetOfdmRate18MbpsBW10MHz ();
  /**
   * Return a WifiMode for ODFM at 24Mbps with 10MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 24Mbps with 10MHz channel spacing
   */
  static WifiMode GetOfdmRate24MbpsBW10MHz ();
  /**
   * Return a WifiMode for ODFM at 27Mbps with 10MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 27Mbps with 10MHz channel spacing
   */
  static WifiMode GetOfdmRate27MbpsBW10MHz ();
  /**
   * Return a WifiMode for ODFM at 1.5Mbps with 5MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 1.5Mbps with 5MHz channel spacing
   */
  static WifiMode GetOfdmRate1_5MbpsBW5MHz ();
  /**
   * Return a WifiMode for ODFM at 2.25Mbps with 5MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 2.25Mbps with 5MHz channel spacing
   */
  static WifiMode GetOfdmRate2_25MbpsBW5MHz ();
  /**
   * Return a WifiMode for ODFM at 3Mbps with 5MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 3Mbps with 5MHz channel spacing
   */
  static WifiMode GetOfdmRate3MbpsBW5MHz ();
  /**
   * Return a WifiMode for ODFM at 4.5Mbps with 5MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 4.5Mbps with 5MHz channel spacing
   */
  static WifiMode GetOfdmRate4_5MbpsBW5MHz ();
  /**
   * Return a WifiMode for ODFM at 6Mbps with 5MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 6Mbps with 5MHz channel spacing
   */
  static WifiMode GetOfdmRate6MbpsBW5MHz ();
  /**
   * Return a WifiMode for ODFM at 9Mbps with 5MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 9Mbps with 5MHz channel spacing
   */
  static WifiMode GetOfdmRate9MbpsBW5MHz ();
  /**
   * Return a WifiMode for ODFM at 12Mbps with 5MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 12Mbps with 5MHz channel spacing
   */
  static WifiMode GetOfdmRate12MbpsBW5MHz ();
  /**
   * Return a WifiMode for ODFM at 13.5Mbps with 5MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 13.5Mbps with 5MHz channel spacing
   */
  static WifiMode GetOfdmRate13_5MbpsBW5MHz ();
  /**
   * Return a WifiMode for ODFM at 6.5Mbps with 20MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 6.5Mbps with 20MHz channel spacing
   */
  static WifiMode GetOfdmRate6_5MbpsBW20MHz ();
  /**
   * Return a WifiMode for ODFM at 13Mbps with 20MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 13Mbps with 20MHz channel spacing
   */
  static WifiMode GetOfdmRate13MbpsBW20MHz ();
  /**
   * Return a WifiMode for ODFM at 19.5Mbps with 20MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 19.5Mbps with 20MHz channel spacing
   */
  static WifiMode GetOfdmRate19_5MbpsBW20MHz ();
  /**
   * Return a WifiMode for ODFM at 26Mbps with 20MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 26Mbps with 20MHz channel spacing
   */
  static WifiMode GetOfdmRate26MbpsBW20MHz ();
  /**
   * Return a WifiMode for ODFM at 39Mbps with 20MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 39Mbps with 20MHz channel spacing
   */
  static WifiMode GetOfdmRate39MbpsBW20MHz ();
  /**
   * Return a WifiMode for ODFM at 52Mbps with 20MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 52Mbps with 20MHz channel spacing
   */
  static WifiMode GetOfdmRate52MbpsBW20MHz ();
  /**
   * Return a WifiMode for ODFM at 58.5Mbps with 20MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 58.5Mbps with 20MHz channel spacing
   */
  static WifiMode GetOfdmRate58_5MbpsBW20MHz ();
  /**
   * Return a WifiMode for ODFM at 65Mbps with 20MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 65Mbps with 20MHz channel spacing
   */
  static WifiMode GetOfdmRate65MbpsBW20MHz ();
  /**
   * Return a WifiMode for ODFM at 13.5Mbps with 40MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 13.5Mbps with 40MHz channel spacing
   */
  static WifiMode GetOfdmRate13_5MbpsBW40MHz ();
  /**
   * Return a WifiMode for ODFM at 27Mbps with 40MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 27Mbps with 40MHz channel spacing
   */
  static WifiMode GetOfdmRate27MbpsBW40MHz ();
  /**
   * Return a WifiMode for ODFM at 40.5Mbps with 40MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 40.5Mbps with 40MHz channel spacing
   */
  static WifiMode GetOfdmRate40_5MbpsBW40MHz ();
  /**
   * Return a WifiMode for ODFM at 54Mbps with 40MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 54Mbps with 40MHz channel spacing
   */
  static WifiMode GetOfdmRate54MbpsBW40MHz ();
  /**
   * Return a WifiMode for ODFM at 81Mbps with 40MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 81Mbps with 40MHz channel spacing
   */
  static WifiMode GetOfdmRate81MbpsBW40MHz ();
  /**
   * Return a WifiMode for ODFM at 108Mbps with 40MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 108Mbps with 40MHz channel spacing
   */
  static WifiMode GetOfdmRate108MbpsBW40MHz ();
  /**
   * Return a WifiMode for ODFM at 121.5Mbps with 40MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 121.5Mbps with 40MHz channel spacing
   */
  static WifiMode GetOfdmRate121_5MbpsBW40MHz ();
  /**
   * Return a WifiMode for ODFM at 135Mbps with 40MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 135Mbps with 40MHz channel spacing
   */
  static WifiMode GetOfdmRate135MbpsBW40MHz ();
  //Rates for clause 20 with short guard interval
  /**
   * Return a WifiMode for ODFM at 7.2Mbps with 20MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 7.2Mbps with 20MHz channel spacing
   */
  static WifiMode GetOfdmRate7_2MbpsBW20MHz ();
  /**
   * Return a WifiMode for ODFM at 14.4Mbps with 20MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 14.4Mbps with 20MHz channel spacing
   */
  static WifiMode GetOfdmRate14_4MbpsBW20MHz ();
  /**
   * Return a WifiMode for ODFM at 21.7Mbps with 20MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 21.7Mbps with 20MHz channel spacing
   */
  static WifiMode GetOfdmRate21_7MbpsBW20MHz ();
  /**
   * Return a WifiMode for ODFM at 28.9Mbps with 20MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 28.9Mbps with 20MHz channel spacing
   */
  static WifiMode GetOfdmRate28_9MbpsBW20MHz ();
  /**
   * Return a WifiMode for ODFM at 43.3Mbps with 20MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 43.3Mbps with 20MHz channel spacing
   */
  static WifiMode GetOfdmRate43_3MbpsBW20MHz ();
  /**
   * Return a WifiMode for ODFM at 57.8Mbps with 20MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 57.8Mbps with 20MHz channel spacing
   */
  static WifiMode GetOfdmRate57_8MbpsBW20MHz ();
  /**
   * Return a WifiMode for ODFM at 65Mbps with 20MHz channel spacing.
   * The rate supports short guard interval.
   *
   * \return a WifiMode for OFDM at 65Mbps with 20MHz channel spacing
   */
  static WifiMode GetOfdmRate65MbpsBW20MHzShGi ();
  /**
   * Return a WifiMode for ODFM at 72.2Mbps with 20MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 72.2Mbps with 20MHz channel spacing
   */
  static WifiMode GetOfdmRate72_2MbpsBW20MHz ();
  /**
   * Return a WifiMode for ODFM at 15Mbps with 40MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 15Mbps with 40MHz channel spacing
   */
  static WifiMode GetOfdmRate15MbpsBW40MHz ();
  /**
   * Return a WifiMode for ODFM at 30Mbps with 40MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 30Mbps with 40MHz channel spacing
   */
  static WifiMode GetOfdmRate30MbpsBW40MHz (); 
  /**
   * Return a WifiMode for ODFM at 45Mbps with 40MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 45Mbps with 40MHz channel spacing
   */
  static WifiMode GetOfdmRate45MbpsBW40MHz ();
  /**
   * Return a WifiMode for ODFM at 60Mbps with 40MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 60Mbps with 40MHz channel spacing
   */
  static WifiMode GetOfdmRate60MbpsBW40MHz ();
  /**
   * Return a WifiMode for ODFM at 90Mbps with 40MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 90Mbps with 40MHz channel spacing
   */
  static WifiMode GetOfdmRate90MbpsBW40MHz ();
  /**
   * Return a WifiMode for ODFM at 120Mbps with 40MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 120Mbps with 40MHz channel spacing
   */
  static WifiMode GetOfdmRate120MbpsBW40MHz ();
  /**
   * Return a WifiMode for ODFM at 135Mbps with 40MHz channel spacing.
   * The rate supports short guard interval.
   *
   * \return a WifiMode for OFDM at 135Mbps with 40MHz channel spacing
   */
  static WifiMode GetOfdmRate135MbpsBW40MHzShGi ();
  /**
   * Return a WifiMode for ODFM at 150Mbps with 40MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 150Mbps with 40MHz channel spacing
   */
  static WifiMode GetOfdmRate150MbpsBW40MHz ();


  /**
   * Public method used to fire a PhyTxBegin trace.  Implemented for encapsulation
   * purposes.
   *
   * \param packet the packet being transmitted
   */
  void NotifyTxBegin (Ptr<const Packet> packet);

  /**
   * Public method used to fire a PhyTxEnd trace.  Implemented for encapsulation
   * purposes.
   *
   * \param packet the packet that was transmitted
   */
  void NotifyTxEnd (Ptr<const Packet> packet);

  /**
   * Public method used to fire a PhyTxDrop trace.  Implemented for encapsulation
   * purposes.
   *
   * \param packet the packet that was failed to transmitted
   */
  void NotifyTxDrop (Ptr<const Packet> packet);

  /**
   * Public method used to fire a PhyRxBegin trace.  Implemented for encapsulation
   * purposes.
   *
   * \param packet the packet being received
   */
  void NotifyRxBegin (Ptr<const Packet> packet);

  /**
   * Public method used to fire a PhyRxEnd trace.  Implemented for encapsulation
   * purposes.
   *
   * \param packet the packet received
   */
  void NotifyRxEnd (Ptr<const Packet> packet);

  /**
   * Public method used to fire a PhyRxDrop trace.  Implemented for encapsulation
   * purposes.
   *
   * \param packet the packet that was not successfully received
   */
  void NotifyRxDrop (Ptr<const Packet> packet);

  /**
   *
   * Public method used to fire a MonitorSniffer trace for a wifi packet being received.  Implemented for encapsulation
   * purposes.
   *
   * \param packet the packet being received
   * \param channelFreqMhz the frequency in MHz at which the packet is
   *        received. Note that in real devices this is normally the
   *        frequency to which  the receiver is tuned, and this can be
   *        different than the frequency at which the packet was originally
   *        transmitted. This is because it is possible to have the receiver
   *        tuned on a given channel and still to be able to receive packets
   *        on a nearby channel.
   * \param channelNumber the channel on which the packet is received
   * \param rate the PHY data rate in units of 500kbps (i.e., the same
   *        units used both for the radiotap and for the prism header)
   * \param isShortPreamble true if short preamble is used, false otherwise
   * \param signalDbm signal power in dBm
   * \param noiseDbm  noise power in dBm
   */
  void NotifyMonitorSniffRx (Ptr<const Packet> packet, uint16_t channelFreqMhz, uint16_t channelNumber, uint32_t rate, bool isShortPreamble,
                             double signalDbm, double noiseDbm);

  /**
   *
   * Public method used to fire a MonitorSniffer trace for a wifi packet being transmitted.  Implemented for encapsulation
   * purposes.
   *
   * \param packet the packet being transmitted
   * \param channelFreqMhz the frequency in MHz at which the packet is
   *        transmitted.
   * \param channelNumber the channel on which the packet is transmitted
   * \param rate the PHY data rate in units of 500kbps (i.e., the same
   *        units used both for the radiotap and for the prism header)
   * \param isShortPreamble true if short preamble is used, false otherwise
   * \param txPower the transmission power in dBm
   */
  void NotifyMonitorSniffTx (Ptr<const Packet> packet, uint16_t channelFreqMhz, uint16_t channelNumber, uint32_t rate, bool isShortPreamble, uint8_t txPower);

 /**
  * Assign a fixed random variable stream number to the random variables
  * used by this model.  Return the number of streams (possibly zero) that
  * have been assigned.
  *
  * \param stream first stream index to use
  * \return the number of stream indices assigned by this model
  */
  virtual int64_t AssignStreams (int64_t stream) = 0;

  /**
   * \param freq the operating frequency on this node.
   */
  virtual void SetFrequency (uint32_t freq)=0;
  /**
   * \return the operating frequency on this node
   */
  virtual uint32_t GetFrequency (void) const=0;
  /**
   * \param tx the number of transmitters on this node.
   */
  virtual void SetNumberOfTransmitAntennas (uint32_t tx)=0;

  /**
   * \return the number of transmit antenna on this device
   */
  virtual uint32_t GetNumberOfTransmitAntennas (void) const=0;
   /**
   * \param rx the number of recievers on this node.
   */
  virtual void SetNumberOfReceiveAntennas (uint32_t rx)=0 ;
  /**
   * \return the number of recievers on this node.
   */
  virtual uint32_t GetNumberOfReceiveAntennas (void) const=0;
  /**
   * \param guardInterval Enable or disable short guard interval
   */
   virtual void SetGuardInterval (bool guardInterval)=0;
   /**
   *  \return true if short guard interval is supported, false otherwise
   */
  virtual bool GetGuardInterval (void) const = 0;
  /**
   * \param ldpc Enable or disable LDPC
   */
  virtual void SetLdpc (bool ldpc)=0;
  /**
   * \return true if LDPC is supported, false otherwise
   */
  virtual bool GetLdpc (void) const=0;
  /**
   * \param stbc Enable or disable STBC is supported
   */
  virtual void SetStbc (bool stbc)=0;
  /**
   *  \return true if STBC is supported, false otherwise
   */
  virtual bool GetStbc (void) const=0;
   
  /**
   * \param greenfield Enable or disable GreenField
   */
  virtual void SetGreenfield (bool greenfield)=0;
  /**
   * \return true if Greenfield is supported, false otherwise
   */
  virtual bool GetGreenfield (void) const=0;
  /**
   * \return true if channel bonding 40 MHz is supported, false otherwise
   */
  virtual bool GetChannelBonding (void) const = 0;
  /**
   *  \param channelbonding Enable or disable channel bonding
   */
  virtual void SetChannelBonding (bool channelbonding) = 0 ;

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
  TracedCallback<Ptr<const Packet>, uint16_t, uint16_t, uint32_t, bool, double, double> m_phyMonitorSniffRxTrace;

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
  TracedCallback<Ptr<const Packet>, uint16_t, uint16_t, uint32_t, bool,uint8_t> m_phyMonitorSniffTxTrace;

};

/**
 * \param os          output stream
 * \param state       wifi state to stringify
 * \return output stream
 */
std::ostream& operator<< (std::ostream& os, enum WifiPhy::State state);

} // namespace ns3

#endif /* WIFI_PHY_H */
