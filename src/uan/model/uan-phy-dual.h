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

#ifndef UANPHYDUAL_H
#define UANPHYDUAL_H

#include "ns3/uan-phy.h"



namespace ns3 {

class UanTxMode;
class UanModesList;


/**
 * \class UanPhyCalcSinrDual
 * \brief Default SINR model for UanPhyDual
 *
 * Considers interfering packet power as additional ambient noise only
 * if there is overlap in frequency band as found from supplied UanTxMode.
 * If there is no overlap, then the packets are considered not to interfere.
 */
class UanPhyCalcSinrDual : public UanPhyCalcSinr
{
public:
  UanPhyCalcSinrDual ();
  virtual ~UanPhyCalcSinrDual ();
  static TypeId GetTypeId (void);
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
 * \class UanPhyDual
 *
 * A class that wraps two generic UAN Phy layers (UanPhyGen) into a single PHY.
 * This is used to simulate two receivers (and transmitters) that use
 * the same front end hardware.  When attached to a UanTransducerHd,
 * this results in a net device able to transmit on one or two channels
 * simultaneously or receive on one or two channels simultaneously but
 * that cannot transmit and receive simultaneously.
 *
 * Many of the standard PHY functions here become ambiguous.  In most cases
 * information for "Phy1" are returned.
 */
class UanPhyDual : public UanPhy
{
public:
  UanPhyDual ();
  virtual ~UanPhyDual ();

  static TypeId GetTypeId ();

  virtual void SetEnergyModelCallback (DeviceEnergyModel::ChangeStateCallback callback);
  virtual void EnergyDepletionHandler (void);
  virtual void SendPacket (Ptr<Packet> pkt, uint32_t modeNum);

  /**
   * \brief Register a class to receive phy state change notifications
   * \param listener Class derived from UanPhyListener to receive notifications
   *
   * Note that, from UanPhyDual, you may receive duplicate
   * messages as underneath there are two generic phys here.
   * Each will notify of state changes independently.
   */
  virtual void RegisterListener (UanPhyListener *listener);
  virtual void StartRxPacket (Ptr<Packet> pkt, double rxPowerDb, UanTxMode txMode, UanPdp pdp);
  virtual void SetReceiveOkCallback (RxOkCallback cb);
  virtual void SetReceiveErrorCallback (RxErrCallback cb);
  virtual void SetRxGainDb (double gain);
  virtual void SetTxPowerDb (double txpwr);
  virtual void SetRxThresholdDb (double thresh);
  virtual void SetCcaThresholdDb (double thresh);
  virtual double GetRxGainDb (void);
  virtual double GetTxPowerDb (void);
  virtual double GetRxThresholdDb (void);
  virtual double GetCcaThresholdDb (void);
  virtual bool IsStateSleep (void);
  virtual bool IsStateIdle (void);
  virtual bool IsStateBusy (void);
  virtual bool IsStateRx (void);
  virtual bool IsStateTx (void);
  virtual bool IsStateCcaBusy (void);
  virtual Ptr<UanChannel> GetChannel (void) const;
  virtual Ptr<UanNetDevice> GetDevice (void);
  virtual void SetChannel (Ptr<UanChannel> channel);
  virtual void SetDevice (Ptr<UanNetDevice> device);
  virtual void SetMac (Ptr<UanMac> mac);
  virtual void NotifyTransStartTx (Ptr<Packet> packet, double txPowerDb, UanTxMode txMode);
  virtual void NotifyIntChange (void);
  virtual void SetTransducer (Ptr<UanTransducer> trans);
  virtual Ptr<UanTransducer> GetTransducer (void);
  virtual uint32_t GetNModes (void);
  virtual UanTxMode GetMode (uint32_t n);
  virtual void Clear (void);

  /**
   * /returns True if Phy1 is Idle
   */
  bool IsPhy1Idle (void);
  /**
   * /returns True if Phy2 is Idle
   */
  bool IsPhy2Idle (void);
  /**
   * /returns True if Phy1 is currently in RX mode
   */
  bool IsPhy1Rx (void);
  /**
   * /returns True if Phy2 is currently in RX mode
   */
  bool IsPhy2Rx (void);
  /**
   * /returns True if Phy1 is in TX mode
   */
  bool IsPhy1Tx (void);
  /**
   * /returns True if Phy2 is in TX mode
   */
  bool IsPhy2Tx (void);

