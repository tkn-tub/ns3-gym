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
#include <cmath>
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

    .AddAttribute ("InternalWallLoss",
                   "Additional loss for each internal wall [dB]",
                   DoubleValue (5.0),
                   MakeDoubleAccessor (&BuildingsPropagationLossModel::m_lossInternalWall),
                   MakeDoubleChecker<double> ());


  return tid;
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

  int nfloors  = node->GetFloorNumber () - 1;
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
