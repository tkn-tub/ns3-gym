/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Andrea Sacco
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
 * Author: Andrea Sacco <andrea.sacco85@gmail.com>
 */

#ifndef ACOUSTIC_MODEM_ENERGY_MODEL_H
#define ACOUSTIC_MODEM_ENERGY_MODEL_H

#include "ns3/device-energy-model.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include "ns3/traced-value.h"

namespace ns3 {

/**
 * \brief WHOI micro-modem energy model
 *
 * Basing on the Device Energy Model interface, has been implemented a specific
 * energy model for the WHOI micro modem. The class follows pretty closely the
 * RadioEnergyModel class as the transducer behaviour is pretty close to the one
 * of a wifi radio, with identical states (rx, tx, idle, sleep).
 *
 * The power consumption values implemented into the model are as follows [1]:
 *
 * Modem State   Power Consumption
 * TX            50 W
 * RX            158 mW
 * Idle          158 mW
 * Sleep         5.8 mW
 *
 * References:
 * [1] Freitag et al., The whoi micro-modem: an acoustic communications and navigation system for multiple platforms,
 *     in In Proc. IEEE OCEANS05 Conf, 2005.
 *     URL: http://ieeexplore.ieee.org/iel5/10918/34367/01639901.pdf
 */
class AcousticModemEnergyModel : public DeviceEnergyModel
{
public:
  /**
   * Callback type for energy depletion handling.
   */
  typedef Callback<void> AcousticModemEnergyDepletionCallback;

public:
  static TypeId GetTypeId (void);
  AcousticModemEnergyModel ();
  virtual ~AcousticModemEnergyModel ();

  /**
   * \brief Sets pointer to node.
   *
   * \param node Pointer to node.
   *
   * Implements DeviceEnergyModel::SetNode.
   */
  virtual void SetNode (Ptr<Node> node);

  /**
   * \brief Gets pointer to node.
   *
   * \return Pointer to node.
   *
   * Implements DeviceEnergyModel::GetNode.
   */
  virtual Ptr<Node> GetNode (void) const;

  /**
   * \brief Sets pointer to EnergySouce installed on node.
   *
   * \param source Pointer to EnergySource installed on node.
   *
   * Implements DeviceEnergyModel::SetEnergySource.
   */
  virtual void SetEnergySource (Ptr<EnergySource> source);

  /**
   * \return Total energy consumption of the modem
   *
   * Implements DeviceEnergyModel::GetTotalEnergyConsumption.
   */
  virtual double GetTotalEnergyConsumption (void) const;

  /**
   *
   * \return The transmission power of the modem in Watts
   */
  double GetTxPowerW (void) const;

  /**
   * Set the transmission power of the modem
   *
   * \param txPowerW Transmission power of the modem in watts
   */
  void SetTxPowerW (double txPowerW);

  /**
   *
   * \return The receiving power of the modem in Watts
   */
  double GetRxPowerW (void) const;

  /**
   * Set the receiving power of the modem
   *
   * \param rxPowerW Receiving power of the modem in watts
   */
  void SetRxPowerW (double rxPowerW);

  /**
   *
   * \return The idle power of the modem in Watts
   */
  double GetIdlePowerW (void) const;

  /**
   * Set the idle power of the modem
   *
   * \param idlePowerW Idle power of the modem in watts
   */
  void SetIdlePowerW (double idlePowerW);

  /**
   *
   * \return The sleep power of the modem in Watts
   */
  double GetSleepPowerW (void) const;

  /**
   * Set the sleep power of the modem
   *
   * \param sleepPowerW Sleep power of the modem in watts
   */
  void SetSleepPowerW (double sleepPowerW);

  /**
   * \return Current state.
   */
  int GetCurrentState (void) const;

  /**
   * \param callback Callback function.
   *
   * Sets callback for energy depletion handling.
   */
  void SetEnergyDepletionCallback (AcousticModemEnergyDepletionCallback callback);

  /**
   * \brief Changes state of the AcousticModemEnergyModel.
   *
   * \param newState New state the modem is in.
   *
   * Implements DeviceEnergyModel::ChangeState.
   */
  virtual void ChangeState (int newState);

  /**
   * \brief Handles energy depletion.
   *
   * Implements DeviceEnergyModel::HandleEnergyDepletion
   */
  virtual void HandleEnergyDepletion (void);


private:
  void DoDispose (void);

  /**
   * \returns Current draw of device, at current state.
   *
   * Implements DeviceEnergyModel::GetCurrentA.
   */
  virtual double DoGetCurrentA (void) const;

  /**
   * \param destState Modem state to switch to.
   * \return True if the transition is allowed.
   *
   * This function checks if a given modem state transition is allowed.
   */
  bool IsStateTransitionValid (const int destState);

  /**
   * \param currentState New state the modem is currently in.
   *
   * Sets current state. This function is private so that only the energy model
   * can change its own state.
   */
  void SetMicroModemState (const int state);

private:
  Ptr<Node> m_node;
  Ptr<EnergySource> m_source;

  // Member variables for power consumption in different modem states.
  double m_txPowerW;
  double m_rxPowerW;
  double m_idlePowerW;
  double m_sleepPowerW;

  // This variable keeps track of the total energy consumed by this model.
  TracedValue<double> m_totalEnergyConsumption;

  // State variables.
  int m_currentState;  // current state the modem is in
  Time m_lastUpdateTime;          // time stamp of previous energy update

  // energy depletion callback
  AcousticModemEnergyDepletionCallback m_energyDepletionCallback;
};

} // namespace ns3

#endif /* ACOUSTIC_MODEM_ENERGY_MODEL_H */
