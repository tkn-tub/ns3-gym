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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "propagation-delay-model.h"
#include "ns3/random-variable.h"
#include "ns3/default-value.h"
#include "ns3/random-variable-default-value.h"
#include "ns3/mobility-model.h"

namespace ns3 {

enum ModelType
{
  RANDOM,
  CONSTANT_SPEED
};

static EnumDefaultValue<enum ModelType> g_modelType
("PropagationDelayModelType",
 "The type of propagation delay model to use.",
 CONSTANT_SPEED, "ConstantSpeed",
 RANDOM, "Random",
 0, (void*)0);

static NumericDefaultValue<double> g_speed
("PropagationDelayConstantSpeed",
 "The speed (m/s) of propagation if a ConstantSpeed propagation delay model is used.",
 300000000.0);

static RandomVariableDefaultValue g_random
("PropagationDelayRandomDistribution", 
 "The delay distribution to use if a Random propagation delay model is used.",
 "Uniform:0:1.0");


PropagationDelayModel::~PropagationDelayModel ()
{}
Ptr<PropagationDelayModel> 
PropagationDelayModel::CreateDefault (void)
{
  switch (g_modelType.GetValue ()) {
  case CONSTANT_SPEED:
    return CreateObject<ConstantSpeedPropagationDelayModel> (g_speed.GetValue ());
    break;
  case RANDOM:
    return CreateObject<RandomPropagationDelayModel> ();
    break;
  default:
    NS_ASSERT (false);
    return 0;
    break;
  }
}

RandomPropagationDelayModel::RandomPropagationDelayModel ()
  : m_variable (g_random.Get ())
{}

RandomPropagationDelayModel::RandomPropagationDelayModel (const RandomVariable &variable)
  : m_variable (variable)
{}
RandomPropagationDelayModel::~RandomPropagationDelayModel ()
{}
Time 
RandomPropagationDelayModel::GetDelay (Ptr<MobilityModel> a, Ptr<MobilityModel> b) const
{
  return Seconds (m_variable.GetValue ());
}

ConstantSpeedPropagationDelayModel::ConstantSpeedPropagationDelayModel (double speed)
  : m_speed (speed)
{}
Time 
ConstantSpeedPropagationDelayModel::GetDelay (Ptr<MobilityModel> a, Ptr<MobilityModel> b) const
{
  double distance = a->GetDistanceFrom (b);
  double seconds = distance / m_speed;
  return Seconds (seconds);
}
void 
ConstantSpeedPropagationDelayModel::SetSpeed (double speed)
{
  m_speed = speed;
}
double 
ConstantSpeedPropagationDelayModel::GetSpeed (void) const
{
  return m_speed;
}

} // namespace ns3
