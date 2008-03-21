/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006,2007 INRIA
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
 * Author: Federico Maguolo <maguolof@dei.unipd.it>
 */

#include "ns3/simulator.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"
#include "ns3/random-variable.h"
#include "ns3/mobility-model.h"
#include "composite-propagation-loss-model.h"
#include <math.h>

namespace ns3 {



NS_OBJECT_ENSURE_REGISTERED (CompositePropagationLossModel);

TypeId
CompositePropagationLossModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CompositePropagationLossModel")
    .SetParent<PropagationLossModel> ()
    .AddConstructor<CompositePropagationLossModel> ()
    ;
  return tid;
}

CompositePropagationLossModel::CompositePropagationLossModel ()
{
  AddDefaults ();
}

CompositePropagationLossModel::~CompositePropagationLossModel ()
{}

void
CompositePropagationLossModel::AddDefaults ()
{}

void
CompositePropagationLossModel::AddPropagationLossModel (Ptr<PropagationLossModel> pl)
{
  m_propagationModels.push_back (pl);
}

double
CompositePropagationLossModel::GetLoss (Ptr<MobilityModel> a,
				        Ptr<MobilityModel> b) const
{
  double rxc = 0.0;
  for(PropagationModelList::const_iterator i = m_propagationModels.begin (); 
                                           i != m_propagationModels.end (); 
					   i++) {
    rxc += (*i)->GetLoss (a, b);
  }
  
  return rxc;
}

} // namespace ns3

