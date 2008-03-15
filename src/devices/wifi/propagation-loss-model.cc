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
#include "ns3/log.h"
#include "ns3/mobility-model.h"
#include "ns3/static-mobility-model.h"
#include "ns3/double.h"
#include <math.h>

NS_LOG_COMPONENT_DEFINE ("PropagationLossModel");

namespace ns3 {


const double FriisPropagationLossModel::PI = 3.1415;

PropagationLossModel::~PropagationLossModel ()
{}

TypeId 
RandomPropagationLossModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("RandomPropagationLossModel")
    .SetParent<PropagationLossModel> ()
    .AddConstructor<RandomPropagationLossModel> ()
    .AddAttribute ("Variable", "XXX",
                   ConstantVariable (1.0),
                   MakeRandomVariableAccessor (&RandomPropagationLossModel::m_variable),
                   MakeRandomVariableChecker ())
    ;
  return tid;
}
RandomPropagationLossModel::RandomPropagationLossModel ()
{}

RandomPropagationLossModel::~RandomPropagationLossModel ()
{}

double 
RandomPropagationLossModel::GetRxPower (double txPowerDbm,
					Ptr<MobilityModel> a,
					Ptr<MobilityModel> b) const
{
  double rxPower = txPowerDbm - m_variable.GetValue ();
  NS_LOG_DEBUG ("tx power="<<txPowerDbm<<"dbm, rx power="<<rxPower<<"Dbm");
  return rxPower;
}

TypeId 
FriisPropagationLossModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("FriisPropagationLossModel")
    .SetParent<PropagationLossModel> ()
    .AddConstructor<FriisPropagationLossModel> ()
    .AddAttribute ("Lambda", 
                   "The wavelength  (default is 5.15 GHz at 300 000 km/s).",
                   Double (300000000.0 / 5.150e9),
                   MakeDoubleAccessor (&FriisPropagationLossModel::m_lambda),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("SystemLoss", "The system loss",
                   Double (1.0),
                   MakeDoubleAccessor (&FriisPropagationLossModel::m_systemLoss),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MinDistance", 
                   "The distance under which the propagation model refuses to give results (m)",
                   Double (0.5),
                   MakeDoubleAccessor (&FriisPropagationLossModel::m_minDistance),
                   MakeDoubleChecker<double> ())
    ;
  return tid;
}

FriisPropagationLossModel::FriisPropagationLossModel ()
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
   *           Gt * Gr * (lambda^2)
   *   P = Pt ---------------------
   *           (4 * pi * d)^2 * L
   *
   * Gt: tx gain (unit-less)
   * Gr: rx gain (unit-less)
   * Pt: tx power (W)
   * d: distance (m)
   * L: system loss
   * lambda: wavelength (m)
   *
   * Here, we ignore tx and rx gain and the input and output values 
   * are in dbm:
   *
   *                           lambda^2
   * rx = tx +  10 log10 (-------------------)
   *                       (4 * pi * d)^2 * L
   *
   * rx: rx power (dbm)
   * tx: tx power (dbm)
   * d: distance (m)
   * L: system loss
   * lambda: wavelength (m)
   */
  double distance = a->GetDistanceFrom (b);
  if (distance <= m_minDistance)
    {
      return txPowerDbm;
    }
  double numerator = m_lambda * m_lambda;
  double denominator = 16 * PI * PI * distance * distance * m_systemLoss;
  double pr = 10 * log10 (numerator / denominator);
  double rxPowerDbm = txPowerDbm + pr;
  NS_LOG_DEBUG ("distance="<<distance<<"m, tx power="<<txPowerDbm<<"dbm, rx power="<<rxPowerDbm<<"dbm");
  return rxPowerDbm;
}

TypeId
LogDistancePropagationLossModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("LogDistancePropagationLossModel")
    .SetParent<PropagationLossModel> ()
    .AddConstructor<LogDistancePropagationLossModel> ()
    .AddAttribute ("Exponent",
                   "The exponent of the Path Loss propagation model",
                   Double (3.0),
                   MakeDoubleAccessor (&LogDistancePropagationLossModel::m_exponent),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("ReferenceDistance",
                   "The distance at which the reference loss is calculated (m)",
                   Double (1.0),
                   MakeDoubleAccessor (&LogDistancePropagationLossModel::m_referenceDistance),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("ReferenceModel",
                   "The reference model at the reference distance.",
                   Ptr<PropagationLossModel> (0),
                   MakePtrAccessor (&LogDistancePropagationLossModel::m_reference),
                   MakePtrChecker<PropagationLossModel> ())
    ;
  return tid;
                   
}

LogDistancePropagationLossModel::LogDistancePropagationLossModel ()
{}

void 
LogDistancePropagationLossModel::SetPathLossExponent (double n)
{
  m_exponent = n;
}
void 
LogDistancePropagationLossModel::SetReferenceDistance (double referenceDistance)
{
  m_referenceDistance = referenceDistance;
}
void 
LogDistancePropagationLossModel::SetReferenceModel (Ptr<PropagationLossModel> model)
{
  m_reference = model;
}
double 
LogDistancePropagationLossModel::GetPathLossExponent (void) const
{
  return m_exponent;
}
  
double 
LogDistancePropagationLossModel::GetRxPower (double txPowerDbm,
                                             Ptr<MobilityModel> a,
                                             Ptr<MobilityModel> b) const
{
  double distance = a->GetDistanceFrom (b);
  if (distance <= m_referenceDistance)
    {
      return txPowerDbm;
    }
  /**
   * The formula is:
   * rx = 10 * log (Pr0(tx)) - n * 10 * log (d/d0)
   *
   * Pr0: rx power at reference distance d0 (W)
   * d0: reference distance: 1.0 (m)
   * d: distance (m)
   * tx: tx power (db)
   * rx: db
   *
   * Which, in our case is:
   *      
   * rx = rx0(tx) - 10 * n * log (d/d0)
   */
  static Ptr<StaticMobilityModel> zero = 
    CreateObject<StaticMobilityModel> ("Position", 
                                       Vector (0.0, 0.0, 0.0));
  static Ptr<StaticMobilityModel> reference = 
    CreateObject<StaticMobilityModel> ("Position", 
                                       Vector (m_referenceDistance, 0.0, 0.0));
  double rx0 = m_reference->GetRxPower (txPowerDbm, zero, reference);
  double pathLossDb = 10 * m_exponent * log10 (distance / m_referenceDistance);
  double rxPowerDbm = rx0 - pathLossDb;
  NS_LOG_DEBUG ("distance="<<distance<<"m, tx-power="<<txPowerDbm<<"dbm, "<<
		"reference-rx-power="<<rx0<<"dbm, "<<
		"rx-power="<<rxPowerDbm<<"dbm");
  return rxPowerDbm;
}

} // namespace ns3
