/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
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
 * Author: Leonard Tracy <lentracy@gmail.com>
 *         Andrea Sacco <andrea.sacco85@gmail.com>
 */


#ifndef UAN_PHY_H
#define UAN_PHY_H

#include "ns3/object.h"
#include "ns3/uan-mac.h"
#include "ns3/uan-tx-mode.h"
#include "ns3/uan-prop-model.h"
#include "ns3/uan-transducer.h"
#include "ns3/device-energy-model.h"

namespace ns3 {


/**
 * \class UanPhyCalcSinr
 *
 * Class used for calculating SINR of packet in UanPhy.
 * Can be set to any derived class using attributes of UanPhy
 * to implement different models.
 */
class UanPhyCalcSinr : public Object
{

public:
  static TypeId GetTypeId (void);

  /**
   * \param pkt Packet to calculate SINR for
   * \param arrTime Arrival time of pkt
   * \param rxPowerDb The received signal strength of the packet in dB re 1 uPa
   * \param ambNoiseDb Ambient channel noise in dB re 1 uPa
   * \param mode TX Mode of pkt
   * \param pdp  Power delay profile of pkt
   * \param arrivalList  List of interfering arrivals given from Transducer.
   */
  virtual double CalcSinrDb (Ptr<Packet> pkt,
                             Time arrTime,
                             double rxPowerDb,
                             double ambNoiseDb,
                             UanTxMode mode,
                             UanPdp pdp,
                             const UanTransducer::ArrivalList &arrivalList
                             ) const = 0;
  /**
   * Clears all pointer references
   */
  virtual void Clear (void);

  virtual void DoDispose (void);

  /**
   * \param db dB value
   * \returns kilopascals
   * \brief Converts dB re 1 uPa to kilopascals
   */
  inline double DbToKp (double db) const
  {
    return std::pow (10, db / 10.0);
  }
  /**
   * \param kp value in kilopascals
   * \returns dB re 1 uPa
   * \brief Converts kilopascals to dB re 1 uPa
   */
  inline double KpToDb (double kp) const
  {
    return 10 * std::log10 (kp);
  }
};

/**
 * \class UanPhyPer
 * Used in UanPhy for determining probability of packet error based on received SINR and
 * modulation (mode).  Can be set in UanPhy via attributes
 */
class UanPhyPer : public Object
{
public:
  static TypeId GetTypeId (void);

  /**
   * Calculates the prob. of packet error based on
   * SINR at the receiver and a tx mode.
   * \param pkt Packet which is under consideration
   * \param sinrDb SINR at receiver
   * \param mode TX mode used to transmit packet
   * \returns Probability of packet error
   */
  virtual double CalcPer (Ptr<Packet> pkt, double sinrDb, UanTxMode mode) = 0;
  /**
   * Clears all pointer references
   */
  virtual void Clear (void);

  virtual void DoDispose (void);
};


/**
 * \class UanPhyListener
 *
 * \brief Interface for PHY event listener
 * A class which implements this interface may register with Phy object
 * to receive notification of TX/RX/CCA events
 */
class UanPhyListener
{
public:
  virtual ~UanPhyListener ()
  {
  }
  /**
   * \brief Function called when Phy object begins receiving packet
   */
  virtual void NotifyRxStart (void) = 0;
  /**
   * \brief Function called when Phy object finishes receiving packet without error
   */
  virtual void NotifyRxEndOk (void) = 0;
  /**
   * \brief Function called when Phy object finishes receiving packet in error
   */
  virtual void NotifyRxEndError (void) = 0;
  /**
   * \brief Function called when Phy object begins sensing channel is busy
   */
  virtual void NotifyCcaStart (void) = 0;
  /**
   * \brief Function called when Phy object stops sensing channel is busy
   */
  virtual void NotifyCcaEnd (void) = 0;
  /**
   * \param duration Duration of transmission
   * \brief Function called when transmission starts from Phy object
   */
  virtual void NotifyTxStart (Time duration) = 0;
};

/**
 * \class UanPhy
 *
 * \brief Base class for UAN Phy models
 */
class UanPhy : public Object
{
public:
  static TypeId GetTypeId (void);

  /// Enum defining possible Phy states
  enum State
  {
    IDLE, CCABUSY, RX, TX, SLEEP
  };

  /**
   * arg1: packet received successfully
   * arg2: snr of packet
   * arg3: mode of packet
   */
  typedef Callback<void, Ptr<Packet>, double, UanTxMode> RxOkCallback;

  /**
   * arg1: packet received successfully
   * arg2: snr of packet
   */
  typedef Callback<void, Ptr<Packet>, double > RxErrCallback;

  /**
   * \param callback DeviceEnergyModel change state callback.
   *
   * This function sets the DeviceEnergyModel callback for UanPhy device. Must
   * be implemented by UanPhy child classes.
   */
  virtual void SetEnergyModelCallback (DeviceEnergyModel::ChangeStateCallback callback) = 0;
  /**
   * This function handles the energy depletion event. Must
   * be implemented by UanPhy child classes.
   */
  virtual void EnergyDepletionHandler (void) = 0;
  /**
   * \param pkt  Packet to transmit
   * \param modeNum  Index of mode in SupportedModes list to use for transmission
   */
  virtual void SendPacket (Ptr<Packet> pkt, uint32_t modeNum) = 0;

