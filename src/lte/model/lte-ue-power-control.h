/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014 Piotr Gawlowicz
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
 * Author: Piotr Gawlowicz <gawlowicz.p@gmail.com>
 *
 */

#ifndef LTE_UE_POWER_CONTROL_H
#define LTE_UE_POWER_CONTROL_H

#include <ns3/ptr.h>
#include <ns3/traced-callback.h>
#include <ns3/object.h>
#include <vector>


namespace ns3 {

/**
 * \brief This class realizes Uplink Power Control functionality
 *
 * When LteUePhy is about sending PUSCH/PUCCH/SRS it should ask
 * LteUePowerControl for current channel TX power level and then
 * use it while creating SpectrumValue for Uplink Transmission
 *
 * LteUePowerControl computes TX power level for PUSCH and SRS.
 * PUCCH is realized in ideal way and PUSCH do not use any resources,
 * so there is no need to compute power for that channel
 *
 * LteUePowerControlcomputes TX power based on some preconfigured
 * parameters and current Path-loss. Path-loss is computed as difference
 * between current RSRP and referenceSignalPower level. Current RSRP
 * is passed to LteUePowerControl by LteUePhy. referenceSignalPower is
 * configurable by attribute system
 *
 * Moreover, LteUePhy pass all received TPC values to LteUePowerControl,
 * what is a part of Closed Loop Power Control functionality
 */

class LteUePowerControl : public Object
{
public:

  LteUePowerControl ();
  virtual ~LteUePowerControl ();

  // inherited from Object
  static TypeId GetTypeId (void);
  virtual void DoInitialize (void);
  virtual void DoDispose (void);

  void SetPcmax (double value);
  double GetPcmax ();

  void SetTxPower (double value);
  void ConfigureReferenceSignalPower (int8_t referenceSignalPower);

  void SetCellId (uint16_t cellId);
  void SetRnti (uint16_t rnti);

  void SetPoNominalPusch (int16_t value);
  void SetPoUePusch (int16_t value);
  void SetAlpha (double value);

  void SetRsrp (double value);
  void ReportTpc (uint8_t tpc);

  void CalculatePuschTxPower ();
  void CalculatePucchTxPower ();
  void CalculateSrsTxPower ();

  double GetPuschTxPower (std::vector <int> rb);
  double GetPucchTxPower (std::vector <int> rb);
  double GetSrsTxPower (std::vector <int> rb);

  /**
   * TracedCallback signature for uplink transmit power.
   *
   * \param [in] cellId Cell identifier.
   * \param [in] rnti The C-RNTI identifying the UE.
   * \param [in] power The current TX power.
   */
  typedef void (* TxPowerTracedCallback)
    (const uint16_t cellId, const uint16_t rnti, const double power);

private:
  void SetSubChannelMask (std::vector <int> mask);

  double m_txPower;
  double m_Pcmax;
  double m_Pcmin;

  double m_curPuschTxPower;
  double m_curPucchTxPower;
  double m_curSrsTxPower;

  double m_referenceSignalPower;
  bool m_rsrpSet;
  double m_rsrp;

  std::vector<int16_t> m_PoNominalPusch;
  std::vector<int16_t> m_PoUePusch;

  int16_t m_PsrsOffset;

  uint16_t m_M_Pusch;
  std::vector<double> m_alpha;
  double m_pathLoss;
  double m_deltaTF;

  std::vector<int8_t> m_deltaPusch;
  double m_fc;

  uint16_t m_srsBandwidth;

  bool m_closedLoop;
  bool m_accumulationEnabled;

  uint16_t m_cellId;
  uint16_t m_rnti;
  /**
   * Trace information regarding Uplink TxPower
   * uint16_t cellId, uint16_t rnti, double txPower
   */
  TracedCallback<uint16_t, uint16_t, double> m_reportPuschTxPower;
  TracedCallback<uint16_t, uint16_t, double> m_reportPucchTxPower;
  TracedCallback<uint16_t, uint16_t, double> m_reportSrsTxPower;

};


}

#endif /* LTE_UE_POWER_CONTROL_H */
