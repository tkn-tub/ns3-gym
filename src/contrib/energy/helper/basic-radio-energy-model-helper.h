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

#ifndef BASIC_RADIO_ENERGY_MODEL_HELPER_H
#define BASIC_RADIO_ENERGY_MODEL_HELPER_H

#include "energy-model-helper.h"
#include "ns3/device-energy-model.h"
#include "ns3/basic-radio-energy-model.h"

namespace ns3 {

/**
 * \brief Assign energy model to nodes.
 *
 * This installer is used for BasicRadioEnergyModel.
 */
class BasicRadioEnergyModelHelper : public DeviceEnergyModelHelper
{
public:
  /**
   * Construct a helper which is used to add a radio energy model to a node
   */
  BasicRadioEnergyModelHelper ();

  /**
   * Destroy a RadioEnergy Helper
   */
  ~BasicRadioEnergyModelHelper ();

  /**
   * \param name the name of the attribute to set
   * \param v the value of the attribute
   *
   * Sets an attribute of the underlying PHY object.
   */
  void Set (std::string name, const AttributeValue &v);

  /**
   * \returns A newly created RadioEnergySource object.
   */
  Ptr<DeviceEnergyModel> Create (void) const;

  /**
   * \param callback Callback function for energy depletion handling.
   *
   * Sets the callback to be invoked when energy is depleted.
   */
  void SetDepletionCallback (
    BasicRadioEnergyModel::BasicEnergyDepletionCallback callback);


private:
  /**
   * \brief Add an energy model to the specified node.
   *
   * \param node The node on which the energy model is to be installed.
   *
   * This method creates an instance of a ns3::BasicRadioEnergyModel and
   * aggregates it to the given node.
   */
  void DoInstall (Ptr<Node> node) const;

private:
  ObjectFactory m_radioEnergy;
  /// Callback function for energy depletion.
  BasicRadioEnergyModel::BasicEnergyDepletionCallback m_depletionCallback;

};

} // namespace ns3

#endif /* BASIC_RADIO_ENERGY_MODEL_HELPER_H */
