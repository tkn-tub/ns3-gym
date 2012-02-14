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

#ifndef BUILDINGS_PROPAGATION_LOSS_MODEL_H_
#define BUILDINGS_PROPAGATION_LOSS_MODEL_H_

#include "ns3/nstime.h"
#include "ns3/propagation-loss-model.h"
#include <ns3/building.h>
#include <ns3/buildings-mobility-model.h>

// #include <ns3/jakes-fading-loss-model.h>
// #include <ns3/shadowing-loss-model.h>

namespace ns3 {

class ShadowingLossModel;
class JakesFadingLossModel;

/**
 * \ingroup propagation
 *
 *  \brief The Building-Propagation-Model is a compound of different models able to evaluate the pathloss from 200 to 2600 MHz, in different environments and with buildings (i.e., indoor and outdoor communications).
 *
 *  This model includes Hata model, COST231, ITU-R P.1411 (short range
 *  communications), ITU-R P.1238 (indoor communications). 
 *  Building-Propagation-Model properly combines the models above in order
 *  to be able to evaluate the pathloss under different scenarios, in detail:
 *  - Environments: urban, suburban, open-areas;
 *  - frequency: from 200 uo to 2600 MHz
 *  - short range communications vs long range communications
 *  - Node position respect to buildings: indoor, outdoor and hybrid (indoor <-> outdoor)
 *  - Building penetretation loss
 *  - floors, etc...
 *
 *  Frequency: 200 MHz to 2000 MHz
 *  Link Distance:up to 20 km
 *  Short/long distance commutation: 1 Km
 *  \warning This model works with BuildingsMobilityModel
 *
 */

class BuildingsPropagationLossModel : public PropagationLossModel
{

public:
  static TypeId GetTypeId (void);
  BuildingsPropagationLossModel ();
  ~BuildingsPropagationLossModel ();
  enum Environment
  {
    Urban, SubUrban, OpenAreas
  };

  enum CitySize
  {
    Small, Medium, Large
  };

  /**
   * \param a the mobility model of the source
   * \param b the mobility model of the destination
   * \returns the propagation loss (in dBm)
   */
  double GetLoss (Ptr<MobilityModel> a, Ptr<MobilityModel> b) const;
  void SetEnvironment (Environment env);
  void SetCitySize (CitySize size);
  void SetLambda (double lambda);
  void SetFrequency (double freq);
  void SetMinDistance (double minDistance);
  Environment GetEnvironment (void) const;
  CitySize GetCitySize (void) const;
  double GetMinDistance (void) const;
  double GetLambda (void) const;
  double GetFrequency (void) const;
//   void SetLambda (double frequency, double speed);


private:
  virtual double DoCalcRxPower (double txPowerDbm, Ptr<MobilityModel> a, Ptr<MobilityModel> b) const;
  double OkumuraHata (Ptr<BuildingsMobilityModel> a, Ptr<BuildingsMobilityModel> b) const;
  double ItuR1411 (Ptr<BuildingsMobilityModel> a, Ptr<BuildingsMobilityModel> b) const;
  double ItuR1411Los (Ptr<BuildingsMobilityModel> a, Ptr<BuildingsMobilityModel> b) const;
  double ItuR1411NlosOverRooftop (Ptr<BuildingsMobilityModel> a, Ptr<BuildingsMobilityModel> b) const;
//   double ItuR1411NlosStreetCanyons (Ptr<BuildingsMobilityModel> a, Ptr<BuildingsMobilityModel> b) const;
  double ItuR1238 (Ptr<BuildingsMobilityModel> a, Ptr<BuildingsMobilityModel> b) const;

  double ExternalWallLoss (Ptr<BuildingsMobilityModel> a) const;
  double HeightLoss (Ptr<BuildingsMobilityModel> n) const;
  double InternalWallsLoss (Ptr<BuildingsMobilityModel> a, Ptr<BuildingsMobilityModel> b) const;

  double C;  // OH loss coefficient for the environment
  double N;  // ITU-R P.1238: power loss coefficient
  double m_lambda;
  Environment m_environment;
  CitySize m_citySize;
  double m_minDistance; // in meter
  double m_frequency; // frequency in MHz
  double m_rooftopHeight; // in meter (used to discriminate OH and short range canyoning)
  double m_itu1411NlosThreshold; // in meters (switch Los -> NLoS)
  double m_itu1411DistanceThreshold; // in meters (above infinite loss)
  double m_streetsOrientation; // in degrees [0,90]
  double m_streetsWidth; // in meters
  double m_buildingsExtend; // in meters
  double m_buildingSeparation; // in meters
  double m_lossInternalWall; // in meters

  
  class ShadowingLoss
  {
  public:
    ShadowingLoss ();
    ShadowingLoss (double mean, double sigma, Ptr<MobilityModel> receiver);
    double GetLoss () const;
    Ptr<MobilityModel> GetReceiver (void) const;
  private:
    Ptr<MobilityModel> m_receiver;
    NormalVariable m_randVariable;
    double m_shadowingValue;
  };

  mutable std::map<Ptr<MobilityModel>,  std::map<Ptr<MobilityModel>, ShadowingLoss> > m_shadowingLossMap;
  double EvaluateSigma (Ptr<BuildingsMobilityModel> a, Ptr<BuildingsMobilityModel> b) const;


  double m_shadowingSigmaExtWalls;
  double m_shadowingSigmaOutdoor;
  double m_shadowingSigmaIndoor;

};

}

#endif /* COST231PROPAGATIONMODEL_H_ */
