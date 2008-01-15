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

namespace ns3 {

class RandomPosition;

/**
 * \brief layout objects randomly in 3d space.
 *
 * This assigns an initial position to each object
 * according to its position model and assigns
 * an instance of a mobility model to each object
 * according to its mobility model class id.
 */
class RandomTopology
{
 public:
  /**
   * Create a default random topology with construction
   * values from \valueref{RandomTopologyPositionType}, and,
   * \valueref{RandomTopologyMobilityType}.
   */
  RandomTopology ();
  /**
   * \param positionModel model to set the initial position
   *        of each object.
   * \param mobilityModel type of mobility model to attach to each object.
   *
   * Create a random topology based on the
   * specified position and mobility models.
   */
  RandomTopology (Ptr<RandomPosition> positionModel,
                  TypeId mobilityModel);

  ~RandomTopology ();

  /**
   * \param interfaceId the type of mobility model attached to each
   *        input object if it does not have one already.
   */
  void SetMobilityModel (TypeId interfaceId);
  /**
   * \param positionModel the position model used to initialize
   *        the position of each object.
   */
  void SetPositionModel (Ptr<RandomPosition> positionModel);

  /**
   * \param object the object to layout
   *
   * Assign an initial position and a mobility model
   * to the object.
   */
  void LayoutOne (Ptr<Object> object);

  /**
   * \param begin iterator which identifies the first
   *        object to configure.
   * \param end iterator which identifies the last
   *        object to configure.
   *
   * Assign an initial position and a mobility model
   * to the objects.
   */
  template <typename T>
  void Layout (const T &begin, const T &end);
 private:
  Ptr<RandomPosition> m_positionModel;
  TypeId m_mobilityModel;
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
