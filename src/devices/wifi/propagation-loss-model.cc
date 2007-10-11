#include "propagation-loss-model.h"
#include "ns3/default-value.h"
#include "ns3/random-variable-default-value.h"
#include <math.h>

namespace ns3 {

enum ModelType {
  FRIIS,
  RANDOM,
  PATH_LOSS
};

static EnumDefaultValue<enum ModelType> g_modelType
("PropagationLossModelType",
 "The type of propagation loss model.",
 PATH_LOSS, "PathLoss",
 FRIIS, "Friis",
 RANDOM, "Random",
 0, (void *)0);

static NumericDefaultValue<double> g_friisLambda
("FriisPropagationLambda",
 "The wavelength to use by default for every FriisPropagationLossModel (default is 5.15 GHz at 300 000 km/s).",
 300000000.0 / 5.150e9);

static NumericDefaultValue<double> g_friisSystemLoss
("FriisPropagationSystemLoss",
 "The system loss to use by default for every FriisPropagationLossModel",
 1.0);

static RandomVariableDefaultValue g_random
("RandomPropagation",
 "The distribution (in dbm) to choose the propagation loss.",
 "Constant:1.0");

static NumericDefaultValue<double> g_pathLossExponent
("PathLossPropagationExponent",
 "The exponent of the Path Loss propagation model",
 3.0);

static EnumDefaultValue<enum ModelType> g_pathLossReference
("PathLossPropagationReferenceType",
 "The type of reference propagation model.",
 FRIIS, "Friis",
 RANDOM, "Random", 
 0, (void *)0);

const double FriisPropagationLossModel::PI = 3.1415;

PropagationLossModel::~PropagationLossModel ()
{}

Ptr<PropagationLossModel> 
PropagationLossModel::CreateDefault (void)
{
  switch (g_modelType.GetValue ()) {
  case FRIIS:
    return Create<FriisPropagationLossModel> ();
    break;
  case RANDOM:
    return Create<RandomPropagationLossModel> ();
    break;
  case PATH_LOSS:
    return Create<PathLossPropagationLossModel> ();
    break;
  default:
    NS_ASSERT (false);
    return 0;
    break;
  }
}

RandomPropagationLossModel::RandomPropagationLossModel ()
  : m_variable (g_random.GetCopy ())
{}

RandomPropagationLossModel::RandomPropagationLossModel (const RandomVariable &variable)
  : m_variable (variable.Copy ())
{}
RandomPropagationLossModel::~RandomPropagationLossModel ()
{
  delete m_variable;
}

double 
RandomPropagationLossModel::GetRxPower (double txPowerDbm,
					double distance) const
{
  return txPowerDbm - m_variable->GetValue ();
}

FriisPropagationLossModel::FriisPropagationLossModel ()
  : m_lambda (g_friisLambda.GetValue ()),
    m_systemLoss (g_friisSystemLoss.GetValue ())
{}
void 
FriisPropagationLossModel::SetSystemLoss (double systemLoss)
{
  m_systemLoss = systemLoss;
}
double 
FriisPropagationLossModel::GetSystemLoss (void) const
{
  return m_systemLoss;
}
void 
FriisPropagationLossModel::SetLambda (double frequency, double speed)
{
  m_lambda = speed / frequency;
}
void 
FriisPropagationLossModel::SetLambda (double lambda)
{
  m_lambda = lambda;
}
double 
FriisPropagationLossModel::GetLambda (void) const
{
  return m_lambda;
}

double 
FriisPropagationLossModel::DbmToW (double dbm) const
{
  double mw = pow(10.0,dbm/10.0);
  return mw / 1000.0;
}

double
FriisPropagationLossModel::DbmFromW (double w) const
{
  double dbm = log10 (w * 1000.0) * 10.0;
  return dbm;
}


double 
FriisPropagationLossModel::GetRxPower (double txPowerDbm,
				       double distance) const
{
  /*
   * Friis free space equation:
   * where Pt, Gr, Gr and P are in Watt units
   * L is in meter units.
   *
   *       Pt * Gt * Gr * (lambda^2)
   *   P = --------------------------
   *       (4 * pi * d)^2 * L
   *
   * Gt: tx gain (W)
   * Gr: rx gain (W)
   * Pt: tx power (W)
   * d: distance (m)
   * L: system loss
   * lambda: wavelength (m)
   *
   * Here, we ignore tx and rx gain.
   */
  double numerator = DbmToW (txPowerDbm) * m_lambda * m_lambda;
  double denominator = 16 * PI * PI * distance * distance * m_systemLoss;
  double pr = numerator / denominator;
  return DbmFromW (pr);
}


PathLossPropagationLossModel::PathLossPropagationLossModel ()
  : m_exponent (g_pathLossExponent.GetValue ()),
    m_reference (CreateDefaultReference ())
{}

void 
PathLossPropagationLossModel::SetPathLossExponent (double n)
{
  m_exponent = n;
}
void 
PathLossPropagationLossModel::SetReferenceModel (Ptr<PropagationLossModel> model)
{
  m_reference = model;
}
double 
PathLossPropagationLossModel::GetPathLossExponent (void) const
{
  return m_exponent;
}

Ptr<PropagationLossModel>
PathLossPropagationLossModel::CreateDefaultReference (void)
{
  switch (g_pathLossReference.GetValue ()) {
  case RANDOM:
    return Create<RandomPropagationLossModel> ();
    break;
  case FRIIS:
    return Create<FriisPropagationLossModel> ();
    break;
  case PATH_LOSS:
  default:
    NS_ASSERT (false);
    return 0;
    break;
  }
}

double
PathLossPropagationLossModel::DbToW (double db) const
{
  return pow(10.0,db/10.0);
}

  
double 
PathLossPropagationLossModel::GetRxPower (double txPowerDbm,
					  double distance) const
{
  if (distance <= 1.0)
    {
      return txPowerDbm;
    }
  double prd0 = m_reference->GetRxPower (txPowerDbm, 1.0);
  double pr = 10*log10(prd0) - m_exponent * 10.0 * log10(distance);
  return DbToW (pr);

}


} // namespace ns3
