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

#include "device-energy-model.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("DeviceEnergyModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (DeviceEnergyModel);

TypeId
DeviceEnergyModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::DeviceEnergyModel")
    .SetParent<Object> ()
  ;
  return tid;
}

DeviceEnergyModel::DeviceEnergyModel ()
{
}

DeviceEnergyModel::~DeviceEnergyModel ()
{
}

void
DeviceEnergyModel::HandleEnergyDepletion (void)
{
  DoHandleEnergyDepletion ();
}

void
DeviceEnergyModel::SetEnergySource (Ptr<EnergySource> source)
{
  NS_LOG_FUNCTION (this << source);
  NS_ASSERT (source != NULL);  // energy source must exist
  m_energySourcePtr = source;
}

/*
 * Private function starts here.
 */

void
DeviceEnergyModel::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_energySourcePtr = NULL;
}

/*
 * Protected functions start here.
 */

void
DeviceEnergyModel::DecreaseRemainingEnergy (double energyJ)
{
  NS_LOG_FUNCTION (this << energyJ);
  NS_ASSERT (m_energySourcePtr != NULL);  // energy source must exist
  m_energySourcePtr->DecreaseRemainingEnergy (energyJ);
}

void
DeviceEnergyModel::IncreaseRemainingEnergy (double energyJ)
{
  NS_LOG_FUNCTION (this << energyJ);
  NS_ASSERT (m_energySourcePtr != NULL);  // energy source must exist
  m_energySourcePtr->IncreaseRemainingEnergy (energyJ);
}

void
DeviceEnergyModel::BreakSourceRefCycle (void)
{
  NS_LOG_FUNCTION (this);
  m_energySourcePtr = NULL;
}

} // namespace ns3
