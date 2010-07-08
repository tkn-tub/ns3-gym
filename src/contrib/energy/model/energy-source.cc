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

#include "energy-source.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("EnergySource");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (EnergySource);

TypeId
EnergySource::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::EnergySource")
    .SetParent<Object> ()
  ;
  return tid;
}

EnergySource::EnergySource ()
{
}

EnergySource::~EnergySource ()
{
}

double
EnergySource::GetInitialEnergy (void) const
{
  return DoGetInitialEnergy ();
}

double
EnergySource::GetRemainingEnergy (void) const
{
  return DoGetRemainingEnergy ();
}

void
EnergySource::DecreaseRemainingEnergy (double energyJ)
{
  DoDecreaseRemainingEnergy (energyJ);
}

void
EnergySource::IncreaseRemainingEnergy (double energyJ)
{
  DoIncreaseRemainingEnergy (energyJ);
}

double
EnergySource::GetEnergyFraction (void) const
{
  return DoGetEnergyFraction ();
}

void
EnergySource::AppendDeviceEnergyModel (
  Ptr<DeviceEnergyModel> deviceEnergyModelPtr)
{
  NS_LOG_FUNCTION (this << deviceEnergyModelPtr);
  NS_ASSERT (deviceEnergyModelPtr != NULL); // model must exist
  m_deviceEnergyModelList.push_back (deviceEnergyModelPtr);
}

EnergySource::DeviceEnergyModelList
EnergySource::FindDeviceEnergyModels (TypeId tid)
{
  NS_LOG_FUNCTION (this << tid);
  DeviceEnergyModelList list;
  DeviceEnergyModelList::iterator listItr;
  for (listItr = m_deviceEnergyModelList.begin ();
       listItr != m_deviceEnergyModelList.end (); listItr++)
    {
      if ((*listItr)->GetInstanceTypeId () == tid)
        {
          list.push_back (*listItr);
        }
    }
  return list;
}

EnergySource::DeviceEnergyModelList
EnergySource::FindDeviceEnergyModels (std::string name)
{
  NS_LOG_FUNCTION (this << name);
  DeviceEnergyModelList list;
  DeviceEnergyModelList::iterator listItr;
  for (listItr = m_deviceEnergyModelList.begin ();
       listItr != m_deviceEnergyModelList.end (); listItr++)
    {
      if ((*listItr)->GetInstanceTypeId ().GetName ().compare (name) == 0)
        {
          list.push_back (*listItr);
        }
    }
  return list;
}

/*
 * Private function starts here.
 */

void
EnergySource::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_deviceEnergyModelList.clear (); // break reference cycle
}

/*
 * Protected functions start here.
 */

void
EnergySource::NotifyEnergyDrained (void)
{
  NS_LOG_FUNCTION (this);
  // notify all device energy models installed on node
  DeviceEnergyModelList::iterator listItr;
  for (listItr = m_deviceEnergyModelList.begin ();
       listItr != m_deviceEnergyModelList.end (); listItr++)
    {
      NS_LOG_DEBUG ("BasicEnergySource:Notifying device energy model: "
                    << (*listItr)->GetInstanceTypeId ());
      (*listItr)->HandleEnergyDepletion ();
    }
}

void
EnergySource::BreakDeviceEnergyModelRefCycle (void)
{
  NS_LOG_FUNCTION (this);
  m_deviceEnergyModelList.clear (); // break reference cycle
}

} // namespace ns3
