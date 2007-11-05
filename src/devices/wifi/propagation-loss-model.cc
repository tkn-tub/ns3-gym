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
#include "propagation-loss-model.h"
#include "ns3/default-value.h"
#include "ns3/random-variable-default-value.h"
#include "ns3/log.h"
#include "ns3/mobility-model.h"
#include "ns3/static-mobility-model.h"
#include <math.h>

NS_LOG_COMPONENT_DEFINE ("PropagationLossModel");

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
("FriisPropagationLossLambda",
 "The wavelength to use by default for every FriisPropagationLossModel (default is 5.15 GHz at 300 000 km/s).",
 300000000.0 / 5.150e9);

static NumericDefaultValue<double> g_friisSystemLoss
("FriisPropagationLossSystemLoss",
 "The system loss to use by default for every FriisPropagationLossModel",
 1.0);
static NumericDefaultValue<double> g_friisPropagationLossMinDistance
("FriisPropagationLossMinDistance",
 "The distance under which the propagation model refuses to give results (m)",
 0.5);

static RandomVariableDefaultValue g_random
("RandomPropagationLossDistribution",
 "The distribution (in dbm) to choose the propagation loss.",
 "Constant:1.0");

static NumericDefaultValue<double> g_pathLossExponent
("PathLossPropagationLossExponent",
 "The exponent of the Path Loss propagation model",
 3.0);

static EnumDefaultValue<enum ModelType> g_pathLossReference
("PathLossPropagationLossReferenceType",
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
					Ptr<MobilityModel> a,
					Ptr<MobilityModel> b) const
{
  double rxPower = txPowerDbm - m_variable->GetValue ();
  NS_LOG_DEBUG ("tx power="<<txPowerDbm<<"dbm, rx power="<<rxPower<<"Dbm");
  return rxPower;
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
				       Ptr<MobilityModel> a,
				       Ptr<MobilityModel> b) const
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
   * Here, we ignore tx and rx gain and the input and output values 
   * are in dbm
   * So, the formula looks like this:
   *
   *              10                lambda^2
   * rx = tx +  ------- * ln (-------------------)
   *            ln (10)        (4 * pi * d)^2 * L
   *
   * rx: rx power (dbm)
   * tx: tx power (dbm)
   * d: distance (m)
   * L: system loss
   * lambda: wavelength (m)
   */
  double distance = a->GetDistanceFrom (b);
  if (distance <= g_friisPropagationLossMinDistance.GetValue ())
    {
      NS_FATAL_ERROR ("The friis propagation loss model is invalid when d="<<
                      distance<<"m << "<<g_friisPropagationLossMinDistance.GetValue ()<<"m");
    }
  double numerator = m_lambda * m_lambda;
  double denominator = 16 * PI * PI * distance * distance * m_systemLoss;
  double pr = log (numerator / denominator) * 10 / log (10);
  double rxPowerDbm = txPowerDbm + pr;
  NS_LOG_DEBUG ("distance="<<distance<<"m, tx power="<<txPowerDbm<<"dbm, rx power="<<rxPowerDbm<<"dbm");
  return rxPowerDbm;
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
					  Ptr<MobilityModel> a,
					  Ptr<MobilityModel> b) const
{
  double distance = a->GetDistanceFrom (b);
  if (distance <= 1.0)
    {
      return txPowerDbm;
    }
  /**
   * The formula is:
   * rx = 10 * ln (Pr0(tx)) / ln (10) + n * 10 * ln (d/d0) / ln (10)
   *
   * Pr0: rx power at reference distance d0 (W)
   * d0: reference distance: 1.0 (m)
   * d: distance (m)
   * tx: tx power (db)
   * rx: db
   *
   * Since we use dbm instead of db, we have:
   *      
   * rx = rx0 + 10 / ln (10) * (n * ln (d/d0) - ln (1000))
   */
  static Ptr<StaticMobilityModel> zero = Create<StaticMobilityModel> (Position (0.0, 0.0, 0.0));
  static Ptr<StaticMobilityModel> one = Create<StaticMobilityModel> (Position (1.0, 1.0, 1.0));
  double rx0 = m_reference->GetRxPower (txPowerDbm, zero, one);
  double rxPowerDbm = rx0 + 10 / log (10) * (m_exponent * log (distance) - log (1000));
  NS_LOG_DEBUG ("distance="<<distance<<"m, tx power="<<txPowerDbm<<"dbm, "<<
		"reference rx power="<<rx0<<"dbm, "<<
		"rx power="<<rxPowerDbm<<"dbm");
  return rxPowerDbm;

}

} // namespace ns3
