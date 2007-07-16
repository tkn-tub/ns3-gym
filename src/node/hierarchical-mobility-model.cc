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
#include "hierarchical-mobility-model.h"
#include "mobility-model-notifier.h"

namespace ns3 {

HierarchicalMobilityModel::HierarchicalMobilityModel (Ptr<MobilityModel> child, Ptr<MobilityModel> parent)
  : m_child (child),
    m_parent (parent)
{
  Ptr<MobilityModelNotifier> childNotifier = 
    m_child->QueryInterface<MobilityModelNotifier> (MobilityModelNotifier::iid);
  Ptr<MobilityModelNotifier> parentNotifier = 
    m_parent->QueryInterface<MobilityModelNotifier> (MobilityModelNotifier::iid);
  if (childNotifier == 0)
    {
      childNotifier = Create<MobilityModelNotifier> ();
      child->AddInterface (childNotifier);
    }
  if (parentNotifier == 0)
    {
      parentNotifier = Create<MobilityModelNotifier> ();
      parent->AddInterface (parentNotifier);
    }
  childNotifier->RegisterListener (MakeCallback (&HierarchicalMobilityModel::ChildChanged, this));
  parentNotifier->RegisterListener (MakeCallback (&HierarchicalMobilityModel::ParentChanged, this));
}

Ptr<MobilityModel> 
HierarchicalMobilityModel::GetChild (void) const
{
  return m_child;
}

Ptr<MobilityModel> 
HierarchicalMobilityModel::GetParent (void) const
{
  return m_parent;
}

Position 
HierarchicalMobilityModel::DoGet (void) const
{
  Position parentPosition = m_parent->Get ();
  Position childPosition = m_child->Get ();
  return Position (parentPosition.x + childPosition.x,
		   parentPosition.y + childPosition.y,
		   parentPosition.z + childPosition.z);
}
void 
HierarchicalMobilityModel::DoSet (const Position &position)
{
  // This implementation of DoSet is really an arbitraty choice.
  // anything else would have been ok.
  Position parentPosition = m_parent->Get ();
  Position childPosition (position.x - parentPosition.x,
			  position.y - parentPosition.y,
			  position.z - parentPosition.z);
  m_child->Set (childPosition);
}

void 
HierarchicalMobilityModel::ParentChanged (Ptr<const MobilityModel> model)
{
  MobilityModel::NotifyCourseChange ();
}

void 
HierarchicalMobilityModel::ChildChanged (Ptr<const MobilityModel> model)
{
  MobilityModel::NotifyCourseChange ();
}



} // namespace ns3