  /**
   * \param listener New listener to register
   *
   * Register an object to be notified of common Phy events
   */
  virtual void RegisterListener (UanPhyListener *listener) = 0;

  /**
   * \brief Packet arriving from channel:  i.e.  leading bit of packet has arrived.
   * \param pkt Packet which is arriving
   * \param rxPowerDb Signal power of incoming packet in dB
   * \param txMode Transmission mode defining modulation of incoming packet
   * \param pdp Power delay profile of incoming packet
   */
  virtual void StartRxPacket (Ptr<Packet> pkt, double rxPowerDb, UanTxMode txMode, UanPdp pdp) = 0;

  /**
   * \param cb Callback to be enabled when packet is received without error
   */
  virtual void SetReceiveOkCallback (RxOkCallback cb) = 0;

  /**
   * \param cb Callback to be enabled when a packet is received with errors
   */
  virtual void SetReceiveErrorCallback (RxErrCallback cb) = 0;

  /**
   * \param gain Gain added at receiver
   */

  virtual void SetRxGainDb (double gain) = 0;

  /**
   * \param txpwr Final output transmission power in dB
   */
  virtual void SetTxPowerDb (double txpwr) = 0;

  /**
   * \param thresh Threshold SINR for propper reception in dB
   */
  virtual void SetRxThresholdDb (double thresh) = 0;

  /**
   * \param thresh Signal power at receiver required for CCA busy state
   *
   */
  virtual void SetCcaThresholdDb (double thresh) = 0;

  /**
   * \returns Gain added to signal at receiver in dB
   */
  virtual double GetRxGainDb (void) = 0;

  /**
   * \returns Current TX power output
   */
  virtual double GetTxPowerDb (void) = 0;

  /**
   * \returns Required signal strength, in dB, to begin receiving packet
   */
  virtual double GetRxThresholdDb (void) = 0;

  /**
   * \returns Threshold signal strength in dB to enter CCA busy mode
   */
  virtual double GetCcaThresholdDb (void) = 0;
  /**
   *
   * \returns True if Phy is SLEEP
   */
  virtual bool IsStateSleep (void) = 0;
  /**
   *
   * \returns True if Phy is IDLE
   */
  virtual bool IsStateIdle (void) = 0;
  /**
   *
   * \returns True if Phy is not IDLE
   */
  virtual bool IsStateBusy (void) = 0;
  /**
   *
   * \returns True if Phy is currently in receive mode
   */
  virtual bool IsStateRx (void) = 0;
  /**
   *
   * \returns True if Phy is busy transmitting
   */
  virtual bool IsStateTx (void) = 0;
  /**
   *
   * \returns True if Phy is not TX or RX but the channel is sensed busy
   */
  virtual bool IsStateCcaBusy (void) = 0;

  /**
   * \returns Channel this phy is attached to
   */
  virtual Ptr<UanChannel> GetChannel (void) const = 0;

  /**
   * \returns the Net Device that this Phy is a part of
   */
  virtual Ptr<UanNetDevice> GetDevice (void) = 0;

  /**
   * \param channel The channel this Phy is attached to
   */
  virtual void SetChannel (Ptr<UanChannel> channel) = 0;

  /**
   * \param device The Net Device this Phy is a part of
   */
  virtual void SetDevice (Ptr<UanNetDevice> device) = 0;

  /**
   * \param mac The MAC forwarding messages to this Phy
   */
  virtual void SetMac (Ptr<UanMac> mac) = 0;

  /**
   * \param packet Packet that is beginning transmission
   * \param txPowerDb Transmit power of packet
   * \param txMode  Transmission mode of packet
   *
   * Notification that there is a transmission beginning
   * on the transducer that this Phy is attached to.
   */

  virtual void NotifyTransStartTx (Ptr<Packet> packet, double txPowerDb, UanTxMode txMode) = 0;

  /**
   *
   * Function called when there has been a change in the
   * ammount of interference this node is experiencing
   * from other transmissions
   */
  virtual void NotifyIntChange (void) = 0;

  /**
   *
   * \param trans Transducer this Phy outputs to / receives from
   */
  virtual void SetTransducer (Ptr<UanTransducer> trans) = 0;

  /**
   * \returns Transducer this Phy outputs to / receives from
   */
  virtual Ptr<UanTransducer> GetTransducer (void) = 0;

  /**
   * \returns Number of TX modes supported by this PHY
   */
  virtual uint32_t GetNModes (void) = 0;

  /**
   * \param n Mode number of mode to return (place in Modeslist)
   * \returns Mode n
   */
  virtual UanTxMode GetMode (uint32_t n) = 0;

  /**
   * \warning Returns non-valid pointer if IsStateRx == false
   * \returns Packet currently being received in PHY
   */
  virtual Ptr<Packet> GetPacketRx (void) const = 0;

  /**
   * Clears all pointer references
   */
  virtual void Clear (void) = 0;

  virtual void SetSleepMode (bool sleep) = 0;


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
  * Assign a fixed random variable stream number to the random variables
  * used by this model.  Return the number of streams (possibly zero) that
  * have been assigned.
  *
  * \param stream first stream index to use
  * \return the number of stream indices assigned by this model
  */
  virtual int64_t AssignStreams (int64_t stream) = 0;

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
};

}

#endif /* UAN_PHY_H */
