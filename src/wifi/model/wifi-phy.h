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
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 *          Sébastien Deronne <sebastien.deronne@gmail.com>
 */

#ifndef WIFI_PHY_H
#define WIFI_PHY_H

#include <stdint.h>
#include <map>
#include "ns3/callback.h"
#include "ns3/event-id.h"
#include "ns3/packet.h"
#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/ptr.h"
#include "ns3/mobility-model.h"
#include "ns3/random-variable-stream.h"
#include "wifi-mode.h"
#include "wifi-preamble.h"
#include "wifi-phy-standard.h"
#include "ns3/traced-callback.h"
#include "wifi-tx-vector.h"
#include "wifi-phy-standard.h"
#include "interference-helper.h"
#include "ns3/net-device.h"
#include "ns3/node.h"
#include "wifi-channel.h"

namespace ns3 {

#define VHT_PHY 126
#define HT_PHY 127

class WifiPhyStateHelper;

/**
 * This enumeration defines the type of an MPDU.
 */
enum mpduType
{
  /** The MPDU is not part of an A-MPDU */
  NORMAL_MPDU = 0,
  /** The MPDU is part of an A-MPDU, but is not the last aggregate */
  MPDU_IN_AGGREGATE,
  /** The MPDU is the last aggregate in an A-MPDU */
  LAST_MPDU_IN_AGGREGATE
};

struct signalNoiseDbm
{
  double signal; //in dBm
  double noise; //in dBm
};

struct mpduInfo
{
  enum mpduType type;
  uint32_t mpduRefNumber;
};

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
   * \param txPowerDbm the nominal tx power in dBm
   *
   * We are about to send the first bit of the packet.
   * We do not send any event to notify the end of
   * transmission. Listeners should assume that the
   * channel implicitely reverts to the idle state
   * unless they have received a cca busy report.
   */
  virtual void NotifyTxStart (Time duration, double txPowerDbm) = 0;
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
  /**
   * Notify listeners that we went to sleep
   */
  virtual void NotifySleep (void) = 0;
  /**
   * Notify listeners that we woke up
   */
  virtual void NotifyWakeup (void) = 0;
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
    SWITCHING,
    /**
     * The PHY layer is sleeping.
     */
    SLEEP
  };

  /**
   * arg1: packet received successfully
   * arg2: snr of packet
   * arg3: TXVECTOR of packet
   * arg4: type of preamble used for packet.
   */
  typedef Callback<void, Ptr<Packet>, double, WifiTxVector, enum WifiPreamble> RxOkCallback;
  /**
   * arg1: packet received unsuccessfully
   * arg2: snr of packet
   */
  typedef Callback<void, Ptr<Packet>, double> RxErrorCallback;

  static TypeId GetTypeId (void);

  WifiPhy ();
  virtual ~WifiPhy ();

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
   * \param txVector the TXVECTOR that has tx parameters such as mode, the transmission mode to use to send
   *        this packet, and txPowerLevel, a power level to use to send this packet. The real transmission
   *        power is calculated as txPowerMin + txPowerLevel * (txPowerMax - txPowerMin) / nTxLevels
   * \param preamble the type of preamble to use to send this packet.
   */
  virtual void SendPacket (Ptr<const Packet> packet, WifiTxVector txVector, enum WifiPreamble preamble) = 0;
  /**
   * \param packet the packet to send
   * \param txVector the TXVECTOR that has tx parameters such as mode, the transmission mode to use to send
   *        this packet, and txPowerLevel, a power level to use to send this packet. The real transmission
   *        power is calculated as txPowerMin + txPowerLevel * (txPowerMax - txPowerMin) / nTxLevels
   * \param preamble the type of preamble to use to send this packet.
   * \param mpdutype the type of the MPDU as defined in WifiPhy::mpduType.
   */
  virtual void SendPacket (Ptr<const Packet> packet, WifiTxVector txVector, enum WifiPreamble preamble, enum mpduType mpdutype) = 0;

  /**
   * \param listener the new listener
   *
   * Add the input listener to the list of objects to be notified of
   * PHY-level events.
   */
  virtual void RegisterListener (WifiPhyListener *listener) = 0;
  /**
   * \param listener the listener to be unregistered
   *
   * Remove the input listener from the list of objects to be notified of
   * PHY-level events.
   */
  virtual void UnregisterListener (WifiPhyListener *listener) = 0;

  /**
   * Put in sleep mode.
   */
  virtual void SetSleepMode (void) = 0;
  /**
   * Resume from sleep mode.
   */
  virtual void ResumeFromSleep (void) = 0;

  /**
   * \return true of the current state of the PHY layer is WifiPhy::IDLE, false otherwise.
   */
  virtual bool IsStateIdle (void);
  /**
   * \return true of the current state of the PHY layer is WifiPhy::CCA_BUSY, false otherwise.
   */
  virtual bool IsStateCcaBusy (void);
  /**
   * \return true of the current state of the PHY layer is not WifiPhy::IDLE, false otherwise.
   */
  virtual bool IsStateBusy (void);
  /**
   * \return true of the current state of the PHY layer is WifiPhy::RX, false otherwise.
   */
  virtual bool IsStateRx (void);
  /**
   * \return true of the current state of the PHY layer is WifiPhy::TX, false otherwise.
   */
  virtual bool IsStateTx (void);
  /**
   * \return true of the current state of the PHY layer is WifiPhy::SWITCHING, false otherwise.
   */
  virtual bool IsStateSwitching (void);
  /**
   * \return true if the current state of the PHY layer is WifiPhy::SLEEP, false otherwise.
   */
  virtual bool IsStateSleep (void);
  /**
   * \return the amount of time since the current state has started.
   */
  virtual Time GetStateDuration (void);
  /**
   * \return the predicted delay until this PHY can become WifiPhy::IDLE.
   *
   * The PHY will never become WifiPhy::IDLE _before_ the delay returned by
   * this method but it could become really idle later.
   */
  virtual Time GetDelayUntilIdle (void);

  /**
   * Return the start time of the last received packet.
   *
   * \return the start time of the last received packet
   */
  virtual Time GetLastRxStartTime (void) const;

  /**
   * \param size the number of bytes in the packet to send
   * \param txVector the TXVECTOR used for the transmission of this packet
   * \param preamble the type of preamble to use for this packet.
   * \param frequency the channel center frequency (MHz)
   *
   * \return the total amount of time this PHY will stay busy for the transmission of these bytes.
   */
  Time CalculateTxDuration (uint32_t size, WifiTxVector txVector, enum WifiPreamble preamble, double frequency);
  /**
   * \param size the number of bytes in the packet to send
   * \param txVector the TXVECTOR used for the transmission of this packet
   * \param preamble the type of preamble to use for this packet.
   * \param frequency the channel center frequency (MHz)
   * \param mpdutype the type of the MPDU as defined in WifiPhy::mpduType.
   * \param incFlag this flag is used to indicate that the static variables need to be update or not. This function is called a couple of times for the same packet so static variables should not be increased each time.
   *
   * \return the total amount of time this PHY will stay busy for the transmission of these bytes.
   */
  Time CalculateTxDuration (uint32_t size, WifiTxVector txVector, enum WifiPreamble preamble, double frequency, enum mpduType mpdutype, uint8_t incFlag);

  /**
   * \param txVector the transmission parameters used for this packet
   * \param preamble the type of preamble to use for this packet.
   *
   * \return the total amount of time this PHY will stay busy for the transmission of the PLCP preamble and PLCP header.
   */
  Time CalculatePlcpPreambleAndHeaderDuration (WifiTxVector txVector, enum WifiPreamble preamble);

  /**
   * \param preamble the type of preamble
   * \param txVector the transmission parameters used for this packet
   *
   * \return the training symbol duration
   */
  static Time GetPlcpHtTrainingSymbolDuration (WifiPreamble preamble, WifiTxVector txVector);
  /**
   * \param payloadMode the WifiMode use for the transmission of the payload
   *
   * \return the WifiMode used for the transmission of the HT-SIG and the HT training fields
   *         in Mixed Format and greenfield format PLCP header
   */
  static WifiMode GetHtPlcpHeaderMode (WifiMode payloadMode);
  /**
   * \param payloadMode the WifiMode use for the transmission of the payload
   *
   * \return the WifiMode used for the transmission of the VHT-STF, VHT-LTF and VHT-SIG-B fields
   */
  static WifiMode GetVhtPlcpHeaderMode (WifiMode payloadMode);
  /**
   * \param preamble the type of preamble
   *
   * \return the duration of the HT-SIG in Mixed Format and greenfield format PLCP header
   */
  static Time GetPlcpHtSigHeaderDuration (WifiPreamble preamble);
  /**
   * \param preamble the type of preamble
   *
   * \return the duration of the VHT-SIG-A1 in PLCP header
   */
  static Time GetPlcpVhtSigA1Duration (WifiPreamble preamble);
  /**
   * \param preamble the type of preamble
   *
   * \return the duration of the VHT-SIG-A2 in PLCP header
   */
  static Time GetPlcpVhtSigA2Duration (WifiPreamble preamble);
  /**
   * \param preamble the type of preamble
   *
   * \return the duration of the VHT-SIG-B in PLCP header
   */
  static Time GetPlcpVhtSigBDuration (WifiPreamble preamble);
  /**
   * \param payloadMode the WifiMode use for the transmission of the payload
   * \param preamble the type of preamble
   * \param txVector the transmission parameters used for this packet
   *
   * \return the WifiMode used for the transmission of the PLCP header
   */
  static WifiMode GetPlcpHeaderMode (WifiMode payloadMode, WifiPreamble preamble, WifiTxVector txVector);
  /**
   * \param txVector the transmission parameters used for this packet
   * \param preamble the type of preamble
   *
   * \return the duration of the PLCP header
   */
  static Time GetPlcpHeaderDuration (WifiTxVector txVector, WifiPreamble preamble);
  /**
   * \param txVector the transmission parameters used for this packet
   * \param preamble the type of preamble
   *
   * \return the duration of the PLCP preamble
   */
  static Time GetPlcpPreambleDuration (WifiTxVector txVector, WifiPreamble preamble);
  /**
   * \param size the number of bytes in the packet to send
   * \param txVector the TXVECTOR used for the transmission of this packet
   * \param preamble the type of preamble to use for this packet
   * \param frequency the channel center frequency (MHz)
   *
   * \return the duration of the payload
   */
  Time GetPayloadDuration (uint32_t size, WifiTxVector txVector, WifiPreamble preamble, double frequency);
  /**
   * \param size the number of bytes in the packet to send
   * \param txVector the TXVECTOR used for the transmission of this packet
   * \param preamble the type of preamble to use for this packet
   * \param frequency the channel center frequency (MHz)
   * \param mpdutype the type of the MPDU as defined in WifiPhy::mpduType.
   * \param incFlag this flag is used to indicate that the static variables need to be update or not. This function is called a couple of times for the same packet so static variables should not be increased each time
   *
   * \return the duration of the payload
   */
  Time GetPayloadDuration (uint32_t size, WifiTxVector txVector, WifiPreamble preamble, double frequency, enum mpduType mpdutype, uint8_t incFlag);

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
  virtual uint32_t GetNModes (void) const;
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
   *
   * \return the mode whose index is specified.
   *
   * \sa WifiPhy::GetNModes()
   */
  virtual WifiMode GetMode (uint32_t mode) const;
  /**
   * Check if the given WifiMode is supported by the PHY.
   *
   * \param mode the wifi mode to check
   *
   * \return true if the given mode is supported,
   *         false otherwise
   */
  virtual bool IsModeSupported (WifiMode mode) const;
  /**
   * Check if the given WifiMode is supported by the PHY.
   *
   * \param mode the wifi mode to check
   *
   * \return true if the given mode is supported,
   *         false otherwise
   */
  virtual bool IsMcsSupported (WifiMode mcs) const;

  /**
   * \param txVector the transmission vector
   * \param ber the probability of bit error rate
   *
   * \return the minimum snr which is required to achieve
   *          the requested ber for the specified transmission vector. (W/W)
   */
  virtual double CalculateSnr (WifiTxVector txVector, double ber) const;

  /**
  * The WifiPhy::NBssMembershipSelectors() method is used
  * (e.g., by a WifiRemoteStationManager) to determine the set of
  * transmission/reception modes that this WifiPhy(-derived class)
  * can support - a set of WifiMode objects which we call the
  * BssMembershipSelectorSet, and which is stored as WifiPhy::m_bssMembershipSelectorSet.
  *
  * \return the memebership selector whose index is specified.
  */
  virtual uint32_t GetNBssMembershipSelectors (void) const;
  /**
  * The WifiPhy::BssMembershipSelector() method is used
  * (e.g., by a WifiRemoteStationManager) to determine the set of
  * transmission/reception modes that this WifiPhy(-derived class)
  * can support - a set of WifiMode objects which we call the
  * BssMembershipSelectorSet, and which is stored as WifiPhy::m_bssMembershipSelectorSet.
  *
  * \param selector index in array of supported memberships
  *
  * \return the memebership selector whose index is specified.
  */
  virtual uint32_t GetBssMembershipSelector (uint32_t selector) const;
  /**
   * The WifiPhy::GetMembershipSelectorModes() method is used
   * (e.g., by a WifiRemoteStationManager) to determine the set of
   * transmission/reception modes that this WifiPhy(-derived class)
   * can support - a set of WifiMode objects which we call the
   * BssMembershipSelectorSet, and which is stored as WifiPhy::m_bssMembershipSelectorSet.
   *
   * \param selector index in array of supported memberships
   *
   * \return a WifiModeList that contains the WifiModes associrated with the selected index.
   *
   * \sa WifiPhy::GetMembershipSelectorModes()
   */
  virtual WifiModeList GetMembershipSelectorModes (uint32_t selector);
  /**
   * The WifiPhy::GetNMcs() method is used
   * (e.g., by a WifiRemoteStationManager) to determine the set of
   * transmission/reception MCS indexes that this WifiPhy(-derived class)
   * can support - a set of MCS indexes which we call the
   * DeviceMcsSet, and which is stored as WifiPhy::m_deviceMcsSet.
   *
   * \return the MCS index whose index is specified.
   */
  virtual uint8_t GetNMcs (void) const;
  /**
   * The WifiPhy::GetMcs() method is used
   * (e.g., by a WifiRemoteStationManager) to determine the set of
   * transmission/reception MCS indexes that this WifiPhy(-derived class)
   * can support - a set of MCS indexes which we call the
   * DeviceMcsSet, and which is stored as WifiPhy::m_deviceMcsSet.
   *
   * \param mcs index in array of supported MCS
   *
   * \return the MCS index whose index is specified.
   */
  virtual WifiMode GetMcs (uint8_t mcs) const;

  /**
   * \brief Set channel number.
   *
   * Channel center frequency = Channel starting frequency + 5 MHz * (nch - 1)
   *
   * where Starting channel frequency is standard-dependent, see SetStandard()
   * as defined in (Section 18.3.8.4.2 "Channel numbering"; IEEE Std 802.11-2012).
   * This method may fail to take action if the Phy model determines that
   * the channel number cannot be switched for some reason (e.g. sleep state)
   *
   * \param id the channel number
   */
  virtual void SetChannelNumber (uint16_t id);
  /**
   * Return current channel number.
   *
   * \return the current channel number
   */
  virtual uint16_t GetChannelNumber (void) const;
  /**
   * \return the required time for channel switch operation of this WifiPhy
   */
  virtual Time GetChannelSwitchDelay (void) const;

  /**
   * Configure the PHY-level parameters for different Wi-Fi standard.
   *
   * \param standard the Wi-Fi standard
   */
  virtual void ConfigureStandard (enum WifiPhyStandard standard);

  /**
   * Get the configured Wi-Fi standard
   *
   * \return the Wi-Fi standard that has been configured
   */
  virtual enum WifiPhyStandard GetStandard (void) const;

  /**
   * Add a channel definition to the WifiPhy.  The pair (channelNumber,
   * WifiPhyStandard) may then be used to lookup a pair (frequency, 
   * channelWidth).
   *
   * If the channel is not already defined for the standard, the method
   * should return true; otherwise false.
   *
   * \param channelNumber the channel number to define
   * \param standard the applicable WifiPhyStandard
   * \param frequency the frequency (MHz)
   * \param channelWidth the channel width (MHz)
   *
   * \return true if the channel definition succeeded
   */
  bool DefineChannelNumber (uint16_t channelNumber, enum WifiPhyStandard standard, uint32_t frequency, uint32_t channelWidth);

  /**
   * A pair of a ChannelNumber and WifiPhyStandard
   */
  typedef std::pair<uint16_t, enum WifiPhyStandard> ChannelNumberStandardPair;
  /**
   * A pair of a center Frequency and a ChannelWidth
   */
  typedef std::pair<uint32_t, uint32_t> FrequencyWidthPair;

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
   * Return a WifiMode for ERP-OFDM at 6Mbps.
   *
   * \return a WifiMode for ERP-OFDM at 6Mbps
   */
  static WifiMode GetErpOfdmRate6Mbps ();
  /**
   * Return a WifiMode for ERP-OFDM at 9Mbps.
   *
   * \return a WifiMode for ERP-OFDM at 9Mbps
   */
  static WifiMode GetErpOfdmRate9Mbps ();
  /**
   * Return a WifiMode for ERP-OFDM at 12Mbps.
   *
   * \return a WifiMode for ERP-OFDM at 12Mbps
   */
  static WifiMode GetErpOfdmRate12Mbps ();
  /**
   * Return a WifiMode for ERP-OFDM at 18Mbps.
   *
   * \return a WifiMode for ERP-OFDM at 18Mbps
   */
  static WifiMode GetErpOfdmRate18Mbps ();
  /**
   * Return a WifiMode for ERP-OFDM at 24Mbps.
   *
   * \return a WifiMode for ERP-OFDM at 24Mbps
   */
  static WifiMode GetErpOfdmRate24Mbps ();
  /**
   * Return a WifiMode for ERP-OFDM at 36Mbps.
   *
   * \return a WifiMode for ERP-OFDM at 36Mbps
   */
  static WifiMode GetErpOfdmRate36Mbps ();
  /**
   * Return a WifiMode for ERP-OFDM at 48Mbps.
   *
   * \return a WifiMode for ERP-OFDM at 48Mbps
   */
  static WifiMode GetErpOfdmRate48Mbps ();
  /**
   * Return a WifiMode for ERP-OFDM at 54Mbps.
   *
   * \return a WifiMode for ERP-OFDM at 54Mbps
   */
  static WifiMode GetErpOfdmRate54Mbps ();
  /**
   * Return a WifiMode for OFDM at 6Mbps.
   *
   * \return a WifiMode for OFDM at 6Mbps
   */
  static WifiMode GetOfdmRate6Mbps ();
  /**
   * Return a WifiMode for OFDM at 9Mbps.
   *
   * \return a WifiMode for OFDM at 9Mbps
   */
  static WifiMode GetOfdmRate9Mbps ();
  /**
   * Return a WifiMode for OFDM at 12Mbps.
   *
   * \return a WifiMode for OFDM at 12Mbps
   */
  static WifiMode GetOfdmRate12Mbps ();
  /**
   * Return a WifiMode for OFDM at 18Mbps.
   *
   * \return a WifiMode for OFDM at 18Mbps
   */
  static WifiMode GetOfdmRate18Mbps ();
  /**
   * Return a WifiMode for OFDM at 24Mbps.
   *
   * \return a WifiMode for OFDM at 24Mbps
   */
  static WifiMode GetOfdmRate24Mbps ();
  /**
   * Return a WifiMode for OFDM at 36Mbps.
   *
   * \return a WifiMode for OFDM at 36Mbps
   */
  static WifiMode GetOfdmRate36Mbps ();
  /**
   * Return a WifiMode for OFDM at 48Mbps.
   *
   * \return a WifiMode for OFDM at 48Mbps
   */
  static WifiMode GetOfdmRate48Mbps ();
  /**
   * Return a WifiMode for OFDM at 54Mbps.
   *
   * \return a WifiMode for OFDM at 54Mbps
   */
  static WifiMode GetOfdmRate54Mbps ();
  /**
   * Return a WifiMode for OFDM at 3Mbps with 10MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 3Mbps with 10MHz channel spacing
   */
  static WifiMode GetOfdmRate3MbpsBW10MHz ();
  /**
   * Return a WifiMode for OFDM at 4.5Mbps with 10MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 4.5Mbps with 10MHz channel spacing
   */
  static WifiMode GetOfdmRate4_5MbpsBW10MHz ();
  /**
   * Return a WifiMode for OFDM at 6Mbps with 10MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 6Mbps with 10MHz channel spacing
   */
  static WifiMode GetOfdmRate6MbpsBW10MHz ();
  /**
   * Return a WifiMode for OFDM at 9Mbps with 10MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 9Mbps with 10MHz channel spacing
   */
  static WifiMode GetOfdmRate9MbpsBW10MHz ();
  /**
   * Return a WifiMode for OFDM at 12Mbps with 10MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 12Mbps with 10MHz channel spacing
   */
  static WifiMode GetOfdmRate12MbpsBW10MHz ();
  /**
   * Return a WifiMode for OFDM at 18Mbps with 10MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 18Mbps with 10MHz channel spacing
   */
  static WifiMode GetOfdmRate18MbpsBW10MHz ();
  /**
   * Return a WifiMode for OFDM at 24Mbps with 10MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 24Mbps with 10MHz channel spacing
   */
  static WifiMode GetOfdmRate24MbpsBW10MHz ();
  /**
   * Return a WifiMode for OFDM at 27Mbps with 10MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 27Mbps with 10MHz channel spacing
   */
  static WifiMode GetOfdmRate27MbpsBW10MHz ();
  /**
   * Return a WifiMode for OFDM at 1.5Mbps with 5MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 1.5Mbps with 5MHz channel spacing
   */
  static WifiMode GetOfdmRate1_5MbpsBW5MHz ();
  /**
   * Return a WifiMode for OFDM at 2.25Mbps with 5MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 2.25Mbps with 5MHz channel spacing
   */
  static WifiMode GetOfdmRate2_25MbpsBW5MHz ();
  /**
   * Return a WifiMode for OFDM at 3Mbps with 5MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 3Mbps with 5MHz channel spacing
   */
  static WifiMode GetOfdmRate3MbpsBW5MHz ();
  /**
   * Return a WifiMode for OFDM at 4.5Mbps with 5MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 4.5Mbps with 5MHz channel spacing
   */
  static WifiMode GetOfdmRate4_5MbpsBW5MHz ();
  /**
   * Return a WifiMode for OFDM at 6Mbps with 5MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 6Mbps with 5MHz channel spacing
   */
  static WifiMode GetOfdmRate6MbpsBW5MHz ();
  /**
   * Return a WifiMode for OFDM at 9Mbps with 5MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 9Mbps with 5MHz channel spacing
   */
  static WifiMode GetOfdmRate9MbpsBW5MHz ();
  /**
   * Return a WifiMode for OFDM at 12Mbps with 5MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 12Mbps with 5MHz channel spacing
   */
  static WifiMode GetOfdmRate12MbpsBW5MHz ();
  /**
   * Return a WifiMode for OFDM at 13.5Mbps with 5MHz channel spacing.
   *
   * \return a WifiMode for OFDM at 13.5Mbps with 5MHz channel spacing
   */
  static WifiMode GetOfdmRate13_5MbpsBW5MHz ();

  /**
   * Return MCS 0 from HT MCS values.
   *
   * \return MCS 0 from HT MCS values
   */
  static WifiMode GetHtMcs0 ();
  /**
   * Return MCS 1 from HT MCS values.
   *
   * \return MCS 1 from HT MCS values
   */
  static WifiMode GetHtMcs1 ();
  /**
   * Return MCS 2 from HT MCS values.
   *
   * \return MCS 2 from HT MCS values
   */
  static WifiMode GetHtMcs2 ();
  /**
   * Return MCS 3 from HT MCS values.
   *
   * \return MCS 3 from HT MCS values
   */
  static WifiMode GetHtMcs3 ();
  /**
   * Return MCS 4 from HT MCS values.
   *
   * \return MCS 4 from HT MCS values
   */
  static WifiMode GetHtMcs4 ();
  /**
   * Return MCS 5 from HT MCS values.
   *
   * \return MCS 5 from HT MCS values
   */
  static WifiMode GetHtMcs5 ();
  /**
   * Return MCS 6 from HT MCS values.
   *
   * \return MCS 6 from HT MCS values
   */
  static WifiMode GetHtMcs6 ();
  /**
   * Return MCS 7 from HT MCS values.
   *
   * \return MCS 7 from HT MCS values
   */
  static WifiMode GetHtMcs7 ();
  /**
   * Return MCS 8 from HT MCS values.
   *
   * \return MCS 8 from HT MCS values
   */
  static WifiMode GetHtMcs8 ();
  /**
   * Return MCS 9 from HT MCS values.
   *
   * \return MCS 9 from HT MCS values
   */
  static WifiMode GetHtMcs9 ();
  /**
   * Return MCS 10 from HT MCS values.
   *
   * \return MCS 10 from HT MCS values
   */
  static WifiMode GetHtMcs10 ();
  /**
   * Return MCS 11 from HT MCS values.
   *
   * \return MCS 11 from HT MCS values
   */
  static WifiMode GetHtMcs11 ();
  /**
   * Return MCS 12 from HT MCS values.
   *
   * \return MCS 12 from HT MCS values
   */
  static WifiMode GetHtMcs12 ();
  /**
   * Return MCS 13 from HT MCS values.
   *
   * \return MCS 13 from HT MCS values
   */
  static WifiMode GetHtMcs13 ();
  /**
   * Return MCS 14 from HT MCS values.
   *
   * \return MCS 14 from HT MCS values
   */
  static WifiMode GetHtMcs14 ();
  /**
   * Return MCS 15 from HT MCS values.
   *
   * \return MCS 15 from HT MCS values
   */
  static WifiMode GetHtMcs15 ();
  /**
   * Return MCS 16 from HT MCS values.
   *
   * \return MCS 16 from HT MCS values
   */
  static WifiMode GetHtMcs16 ();
  /**
   * Return MCS 17 from HT MCS values.
   *
   * \return MCS 17 from HT MCS values
   */
  static WifiMode GetHtMcs17 ();
  /**
   * Return MCS 18 from HT MCS values.
   *
   * \return MCS 18 from HT MCS values
   */
  static WifiMode GetHtMcs18 ();
  /**
   * Return MCS 19 from HT MCS values.
   *
   * \return MCS 19 from HT MCS values
   */
  static WifiMode GetHtMcs19 ();
  /**
   * Return MCS 20 from HT MCS values.
   *
   * \return MCS 20 from HT MCS values
   */
  static WifiMode GetHtMcs20 ();
  /**
   * Return MCS 21 from HT MCS values.
   *
   * \return MCS 21 from HT MCS values
   */
  static WifiMode GetHtMcs21 ();
  /**
   * Return MCS 22 from HT MCS values.
   *
   * \return MCS 22 from HT MCS values
   */
  static WifiMode GetHtMcs22 ();
  /**
   * Return MCS 23 from HT MCS values.
   *
   * \return MCS 23 from HT MCS values
   */
  static WifiMode GetHtMcs23 ();
  /**
   * Return MCS 24 from HT MCS values.
   *
   * \return MCS 24 from HT MCS values
   */
  static WifiMode GetHtMcs24 ();
  /**
   * Return MCS 25 from HT MCS values.
   *
   * \return MCS 25 from HT MCS values
   */
  static WifiMode GetHtMcs25 ();
  /**
   * Return MCS 26 from HT MCS values.
   *
   * \return MCS 26 from HT MCS values
   */
  static WifiMode GetHtMcs26 ();
  /**
   * Return MCS 27 from HT MCS values.
   *
   * \return MCS 27 from HT MCS values
   */
  static WifiMode GetHtMcs27 ();
  /**
   * Return MCS 28 from HT MCS values.
   *
   * \return MCS 28 from HT MCS values
   */
  static WifiMode GetHtMcs28 ();
  /**
   * Return MCS 29 from HT MCS values.
   *
   * \return MCS 29 from HT MCS values
   */
  static WifiMode GetHtMcs29 ();
  /**
   * Return MCS 30 from HT MCS values.
   *
   * \return MCS 30 from HT MCS values
   */
  static WifiMode GetHtMcs30 ();
  /**
   * Return MCS 31 from HT MCS values.
   *
   * \return MCS 31 from HT MCS values
   */
  static WifiMode GetHtMcs31 ();

  /**
   * Return MCS 0 from VHT MCS values.
   *
   * \return MCS 0 from VHT MCS values
   */
  static WifiMode GetVhtMcs0 ();
  /**
   * Return MCS 1 from VHT MCS values.
   *
   * \return MCS 1 from VHT MCS values
   */
  static WifiMode GetVhtMcs1 ();
  /**
   * Return MCS 2 from VHT MCS values.
   *
   * \return MCS 2 from VHT MCS values
   */
  static WifiMode GetVhtMcs2 ();
  /**
   * Return MCS 3 from VHT MCS values.
   *
   * \return MCS 3 from VHT MCS values
   */
  static WifiMode GetVhtMcs3 ();
  /**
   * Return MCS 4 from VHT MCS values.
   *
   * \return MCS 4 from VHT MCS values
   */
  static WifiMode GetVhtMcs4 ();
  /**
   * Return MCS 5 from VHT MCS values.
   *
   * \return MCS 5 from VHT MCS values
   */
  static WifiMode GetVhtMcs5 ();
  /**
   * Return MCS 6 from VHT MCS values.
   *
   * \return MCS 6 from VHT MCS values
   */
  static WifiMode GetVhtMcs6 ();
  /**
   * Return MCS 7 from VHT MCS values.
   *
   * \return MCS 7 from VHT MCS values
   */
  static WifiMode GetVhtMcs7 ();
  /**
   * Return MCS 8 from VHT MCS values.
   *
   * \return MCS 8 from VHT MCS values
   */
  static WifiMode GetVhtMcs8 ();
  /**
   * Return MCS 9 from VHT MCS values.
   *
   * \return MCS 9 from VHT MCS values
   */
  static WifiMode GetVhtMcs9 ();
  /**
   * The standard disallows certain combinations of WifiMode, number of
   * spatial streams, and channel widths.  This method can be used to
   * check whether this WifiTxVector contains an invalid combination.
   *
   * \param txVector the WifiTxVector to inspect
   * \return true if the WifiTxVector parameters are allowed by the standard
   */
  static bool IsValidTxVector (WifiTxVector txVector);

  /**
   * Public method used to fire a PhyTxBegin trace.
   * Implemented for encapsulation purposes.
   *
   * \param packet the packet being transmitted
   */
  void NotifyTxBegin (Ptr<const Packet> packet);
  /**
   * Public method used to fire a PhyTxEnd trace.
   * Implemented for encapsulation purposes.
   *
   * \param packet the packet that was transmitted
   */
  void NotifyTxEnd (Ptr<const Packet> packet);
  /**
   * Public method used to fire a PhyTxDrop trace.
   * Implemented for encapsulation purposes.
   *
   * \param packet the packet that was failed to transmitted
   */
  void NotifyTxDrop (Ptr<const Packet> packet);
  /**
   * Public method used to fire a PhyRxBegin trace.
   * Implemented for encapsulation purposes.
   *
   * \param packet the packet being received
   */
  void NotifyRxBegin (Ptr<const Packet> packet);
  /**
   * Public method used to fire a PhyRxEnd trace.
   * Implemented for encapsulation purposes.
   *
   * \param packet the packet received
   */
  void NotifyRxEnd (Ptr<const Packet> packet);
  /**
   * Public method used to fire a PhyRxDrop trace.
   * Implemented for encapsulation purposes.
   *
   * \param packet the packet that was not successfully received
   */
  void NotifyRxDrop (Ptr<const Packet> packet);

  /**
   * Public method used to fire a MonitorSniffer trace for a wifi packet being received.
   * Implemented for encapsulation purposes.
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
   * \param preamble the preamble of the packet
   * \param txVector the TXVECTOR that holds rx parameters
   * \param aMpdu the type of the packet (0 is not A-MPDU, 1 is a MPDU that is part of an A-MPDU and 2 is the last MPDU in an A-MPDU)
   *        and the A-MPDU reference number (must be a different value for each A-MPDU but the same for each subframe within one A-MPDU)
   * \param signalNoise signal power and noise power in dBm
   */
  void NotifyMonitorSniffRx (Ptr<const Packet> packet, uint16_t channelFreqMhz,
                             uint16_t channelNumber, uint32_t rate, WifiPreamble preamble,
                             WifiTxVector txVector, struct mpduInfo aMpdu, struct signalNoiseDbm signalNoise);

  /**
   * TracedCallback signature for monitor mode receive events.
   *
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
   * \param preamble the preamble of the packet
   * \param txVector the TXVECTOR that holds rx parameters
   * \param aMpdu the type of the packet (0 is not A-MPDU, 1 is a MPDU that is part of an A-MPDU and 2 is the last MPDU in an A-MPDU)
   *        and the A-MPDU reference number (must be a different value for each A-MPDU but the same for each subframe within one A-MPDU)
   * \param signalNoise signal power and noise power in dBm
   * \todo WifiTxVector should be passed by const reference because
   * of its size.
   */
  typedef void (* MonitorSnifferRxCallback)(Ptr<const Packet> packet, uint16_t channelFreqMhz,
                                            uint16_t channelNumber, uint32_t rate, WifiPreamble preamble,
                                            WifiTxVector txVector, struct mpduInfo aMpdu, struct signalNoiseDbm signalNoise);

  /**
   * Public method used to fire a MonitorSniffer trace for a wifi packet being transmitted.
   * Implemented for encapsulation purposes.
   *
   * \param packet the packet being transmitted
   * \param channelFreqMhz the frequency in MHz at which the packet is
   *        transmitted.
   * \param channelNumber the channel on which the packet is transmitted
   * \param rate the PHY data rate in units of 500kbps (i.e., the same
   *        units used both for the radiotap and for the prism header)
   * \param preamble the preamble of the packet
   * \param txVector the TXVECTOR that holds tx parameters
   * \param aMpdu the type of the packet (0 is not A-MPDU, 1 is a MPDU that is part of an A-MPDU and 2 is the last MPDU in an A-MPDU)
   *        and the A-MPDU reference number (must be a different value for each A-MPDU but the same for each subframe within one A-MPDU)
   */
  void NotifyMonitorSniffTx (Ptr<const Packet> packet, uint16_t channelFreqMhz,
                             uint16_t channelNumber, uint32_t rate, WifiPreamble preamble,
                             WifiTxVector txVector, struct mpduInfo aMpdu);

  /**
   * TracedCallback signature for monitor mode transmit events.
   *
   * \param packet the packet being transmitted
   * \param channelFreqMhz the frequency in MHz at which the packet is
   *        transmitted.
   * \param channelNumber the channel on which the packet is transmitted
   * \param rate the PHY data rate in units of 500kbps (i.e., the same
   *        units used both for the radiotap and for the prism header)
   * \param preamble the preamble of the packet
   * \param txVector the TXVECTOR that holds tx parameters
   * \param aMpdu the type of the packet (0 is not A-MPDU, 1 is a MPDU that is part of an A-MPDU and 2 is the last MPDU in an A-MPDU)
   *        and the A-MPDU reference number (must be a different value for each A-MPDU but the same for each subframe within one A-MPDU)
   * \todo WifiTxVector should be passed by const reference because
   * of its size.
   */
  typedef void (* MonitorSnifferTxCallback)(const Ptr<const Packet> packet, uint16_t channelFreqMhz,
                                            uint16_t channelNumber, uint32_t rate, WifiPreamble preamble,
                                            WifiTxVector txVector, struct mpduInfo aMpdu);

  /**
   * Assign a fixed random variable stream number to the random variables
   * used by this model. Return the number of streams (possibly zero) that
   * have been assigned.
   *
   * \param stream first stream index to use
   * \return the number of stream indices assigned by this model
   */
  virtual int64_t AssignStreams (int64_t stream);

  /**
   * Sets the energy detection threshold (dBm).
   * The energy of a received signal should be higher than
   * this threshold (dbm) to allow the PHY layer to detect the signal.
   *
   * \param threshold the energy detction threshold in dBm
   */
  void SetEdThreshold (double threshold);
  /**
   * Return the energy detection threshold (dBm).
   *
   * \return the energy detection threshold in dBm
   */
  double GetEdThreshold (void) const;
  /**
   * Return the energy detection threshold.
   *
   * \return the energy detection threshold.
   */
  double GetEdThresholdW (void) const;
  /**
   * Sets the CCA threshold (dBm). The energy of a received signal
   * should be higher than this threshold to allow the PHY
   * layer to declare CCA BUSY state.
   *
   * \param threshold the CCA threshold in dBm
   */
  void SetCcaMode1Threshold (double threshold);
  /**
   * Return the CCA threshold (dBm).
   *
   * \return the CCA threshold in dBm
   */
  double GetCcaMode1Threshold (void) const;
  /**
   * Sets the RX loss (dB) in the Signal-to-Noise-Ratio due to non-idealities in the receiver.
   *
   * \param noiseFigureDb noise figure in dB
   */
  void SetRxNoiseFigure (double noiseFigureDb);
  /**
   * Return the RX noise figure (dBm).
   *
   * \return the RX noise figure in dBm
   */
  double GetRxNoiseFigure (void) const;
  /**
   * Sets the minimum available transmission power level (dBm).
   *
   * \param start the minimum transmission power level (dBm)
   */
  void SetTxPowerStart (double start);
  /**
   * Return the minimum available transmission power level (dBm).
   *
   * \return the minimum available transmission power level (dBm)
   */
  virtual double GetTxPowerStart (void) const;
  /**
   * Sets the maximum available transmission power level (dBm).
   *
   * \param end the maximum transmission power level (dBm)
   */
  void SetTxPowerEnd (double end);
  /**
   * Return the maximum available transmission power level (dBm).
   *
   * \return the maximum available transmission power level (dBm)
   */
  virtual double GetTxPowerEnd (void) const;
  /**
   * Sets the number of transmission power levels available between the
   * minimum level and the maximum level. Transmission power levels are
   * equally separated (in dBm) with the minimum and the maximum included.
   *
   * \param n the number of available levels
   */
  void SetNTxPower (uint32_t n);
  /**
   * Return the number of available transmission power levels.
   *
   * \return the number of available transmission power levels
   */
  virtual uint32_t GetNTxPower (void) const;
  /**
   * Sets the transmission gain (dB).
   *
   * \param gain the transmission gain in dB
   */
  void SetTxGain (double gain);
  /**
   * Return the transmission gain (dB).
   *
   * \return the transmission gain in dB
   */
  double GetTxGain (void) const;
  /**
   * Sets the reception gain (dB).
   *
   * \param gain the reception gain in dB
   */
  void SetRxGain (double gain);
  /**
   * Return the reception gain (dB).
   *
   * \return the reception gain in dB
   */
  double GetRxGain (void) const;
  /**
   * Sets the device this PHY is associated with.
   *
   * \param device the device this PHY is associated with
   */
  void SetDevice (Ptr<NetDevice> device);
  /**
   * Return the device this PHY is associated with
   *
   * \return the device this PHY is associated with
   */
  Ptr<NetDevice> GetDevice (void) const;
  /**
   * \brief assign a mobility model to this device
   *
   * This method allows a user to specify a mobility model that should be
   * associated with this physical layer.  Calling this method is optional
   * and only necessary if the user wants to override the mobility model
   * that is aggregated to the node.
   *
   * \param mobility the mobility model this PHY is associated with
   */
  void SetMobility (Ptr<MobilityModel> mobility);
  /**
   * Return the mobility model this PHY is associated with.
   * This method will return either the mobility model that has been
   * explicitly set by a call to YansWifiPhy::SetMobility(), or else
   * will return the mobility model (if any) that has been aggregated
   * to the node.
   *
   * \return the mobility model this PHY is associated with
   */
  Ptr<MobilityModel> GetMobility (void);

  /**
   * \param freq the operating center frequency (MHz) on this node.
   */
  virtual void SetFrequency (uint32_t freq);
  /**
   * \return the operating center frequency (MHz) 
   */
  virtual uint32_t GetFrequency (void) const;
  /**
   * \param tx the number of transmitters on this node.
   */
  virtual void SetNumberOfTransmitAntennas (uint32_t tx);
  /**
   * \return the number of transmit antenna on this device
   */
  virtual uint32_t GetNumberOfTransmitAntennas (void) const;
  /**
  * \param rx the number of receivers on this node.
  */
  virtual void SetNumberOfReceiveAntennas (uint32_t rx);
  /**
   * \return the number of receivers on this node.
   */
  virtual uint32_t GetNumberOfReceiveAntennas (void) const;

  /**
   * Enable or disable short/long guard interval.
   *
   * \param guardInterval Enable or disable guard interval
   */
  virtual void SetGuardInterval (bool guardInterval);
  /**
   * Return whether guard interval is being used.
   *
   * \return true if guard interval is being used, false otherwise
   */
  virtual bool GetGuardInterval (void) const;
  /**
   * Enable or disable LDPC.
   * \param ldpc Enable or disable LDPC
   */
  virtual void SetLdpc (bool ldpc);
  /**
   * Return if LDPC is supported.
   *
   * \return true if LDPC is supported, false otherwise
   */
  virtual bool GetLdpc (void) const;
  /**
   * Enable or disable STBC.
   *
   * \param stbc Enable or disable STBC
   */
  virtual void SetStbc (bool stbc);
  /**
   * Return whether STBC is supported.
   *
   * \return true if STBC is supported, false otherwise
   */
  virtual bool GetStbc (void) const;
  /**
   * Enable or disable Greenfield support.
   *
   * \param greenfield Enable or disable Greenfield
   */
  virtual void SetGreenfield (bool greenfield);
  /**
   * Return whether Greenfield is supported.
   *
   * \return true if Greenfield is supported, false otherwise
   */
  virtual bool GetGreenfield (void) const;
  /**
   * Enable or disable short PLCP preamble.
   *
   * \param preamble sets whether short PLCP preamble is supported or not
   */
  virtual void SetShortPlcpPreambleSupported (bool preamble);
  /**
   * Return whether short PLCP preamble is supported.
   *
   * \returns if short PLCP preamble is supported or not
   */
  virtual bool GetShortPlcpPreambleSupported (void) const;

  /**
   * Sets the error rate model.
   *
   * \param rate the error rate model
   */
  void SetErrorRateModel (Ptr<ErrorRateModel> rate);
  /**
   * Return the error rate model this PHY is using.
   *
   * \return the error rate model this PHY is using
   */
  Ptr<ErrorRateModel> GetErrorRateModel (void) const;

  /**
   * \return the channel width
   */
  virtual uint32_t GetChannelWidth (void) const;
  /**
   * \param channelwidth channel width
   */
  virtual void SetChannelWidth (uint32_t channelwidth);
  /**
   * \param channelwidth channel width (in MHz) to support
   */
  virtual void AddSupportedChannelWidth (uint32_t channelwidth);
  /**
   * \return a vector containing the supported channel widths, values in MHz
   */
  virtual std::vector<uint32_t> GetSupportedChannelWidthSet (void) const;
  /**
   * \return the maximum number of supported Rx spatial streams
   */
  virtual uint8_t GetSupportedRxSpatialStreams (void) const;
  /**
   * \return the maximum number of supported Tx spatial streams
   */
  virtual uint8_t GetSupportedTxSpatialStreams (void) const;
  /**
   * Convert from dBm to Watts.
   *
   * \param dbm the power in dBm
   *
   * \return the equivalent Watts for the given dBm
   */
  double DbmToW (double dbm) const;
  /**
   * Convert from dB to ratio.
   *
   * \param db
   *
   * \return ratio
   */
  double DbToRatio (double db) const;
  /**
   * Convert from Watts to dBm.
   *
   * \param w the power in Watts
   *
   * \return the equivalent dBm for the given Watts
   */
  double WToDbm (double w) const;
  /**
   * Convert from ratio to dB.
   *
   * \param ratio
   *
   * \return dB
   */
  double RatioToDb (double ratio) const;

