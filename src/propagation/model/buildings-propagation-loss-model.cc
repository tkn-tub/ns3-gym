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

NS_LOG_COMPONENT_DEFINE ("BuildingsPropagationLossModel");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (BuildingsPropagationLossModel);

TypeId
BuildingsPropagationLossModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::BuildingsPropagationLossModel")

    .SetParent<PropagationLossModel> ()

    .AddConstructor<BuildingsPropagationLossModel> ()

    .AddAttribute ("Lambda",
                   "The wavelength  (default is 2.3 GHz at 300 000 km/s).",
                   DoubleValue (300000000.0 / 2.3e9),
                   MakeDoubleAccessor (&BuildingsPropagationLossModel::m_lambda),
                   MakeDoubleChecker<double> ())

    .AddAttribute ("Frequency",
                   "The Frequency  (default is 2.3 GHz).",
                   DoubleValue (2.3e9),
                   MakeDoubleAccessor (&BuildingsPropagationLossModel::m_frequency),
                   MakeDoubleChecker<double> ())
                   
    .AddAttribute ("RooftopLevel",
                  " The height of the rooftop [m].",
                  DoubleValue (30.0),
                  MakeDoubleAccessor (&BuildingsPropagationLossModel::m_rooftopHeight),
                  MakeDoubleChecker<double> ())
                   
    .AddAttribute ("MinDistance",
                   "The distance under which the propagation model refuses to give results (m) ",
                   DoubleValue (0.5),
                   MakeDoubleAccessor (&BuildingsPropagationLossModel::SetMinDistance, &BuildingsPropagationLossModel::GetMinDistance),
                   MakeDoubleChecker<double> ());
    
  return tid;
}

BuildingsPropagationLossModel::BuildingsPropagationLossModel () :
  C (0),
  m_environment (Urban),
  m_citySize (Large)
{
  NS_LOG_INFO (this << " BuildingsPropagationLossModel");
}

void
BuildingsPropagationLossModel::SetLambda (double frequency, double speed)
{
  m_lambda = speed / frequency;
  m_frequency = frequency;
}

void
BuildingsPropagationLossModel::SetLambda (double lambda)
{
  m_lambda = lambda;
  m_frequency = 300000000 / lambda;
}

