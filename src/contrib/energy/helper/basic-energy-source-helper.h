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

#ifndef BASIC_ENERGY_SOURCE_HELPER
#define BASIC_ENERGY_SOURCE_HELPER

#include "energy-model-helper.h"

namespace ns3 {

class Node;

/**
 * \brief Creates a BasicEnergySource object.
 */
class BasicEnergySourceHelper : public EnergySourceHelper
{
public:
  BasicEnergySourceHelper ();
  ~BasicEnergySourceHelper ();

  void Set (std::string name, const AttributeValue &v);

  Ptr<EnergySource> Create (void) const;

  void Install (Ptr<Node> node) const;

private:
  ObjectFactory m_basicEnergySource;

};

} // namespace ns3

#endif  /* BASIC_ENERGY_SOURCE_HELPER */
