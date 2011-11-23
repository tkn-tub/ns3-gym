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
 * Author: Marco Miozzo  <marco.miozzo@cttc.es>
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

//#include <ns3/shadowing-loss-model.h>
//#include <ns3/jakes-fading-loss-model.h>


NS_LOG_COMPONENT_DEFINE ("BuildingsPropagationLossModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (BuildingsPropagationLossModel);



class BuildingsPropagationLossModel::ShadowingLoss 
{
public:
  ShadowingLoss (double mean, double sigma, Ptr<MobilityModel> receiver);
  ~ShadowingLoss ();
  double GetLoss ();
  Ptr<MobilityModel> GetReceiver (void);
private:
  Ptr<MobilityModel> m_receiver;
  NormalVariable m_randVariable;
  double m_shadowingValue;
};


BuildingsPropagationLossModel::ShadowingLoss::ShadowingLoss (double mean, double sigma, Ptr<MobilityModel> receiver)
  : m_receiver (receiver),
    m_randVariable (mean, sigma * sigma) // NormalVariable class wants mean and variance (sigma is a standard deviation)
{
  m_shadowingValue = m_randVariable.GetValue ();
  NS_LOG_INFO (this << " New Shadowing: sigma " << sigma << " value " << m_shadowingValue);
}

BuildingsPropagationLossModel::ShadowingLoss::~ShadowingLoss ()
{

}

double
BuildingsPropagationLossModel::ShadowingLoss::GetLoss ()
{
  return (m_shadowingValue);
}

Ptr<MobilityModel>
BuildingsPropagationLossModel::ShadowingLoss::GetReceiver ()
{
  return m_receiver;
}

TypeId
BuildingsPropagationLossModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::BuildingsPropagationLossModel")

    .SetParent<PropagationLossModel> ()

    .AddConstructor<BuildingsPropagationLossModel> ()

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

    .AddAttribute ("ITU1411DistanceThr",
                   " Threshold for ITU 1411 [m].",
                   DoubleValue (2000.0),
                   MakeDoubleAccessor (&BuildingsPropagationLossModel::m_itu1411DistanceThreshold),
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
                                    BuildingsPropagationLossModel::Large, "Large"));


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
  for (PairsList::reverse_iterator i = m_shadowingPairs.rbegin (); i != m_shadowingPairs.rend (); i++)
    {
      PairsSet *ps = *i;
      for (DestinationList::iterator r = ps->receivers.begin (); r != ps->receivers.end (); r++)
        {
          ShadowingLoss *pc = *r;
          delete pc;
        }
      ps->sender = 0;
      ps->receivers.clear ();
      delete ps;
    }
  m_shadowingPairs.clear ();
}

// void
// BuildingsPropagationLossModel::SetLambda (double frequency, double speed)
// {
//   m_lambda = speed / frequency;
//   m_frequency = frequency;
// }

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
//   if (env==Urban)
//   {
//      NS_LOG_INFO (this << " Urban");
//   }
//   else if (env==SubUrban)
//   {
//     NS_LOG_INFO (this << " SubUrban");
//   }
//   else if (env==OpenAreas)
//   {
//     NS_LOG_INFO (this << " OpenAreas");
//   }
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

// double
// BuildingsPropagationLossModel::ItuR1411NlosStreetCanyons (Ptr<BuildingsMobilityModel> a, Ptr<BuildingsMobilityModel> b) const
// {
//   NS_LOG_INFO (this);
//   // reflection pathloss
//   double x1 = a->GetStreetCrossingDistence ();
//   double x2 = b->GetStreetCrossingDistence ();
//   double f_alpha = 0.0;
//   if (m_cornerAngle<= 0.33)
//   {
//     f_alpha = -41.0 + 110*m_cornerAngle;
//   }
//   else if (m_cornerAngle<= 0.42)
//   {
//     f_alpha = -13.94 + 28*m_cornerAngle;
//   }
//   else if (m_cornerAngle<= 0.71)
//   {
//     f_alpha = -5.33 + 7.51*m_cornerAngle;
//   }
//   double pi = 3.141592653589793;
//   double Lr = -20*log10 (x1+x2) + (x1*x2*f_alpha/(m_streetsWidth*m_streetsWidth)) - 20*log10 (4*pi/m_lambda);
//
//   // diffraction pathloss
//   double Da = -1*(40/(2*pi))*(atan (x2/m_streetsWidth) + atan (x1/m_streetsWidth) - (pi/2));
//   double Ld = -10*log10 (x2*x1*(x1+x2)) + 2*Da + 0.1*(90 - m_cornerAngle*(180/pi)) - 20*log10 (4*pi/m_lambda);
//
//   double loss = -10*log10 (pow (10, Lr/10) + pow (10, Ld/10));
//   return (loss);
// }


