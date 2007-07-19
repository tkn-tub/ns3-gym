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
#ifndef HIERARCHICAL_MOBILITY_MODEL_H
#define HIERARCHICAL_MOBILITY_MODEL_H

#include "mobility-model.h"

namespace ns3 {

class HierarchicalMobilityModel : public MobilityModel
{
public:
  static const InterfaceId iid;

  HierarchicalMobilityModel (Ptr<MobilityModel> child, Ptr<MobilityModel> parent);

  Ptr<MobilityModel> GetChild (void) const;
  Ptr<MobilityModel> GetParent (void) const;
  
private:
  virtual Position DoGet (void) const;
  virtual void DoSet (const Position &position);
  virtual Speed DoGetSpeed (void) const;

  void ParentChanged (Ptr<const MobilityModel> model);
  void ChildChanged (Ptr<const MobilityModel> model);

  Ptr<MobilityModel> m_child;
  Ptr<MobilityModel> m_parent;
};


} // namespace ns3

#endif /* HIERARCHICAL_MOBILITY_MODEL_H */
