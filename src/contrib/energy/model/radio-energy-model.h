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
 * Author: Sidharth Nabar <snabar@uw.edu>, He Wu <mdzz@u.washington.edu>
 */

#ifndef RADIO_ENERGY_MODEL_H
#define RADIO_ENERGY_MODEL_H

#include "device-energy-model.h"

namespace ns3 {

/**
 * \brief Keep track of energy consumption at a radio of a node.
 *
 * Assumption is that the radio is responsible for a significantly higher 
 * percentage of energy consumption than the computation at the node. 
 *
 * The units used are: Joules (J) for energy, Watts (W) for power.
 *
 * This is a base class, which should be inherited and implemented by children
 * classes.
 */
class RadioEnergyModel : public DeviceEnergyModel
{
public:
  /**
   * The states of the PHY layer.
   */
  enum RadioState
  {
    /**
     * The radio is transmitting.
     */
    TX = 0,
    /**
     * The radio is receiving.
     */
    RX,
    /**
     * The radio is idle.
     */
    IDLE,
    /**
     * The radio is in sleep mode.
     */
    SLEEP
  };

public:
  static TypeId GetTypeId (void);
  RadioEnergyModel ();
  virtual ~RadioEnergyModel ();

  /**
   * \returns the radio TX power in W.
   */
  double GetTxPowerW (void) const;

  /**
   * \param txPowerW the radio TX power in W
   */
  void SetTxPowerW (double txPowerW);

  /**
   * \returns the radio Rx power in W
   */
  double GetRxPowerW (void) const;

  /**
   * \param rxPowerW the radio RX power in W
   */
  void SetRxPowerW (double rxPowerW);

  /**
   * \returns the radio IDLE power in W
   */
  double GetIdlePowerW (void) const;

  /**
   * \param idlePowerW the radio IDLE power in W
   */
  void SetIdlePowerW (double idlePowerW);

  /**
   * \returns the radio SLEEP power in W
   */
  double GetSleepPowerW (void) const;

  /**
   * \param sleepPowerW the radio SLEEP power in W.
   */
  void SetSleepPowerW (double sleepPowerW);

  /**
   * \param destState Radio state to switch to.
   *
   * This function will update the remaining energy. Its implementation will be
   * completed in child classes.
   */
  void UpdateRemainingEnergy (const RadioState destState);


private:
  /**
   * \returns the radio TX power in W.
   *
   * Concrete subclasses of this base class must implement this method.
   */
  virtual double DoGetTxPowerW (void) const = 0;

  /**
   * \param the radio TX power in W
   *
   * Concrete subclasses of this base class must implement this method.
   */
  virtual void DoSetTxPowerW (double txPowerW) = 0;

  /**
   * \returns the radio RX power in W
   *
   * Concrete subclasses of this base class must implement this method.
   */
  virtual double DoGetRxPowerW (void) const = 0;

  /**
   * \param the radio RX power in W
   *
   * Concrete subclasses of this base class must implement this method.
   */
  virtual void DoSetRxPowerW (double rxPowerW) = 0;

  /**
   * \returns the radio IDLE power in W
   *
   * Concrete subclasses of this base class must implement this method.
   */
  virtual double DoGetIdlePowerW (void) const = 0;

  /**
   * \param idlePowerW the radio IDLE power in W
   *
   * Concrete subclasses of this base class must implement this method.
   */
  virtual void DoSetIdlePowerW (double idlePowerW) = 0;

  /**
   * \returns the radio SLEEP power in W
   *
   * Concrete subclasses of this base class must implement this method.
   */
  virtual double DoGetSleepPowerW (void) const = 0;

  /**
   * \param sleepPowerW the radio SLEEP power in W
   *
   * Concrete subclasses of this base class must implement this method.
   */
  virtual void DoSetSleepPowerW (double sleepPowerW) = 0;

  /**
   * \param destState Radio state to switch to.
   *
   * Concrete subclasses of this base class must implement this method.
   */
  virtual void DoUpdateRemainingEnergy (const RadioState destState) = 0;
};

}; // namespace ns3

#endif /* RADIO_ENERGY_MODEL_H */
