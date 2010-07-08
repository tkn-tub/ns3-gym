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

#include "basic-energy-source-helper.h"
#include "ns3/energy-source.h"

namespace ns3 {

BasicEnergySourceHelper::BasicEnergySourceHelper ()
{
  m_basicEnergySource.SetTypeId ("ns3::BasicEnergySource");
}

BasicEnergySourceHelper::~BasicEnergySourceHelper ()
{
}

void
BasicEnergySourceHelper::Set (std::string name, const AttributeValue &v)
{
  m_basicEnergySource.Set (name, v);
}

Ptr<EnergySource>
BasicEnergySourceHelper::Create (void) const
{
  Ptr<EnergySource> source = m_basicEnergySource.Create<EnergySource> ();
  NS_ASSERT (source != 0);
  return source;
}

void
BasicEnergySourceHelper::Install (Ptr<Node> node) const
{
  NS_ASSERT (node != NULL);
  // check if energy source already exists
  Ptr<EnergySource> source = node->GetObject<EnergySource> ();
  if (source != NULL)
    {
      NS_FATAL_ERROR ("Energy source already installed!");
      return;
    }
  // create energy source
  source = Create ();
  NS_ASSERT (source != NULL);
  // aggregate source to node
  node->AggregateObject (source);
}

} // namespace ns3