protected:
  // Inherited
  virtual void DoInitialize (void);
  virtual void DoDispose (void);

  /**
   * The default implementation does nothing and returns true.  This method 
   * is typically called internally by SetChannelNumber ().
   *
   * \brief Perform any actions necessary when user changes channel number
   * \param id channel number to try to switch to
   * \return true if WifiPhy can actually change the number; false if not
   * \see SetChannelNumber
   */
  virtual bool DoChannelSwitch (uint16_t id);
  /**
   * The default implementation does nothing and returns true.  This method
   * is typically called internally by SetFrequency ().
   *
   * \brief Perform any actions necessary when user changes frequency
   * \param frequency frequency to try to switch to
   * \return true if WifiPhy can actually change the frequency; false if not
   * \see SetFrequency
   */
  virtual bool DoFrequencySwitch (uint32_t frequency);

  /**
   * Get the power of the given power level in dBm.
   * In SpectrumWifiPhy implementation, the power levels are equally spaced (in dBm).
   *
   * \param power the power level
   *
   * \return the transmission power in dBm at the given power level
   */
  double GetPowerDbm (uint8_t power) const;
  
  InterferenceHelper m_interference;   //!< Pointer to InterferenceHelper
  Ptr<UniformRandomVariable> m_random; //!< Provides uniform random variables.
  Ptr<WifiPhyStateHelper> m_state;     //!< Pointer to WifiPhyStateHelper

  uint16_t m_mpdusNum;                 //!< carries the number of expected mpdus that are part of an A-MPDU
  bool m_plcpSuccess;                  //!< Flag if the PLCP of the packet or the first MPDU in an A-MPDU has been received
  uint32_t m_txMpduReferenceNumber;    //!< A-MPDU reference number to identify all transmitted subframes belonging to the same received A-MPDU
  uint32_t m_rxMpduReferenceNumber;    //!< A-MPDU reference number to identify all received subframes belonging to the same received A-MPDU
  
  EventId m_endRxEvent;
  EventId m_endPlcpRxEvent;