double
BuildingsPropagationLossModel::ItuR1238 (Ptr<BuildingsMobilityModel> a, Ptr<BuildingsMobilityModel> b) const
{
  double N = 0.0;
  int n = abs (a->GetFloorNumber () - b->GetFloorNumber ());
//   NS_LOG_INFO (this << " A floor " << (uint16_t)a->GetFloorNumber () << " B floor " << (uint16_t)b->GetFloorNumber () << " n " << n);
  double Lf = 0.0;
  Ptr<Building> aBuilding = a->GetBuilding ();
  if (aBuilding->GetBuildingType () == Building::Residential)
    {
      N = 28;
      if (n >= 1)
        {
          Lf = 4 * n;
        }
//     NS_LOG_INFO (this << " Residential ");
    }
  else if (aBuilding->GetBuildingType () == Building::Office)
    {
      N = 30;
      if (n >= 1)
        {
          Lf = 15 + (4 * (n - 1));
        }
//     NS_LOG_INFO (this << " Office ");
    }
  else if (aBuilding->GetBuildingType () == Building::Commercial)
    {
      N = 22;
      if (n >= 1)
        {
          Lf = 6 + (3 * (n - 1));
        }
//     NS_LOG_INFO (this << " Commercial ");
    }
  else
    {
      NS_LOG_ERROR (this << " Unkwnon Wall Type");
    }
  NS_LOG_INFO (this << " Node " << a->GetPosition () << " <-> " << b->GetPosition ());
  double loss = 20 * log10 (m_frequency / 1e6 /*MHz*/) + N*log10 (a->GetDistanceFrom (b)) + Lf - 28.0;

  return (loss);
}


double
BuildingsPropagationLossModel::BEWPL (Ptr<BuildingsMobilityModel> a) const
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
BuildingsPropagationLossModel::HeightGain (Ptr<BuildingsMobilityModel> node) const
{
  double loss = 0.0;

  int nfloors  = node->GetFloorNumber ();
  loss = -2 * (nfloors);
  return (loss);
}




