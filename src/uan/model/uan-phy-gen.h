/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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

#ifndef UANPHYGEN_H
#define UANPHYGEN_H


#include "uan-phy.h"
#include "ns3/traced-callback.h"
#include "ns3/nstime.h"
#include "ns3/device-energy-model.h"
#include <list>

namespace ns3 {

/**
 * \class UanPhyPerGenDefault
 * \brief Default Packet Error Rate calculator for UanPhyGen
 * Considers no error if SINR is > user defined threshold
 * (configured by an attribute).
 */
class UanPhyPerGenDefault : public UanPhyPer
{
public:
  UanPhyPerGenDefault ();
  virtual ~UanPhyPerGenDefault ();

  static TypeId GetTypeId (void);
  virtual double CalcPer (Ptr<Packet> pkt, double sinrDb, UanTxMode mode);
private:
  double m_thresh;

};

/**
 * \class UanPhyPerUmodem
 * \brief Packet error rate calculation assuming WHOI Micromodem like PHY
 * Calculates PER assuming rate 1/2 convolutional code with constraint length 9
 * with soft decision viterbi decoding and a CRC capable of correcting 1 bit error
 */
class UanPhyPerUmodem : public UanPhyPer
{
public:
  UanPhyPerUmodem ();
  virtual ~UanPhyPerUmodem ();

  static TypeId GetTypeId (void);

  /**
   *
   * This implementation uses calculations
   * for binary FSK modulation coded by a rate 1/2 convolutional code
   * with constraint length = 9 and a viterbi decoder and finally a CRC capable
   * of correcting one bit error.  These equations can be found in
   * the book, Digital Communications, by Proakis (Any version I think)
   *
   * \param pkt Packet which is under consideration
   * \param sinrDb SINR at receiver
   * \param mode TX mode used to transmit packet
   * \returns Probability of packet error
   */
  virtual double CalcPer (Ptr<Packet> pkt, double sinrDb, UanTxMode mode);
private:
  double NChooseK (uint32_t n, uint32_t k);

};
/**
 * \class UanPhyCalcSinrDefault
 * \brief Default SINR calculator for UanPhyGen
 * The default ignores mode data and assumes that all rxpower transmitted is
 * captured by the receiver, and that all signal power associated with
 * interfering packets affects SINR identically to additional ambient noise.
 */
class UanPhyCalcSinrDefault : public UanPhyCalcSinr
{

public:
  UanPhyCalcSinrDefault ();
  virtual ~UanPhyCalcSinrDefault ();
  static TypeId GetTypeId (void);
  /**
   * This implementation simply adds all arriving signal power
   * and assumes it acts identically to additional noise.
   *
   * \param pkt Packet which is under consideration
   * \param arrTime Arrival time of packet pkt
   * \param rxPowerDb Received signal power at receiver
   * \param ambNoiseDb Ambient channel noise in dB re 1 uPa
   * \param mode TX mode used to transmit packet
   * \param pdp Power delay profile of arriving packet pkt
   * \param arrivalList List of other, simultaneously arriving packets
   * \returns Probability of packet error
   */
  virtual double CalcSinrDb (Ptr<Packet> pkt,
                             Time arrTime,
                             double rxPowerDb,
                             double ambNoiseDb,
                             UanTxMode mode,
                             UanPdp pdp,
                             const UanTransducer::ArrivalList &arrivalList
                             ) const;
};

/**
 * \class UanPhyCalcSinrFhFsk
 * \brief WHOI Micromodem like FH-FSK model
 *
 * Model of interference calculation for FH-FSK wherein all nodes
 * use an identical hopping pattern.  In this case, there is an (M-1)*SymbolTime
 * clearing time between symbols transmitted on the same frequency.
 * This clearing time combats ISI from channel delay spread and also has
 * a byproduct of possibly reducing interference from other transmitted packets.
 */
class UanPhyCalcSinrFhFsk : public UanPhyCalcSinr
{

public:
  UanPhyCalcSinrFhFsk ();
  virtual ~UanPhyCalcSinrFhFsk ();
  static TypeId GetTypeId (void);
  virtual double CalcSinrDb (Ptr<Packet> pkt,
                             Time arrTime,
                             double rxPowerDb,
                             double ambNoiseDb,
                             UanTxMode mode,
                             UanPdp pdp,
                             const UanTransducer::ArrivalList &arrivalList
                             ) const;
private:
  uint32_t m_hops;
};

/**
 * \class UanPhyGen
 * \brief Generic PHY model
 *
 * This is a generic PHY class.  SINR and PER information
 * are controlled via attributes.  By adapting the SINR
 * and PER models to a specific situation, this PHY should
 * be able to model a wide variety of networks.
 */
class UanPhyGen : public UanPhy
{
public:
  UanPhyGen ();
  virtual ~UanPhyGen ();
  /**
   * \returns Default TX modes of UanPhyGen
   */
  static UanModesList GetDefaultModes (void);