double
BuildingsPropagationLossModel::GetLambda (void) const
{
  return m_lambda;
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


double
BuildingsPropagationLossModel::OkumuraHata (Ptr<BuildingsMobilityModel> a, Ptr<BuildingsMobilityModel> b) const
{
  // Hp: a is the rooftop antenna (from GetLoss logic)
  double loss = 0.0;
  if (m_frequency<=1.500e9)
    {
      // standard Okumura Hata (from wikipedia)
      double log_f = log10 (m_frequency);
      double log_aHeight = 13.82 * log10 (a->GetPosition ().z);
      double log_bHeight = 0.0;
      if (m_citySize == Large)
        {
          if (m_frequency<200)
            {
              log_bHeight = 8.29 * pow (log10 (1.54 * b->GetPosition ().z), 2) -  1.1;
            }
          else
            {
              log_bHeight = 3.2 * pow (log10 (11.75 * b->GetPosition ().z), 2) - 4.97;
            }
        }
      else
        {
          log_bHeight = 0.8 + (1.1*log10(m_frequency) - 0.7)*b->GetPosition ().z - 1.56*log10(m_frequency);
        }
      
      loss = 69.55 + (26.16 * log_f) - log_aHeight + (((44.9 - (6.55 * log_f) ))*log10 (a->GetDistanceFrom (b))) - log_bHeight;
      
      if (m_environment == SubUrban)
        {
          loss += 2 * (pow(log10 (m_frequency / 28), 2)) - 5.4;
        }
      else if (m_environment == OpenAreas)
        {
          loss += -4.70*pow(log10(m_frequency),2) + 18.33*log10(m_frequency) - 40.94;
        }
          
    }
  else if (m_frequency <= 2.170e9) // max 3GPP freq EUTRA band #1
    {
      // COST 231 Okumura model
      double log_f = log10 (m_frequency);
      double log_aHeight = 13.83 * log10 (a->GetPosition ().z);
      double log_bHeight = 0.0;
      if (m_citySize == Large)
        {
          log_bHeight = 3.2 * pow ((log10(11.75 * b->GetPosition ().z)),2);
        }
      else
        {
          log_bHeight = 1.1*log10(m_frequency) - 0.7*b->GetPosition ().z - (1.56*log10(m_frequency) - 0.8);
        }
      
      loss = 46.3 + (33.9 * log_f) - log_aHeight + (((44.9 - (6.55 * log_f) ))*log10 (a->GetDistanceFrom (b))) - log_bHeight;
    }
  else if (m_frequency <= 2.690e9) // max 3GPP freq EUTRA band #1
    {
      // Empirical model from
      // "Path Loss Models for Suburban Scenario at 2.3GHz, 2.6GHz and 3.5GHz"
      // Sun Kun, Wang Ping, Li Yingze
      // Antennas, Propagation and EM Theory, 2008. ISAPE 2008. 8th International Symposium on 
      loss = 36 + 26*log10(a->GetDistanceFrom (b));
    }
      
  return (loss);
}



double
BuildingsPropagationLossModel::ItuR1411 (Ptr<BuildingsMobilityModel> a, Ptr<BuildingsMobilityModel> b) const
{
  return (ItuR1411Los (a,b));
}


double
BuildingsPropagationLossModel::ItuR1411Los (Ptr<BuildingsMobilityModel> a, Ptr<BuildingsMobilityModel> b) const
{
  double dist = a->GetDistanceFrom (b);
  double lossLow = 0.0;
  double lossUp = 0.0;
  double pi = 3.141592653589793;
  double Lbp = 20*log10(m_lambda*m_lambda/(8*pi*a->GetPosition ().z*b->GetPosition ().z));
  double Rbp = (4 * a->GetPosition ().z * b->GetPosition ().z) / m_lambda;
  //   NS_LOG_INFO (this << " Lbp " << Lbp << " Rbp " << Rbp);
  if (dist <= Rbp)
  {
    lossLow = Lbp + 20*log10(dist/Rbp);
    lossUp = Lbp + 20 + 25*log10(dist/Rbp);
  }
  else
  {
    lossLow = Lbp + 40*log10(dist/Rbp);
    lossUp = Lbp + 20 + 40*log10(dist/Rbp);
  }
  
  double loss = (lossUp + lossLow) / 2;
  
  return (loss);
}

double
BuildingsPropagationLossModel::ItuR1411Nlos (Ptr<BuildingsMobilityModel> a, Ptr<BuildingsMobilityModel> b) const
{
  if ((a->GetPosition ().z>m_rooftopHeight) || (b->GetPosition ().z>m_rooftopHeight))
  {
    return (ItuR1411NlosOverRooftop (a,b));
  }
  else
  {
    return (ItuR1411NlosStreetCanyons (a,b));
  }
}

double
BuildingsPropagationLossModel::ItuR1411NlosOverRooftop (Ptr<BuildingsMobilityModel> a, Ptr<BuildingsMobilityModel> b) const
{
  double Lori = 0.0;
  if ((m_streetsOrientation>=0)&&(m_streetsOrientation<35))
    {
      Lori = -10.0 + 0.354*m_streetsOrientation;
    }
  else if ((m_streetsOrientation>=35)&&(m_streetsOrientation<55))
    {
      Lori = 2.5 + 0.075*m_streetsOrientation;
    }
  else if ((m_streetsOrientation>=55)&&(m_streetsOrientation<90))
    {
      Lori = 2.5 + 0.075*m_streetsOrientation;
    }
  else
    {
      NS_LOG_ERROR (this << " Street Orientation must be in [0,90]");
    }
    
  double Lrts = -8.2 -10*log10(m_streetsWidth) + 20*log10(m_rooftopHeight - b->GetPosition ().z) + Lori;
  double distance = a->GetDistanceFrom (b);
  double Dhb = a->GetPosition ().z - m_rooftopHeight;
  double ds = (m_lambda * distance * distance) / (Dhb * Dhb);
  double Lmsd = 0.0;
  double pi = 3.141592653589793;
  if (ds < m_buildingsExtend)
    {
      double Lbsh = 0.0;
      double ka = 0.0;
      double kd = 0.0;
      double kf = 0.0;
      if ((a->GetPosition ().z > m_rooftopHeight) || (b->GetPosition ().z > m_rooftopHeight))
        {
          Lbsh = -18*log10(1+Dhb);
          ka = 54.0;
          kd = 18.0;
        }
      else 
        {
          Lbsh = 0;
          kd = 18.0 - 15*Dhb/a->GetPosition ().z;
          if (distance <500)
            {
              ka = 54.0 - 1.6*Dhb*distance/1000;
            }
            else
            {
              ka = 54.0 - 0.8*Dhb;
            }
        }
      if ((m_environment==Urban)&&(m_citySize==Large))
        {
          kf = 0.7*(m_frequency/925.0 -1);
        }
      else
        {
          kf = 1.5*(m_frequency/925.0 -1);
        }
      Lmsd = Lbsh + ka + kd*log10(distance/1000.0) + kf*log10(m_frequency) -9.0*log10(m_buildingSeparation); // CHECK last d (it's "b" in ITU)
    }
  else
    {
      double theta = atan (Dhb/m_buildingSeparation);
      double rho = sqrt(Dhb*Dhb+m_buildingSeparation*m_buildingSeparation);
      double hb = a->GetPosition ().z;
      double Qm = 0.0;
      if ((hb > m_rooftopHeight -1.0) && (hb < m_rooftopHeight + 1.0))
        {
          Qm = m_buildingSeparation / distance;
        }
      else if (hb > m_rooftopHeight)
        {
          Qm = 2.35*pow(Dhb/distance*sqrt(m_buildingSeparation/m_lambda), 0.9);
        }
      else
        {
          Qm = m_buildingSeparation/(2*pi*distance)*sqrt(m_lambda/rho)*(1/theta-(1/(2*pi+theta)));
        }
        
      Lmsd = -10*log10(Qm*Qm);
    }
  double Lbf = 32.4 + 20*log10(distance/1000) + 20*log10(m_frequency);
  
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
BuildingsPropagationLossModel::ItuR1411NlosStreetCanyons (Ptr<BuildingsMobilityModel> a, Ptr<BuildingsMobilityModel> b) const
{
  
  return (0.0);
}


double
BuildingsPropagationLossModel::ItuR1238 (Ptr<BuildingsMobilityModel> a, Ptr<BuildingsMobilityModel> b) const
{
  double N = 0.0;
  int n = abs (a->GetFloorNumber () - b->GetFloorNumber ());
  NS_LOG_INFO (this << " A floor " << (uint16_t)a->GetFloorNumber () << " B floor " << (uint16_t)b->GetFloorNumber () << " n " << n);
  double Lf = 0.0;
  Ptr<Building> aBuilding = a->GetBuilding ();
  if (aBuilding->GetBuildingType () == Building::Residential)
  {
    N = 28;
    Lf = 4 * n;
  }
  else if (aBuilding->GetBuildingType () == Building::Office)
  {
    N = 30;
    Lf = 15 + (4 * (n-1));
  }
  else if (aBuilding->GetBuildingType () == Building::Commercial)
  {
    N = 22;
    Lf = 6 + (3 * (n-1));
  }
  else
  {
    NS_LOG_ERROR (this << " Unkwnon Wall Type");
  }
  
  double loss = 20*log10(m_frequency) + N*log10(a->GetDistanceFrom (b)) + Lf - 28.0;
  
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
      loss = 10; // 10 ~ 20 dB
    }
  else if (aBuilding->GetExtWallsType () == Building::StoneBlocks)
    {
      loss = 12;
    }
  
    
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
  NS_LOG_INFO (this << " rooftop " << m_rooftopHeight);
  
  if (a1->IsOutdoor ())
    {
      if (b1->IsOutdoor ())
        {
          if (distance > 1000)
            {
              if ((a1->GetPosition ().z > m_rooftopHeight)
                || (b1->GetPosition ().z > m_rooftopHeight))
                {
                  // Over the rooftop tranmission -> Okumura Hata
                  loss = OkumuraHata (a1, b1);
                  NS_LOG_INFO (this << " O-O (>1000): Over the rooftop -> OH : " << loss);
                }
              else
                {
                  // TODO put a limit in distance (i.e., < 2000 for small cells)
                  
                  // short range outdoor communication within street canyon
                  loss = ItuR1411 (a1, b1);
                  NS_LOG_INFO (this << " 0-0 (>1000): down rooftop -> ITUR1411 : " << loss);
                }
            }
          else
            {
              // short range outdoor communication within street canyon
              loss = ItuR1411 (a1, b1);
              NS_LOG_INFO (this << " 0-0 (<1000) Street canyon -> ITUR1411 : " << loss);
            }
        }
      else
        {
          // b indoor
          if (distance > 1000)
            {
              loss = OkumuraHata (a1, b1) + BEWPL(b1);
              NS_LOG_INFO (this << " 0-I (>1000) OH + BEL : " << loss);
            }
          else
            {
              loss = ItuR1411 (a1, b1) + BEWPL(b1);
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
              loss = ItuR1411 (a1, b1) + BEWPL(a1) + BEWPL(b1);
              NS_LOG_INFO (this << " I-I (different) ITUR1238 + 2*BEL : " << loss);
            }
        }
      else
        {
          // b is outdoor
          if (distance > 1000)
            {
              if ((a1->GetPosition ().z > m_rooftopHeight)
                || (b1->GetPosition ().z > m_rooftopHeight))
                {
                  loss = OkumuraHata (a1, b1) + BEWPL(a1);
                  NS_LOG_INFO (this << " I-0 (>1000) over rooftop OH + BEL : " << loss);
                }
              else
                {
                  loss = ItuR1411 (a1, b1) + BEWPL(a1);
                  NS_LOG_INFO (this << " I-0 (>1000) down rooftop ITUR1411 + BEL : " << loss);
                }
            }
          else
            {
              loss = ItuR1411 (a1, b1) + BEWPL(a1);
              NS_LOG_INFO (this << " I-0 (<1000)  ITUR1411 + BEL : " << loss);
            }
        } // end b1->IsIndoor ()
    } // end a1->IsOutdoor ()

  return (loss);

}

double
BuildingsPropagationLossModel::DoCalcRxPower (double txPowerDbm, Ptr<MobilityModel> a, Ptr<MobilityModel> b) const
{
  return txPowerDbm + GetLoss (a, b);
}

} // namespace ns3