double
BuildingsPropagationLossModel::GetLoss (Ptr<MobilityModel> a, Ptr<MobilityModel> b) const
{

  double distance = a->GetDistanceFrom (b);
  if (distance <= m_minDistance)
    {
      return 0.0;
    }

  // get the BuildingsMobilityModel pointers
  Ptr<BuildingsMobilityModel> a1 = DynamicCast<BuildingsMobilityModel> (a);
  Ptr<BuildingsMobilityModel> b1 = DynamicCast<BuildingsMobilityModel> (b);

  double loss = 0.0;

  if (a1->IsOutdoor ())
    {
      if (b1->IsOutdoor ())
        {
          if (distance > 1000)
            {
              NS_LOG_INFO (this << a1->GetPosition ().z << b1->GetPosition ().z << m_rooftopHeight);
              if ((a1->GetPosition ().z < m_rooftopHeight)
                  && (b1->GetPosition ().z < m_rooftopHeight))
                {
                  // ITU limit in distance (i.e., < 2000 for small cells)
                  if (distance < m_itu1411DistanceThreshold)
                    {
                      // short range communication
                      loss = ItuR1411 (a1, b1);
                      NS_LOG_INFO (this << " 0-0 (>1000): down rooftop -> ITUR1411 : " << loss);
                    }
                  else
                    {
                      // out of bound
                      loss = std::numeric_limits<double>::infinity ();
                      NS_LOG_INFO (this << " 0-0 (>2000): down rooftop -> ITUR1411 : " << loss);
                    }
                }
              else
                {
                  // Over the rooftop tranmission -> Okumura Hata
                  loss = OkumuraHata (a1, b1);
                  NS_LOG_INFO (this << " O-O (>1000): Over the rooftop -> OH : " << loss);
                }
            }
          else
            {
              // short range outdoor communication
              loss = ItuR1411 (a1, b1);
              NS_LOG_INFO (this << " 0-0 (<1000) Street canyon -> ITUR1411 : " << loss);
            }
        }
      else
        {
          // b indoor
          if (distance > 1000)
            {
              if ((a1->GetPosition ().z < m_rooftopHeight)
                  && (b1->GetPosition ().z < m_rooftopHeight))
                {

                  // ITU limit in distance (i.e., < 2000 for small cells)
                  if (distance < m_itu1411DistanceThreshold)
                    {
                      // short range communication
                      loss = ItuR1411 (a1, b1) + BEWPL (b1) + HeightGain (a1);
                      NS_LOG_INFO (this << " 0-I (>1000): down rooftop -> ITUR1411 : " << loss);
                    }
                  else
                    {
                      // out of bound
                      loss = std::numeric_limits<double>::infinity ();
                      NS_LOG_INFO (this << " 0-I (>2000): down rooftop -> ITUR1411 : " << loss);
                    }
                }
              else
                {
                  // Over the rooftop tranmission -> Okumura Hata
                  loss = OkumuraHata (a1, b1) + BEWPL (b1);
                  NS_LOG_INFO (this << " O-I (>1000): Over the rooftop -> OH : " << loss);
                }
            }
          else
            {
              loss = ItuR1411 (a1, b1) + BEWPL (b1) + HeightGain (b1);
              NS_LOG_INFO (this << " 0-I (<1000) ITUR1411 + BEL : " << loss);
            }
        } // end b1->isIndoor ()
    }
  else
    {
      // a is indoor
      if (b1->IsIndoor ())
        {
          if (a1->GetBuilding () == b1->GetBuilding ())
            {
              // nodes are in same building -> indoor communication ITU-R P.1238
              loss = ItuR1238 (a1, b1);
              NS_LOG_INFO (this << " I-I (same building) ITUR1238 : " << loss);

            }
          else
            {
              // nodes are in different buildings
              loss = ItuR1411 (a1, b1) + BEWPL (a1) + BEWPL (b1);
              NS_LOG_INFO (this << " I-I (different) ITUR1238 + 2*BEL : " << loss);
            }
        }
      else
        {
          // b is outdoor
          if (distance > 1000)
            {
              if ((a1->GetPosition ().z < m_rooftopHeight)
                  && (b1->GetPosition ().z < m_rooftopHeight))
                {

                  // ITU limit in distance (i.e., < 2000 for small cells)
                  if (distance < m_itu1411DistanceThreshold)
                    {
                      // short range communication
                      loss = ItuR1411 (a1, b1) + BEWPL (a1) + HeightGain (a1);
                      NS_LOG_INFO (this << " I-O (>1000): down rooftop -> ITUR1411 : " << loss);
                    }
                  else
                    {
                      // out of bound
                      loss = std::numeric_limits<double>::infinity ();
                      NS_LOG_INFO (this << " I-O (>2000): down rooftop -> ITUR1411 : " << loss);
                    }
                }
              else
                {
                  // above rooftop -> OH
                  loss = OkumuraHata (a1, b1) + BEWPL (a1) + HeightGain (a1);
                  NS_LOG_INFO (this << " =I-O (>1000) over rooftop OH + BEL + HG: " << loss);
                }
            }
          else
            {
              loss = ItuR1411 (a1, b1) + BEWPL (a1)  + HeightGain (a1);
              NS_LOG_INFO (this << " I-O (<1000)  ITUR1411 + BEL + HG: " << loss);
            }
        } // end b1->IsIndoor ()
    } // end a1->IsOutdoor ()

  // Evaluate the shadowing
  PairsList::iterator i = m_shadowingPairs.end ();
  while (i != m_shadowingPairs.begin ()) 
    {
      i--;
      PairsSet *ps = *i;
      if (ps->sender == a) 
        {
          for (DestinationList::iterator r = ps->receivers.begin (); r != ps->receivers.end (); r++) 
            {
              ShadowingLoss *pc = *r;
              if (pc->GetReceiver () == b) 
                {
                  return loss + pc->GetLoss ();
                }
            }
          double sigma = EvaluateSigma (a1, b1);
          ShadowingLoss *pc = new ShadowingLoss (0.0, sigma,b);
          ps->receivers.push_back (pc);
          return loss + pc->GetLoss ();
        }
    }
  PairsSet *ps = new PairsSet;
  ps->sender = a;
  double sigma = EvaluateSigma (a1, b1);
  ShadowingLoss *pc = new ShadowingLoss (0.0, sigma, b);
  ps->receivers.push_back (pc);
  m_shadowingPairs.push_back (ps);
  return loss + pc->GetLoss ();

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
  return txPowerDbm - GetLoss (a, b);
}


} // namespace ns3