  static TypeId GetTypeId (void);
  virtual void SetEnergyModelCallback (DeviceEnergyModel::ChangeStateCallback cb);
  virtual void EnergyDepletionHandler (void);
  virtual void SendPacket (Ptr<Packet> pkt, uint32_t modeNum);
  virtual void RegisterListener (UanPhyListener *listener);
  virtual void StartRxPacket (Ptr<Packet> pkt, double rxPowerDb, UanTxMode txMode, UanPdp pdp);
  virtual void SetReceiveOkCallback (RxOkCallback cb);
  virtual void SetReceiveErrorCallback (RxErrCallback cb);
  virtual bool IsStateSleep (void);
  virtual bool IsStateIdle (void);
  virtual bool IsStateBusy (void);
  virtual bool IsStateRx (void);
  virtual bool IsStateTx (void);
  virtual bool IsStateCcaBusy (void);
  virtual void SetRxGainDb (double gain);
  virtual void SetTxPowerDb (double txpwr);
  virtual void SetRxThresholdDb (double thresh);
  virtual void SetCcaThresholdDb (double thresh);
  virtual double GetRxGainDb (void);
  virtual double GetTxPowerDb (void);
  virtual double GetRxThresholdDb (void);
  virtual double GetCcaThresholdDb (void);
  virtual Ptr<UanChannel> GetChannel (void) const;
  virtual Ptr<UanNetDevice> GetDevice (void);
  virtual Ptr<UanTransducer> GetTransducer (void);
  virtual void SetChannel (Ptr<UanChannel> channel);
  virtual void SetDevice (Ptr<UanNetDevice> device);
  virtual void SetMac (Ptr<UanMac> mac);
  virtual void SetTransducer (Ptr<UanTransducer> trans);
  virtual void NotifyTransStartTx (Ptr<Packet> packet, double txPowerDb, UanTxMode txMode);
  virtual void NotifyIntChange (void);
  virtual uint32_t GetNModes (void);
  virtual UanTxMode GetMode (uint32_t n);
  virtual Ptr<Packet> GetPacketRx (void) const;
  virtual void Clear (void);

private:
  typedef std::list<UanPhyListener *> ListenerList;

  UanModesList m_modes;

  State m_state;
  ListenerList m_listeners;
  RxOkCallback m_recOkCb;
  RxErrCallback m_recErrCb;
  Ptr<UanChannel> m_channel;
  Ptr<UanTransducer> m_transducer;
  Ptr<UanNetDevice> m_device;
  Ptr<UanMac> m_mac;
  Ptr<UanPhyPer> m_per;
  Ptr<UanPhyCalcSinr> m_sinr;

  double m_rxGainDb;
  double m_txPwrDb;
  double m_rxThreshDb;
  double m_ccaThreshDb;

  Ptr<Packet> m_pktRx;
  double m_minRxSinrDb;
  double m_rxRecvPwrDb;
  Time m_pktRxArrTime;
  UanPdp m_pktRxPdp;
  UanTxMode m_pktRxMode;

  bool m_cleared;
  bool m_disabled;

  DeviceEnergyModel::ChangeStateCallback m_energyCallback;
  TracedCallback<Ptr<const Packet>, double, UanTxMode > m_rxOkLogger;
  TracedCallback<Ptr<const Packet>, double, UanTxMode > m_rxErrLogger;
  TracedCallback<Ptr<const Packet>, double, UanTxMode > m_txLogger;


  double CalculateSinrDb (Ptr<Packet> pkt, Time arrTime, double rxPowerDb, UanTxMode mode, UanPdp pdp);
  double GetInterferenceDb (Ptr<Packet> pkt);
  double DbToKp (double db);
  double KpToDb (double kp);
  void RxEndEvent (Ptr<Packet> pkt, double rxPowerDb, UanTxMode txMode);
  void TxEndEvent ();
  void UpdatePowerConsumption (const State state);

  void NotifyListenersRxStart (void);
  void NotifyListenersRxGood (void);
  void NotifyListenersRxBad (void);
  void NotifyListenersCcaStart (void);
  void NotifyListenersCcaEnd (void);
  void NotifyListenersTxStart (Time duration);
protected:
  virtual void DoDispose ();

};

} // namespace ns3

#endif // UANPHYGEN_H
