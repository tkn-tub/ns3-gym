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

#include "basic-radio-energy-model-helper.h"
#include "basic-energy-source-helper.h"
#include "ns3/radio-energy-model.h"
#include "ns3/config.h"
#include "ns3/names.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("BasicRadioEnergyModelHelper");

BasicRadioEnergyModelHelper::BasicRadioEnergyModelHelper ()
{
  m_radioEnergy.SetTypeId ("ns3::BasicRadioEnergyModel");
  m_depletionCallback.Nullify ();
}

BasicRadioEnergyModelHelper::~BasicRadioEnergyModelHelper ()
{
}

void
BasicRadioEnergyModelHelper::Set (std::string name, const AttributeValue &v)
{
  m_radioEnergy.Set (name, v);
}

Ptr<DeviceEnergyModel>
BasicRadioEnergyModelHelper::Create (void) const
{
  return m_radioEnergy.Create<DeviceEnergyModel> ();
}

void
BasicRadioEnergyModelHelper::SetDepletionCallback (
  BasicRadioEnergyModel::BasicEnergyDepletionCallback callback)
{
  m_depletionCallback = callback;
}

/*
 * Private function starts here.
 */

void
BasicRadioEnergyModelHelper::DoInstall (Ptr<Node> node) const
{
  NS_LOG_FUNCTION (this << node);
  // check if energy source is installed
  Ptr<EnergySource> source = node->GetObject<EnergySource> ();
  if (source != NULL)
    {
      Ptr<BasicRadioEnergyModel> model = m_radioEnergy.Create ()->
        GetObject<BasicRadioEnergyModel> ();
      if (model == NULL)
        {
          NS_FATAL_ERROR ( "The requested radio energy model does not exist: "
                           << m_radioEnergy.GetTypeId ().GetName ());
        }
      else
        {
          // set energy source pointer
          model->SetEnergySource (source);
          // set energy depletion callback
          model->SetEnergyDepletionCallback (m_depletionCallback);
          // add model to device model list in energy source
          source->AppendDeviceEnergyModel (model);
        }
    }
  else
    {
      NS_FATAL_ERROR ("Energy source NOT installed!");
    }
}

} // namespace ns3
