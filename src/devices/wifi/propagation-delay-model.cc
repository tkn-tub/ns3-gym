#include "propagation-delay-model.h"
#include "ns3/random-variable.h"

namespace ns3 {

PropagationDelayModel::~PropagationDelayModel ()
{}

RandomPropagationDelayModel::RandomPropagationDelayModel (const RandomVariable &variable)
  : m_variable (variable.Copy ())
{}
RandomPropagationDelayModel::~RandomPropagationDelayModel ()
{
  delete m_variable;
}
Time 
RandomPropagationDelayModel::GetDelay (double distance) const
{
  return Seconds (m_variable->GetValue ());
}

ConstantSpeedPropagationDelayModel::ConstantSpeedPropagationDelayModel ()
  : m_speed (300000000.0)
{}
ConstantSpeedPropagationDelayModel::ConstantSpeedPropagationDelayModel (double speed)
  : m_speed (speed)
{}
Time 
ConstantSpeedPropagationDelayModel::GetDelay (double distance) const
{
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
