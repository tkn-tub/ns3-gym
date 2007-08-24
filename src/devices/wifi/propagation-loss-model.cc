#include "propagation-loss-model.h"
#include <math.h>

namespace ns3 {

const double FriisPropagationLossModel::PI = 3.1415;

PropagationLossModel::~PropagationLossModel ()
{}

FriisPropagationLossModel::FriisPropagationLossModel ()
  : m_lambda (),
    m_systemLoss (1.0)
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
{}

void 
PathLossPropagationLossModel::SetPathLossExponent (double n)
{
  m_exponent = n;
}
double 
PathLossPropagationLossModel::GetPathLossExponent (void) const
{
  return m_exponent;
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
