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

#include "energy-model-helper.h"
#include "ns3/log.h"
#include "ns3/config.h"
#include "ns3/names.h"

namespace ns3 {

/*
 * EnergySourceHelper
 */
EnergySourceHelper::~EnergySourceHelper ()
{
}

/*
 * DeviceEnergyModelHelper
 */
DeviceEnergyModelHelper::~DeviceEnergyModelHelper ()
{
}

void
DeviceEnergyModelHelper::Install (Ptr<Node> node) const
{
  NS_ASSERT (node != NULL);
  DoInstall (node);
}

void
DeviceEnergyModelHelper::Install (NodeContainer c) const
{
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Install (*i);
    }
}

void
DeviceEnergyModelHelper::Install (std::string nodeName) const
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  Install (node);
}

void
DeviceEnergyModelHelper::InstallAll (void) const
{
  Install (NodeContainer::GetGlobal ());
}

/*
 * EnergyModelHelper
 */
EnergyModelHelper::EnergyModelHelper ()
{
}

void
EnergyModelHelper::Install (const EnergySourceHelper &sourceHelper,
                            const DeviceEnergyModelHelper &modelHelper, Ptr<Node> node) const
{
  NS_ASSERT (node != NULL);
  // install source. source must be installed before installing device model.
  sourceHelper.Install (node);
  // install device model
  modelHelper.Install (node);
}

void
EnergyModelHelper::Install (const EnergySourceHelper &sourceHelper,
                            const DeviceEnergyModelHelper &modelHelper, NodeContainer c) const
{
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Install (sourceHelper, modelHelper, *i);
    }
}

void
EnergyModelHelper::Install (const EnergySourceHelper &sourceHelper,
                            const DeviceEnergyModelHelper &modelHelper, std::string nodeName) const
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  Install (sourceHelper, modelHelper, node);
}

void
EnergyModelHelper::InstallAll (const EnergySourceHelper &sourceHelper,
                               const DeviceEnergyModelHelper &modelHelper) const
{
  Install (sourceHelper, modelHelper, NodeContainer::GetGlobal ());
}

} // namespace ns3
