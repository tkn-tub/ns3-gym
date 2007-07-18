/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
 * All rights reserved.
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef RANDOM_TOPOLOGY_H
#define RANDOM_TOPOLOGY_H

#include "ns3/ptr.h"
#include "ns3/object.h"
#include "ns3/component-manager.h"

namespace ns3 {

class Random2dPosition;

class RandomTopology
{
 public:
  RandomTopology ();
  RandomTopology (Ptr<Random2dPosition> positionModel,
                           ClassId mobilityModel);

  ~RandomTopology ();

  void SetMobilityModel (ClassId classId);
  void SetPositionModel (Ptr<Random2dPosition> positionModel);

  void LayoutOne (Ptr<Object> object);

  template <typename T>
  void Layout (const T &begin, const T &end);
 private:
  Ptr<Random2dPosition> m_positionModel;
  ClassId m_mobilityModel;
};

} // namespace ns3

namespace ns3 {

template <typename T>
void 
RandomTopology::Layout (const T &begin, const T &end)
{
  for (T i = begin; i != end; i++)
    {
      LayoutOne (*i);
    }
}


} // namespace ns3 

#endif /* RANDOM_TOPOLOGY_H */
