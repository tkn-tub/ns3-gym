#ifndef PROPAGATION_DELAY_MODEL_H
#define PROPAGATION_DELAY_MODEL_H

#include "ns3/ptr.h"
#include "ns3/object.h"
#include "ns3/nstime.h"

namespace ns3 {

class MobilityModel;

class RandomVariable;

class PropagationDelayModel : public Object
{
public:
  virtual ~PropagationDelayModel ();
  virtual Time GetDelay (Ptr<MobilityModel> a, Ptr<MobilityModel> b) const = 0;
  static Ptr<PropagationDelayModel> CreateDefault (void);
};

class RandomPropagationDelayModel : public PropagationDelayModel
{
public:
  RandomPropagationDelayModel ();
  RandomPropagationDelayModel (const RandomVariable &variable);
  virtual ~RandomPropagationDelayModel ();
  virtual Time GetDelay (Ptr<MobilityModel> a, Ptr<MobilityModel> b) const;
private:
  RandomVariable *m_variable;
};

class ConstantSpeedPropagationDelayModel : public PropagationDelayModel
{
public:
  ConstantSpeedPropagationDelayModel (double speed);
  virtual Time GetDelay (Ptr<MobilityModel> a, Ptr<MobilityModel> b) const;
  void SetSpeed (double speed);
  double GetSpeed (void) const;
private:
  double m_speed;
};

} // namespace ns3

#endif /* PROPAGATION_DELAY_MODEL_H */
