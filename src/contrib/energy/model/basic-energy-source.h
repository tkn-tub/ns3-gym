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

#ifndef BASIC_ENERGY_SOURCE
#define BASIC_ENERGY_SOURCE

#include "ns3/traced-value.h"
#include "energy-source.h"

namespace ns3 {

class BasicEnergySource : public EnergySource
{
public:
  static TypeId GetTypeId (void);
  BasicEnergySource ();
  virtual ~BasicEnergySource ();


  /**
   * \param initialEnergyJ Initial energy, in Joules
   *
   * Implements SetInitialEnergy. Note that initial energy is assumed to be set
   * before simulation starts and is set only once per simulation.
   */
  void SetInitialEnergy (double initialEnergyJ);


private:
  void DoDispose (void);

  /**
   * \return Initial energy stored in energy source, in Joules.
   *
   * Implements GetInitialEnergy.
   */
  double DoGetInitialEnergy (void) const;

  /**
   * \return Remaining energy in energy source, in Joules
   *
   * Implements GetRemainingEnergy.
   */
  double DoGetRemainingEnergy (void) const;

  /**
   * \param energy Amount of energy (in Joules) to decrease from energy source.
   *
   * Implements DecreaseRemainingEnergy.
   */
  void DoDecreaseRemainingEnergy (double energyJ);

  /**
   * Handles the remaining energy going to zero event. This function notifies
   * all the energy models aggregated to the node about the energy being
   * depleted. Each energy model is then responsible for its own handler.
   */
  void HandleEnergyDrainedEvent (void);

  /**
   * \param energy Amount of energy (in Joules) to increase from energy source.
   *
   * Implements IncreaseRemainingEnergy.
   */
  void DoIncreaseRemainingEnergy (double energyJ);

  /// Implements GetEnergyFraction.
  double DoGetEnergyFraction (void) const;

private:
  double m_initialEnergyJ;                // initial energy, in Joules
  TracedValue<double> m_remainingEnergyJ; // remaining energy, in Joules

};

} // namespace ns3

#endif /* BASIC_ENERGY_SOURCE */
