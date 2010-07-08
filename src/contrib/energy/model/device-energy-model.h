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

#ifndef DEVICE_ENERGY_MODEL
#define DEVICE_ENERGY_MODEL

#include "ns3/object.h"
#include "energy-source.h"

namespace ns3 {

class EnergySource;

/**
 * \brief Base class for device energy models.
 *
 * A device energy model should represent the energy consumption behavior of a
 * specific device. It will update remaining energy stored in the EnergySource
 * object installed on node. When energy is depleted, each DeviceEnergyModel
 * object installed on the same node will be informed by the EnergySource.
 */
class DeviceEnergyModel : public Object
{
public:
  static TypeId GetTypeId (void);
  DeviceEnergyModel ();
  virtual ~DeviceEnergyModel ();

  /**
   * This function is called by the EnergySource object when energy stored in
   * the energy source is depleted. Should be implemented by child classes.
   */
  void HandleEnergyDepletion (void);

  /// Sets the pointer to node energy source.
  void SetEnergySource (Ptr<EnergySource> source);

private:
  /*
   * Do not include DoStart in any child of this base class. DeviceEnergyModel
   * is *not* aggregated to the node hence DoStart will *not* be called.
   */

  /**
   * All child's implementation must call BreakSourceRefCycle to ensure
   * reference cycle to EnergySource object is broken.
   */
  void DoDispose (void);

  /// Implements HandleEnergyDepletion.
  virtual void DoHandleEnergyDepletion (void) = 0;

private:
  /// Pointer to EnergySource object
  Ptr<EnergySource> m_energySourcePtr;

protected:
  /// This function is used to access the private energy source pointer.
  void DecreaseRemainingEnergy (double energyJ);

  /// This function is used to access the private energy source pointer.
  void IncreaseRemainingEnergy (double energyJ);

  /**
   * This function is called to break reference cycle between DeviceEnergyModel
   * and EnergySource. Child of the DeviceEnergyModel base class must call this
   * function in their implementation of DoDispose to make sure the reference
   * cycle is broken.
   *
   * Normally this work will be completed by the DoDispose function. However it
   * will be overridden in the child class. Hence we introduced this function.
   */
  void BreakSourceRefCycle (void);

};

}

#endif /* DEVICE_ENERGY_MODEL */
