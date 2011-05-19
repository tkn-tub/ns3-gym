/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 Georgia Institute of Technology
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
 * Author: George Riley<riley@ece.gatech.edu>
 */
#ifndef CANVAS_LOCATION_H
#define CANVAS_LOCATION_H

#include "ns3/object.h"
#include "ns3/traced-callback.h"
#include "ns3/vector.h"

namespace ns3 {


/**
 * \ingroup netanim
 *
 * \brief Keep track of the current location of an object
 *
 * This can be used anytime a logical node location is needed
 * (as opposed to a physical location used by the wireless PHY
 * layer to calculate path loss).  One potential use of 
 * this is by the animator to determine where to position the
 * node icon on the animation display.  Location units are 
 * arbitrary and dimensionless.  In the case of use by the
 * animator they dimensions are in pixels.
 */
class CanvasLocation : public Object
{
public:
  static TypeId GetTypeId (void);
  CanvasLocation ();
  virtual ~CanvasLocation ();

  /**
   * \returns the current location
   */
  Vector GetLocation (void) const;
  /**
   * \param location the location to set.
   */
  void SetLocation (const Vector &location);
private:
  Vector m_location;
};

}; // namespace ns3

#endif /* CANVAS_LOCATION_H */
