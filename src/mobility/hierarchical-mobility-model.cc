/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
#include "ns3/pointer.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (HierarchicalMobilityModel);

TypeId 
HierarchicalMobilityModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::HierarchicalMobilityModel")
    .SetParent<MobilityModel> ()
    .AddConstructor<HierarchicalMobilityModel> ()
    .AddAttribute ("Child", "The child mobility model.",
                   PointerValue (),
                   MakePointerAccessor (&HierarchicalMobilityModel::SetChild,
                                        &HierarchicalMobilityModel::GetChild),
                   MakePointerChecker<MobilityModel> ())
    .AddAttribute ("Parent", "The parent mobility model.",
                   PointerValue (),
                   MakePointerAccessor (&HierarchicalMobilityModel::SetParent,
                                        &HierarchicalMobilityModel::GetParent),
                   MakePointerChecker<MobilityModel> ())
    ;
  return tid;
}

HierarchicalMobilityModel::HierarchicalMobilityModel ()
  : m_child (0),
    m_parent (0)
{}

void 
HierarchicalMobilityModel::SetChild (Ptr<MobilityModel> model)
{
  m_child = model;
  Ptr<MobilityModelNotifier> notifier = 
    m_child->GetObject<MobilityModelNotifier> ();
  if (notifier == 0)
    {
      notifier = CreateObject<MobilityModelNotifier> ();
      m_child->AggregateObject (notifier);
    }
  notifier->TraceConnectWithoutContext ("CourseChange", MakeCallback (&HierarchicalMobilityModel::ChildChanged, this));
}

void 
HierarchicalMobilityModel::SetParent (Ptr<MobilityModel> model)
{
  m_parent = model;
  Ptr<MobilityModelNotifier> notifier = 
    m_parent->GetObject<MobilityModelNotifier> ();
  if (notifier == 0)
    {
      notifier = CreateObject<MobilityModelNotifier> ();
      m_parent->AggregateObject (notifier);
    }
  notifier->TraceConnectWithoutContext ("CourseChange", MakeCallback (&HierarchicalMobilityModel::ParentChanged, this));
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

Vector
HierarchicalMobilityModel::DoGetPosition (void) const
{
  Vector parentPosition = m_parent->GetPosition ();
  Vector childPosition = m_child->GetPosition ();
  return Vector (parentPosition.x + childPosition.x,
		   parentPosition.y + childPosition.y,
		   parentPosition.z + childPosition.z);
}
void 
HierarchicalMobilityModel::DoSetPosition (const Vector &position)
{
  if (m_parent == 0 || m_child == 0)
    {
      return;
    }
  // This implementation of DoSetPosition is really an arbitraty choice.
  // anything else would have been ok.
  Vector parentPosition = m_parent->GetPosition ();
  Vector childPosition (position.x - parentPosition.x,
			  position.y - parentPosition.y,
			  position.z - parentPosition.z);
  m_child->SetPosition (childPosition);
}
Vector
HierarchicalMobilityModel::DoGetVelocity (void) const
{
  Vector parentSpeed = m_parent->GetVelocity ();
  Vector childSpeed = m_child->GetVelocity ();
  Vector speed (parentSpeed.x + childSpeed.x,
               parentSpeed.y + childSpeed.y,
               parentSpeed.z + childSpeed.z);
  return speed;
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