private:
  /**
   * \brief post-construction setting of frequency and/or channel number
   *
   * This method exists to handle the fact that two attribute values,
   * Frequency and ChannelNumber, are coupled.  The initialization of
   * these values needs to be deferred until after attribute construction 
   * time, to avoid static initialization order issues.  This method is
   * typically called either when ConfigureStandard () is called or when
   * DoInitialize () is called.  
   */
  void InitializeFrequencyChannelNumber (void);
  /**
   * Configure WifiPhy with appropriate channel frequency and
   * supported rates for 802.11a standard.
   */
  void Configure80211a (void);
  /**
   * Configure WifiPhy with appropriate channel frequency and
   * supported rates for 802.11b standard.
   */
  void Configure80211b (void);
  /**
   * Configure WifiPhy with appropriate channel frequency and
   * supported rates for 802.11g standard.
   */
  void Configure80211g (void);
  /**
   * Configure WifiPhy with appropriate channel frequency and
   * supported rates for 802.11a standard with 10MHz channel spacing.
   */
  void Configure80211_10Mhz (void);
  /**
   * Configure WifiPhy with appropriate channel frequency and
   * supported rates for 802.11a standard with 5MHz channel spacing.
   */
  void Configure80211_5Mhz ();
  void ConfigureHolland (void);
  /**
   * Configure WifiPhy with appropriate channel frequency and
   * supported rates for 802.11n standard.
   */
  void Configure80211n (void);
  /**
   * Configure WifiPhy with appropriate channel frequency and
   * supported rates for 802.11ac standard.
   */
  void Configure80211ac (void);
  /**
   * Configure the device Mcs set with the appropriate HtMcs modes for
   * the number of available transmit spatial streams
   */
  void ConfigureHtDeviceMcsSet (void);
  /**
   * Configure the PHY-level parameters for different Wi-Fi standard.
   * This method is called when defaults for each standard must be 
   * selected.
   *
   * \param standard the Wi-Fi standard
   */
  virtual void ConfigureDefaultsForStandard (enum WifiPhyStandard standard);
  /**
   * Configure the PHY-level parameters for different Wi-Fi standard.
   * This method is called when the Frequency or ChannelNumber attributes
   * are set by the user.  If the Frequency or ChannelNumber are valid for
   * the standard, they are used instead.
   *
   * \param standard the Wi-Fi standard
   */
  virtual void ConfigureChannelForStandard (enum WifiPhyStandard standard);

  /**
   * Look for channel number matching the frequency and width
   * \param frequency The center frequency to use
   * \param width The channel width to use
   * \return the channel number if found, zero if not
   */
  uint16_t FindChannelNumberForFrequencyWidth (uint32_t frequency, uint32_t width) const;
  /**
   * Lookup frequency/width pair for channelNumber/standard pair
   * \param channelNumber The channel number to check
   * \param standard The WifiPhyStandard to check
   * \return the FrequencyWidthPair found
   */
  FrequencyWidthPair GetFrequencyWidthForChannelNumberStandard (uint16_t channelNumber, enum WifiPhyStandard standard) const;

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
   * \todo WifiTxVector and signalNoiseDbm should be be passed as
   * const  references because of their sizes.
   */
  TracedCallback<Ptr<const Packet>, uint16_t, uint16_t, uint32_t,
                 WifiPreamble, WifiTxVector, struct mpduInfo, struct signalNoiseDbm> m_phyMonitorSniffRxTrace;

  /**
   * A trace source that emulates a wifi device in monitor mode
   * sniffing a packet being transmitted.
   *
   * As a reference with the real world, firing this trace
   * corresponds in the madwifi driver to calling the function
   * ieee80211_input_monitor()
   *
   * \see class CallBackTraceSource
   * \todo WifiTxVector should be passed by const reference because
   * of its size.
   */
  TracedCallback<Ptr<const Packet>, uint16_t, uint16_t, uint32_t,
                 WifiPreamble, WifiTxVector, struct mpduInfo> m_phyMonitorSniffTxTrace;
    
  /**
   * This vector holds the set of transmission modes that this
   * WifiPhy(-derived class) can support. In conversation we call this
   * the DeviceRateSet (not a term you'll find in the standard), and
   * it is a superset of standard-defined parameters such as the
   * OperationalRateSet, and the BSSBasicRateSet (which, themselves,
   * have a superset/subset relationship).
   *
   * Mandatory rates relevant to this WifiPhy can be found by
   * iterating over this vector looking for WifiMode objects for which
   * WifiMode::IsMandatory() is true.
   *
   * A quick note is appropriate here (well, here is as good a place
   * as any I can find)...
   *
   * In the standard there is no text that explicitly precludes
   * production of a device that does not support some rates that are
   * mandatory (according to the standard) for PHYs that the device
   * happens to fully or partially support.
   *
   * This approach is taken by some devices which choose to only support,
   * for example, 6 and 9 Mbps ERP-OFDM rates for cost and power
   * consumption reasons (i.e., these devices don't need to be designed
   * for and waste current on the increased linearity requirement of
   * higher-order constellations when 6 and 9 Mbps more than meet their
   * data requirements). The wording of the standard allows such devices
   * to have an OperationalRateSet which includes 6 and 9 Mbps ERP-OFDM
   * rates, despite 12 and 24 Mbps being "mandatory" rates for the
   * ERP-OFDM PHY.
   *
   * Now this doesn't actually have any impact on code, yet. It is,
   * however, something that we need to keep in mind for the
   * future. Basically, the key point is that we can't be making
   * assumptions like "the Operational Rate Set will contain all the
   * mandatory rates".
   */
  WifiModeList m_deviceRateSet;
  WifiModeList m_deviceMcsSet;

  std::vector<uint32_t> m_bssMembershipSelectorSet;

  enum WifiPhyStandard m_standard;     //!< WifiPhyStandard
  bool m_isConstructed;                //!< true when ready to set frequency
  uint32_t m_channelCenterFrequency;   //!< Center frequency in MHz
  uint32_t m_initialFrequency;         //!< Store frequency until initialization
  bool m_frequencyChannelNumberInitialized; //!< Store initialization state
  uint32_t m_channelWidth;             //!< Channel width

  double m_edThresholdW;          //!< Energy detection threshold in watts
  double   m_ccaMode1ThresholdW;  //!< Clear channel assessment (CCA) threshold in watts
  double   m_txGainDb;            //!< Transmission gain (dB)
  double   m_rxGainDb;            //!< Reception gain (dB)
  double   m_txPowerBaseDbm;      //!< Minimum transmission power (dBm)
  double   m_txPowerEndDbm;       //!< Maximum transmission power (dBm)
  uint32_t m_nTxPower;            //!< Number of available transmission power levels
  
  bool     m_ldpc;                  //!< Flag if LDPC is used
  bool     m_stbc;                  //!< Flag if STBC is used
  bool     m_greenfield;            //!< Flag if GreenField format is supported
  bool     m_guardInterval;         //!< Flag if short guard interval is used
  bool     m_shortPreamble;         //!< Flag if short PLCP preamble is supported

  uint32_t m_numberOfTransmitters;     //!< Number of transmitters
  uint32_t m_numberOfReceivers;        //!< Number of receivers

  typedef std::map<ChannelNumberStandardPair,FrequencyWidthPair> ChannelToFrequencyWidthMap;
  static ChannelToFrequencyWidthMap m_channelToFrequencyWidth;

  std::vector<uint32_t> m_supportedChannelWidthSet; //!< Supported channel width
  uint16_t             m_channelNumber;  //!< Operating channel number
  uint16_t             m_initialChannelNumber;  //!< Initial channel number

  Time m_channelSwitchDelay;     //!< Time required to switch between channel
  uint32_t m_totalAmpduSize;     //!< Total size of the previously transmitted MPDUs in an A-MPDU, used for the computation of the number of symbols needed for the last MPDU in the A-MPDU
  double m_totalAmpduNumSymbols; //!< Number of symbols previously transmitted for the MPDUs in an A-MPDU, used for the computation of the number of symbols needed for the last MPDU in the A-MPDU
  
  Ptr<NetDevice>     m_device;   //!< Pointer to the device
  Ptr<MobilityModel> m_mobility; //!< Pointer to the mobility model
};

/**
 * \param os          output stream
 * \param state       wifi state to stringify
 * \return output stream
 */
std::ostream& operator<< (std::ostream& os, enum WifiPhy::State state);

} //namespace ns3

#endif /* WIFI_PHY_H */