  // Attribute getters and setters
  /**
   * \returns Clear channel assessment threshold of Phy1
   */
  double GetCcaThresholdPhy1 (void) const;
  /**
   * \returns Clear channel assessment threshold of Phy2
   */
  double GetCcaThresholdPhy2 (void) const;
  /**
   * \param thresh Signal power threshold in dB to set Phy1 to
   */
  void SetCcaThresholdPhy1 (double thresh);
  /**
   * \param thresh Signal power threshold in dB to set Phy2 to
   */
  void SetCcaThresholdPhy2 (double thresh);

  /**
   * \returns Current TX power setting of Phy 1 in dB
   */
  double GetTxPowerDbPhy1 (void) const;
  /**
   * \returns Current TX power setting of Phy 2 in dB
   */
  double GetTxPowerDbPhy2 (void) const;
  /**
   * \returns TX power setting of Phy1 in dB
   */
  void SetTxPowerDbPhy1 (double);
  /**
   * \returns TX power setting of Phy2 in dB
   */
  void SetTxPowerDbPhy2 (double);

  /**
   * \returns RX gain of Phy1 in dB
   */
  double GetRxGainDbPhy1 (void) const;
  /**
   * \returns RX gain of Phy2 in dB
   */
  double GetRxGainDbPhy2 (void) const;
  /**
   * \param gain value in dB to apply to RX gain of Phy1
   */
  void SetRxGainDbPhy1 (double gain);
  /**
   * \param gain value in dB to apply to RX gain of Phy2
   */
  void SetRxGainDbPhy2 (double gain);

  /**
   * \returns List of available modes on Phy1
   */
  UanModesList GetModesPhy1 (void) const;
  /**
   * \returns List of available modes on Phy2
   */
  UanModesList GetModesPhy2 (void) const;
  /**
   * \param modes List of modes to use on Phy1 (index corresponds to mode #)
   */
  void SetModesPhy1 (UanModesList modes);
  /**
   * \param modes List of modes to use on Phy2 (index corresponds to mode #)
   */
  void SetModesPhy2 (UanModesList modes);

  /**
   * \returns Ptr to PER model for Phy1
   */
  Ptr<UanPhyPer> GetPerModelPhy1 (void) const;
  /**
   * \returns Ptr to PER model for Phy2
   */
  Ptr<UanPhyPer> GetPerModelPhy2 (void) const;
  /**
   * \param per Ptr to PER model to use on Phy1
   */
  void SetPerModelPhy1 (Ptr<UanPhyPer> per);
  /**
   * \param per Ptr to PER model to use on Phy2
   */
  void SetPerModelPhy2 (Ptr<UanPhyPer> per);

  /**
   * \returns Ptr to SINR model for Phy1
   */
  Ptr<UanPhyCalcSinr> GetSinrModelPhy1 (void) const;
  /**
   * \returns Ptr to SINR model for Phy2
   */
  Ptr<UanPhyCalcSinr> GetSinrModelPhy2 (void) const;
  /**
   * \param calcSinr Ptr to SINR model to use on Phy1
   */
  void SetSinrModelPhy1 (Ptr<UanPhyCalcSinr> calcSinr);
  /**
   * \param calcSinr Ptr to SINR model to use on Phy2
   */
  void SetSinrModelPhy2 (Ptr<UanPhyCalcSinr> calcSinr);

  /**
   * \returns Packet currently being received on Phy1 (Null Ptr if none)
   */
  Ptr<Packet> GetPhy1PacketRx (void) const;
  /**
   * \returns Packet currently being received on Phy2 (Null Ptr if none)
   */
  Ptr<Packet> GetPhy2PacketRx (void) const;
  /**
   * \returns Packet currenty being received on Phy1 (Null Ptr if none)
   */
  Ptr<Packet> GetPacketRx (void) const;

private:
  Ptr<UanPhy> m_phy1;
  Ptr<UanPhy> m_phy2;
  TracedCallback<Ptr<const Packet>, double, UanTxMode > m_rxOkLogger;
  TracedCallback<Ptr<const Packet>, double, UanTxMode > m_rxErrLogger;
  TracedCallback<Ptr<const Packet>, double, UanTxMode > m_txLogger;
  RxOkCallback m_recOkCb;
  RxErrCallback m_recErrCb;


  void RxOkFromSubPhy (Ptr<Packet> pkt, double sinr, UanTxMode mode);
  void RxErrFromSubPhy (Ptr<Packet> pkt, double sinr);
protected:
  virtual void DoDispose ();
};

}

#endif // UANPHYDUAL_H
