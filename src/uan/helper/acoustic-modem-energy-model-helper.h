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
* Author: Author: Andrea Sacco <andrea.sacco85@gmail.com>
*/

#ifndef ACOUSTIC_MODEM_ENERGY_MODEL_HELPER_H
#define ACOUSTIC_MODEM_ENERGY_MODEL_HELPER_H

#include "ns3/energy-model-helper.h"
#include "ns3/acoustic-modem-energy-model.h"

namespace ns3 {

/**
 * \brief Assign AcousticModemEnergyModel to uan devices.
 *
 * This installer installs AcousticModemEnergyModel for only UanNetDevice objects.
 */
class AcousticModemEnergyModelHelper : public DeviceEnergyModelHelper
{
public:
  /**
   * Construct a helper which is used to add a radio energy model to a node
   */
  AcousticModemEnergyModelHelper ();

  /**
   * Destroy an AcousticModemEnergy Helper
   */
  ~AcousticModemEnergyModelHelper ();

  /**
   * \param name the name of the attribute to set
   * \param v the value of the attribute
   *
   * Sets an attribute of the underlying energy model object.
   */
  void Set (std::string name, const AttributeValue &v);

  /**
   * \param callback Callback function for energy depletion handling.
   *
   * Sets the callback to be invoked when energy is depleted.
   */
  void SetDepletionCallback (
    AcousticModemEnergyModel::AcousticModemEnergyDepletionCallback callback);


private:
  /**
   * \param device Pointer to the NetDevice to install DeviceEnergyModel.
   *
   * Implements DeviceEnergyModel::Install.
   */
  virtual Ptr<DeviceEnergyModel> DoInstall (Ptr<NetDevice> device,
                                            Ptr<EnergySource> source) const;

private:
  ObjectFactory m_modemEnergy;
  AcousticModemEnergyModel::AcousticModemEnergyDepletionCallback m_depletionCallback;

};

} // namespace ns3

#endif /* ACOUSTIC_MODEM_ENERGY_MODEL_HELPER_H */
