#ifndef PROPAGATION_DELAY_MODEL_H
#define PROPAGATION_DELAY_MODEL_H

#include "ns3/ptr.h"
#include "ns3/object.h"
#include "ns3/nstime.h"

namespace ns3 {

class RandomVariable;

class PropagationDelayModel : public Object
{
public:
  virtual ~PropagationDelayModel ();
  virtual Time GetDelay (double distance) const = 0;
};

class RandomPropagationDelayModel : public PropagationDelayModel
{
public:
  RandomPropagationDelayModel (const RandomVariable &variable);
  virtual ~RandomPropagationDelayModel ();
  virtual Time GetDelay (double distance) const;
private:
  RandomVariable *m_variable;
};

class ConstantSpeedPropagationDelayModel : public PropagationDelayModel
{
public:
  ConstantSpeedPropagationDelayModel ();
  ConstantSpeedPropagationDelayModel (double speed);
  virtual Time GetDelay (double distance) const;
  void SetSpeed (double speed);
  double GetSpeed (void) const;
private:
  double m_speed;
};

} // namespace ns3

#endif /* PROPAGATION_DELAY_MODEL_H */
