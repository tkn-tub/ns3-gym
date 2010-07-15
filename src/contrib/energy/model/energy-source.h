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

#ifndef ENERGY_SOURCE_H
#define ENERGY_SOURCE_H

#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/type-id.h"
#include "device-energy-model.h"

namespace ns3 {

class DeviceEnergyModel;

/**
 * \brief Energy source base class.
 *
 * This is the base class for energy sources. Energy sources keep track of
 * remaining energy. Device energy models will be updating the remaining energy
 * in the energy source. The energy source itself does not update the remaining
 * energy. Energy source also keeps a list of device energy models installed on
 * the same node. When the remaining energy level reaches 0, the energy source
 * will notify all device energy models stored in the list.
 *
 * Unit of energy is chosen as Joules since energy models typically calculate
 * energy as (time in seconds * power in Watts). If the energy source stores
 * energy in different units (eg. kWh), a simple converter function should
 * suffice.
 */
class EnergySource : public Object
{
public:
  /// List of pointers to DeviceEnergyModel objects.
  typedef std::vector< Ptr<DeviceEnergyModel> > DeviceEnergyModelList;

public:
  static TypeId GetTypeId (void);
  EnergySource ();
  virtual ~EnergySource ();

  /// This function returns initial energy stored in energy source.
  double GetInitialEnergy (void) const;

  /// This function returns the remaining energy stored in the energy source.
  double GetRemainingEnergy (void) const;

  /**
   * \param energyJ Amount of energy to decrease (in Joules)
   *
   * This function decreases the remaining energy in the energy source by the
   * specified amount.
   */
  void DecreaseRemainingEnergy (double energyJ);

  /**
   * \param energyJ Amount of energy to increase (in Joules)
   *
   * This function increases the remaining energy in the energy source by the
   * specified amount. Provided for supporting re-charging or scavenging.
   */
  void IncreaseRemainingEnergy (double energyJ);

  /**
   * \return Energy fraction = remaining energy / initial energy [0, 1]
   *
   * This function returns the percentage of energy left in the energy source.
   */
  double GetEnergyFraction (void) const;

  /**
   * \param deviceEnergyModelPtr Pointer to device energy model.
   *
   * This function appends a device energy model to the end of a list of
   * DeviceEnergyModelInfo structs.
   */
  void AppendDeviceEnergyModel (Ptr<DeviceEnergyModel> deviceEnergyModelPtr);

  /**
   * \param tid TypeId of the DeviceEnergyModel we are searching for.
   * \returns List of pointers to DeviceEnergyModel objects installed on node.
   */
  DeviceEnergyModelList FindDeviceEnergyModels (TypeId tid);
  /**
   * \param name name of the DeviceEnergyModel we are searching for.
   * \returns List of pointers to DeviceEnergyModel objects installed on node.
   */
  DeviceEnergyModelList FindDeviceEnergyModels (std::string name);


private:
  /**
   * All child's implementation must call BreakDeviceEnergyModelRefCycle to
   * ensure reference cycles to DeviceEnergyModel objects are broken.
   */
  void DoDispose (void);

  /// Implements GetInitialEnergy.
  virtual double DoGetInitialEnergy (void) const = 0;

  /// Implements GetRemainingEnergy.
  virtual double DoGetRemainingEnergy (void) const = 0;

  /// Implements DecreaseRemainingEnergy.
  virtual void DoDecreaseRemainingEnergy (double energyJ) = 0;

  /// Implements IncreaseRemainingEnergy.
  virtual void DoIncreaseRemainingEnergy (double energyJ) = 0;

  /// Implements GetEnergyFraction.
  virtual double DoGetEnergyFraction (void) const = 0;

private:
  /// List of device energy models installed on the same node.
  DeviceEnergyModelList m_deviceEnergyModelList;


protected:
  /**
   * This function notifies all DeviceEnergyModel of energy depletion event. It
   * is called by the child EnergySource class when energy depletion happens.
   */
  void NotifyEnergyDrained (void);

  /**
   * This function is called to break reference cycle between EnergySource and
   * DeviceEnergyModel. Child of the EnergySource base class must call this
   * function in their implementation of DoDispose to make sure the reference
   * cycle is broken.
   *
   * Normally this work will be completed by the DoDispose function. However it
   * will be overridden in the child class. Hence we introduced this function.
   */
  void BreakDeviceEnergyModelRefCycle (void);

};

} // namespace ns3

#endif /* ENERGY_SOURCE_H */
