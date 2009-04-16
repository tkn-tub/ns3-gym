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

#ifndef YANS_WIFI_PHY_H
#define YANS_WIFI_PHY_H

#include <stdint.h>
#include "ns3/callback.h"
#include "ns3/event-id.h"
#include "ns3/packet.h"
#include "ns3/object.h"
#include "ns3/traced-callback.h"
#include "ns3/nstime.h"
#include "ns3/ptr.h"
#include "ns3/random-variable.h"
#include "wifi-phy.h"
#include "wifi-mode.h"
#include "wifi-preamble.h"
#include "wifi-phy-standard.h"
#include "interference-helper.h"


namespace ns3 {

class RandomUniform;
class RxEvent;
class YansWifiChannel;
class WifiPhyStateHelper;


/**
 * \brief 802.11 PHY layer model
 *
 * This PHY implements a model of 802.11a. The model
 * implemented here is based on the model described
 * in "Yet Another Network Simulator", 
 * (http://cutebugs.net/files/wns2-yans.pdf).
 *
 *
 * This PHY model depends on a channel loss and delay
 * model as provided by the ns3::PropagationLossModel
 * and ns3::PropagationDelayModel classes, both of which are
 * members of the ns3::YansWifiChannel class.
 */
class YansWifiPhy : public WifiPhy
{
public:

  static TypeId GetTypeId (void);

  YansWifiPhy ();
  virtual ~YansWifiPhy ();

  void SetChannel (Ptr<YansWifiChannel> channel);
  void StartReceivePacket (Ptr<Packet> packet,
                           double rxPowerDbm,
                           WifiMode mode,
                           WifiPreamble preamble);

  void SetStandard (enum WifiPhyStandard standard);
  void SetRxNoiseFigure (double noiseFigureDb);
  void SetTxPowerStart (double start);
  void SetTxPowerEnd (double end);
  void SetNTxPower (uint32_t n);
  void SetTxGain (double gain);
  void SetRxGain (double gain);
  void SetEdThreshold (double threshold);
  void SetCcaMode1Threshold (double threshold);
  void SetErrorRateModel (Ptr<ErrorRateModel> rate);
  void SetDevice (Ptr<Object> device);
  void SetMobility (Ptr<Object> mobility);
  double GetRxNoiseFigure (void) const;
  double GetTxGain (void) const;
  double GetRxGain (void) const;
  double GetEdThreshold (void) const;
  double GetCcaMode1Threshold (void) const;
  Ptr<ErrorRateModel> GetErrorRateModel (void) const;
  Ptr<Object> GetDevice (void) const;
  Ptr<Object> GetMobility (void);


  virtual double GetTxPowerStart (void) const;
  virtual double GetTxPowerEnd (void) const;
  virtual uint32_t GetNTxPower (void) const;
  virtual void SetReceiveOkCallback (WifiPhy::SyncOkCallback callback);
  virtual void SetReceiveErrorCallback (WifiPhy::SyncErrorCallback callback);
  virtual void SendPacket (Ptr<const Packet> packet, WifiMode mode, enum WifiPreamble preamble, uint8_t txPowerLevel);
  virtual void RegisterListener (WifiPhyListener *listener);
  virtual bool IsStateCcaBusy (void);
  virtual bool IsStateIdle (void);
  virtual bool IsStateBusy (void);
  virtual bool IsStateSync (void);
  virtual bool IsStateTx (void);
  virtual Time GetStateDuration (void);
  virtual Time GetDelayUntilIdle (void);
  virtual Time GetLastRxStartTime (void) const;
  virtual Time CalculateTxDuration (uint32_t size, WifiMode payloadMode, enum WifiPreamble preamble) const;
  virtual uint32_t GetNModes (void) const;
  virtual WifiMode GetMode (uint32_t mode) const;
  virtual double CalculateSnr (WifiMode txMode, double ber) const;
  virtual Ptr<WifiChannel> GetChannel (void) const;

private:
  typedef std::vector<WifiMode> Modes;

private:
  YansWifiPhy (const YansWifiPhy &o);
  virtual void DoDispose (void);
  void Configure80211a (void);
  void ConfigureHolland (void);
  double GetEdThresholdW (void) const;
  double DbmToW (double dbm) const;
  double DbToRatio (double db) const;
  double WToDbm (double w) const;
  double RatioToDb (double ratio) const;
  double GetPowerDbm (uint8_t power) const;
  void EndSync (Ptr<Packet> packet, Ptr<InterferenceHelper::Event> event);

private:
  double   m_edThresholdW;
  double   m_ccaMode1ThresholdW;
  double   m_txGainDb;
  double   m_rxGainDb;
  double   m_txPowerBaseDbm;
  double   m_txPowerEndDbm;
  uint32_t m_nTxPower;

  Ptr<YansWifiChannel> m_channel;
  Ptr<Object> m_device;
  Ptr<Object> m_mobility;
  Modes m_modes;
  EventId m_endSyncEvent;
  UniformVariable m_random;
  WifiPhyStandard m_standard;
  Ptr<WifiPhyStateHelper> m_state;
  InterferenceHelper m_interference;

};

} // namespace ns3


#endif /* YANS_WIFI_PHY_H */
