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
    return Create<ConstantSpeedPropagationDelayModel> (g_speed.GetValue ());
    break;
  case RANDOM:
    return Create<RandomPropagationDelayModel> ();
    break;
  default:
    NS_ASSERT (false);
    return 0;
    break;
  }
}

RandomPropagationDelayModel::RandomPropagationDelayModel ()
  : m_variable (g_random.GetCopy ())
{}

RandomPropagationDelayModel::RandomPropagationDelayModel (const RandomVariable &variable)
  : m_variable (variable.Copy ())
{}
RandomPropagationDelayModel::~RandomPropagationDelayModel ()
{
  delete m_variable;
}
Time 
RandomPropagationDelayModel::GetDelay (Ptr<MobilityModel> a, Ptr<MobilityModel> b) const
{
  return Seconds (m_variable->GetValue ());
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
