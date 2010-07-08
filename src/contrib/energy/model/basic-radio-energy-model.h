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

#ifndef BASIC_RADIO_ENERGY_MODEL_H
#define BASIC_RADIO_ENERGY_MODEL_H

#include "radio-energy-model.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include "ns3/traced-value.h"

namespace ns3 {

/**
 * \brief a basic radio energy model
 *
 * This is a child class of the RadioEnergyModel class.
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
 * Energy calculation: For each transaction, we see how much time the radio
 * spent in each state and calculate energy consumption accordingly.
 *
 * Energy update: Remaining energy =
 *                Previous remaining energy - Energy_transaction.
 *
 * Default values for power consumption are based on CC2420 radio chip, with
 * supply voltage as 2.5V and currents as 17.4 mA (TX), 18.8 mA (RX), 20 uA
 * (sleep) and 426 uA (idle).
 *
 * TODO change default values to wifi radio chip numbers.
 */
class BasicRadioEnergyModel : public RadioEnergyModel
{
public:
  /// Callback type for energy depletion handling.
  typedef Callback<void> BasicEnergyDepletionCallback;

public:
  static TypeId GetTypeId (void);
  BasicRadioEnergyModel ();
  virtual ~BasicRadioEnergyModel ();

  /**
   * For current state, the getter is public, but the setter method should be
   * private.
   */
  RadioState GetCurrentState (void) const;

  /**
   * \param interval Energy update interval.
   *
   * Setter for the energy update interval - time between two consecutive energy
   * updates.
   */
  void SetEnergyUpdateInterval (const Time interval);

  /**
   * Getter for the energy update interval - time between two consecutive energy
   * updates.
   */
  Time GetEnergyUpdateInterval (void) const;

  /**
   * \param callback Callback function.
   *
   * Sets callback for energy depletion handling.
   */
  void SetEnergyDepletionCallback (BasicEnergyDepletionCallback callback);

  /**
   * \param destState Radio state to switch to.
   * \return True if the transition is allowed.
   *
   * This function checks if a given radio state transition is allowed.
   */
  bool IsStateTransitionValid (const RadioState destState);


private:
  void DoDispose (void);

  /// Handles energy depletion.
  void DoHandleEnergyDepletion (void);

  /// Implements GetTotalEnergyConsumption.
  double DoGetTotalEnergyConsumption (void) const;

  double DoGetTxPowerW (void) const;
  void DoSetTxPowerW (double txPowerW);
  double DoGetRxPowerW (void) const;
  void DoSetRxPowerW (double rxPowerW);
  double DoGetIdlePowerW (void) const;
  void DoSetIdlePowerW (double idlePowerW);
  double DoGetSleepPowerW (void) const;
  void DoSetSleepPowerW (double sleepPowerW);

  void DoUpdateRemainingEnergy (const RadioState destState);

  /**
   * \param currentState New state the radio device is currently in.
   *
   * Sets current state. This function is private so that only the energy model
   * can change its own state.
   */
  void SetCurrentState (const RadioState currentState);

private:
  /*
   * Member variables for the power consumption in different radio modes.
   * Power = (supply voltage * current draw)
   */
  double m_txPower;
  double m_rxPower;
  double m_idlePower;
  double m_sleepPower;

  // This variable keeps track of the total energy consumed by this particular model.
  TracedValue<double> m_totalEnergyConsumption;

  // State variables.
  RadioState m_currentState;  // current state the radio is in
  Time m_lastUpdateTime;      // time stamp of previous energy update
  EventId m_periodicEnergyUpdateEvent;  // scheduled event to update remaining energy periodically
  Time m_energyUpdateInterval;  // Time between consecutive periodic energy updates.

  // energy depletion callback
  BasicEnergyDepletionCallback m_energyDepletionCallback;
};

} // namespace ns3

#endif /* BASIC_RADIO_ENERGY_MODEL_H */
