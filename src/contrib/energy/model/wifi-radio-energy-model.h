/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Network Security Lab, University of Washington, Seattle.
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
 * Authors: Sidharth Nabar <snabar@uw.edu>, He Wu <mdzz@u.washington.edu>
 */

#ifndef WIFI_RADIO_ENERGY_MODEL_H
#define WIFI_RADIO_ENERGY_MODEL_H

#include "ns3/device-energy-model.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include "ns3/traced-value.h"

namespace ns3 {

/**
 * \brief A WiFi radio energy model.
 * 
 * 4 states are defined for the radio: TX, RX, IDLE, SLEEP. Default state is
 * IDLE.
 * The different types of transactions that are defined are: 
 *  1. Tx: State goes from IDLE to TX, radio is in TX state for TX_duration,
 *     then state goes from TX to IDLE.
 *  2. Rx: State goes from IDLE to RX, radio is in RX state for RX_duration,
 *     then state goes from RX to IDLE.
 *  3. Go_to_Sleep: State goes from IDLE to SLEEP.
 *  4. End_of_Sleep: State goes from SLEEP to IDLE.
 * The class keeps track of what state the radio is currently in.
 *
 * Energy calculation: For each transaction, this model notifies EnergySource
 * object. The EnergySource object will query this model for the total current.
 * Then the EnergySource object uses the total current to calculate energy.
 *
 * Default values for power consumption are based on CC2420 radio chip, with
 * supply voltage as 2.5V and currents as 17.4 mA (TX), 18.8 mA (RX), 20 uA
 * (sleep) and 426 uA (idle).
 */
class WifiRadioEnergyModel : public DeviceEnergyModel
{
public:
  /**
   * Wifi radio states.
   */
  enum WifiRadioState
  {
    /**
     * Radio is transmitting.
     */
    TX = 0,
    /**
     * Radio is receiving.
     */
    RX,
    /**
     * Radio is idling.
     */
    IDLE,
    /**
     * Radio is asleep.
     */
    SLEEP
  };

  /**
   * Callback type for energy depletion handling.
   */
  typedef Callback<void> WifiRadioEnergyDepletionCallback;

public:
  static TypeId GetTypeId (void);
  WifiRadioEnergyModel ();
  virtual ~WifiRadioEnergyModel ();

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
   * \returns Pointer to node.
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
   * \returns Total energy consumption of the wifi device.
   *
   * Implements DeviceEnergyModel::GetTotalEnergyConsumption.
   */
  virtual double GetTotalEnergyConsumption (void) const;

  // Setter & getters for state power consumption.
  double GetTxCurrentA (void) const;
  void SetTxCurrentA (double txCurrentA);
  double GetRxCurrentA (void) const;
  void SetRxCurrentA (double rxCurrentA);
  double GetIdleCurrentA (void) const;
  void SetIdleCurrentA (double idleCurrentA);
  double GetSleepCurrentA (void) const;
  void SetSleepCurrentA (double sleepCurrentA);

  /**
   * \returns Current state.
   */
  WifiRadioState GetCurrentState (void) const;

  /**
   * \param callback Callback function.
   *
   * Sets callback for energy depletion handling.
   */
  void SetEnergyDepletionCallback (WifiRadioEnergyDepletionCallback callback);

  /**
   * \brief Changes state of the WifiRadioEnergyMode.
   *
   * \param newState New state the wifi radio is in.
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
   * \param destState Radio state to switch to.
   * \return True if the transition is allowed.
   *
   * This function checks if a given radio state transition is allowed.
   */
  bool IsStateTransitionValid (const WifiRadioState destState);

  /**
   * \param currentState New state the radio device is currently in.
   *
   * Sets current state. This function is private so that only the energy model
   * can change its own state.
   */
  void SetWifiRadioState (const WifiRadioState state);

private:
  Ptr<Node> m_node;
  Ptr<EnergySource> m_source;

  // Member variables for current draw in different radio modes.
  double m_txCurrentA;
  double m_rxCurrentA;
  double m_idleCurrentA;
  double m_sleepCurrentA;

  // This variable keeps track of the total energy consumed by this model.
  TracedValue<double> m_totalEnergyConsumption;

  // State variables.
  WifiRadioState m_currentState;  // current state the radio is in
  Time m_lastUpdateTime;          // time stamp of previous energy update

  // energy depletion callback
  WifiRadioEnergyDepletionCallback m_energyDepletionCallback;
};

} // namespace ns3

#endif /* WIFI_RADIO_ENERGY_MODEL_H */
