#ifndef PROPAGATION_LOSS_MODEL_H
#define PROPAGATION_LOSS_MODEL_H

#include "ns3/object.h"

namespace ns3 {

class RandomVariable;

class PropagationLossModel : public Object
{
public:
  virtual ~PropagationLossModel ();
  virtual double GetRxPower (double txPowerDbm,
			     double distance) const = 0;

  static Ptr<PropagationLossModel> CreateDefault (void);
};

class RandomPropagationLossModel : public PropagationLossModel
{
public:
  RandomPropagationLossModel ();
  RandomPropagationLossModel (const RandomVariable &variable);
  virtual ~RandomPropagationLossModel ();

  virtual double GetRxPower (double txPowerDbm,
			     double distance) const;
private:
  RandomVariable *m_variable;
};

class FriisPropagationLossModel : public PropagationLossModel
{
public:
  FriisPropagationLossModel ();
  void SetLambda (double frequency, double speed);
  void SetLambda (double lambda);
  void SetSystemLoss (double systemLoss);
  double GetLambda (void) const;
  double GetSystemLoss (void) const;

  virtual double GetRxPower (double txPowerDbm,
			     double distance) const;
private:
  double DbmToW (double dbm) const;
  double DbmFromW (double w) const;

  static const double PI;
  double m_lambda;
  double m_systemLoss;
};

class PathLossPropagationLossModel : public PropagationLossModel
{
public:
  PathLossPropagationLossModel ();

  void SetPathLossExponent (double n);
  double GetPathLossExponent (void) const;

  void SetReferenceModel (Ptr<PropagationLossModel> model);
  
  virtual double GetRxPower (double txPowerDbm,
			     double distance) const;
private:
  double DbToW (double db) const;
  static Ptr<PropagationLossModel> CreateDefaultReference (void);

  double m_exponent;
  Ptr<PropagationLossModel> m_reference;
};

} // namespace ns3

#endif /* PROPAGATION_LOSS_MODEL_H */
