/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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

void
EnergySource::SetNode (Ptr<Node> node)
{
  NS_ASSERT (node != NULL);
  m_node = node;
}

Ptr<Node>
EnergySource::GetNode (void) const
{
  return m_node;
}

void
EnergySource::AppendDeviceEnergyModel (Ptr<DeviceEnergyModel> deviceEnergyModelPtr)
{
  NS_LOG_FUNCTION (this << deviceEnergyModelPtr);
  NS_ASSERT (deviceEnergyModelPtr != NULL); // model must exist
  m_models.Add (deviceEnergyModelPtr);
}

DeviceEnergyModelContainer
EnergySource::FindDeviceEnergyModels (TypeId tid)
{
  NS_LOG_FUNCTION (this << tid);
  DeviceEnergyModelContainer container;
  DeviceEnergyModelContainer::Iterator i;
  for (i = m_models.Begin (); i != m_models.End (); i++)
    {
      if ((*i)->GetInstanceTypeId () == tid)
        {
          container.Add (*i);
        }
    }
  return container;
}

DeviceEnergyModelContainer
EnergySource::FindDeviceEnergyModels (std::string name)
{
  NS_LOG_FUNCTION (this << name);
  DeviceEnergyModelContainer container;
  DeviceEnergyModelContainer::Iterator i;
  for (i = m_models.Begin (); i != m_models.End (); i++)
    {
      if ((*i)->GetInstanceTypeId ().GetName ().compare (name) == 0)
        {
          container.Add (*i);
        }
    }
  return container;
}

void
EnergySource::StartDeviceModels (void)
{
  /*
   * Device models are not aggregated to the node, hence we have to manually
   * call dispose method here.
   */
  DeviceEnergyModelContainer::Iterator i;
  for (i = m_models.Begin (); i != m_models.End (); i++)
    {
      (*i)->Start ();
    }
}

void
EnergySource::DisposeDeviceModels (void)
{
  /*
   * Device models are not aggregated to the node, hence we have to manually
   * call dispose method here.
   */
  DeviceEnergyModelContainer::Iterator i;
  for (i = m_models.Begin (); i != m_models.End (); i++)
    {
      (*i)->Dispose ();
    }
}

/*
 * Private function starts here.
 */

void
EnergySource::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  BreakDeviceEnergyModelRefCycle ();
}

/*
 * Protected functions start here.
 */

double
EnergySource::CalculateTotalCurrent (void)
{
  NS_LOG_FUNCTION (this);
  double totalCurrentA = 0.0;
  DeviceEnergyModelContainer::Iterator i;
  for (i = m_models.Begin (); i != m_models.End (); i++)
    {
      totalCurrentA += (*i)->GetCurrentA ();
    }
  return totalCurrentA;
}

void
EnergySource::NotifyEnergyDrained (void)
{
  NS_LOG_FUNCTION (this);
  // notify all device energy models installed on node
  DeviceEnergyModelContainer::Iterator i;
  for (i = m_models.Begin (); i != m_models.End (); i++)
    {
      (*i)->HandleEnergyDepletion ();
    }
}

void
EnergySource::BreakDeviceEnergyModelRefCycle (void)
{
  NS_LOG_FUNCTION (this);
  m_models.Clear ();
  m_node = NULL;
}

} // namespace ns3
