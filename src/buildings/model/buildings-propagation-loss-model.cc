/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Marco Miozzo  <marco.miozzo@cttc.es>,
 *         Nicola Baldo <nbaldo@cttc.es>
 * 
 */

#include "ns3/propagation-loss-model.h"
#include "ns3/log.h"
#include "ns3/mobility-model.h"
#include "ns3/double.h"
#include "ns3/pointer.h"
#include <math.h>
#include "buildings-propagation-loss-model.h"
#include "ns3/buildings-mobility-model.h"
#include "ns3/enum.h"


NS_LOG_COMPONENT_DEFINE ("BuildingsPropagationLossModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (BuildingsPropagationLossModel);


BuildingsPropagationLossModel::ShadowingLoss::ShadowingLoss ()
{
}


BuildingsPropagationLossModel::ShadowingLoss::ShadowingLoss (double mean, double sigma, Ptr<MobilityModel> receiver)
  : m_receiver (receiver),
    m_randVariable (mean, sigma * sigma) // NormalVariable class wants mean and variance (sigma is a standard deviation)
{
  m_shadowingValue = m_randVariable.GetValue ();
  NS_LOG_INFO (this << " New Shadowing: sigma " << sigma << " value " << m_shadowingValue);
}

double
BuildingsPropagationLossModel::ShadowingLoss::GetLoss () const
{
  return (m_shadowingValue);
}

Ptr<MobilityModel>
BuildingsPropagationLossModel::ShadowingLoss::GetReceiver () const
{
  return m_receiver;
}

TypeId
BuildingsPropagationLossModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::BuildingsPropagationLossModel")

    .SetParent<PropagationLossModel> ()

    .AddAttribute ("Lambda",
                   "The wavelength  (default is 2.106 GHz at 300 000 km/s).",
                   DoubleValue (300000000.0 / 2160e6),
                   MakeDoubleAccessor (&BuildingsPropagationLossModel::SetLambda, &BuildingsPropagationLossModel::GetLambda),
                   MakeDoubleChecker<double> ())

    .AddAttribute ("Frequency",
                   "The Frequency  (default is 2.106 GHz).",
                   DoubleValue (2160e6),
                   MakeDoubleAccessor (&BuildingsPropagationLossModel::SetFrequency),
                   MakeDoubleChecker<double> ())

    .AddAttribute ("ShadowSigmaOutdoor",
                   "Standard deviation of the normal distribution used for calculate the shadowing for outdoor nodes",
                   DoubleValue (7.0),
                   MakeDoubleAccessor (&BuildingsPropagationLossModel::m_shadowingSigmaOutdoor),
                   MakeDoubleChecker<double> ())

    .AddAttribute ("ShadowSigmaIndoor",
                   "Standard deviation of the normal distribution used for calculate the shadowing for indoor nodes ",
                   DoubleValue (8.0),
                   MakeDoubleAccessor (&BuildingsPropagationLossModel::m_shadowingSigmaIndoor),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("ShadowSigmaExtWalls",
                   "Standard deviation of the normal distribution used for calculate the shadowing due to ext walls ",
                   DoubleValue (5.0),
                   MakeDoubleAccessor (&BuildingsPropagationLossModel::m_shadowingSigmaExtWalls),
                   MakeDoubleChecker<double> ())

    .AddAttribute ("RooftopLevel",
                   " The height of the rooftop [m].",
                   DoubleValue (20.0),
                   MakeDoubleAccessor (&BuildingsPropagationLossModel::m_rooftopHeight),
                   MakeDoubleChecker<double> ())

    .AddAttribute ("Los2NlosThr",
                   " Threshold from LoS to NLoS in ITU 1411 [m].",
                   DoubleValue (200.0),
                   MakeDoubleAccessor (&BuildingsPropagationLossModel::m_itu1411NlosThreshold),
                   MakeDoubleChecker<double> ())

    .AddAttribute ("MinDistance",
                   "The distance under which the propagation model refuses to give results (m) ",
                   DoubleValue (0.5),
                   MakeDoubleAccessor (&BuildingsPropagationLossModel::SetMinDistance, &BuildingsPropagationLossModel::GetMinDistance),
                   MakeDoubleChecker<double> ())

    .AddAttribute ("Environment",
                   "Environment Scenario",
                   EnumValue (BuildingsPropagationLossModel::Urban),
                   MakeEnumAccessor (&BuildingsPropagationLossModel::SetEnvironment,
                                     &BuildingsPropagationLossModel::GetEnvironment),
                   MakeEnumChecker (BuildingsPropagationLossModel::Urban, "Urban",
                                    BuildingsPropagationLossModel::SubUrban, "SubUrban",
                                    BuildingsPropagationLossModel::OpenAreas, "OpenAreas"))

    .AddAttribute ("CitySize",
                   "Dimension of the city",
                   EnumValue (BuildingsPropagationLossModel::Large),
                   MakeEnumAccessor (&BuildingsPropagationLossModel::SetCitySize),
                   MakeEnumChecker (BuildingsPropagationLossModel::Small, "Small",
                                    BuildingsPropagationLossModel::Medium, "Medium",
                                    BuildingsPropagationLossModel::Large, "Large"))
    .AddAttribute ("InternalWallLoss",
                   "Additional loss for each internal wall [dB]",
                   DoubleValue (5.0),
                   MakeDoubleAccessor (&BuildingsPropagationLossModel::m_lossInternalWall),
                   MakeDoubleChecker<double> ());


  return tid;
}

BuildingsPropagationLossModel::BuildingsPropagationLossModel ()
  : C (0),
    m_environment (Urban),
    m_citySize (Large),
    m_streetsOrientation (45.0),
    m_streetsWidth (20.0),
    m_buildingsExtend (80.0),
    m_buildingSeparation (50.0)
{
}

BuildingsPropagationLossModel::~BuildingsPropagationLossModel ()
{
}

void
BuildingsPropagationLossModel::SetLambda (double lambda)
{
  m_lambda = lambda;
  m_frequency = 300000000 / lambda;
}

void
BuildingsPropagationLossModel::SetFrequency (double freq)
{
  m_frequency = freq;
  m_lambda = 300000000 / m_frequency;
}

double
BuildingsPropagationLossModel::GetLambda (void) const
{
  return m_lambda;
}

double
BuildingsPropagationLossModel::GetFrequency (void) const
{
  return m_frequency;
}

void
BuildingsPropagationLossModel::SetMinDistance (double minDistance)
{
  m_minDistance = minDistance;
}
double
BuildingsPropagationLossModel::GetMinDistance (void) const
{
  return m_minDistance;
}

void
BuildingsPropagationLossModel::SetEnvironment (Environment env)
{
  m_environment = env;
}

BuildingsPropagationLossModel::Environment
BuildingsPropagationLossModel::GetEnvironment (void) const
{
  return m_environment;
}

void
BuildingsPropagationLossModel::SetCitySize (CitySize size)
{
  m_citySize = size;
}

BuildingsPropagationLossModel::CitySize
BuildingsPropagationLossModel::GetCitySize (void) const
{
  return m_citySize;
}



double
BuildingsPropagationLossModel::OkumuraHata (Ptr<BuildingsMobilityModel> a, Ptr<BuildingsMobilityModel> b) const
{
  // Hp: a is the rooftop antenna (from GetLoss logic)
  double loss = 0.0;
  double fmhz = m_frequency / 1e6;
  double dist = a->GetDistanceFrom (b) / 1000.0; 
  if (m_frequency <= 1.500e9)
    {
      // standard Okumura Hata (from wikipedia)
      double log_f = log10 (fmhz);
      double hb = (a->GetPosition ().z > b->GetPosition ().z ? a->GetPosition ().z : b->GetPosition ().z);
      double hm = (a->GetPosition ().z < b->GetPosition ().z ? a->GetPosition ().z : b->GetPosition ().z);
      NS_ASSERT_MSG (hb > 0 && hm > 0, "nodes' height must be greater then 0");
      double log_aHeight = 13.82 * log10 (hb);
      double log_bHeight = 0.0;
      if (m_citySize == Large)
        {
          if (m_frequency < 200)
            {
              log_bHeight = 8.29 * pow (log10 (1.54 * hm), 2) -  1.1;
            }
          else
            {
              log_bHeight = 3.2 * pow (log10 (11.75 * hm), 2) - 4.97;
            }
        }
      else
        {
          log_bHeight = 0.8 + (1.1 * log_f - 0.7) * hm - 1.56 * log_f;
        }

//       NS_LOG_INFO (this << " logf " << 26.16 * log_f << " loga " << log_aHeight << " X " << (((44.9 - (6.55 * log10(hb)) ))*log10 (a->GetDistanceFrom (b))) << " logb " << log_bHeight);
      loss = 69.55 + (26.16 * log_f) - log_aHeight + (((44.9 - (6.55 * log10 (hb)) )) * log10 (dist)) - log_bHeight;
      if (m_environment == SubUrban)
        {
          loss += -2 * (pow (log10 (fmhz / 28), 2)) - 5.4;
        }
      else if (m_environment == OpenAreas)
        {
          loss += -4.70 * pow (log10 (fmhz),2) + 18.33 * log10 (fmhz) - 40.94;
        }

    }
  else if (m_frequency <= 2.170e9) // max 3GPP freq EUTRA band #1
    {
      // COST 231 Okumura model
      double log_f = log10 (fmhz);
      double hb = (a->GetPosition ().z > b->GetPosition ().z ? a->GetPosition ().z : b->GetPosition ().z);
      double hm = (a->GetPosition ().z < b->GetPosition ().z ? a->GetPosition ().z : b->GetPosition ().z);
      NS_ASSERT_MSG (hb > 0 && hm > 0, "nodes' height must be greater then 0");
      double log_aHeight = 13.82 * log10 (hb);
      double log_bHeight = 0.0;
      double C = 0.0;

      if (m_citySize == Large)
        {
          log_bHeight = 3.2 * pow ((log10 (11.75 * hm)),2);
          C = 3;
        }
      else
        {
          log_bHeight = 1.1 * log_f - 0.7 * hm - (1.56 * log_f - 0.8);
        }

      loss = 46.3 + (33.9 * log_f) - log_aHeight + (((44.9 - (6.55 * log10 (hb)) )) * log10 (dist)) - log_bHeight + C;
    }
  else if (m_frequency <= 2.690e9) // max 3GPP freq EUTRA band #1
    {
      // Empirical model from
      // "Path Loss Models for Suburban Scenario at 2.3GHz, 2.6GHz and 3.5GHz"
      // Sun Kun, Wang Ping, Li Yingze
      // Antennas, Propagation and EM Theory, 2008. ISAPE 2008. 8th International Symposium on 
      loss = 36 + 26 * log10 (dist * 1000);
    }

  return (loss);
}



double
BuildingsPropagationLossModel::ItuR1411 (Ptr<BuildingsMobilityModel> a, Ptr<BuildingsMobilityModel> b) const
{
  if (a->GetDistanceFrom (b) < m_itu1411NlosThreshold)
    {
      return (ItuR1411Los (a,b));
    }
  else
    {
      return (ItuR1411NlosOverRooftop (a,b));
    }
}


double
BuildingsPropagationLossModel::ItuR1411Los (Ptr<BuildingsMobilityModel> a, Ptr<BuildingsMobilityModel> b) const
{
  NS_LOG_INFO (this);
  double dist = a->GetDistanceFrom (b);
  double lossLow = 0.0;
  double lossUp = 0.0;
  double pi = 3.141592653589793;
  NS_ASSERT_MSG (a->GetPosition ().z > 0 && b->GetPosition ().z > 0, "nodes' height must be greater then 0");
  double Lbp = fabs (20 * log10 ((m_lambda * m_lambda) / (8 * pi * a->GetPosition ().z * b->GetPosition ().z)));
  double Rbp = (4 * a->GetPosition ().z * b->GetPosition ().z) / m_lambda;
//   NS_LOG_INFO (this << " Lbp " << Lbp << " Rbp " << Rbp << " lambda " << m_lambda);
  if (dist <= Rbp)
    {
      lossLow = Lbp + 20 * log10 (dist / Rbp);
      lossUp = Lbp + 20 + 25 * log10 (dist / Rbp);
    }
  else
    {
      lossLow = Lbp + 40 * log10 (dist / Rbp);
      lossUp = Lbp + 20 + 40 * log10 (dist / Rbp);
    }

  double loss = (lossUp + lossLow) / 2;

  return (loss);
}


double
BuildingsPropagationLossModel::ItuR1411NlosOverRooftop (Ptr<BuildingsMobilityModel> a, Ptr<BuildingsMobilityModel> b) const
{
  NS_LOG_INFO (this);
  double Lori = 0.0;
  double fmhz = m_frequency / 1e6;
  if ((m_streetsOrientation >= 0)&&(m_streetsOrientation < 35))
    {
      Lori = -10.0 + 0.354 * m_streetsOrientation;
    }
  else if ((m_streetsOrientation >= 35)&&(m_streetsOrientation < 55))
    {
      Lori = 2.5 + 0.075 * (m_streetsOrientation - 35);
    }
  else if ((m_streetsOrientation >= 55)&&(m_streetsOrientation < 90))
    {
      Lori = 2.5 + 0.075 * (m_streetsOrientation - 55);
    }
  else
    {
      NS_LOG_ERROR (this << " Street Orientation must be in [0,90]");
    }
  double distance = a->GetDistanceFrom (b);
  double hb = (a->GetPosition ().z > b->GetPosition ().z ? a->GetPosition ().z : b->GetPosition ().z);
  double hm = (a->GetPosition ().z < b->GetPosition ().z ? a->GetPosition ().z : b->GetPosition ().z);
  NS_ASSERT_MSG (hm > 0 && hb > 0, "nodes' height must be greater then 0");
  double Dhb = hb - m_rooftopHeight;
  double ds = (m_lambda * distance * distance) / (Dhb * Dhb);
  double Lmsd = 0.0;
  double pi = 3.141592653589793;
//   NS_LOG_INFO (this << " build " << m_buildingsExtend << " ds " << ds << " roof " << m_rooftopHeight << " hb " << hb << " lambda " << m_lambda);
  if (ds < m_buildingsExtend)
    {
      double Lbsh = 0.0;
      double ka = 0.0;
      double kd = 0.0;
      double kf = 0.0;
      if (hb > m_rooftopHeight)
        {
          Lbsh = -18 * log10 (1 + Dhb);
          ka = (fmhz > 2000 ? 71.4 : 54.0);
          kd = 18.0;
        }
      else 
        {
          Lbsh = 0;
          kd = 18.0 - 15 * Dhb / a->GetPosition ().z;
          if (distance < 500)
            {
              ka = 54.0 - 1.6 * Dhb * distance / 1000;
            }
          else
            {
              ka = 54.0 - 0.8 * Dhb;
            }
        }
      if (fmhz > 2000)
        {
          kf = -8;
        }
      else if ((m_environment == Urban)&&(m_citySize == Large))
        {
          kf = -4 + 0.7 * (fmhz / 925.0 - 1);
        }
      else
        {
          kf = -4 + 1.5 * (fmhz / 925.0 - 1);
        }

      Lmsd = Lbsh + ka + kd*log10 (distance / 1000.0) + kf*log10 (fmhz) - 9.0 * log10 (m_buildingSeparation);
    }
  else
    {
      double theta = atan (Dhb / m_buildingSeparation);
      double rho = sqrt (Dhb * Dhb + m_buildingSeparation * m_buildingSeparation);
      double Qm = 0.0;
      if ((hb > m_rooftopHeight - 1.0) && (hb < m_rooftopHeight + 1.0))
        {
          Qm = m_buildingSeparation / distance;
        }
      else if (hb > m_rooftopHeight)
        {
          Qm = 2.35 * pow (Dhb / distance * sqrt (m_buildingSeparation / m_lambda), 0.9);
        }
      else
        {
          Qm = m_buildingSeparation / (2 * pi * distance) * sqrt (m_lambda / rho) * (1 / theta - (1 / (2 * pi + theta)));
        }
      Lmsd = -10 * log10 (Qm * Qm);
    }
  double Lbf = 32.4 + 20 * log10 (distance / 1000) + 20 * log10 (fmhz);
  double Dhm = m_rooftopHeight - hm;
  double Lrts = -8.2 - 10 * log10 (m_streetsWidth) + 10 * log10 (fmhz) + 20 * log10 (Dhm) + Lori;
//   NS_LOG_INFO (this << " Lbf " << Lbf << " Lrts " << Lrts << " Dhm" << Dhm << " Lmsd "  << Lmsd);
  double loss = 0.0;
  if (Lrts + Lmsd > 0)
    {
      loss = Lbf + Lrts + Lmsd;
    }
  else
    {
      loss = Lbf;
    }
  return (loss);
}


double
BuildingsPropagationLossModel::ItuR1238 (Ptr<BuildingsMobilityModel> a, Ptr<BuildingsMobilityModel> b) const
{
  double N = 0.0;
  int n = abs (a->GetFloorNumber () - b->GetFloorNumber ());
  NS_LOG_LOGIC (this << " A floor " << (uint16_t)a->GetFloorNumber () << " B floor " << (uint16_t)b->GetFloorNumber () << " n " << n);
  double Lf = 0.0;
  Ptr<Building> aBuilding = a->GetBuilding ();
  if (aBuilding->GetBuildingType () == Building::Residential)
    {
      N = 28;
      if (n >= 1)
        {
          Lf = 4 * n;
        }
      NS_LOG_LOGIC (this << " Residential ");
    }
  else if (aBuilding->GetBuildingType () == Building::Office)
    {
      N = 30;
      if (n >= 1)
        {
          Lf = 15 + (4 * (n - 1));
        }
      NS_LOG_LOGIC (this << " Office ");
    }
  else if (aBuilding->GetBuildingType () == Building::Commercial)
    {
      N = 22;
      if (n >= 1)
        {
          Lf = 6 + (3 * (n - 1));
        }
      NS_LOG_LOGIC (this << " Commercial ");
    }
  else
    {
      NS_LOG_ERROR (this << " Unkwnon Wall Type");
    }
  double loss = 20 * log10 (m_frequency / 1e6 /*MHz*/) + N*log10 (a->GetDistanceFrom (b)) + Lf - 28.0;
  NS_LOG_INFO (this << " Node " << a->GetPosition () << " <-> " << b->GetPosition () << " loss = " << loss << " dB");
  return (loss);
}


double
BuildingsPropagationLossModel::ExternalWallLoss (Ptr<BuildingsMobilityModel> a) const
{
  double loss = 0.0;
  Ptr<Building> aBuilding = a->GetBuilding ();
  if (aBuilding->GetExtWallsType () == Building::Wood)
    {
      loss = 4;
    }
  else if (aBuilding->GetExtWallsType () == Building::ConcreteWithWindows)
    {
      loss = 7;
    }
  else if (aBuilding->GetExtWallsType () == Building::ConcreteWithoutWindows)
    {
      loss = 15; // 10 ~ 20 dB
    }
  else if (aBuilding->GetExtWallsType () == Building::StoneBlocks)
    {
      loss = 12;
    }
  return (loss);
}

double
BuildingsPropagationLossModel::HeightLoss (Ptr<BuildingsMobilityModel> node) const
{
  double loss = 0.0;

  int nfloors  = node->GetFloorNumber ();
  loss = -2 * (nfloors);
  return (loss);
}

double
BuildingsPropagationLossModel::InternalWallsLoss (Ptr<BuildingsMobilityModel> a, Ptr<BuildingsMobilityModel>b) const
{
  // approximate the number of internal walls with the Manhattan distance in "rooms" units
  double dx = abs (a->GetRoomNumberX () - b->GetRoomNumberX ());
  double dy = abs (a->GetRoomNumberY () - b->GetRoomNumberY ());    
  return m_lossInternalWall * (dx+dy);
}



double
BuildingsPropagationLossModel::GetShadowing (Ptr<MobilityModel> a, Ptr<MobilityModel> b)
const
{
    Ptr<BuildingsMobilityModel> a1 = DynamicCast<BuildingsMobilityModel> (a);
    Ptr<BuildingsMobilityModel> b1 = DynamicCast<BuildingsMobilityModel> (b);
    NS_ASSERT_MSG ((a1 != 0) && (b1 != 0), "BuildingsPropagationLossModel only works with BuildingsMobilityModel");
  
  std::map<Ptr<MobilityModel>,  std::map<Ptr<MobilityModel>, ShadowingLoss> >::iterator ait = m_shadowingLossMap.find (a);
  if (ait != m_shadowingLossMap.end ())
    {
      std::map<Ptr<MobilityModel>, ShadowingLoss>::iterator bit = ait->second.find (b);
      if (bit != ait->second.end ())
        {
          return (bit->second.GetLoss ());
        }
      else
        {
          double sigma = EvaluateSigma (a1, b1);
          // side effect: will create new entry          
          ait->second[b] = ShadowingLoss (0.0, sigma, b);          
          return (ait->second[b].GetLoss ());
        }
    }
  else
    {
      double sigma = EvaluateSigma (a1, b1);
      // side effect: will create new entries in both maps
      m_shadowingLossMap[a][b] = ShadowingLoss (0.0, sigma, b);  
      return (m_shadowingLossMap[a][b].GetLoss ());       
    }
}



double
BuildingsPropagationLossModel::EvaluateSigma (Ptr<BuildingsMobilityModel> a, Ptr<BuildingsMobilityModel> b)
const
{
  if (a->IsOutdoor ())
    {
      if (b->IsOutdoor ())
        {
          return (m_shadowingSigmaOutdoor);
        }
      else
        {
          double sigma = sqrt ((m_shadowingSigmaOutdoor * m_shadowingSigmaOutdoor) + (m_shadowingSigmaExtWalls * m_shadowingSigmaExtWalls));
          return (sigma);
        }
    }
  else
  if (b->IsIndoor ())
    {
      return (m_shadowingSigmaIndoor);
    }
  else
    {
      double sigma = sqrt ((m_shadowingSigmaOutdoor * m_shadowingSigmaOutdoor) + (m_shadowingSigmaExtWalls * m_shadowingSigmaExtWalls));
      return (sigma);
    }
}


double
BuildingsPropagationLossModel::DoCalcRxPower (double txPowerDbm, Ptr<MobilityModel> a, Ptr<MobilityModel> b) const
{
  return txPowerDbm - GetLoss (a, b) - GetShadowing (a, b);
}


} // namespace ns3
