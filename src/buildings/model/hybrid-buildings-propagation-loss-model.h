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
 *         Nicola Baldo <nbaldo@cttc.es>
 * 
 */

#ifndef HYBRID_BUILDINGS_PROPAGATION_LOSS_MODEL_H_
#define HYBRID_BUILDINGS_PROPAGATION_LOSS_MODEL_H_

#include <ns3/buildings-propagation-loss-model.h>
#include <ns3/propagation-environment.h>

namespace ns3 {

class OkumuraHataPropagationLossModel;
class ItuR1411LosPropagationLossModel;
class ItuR1411NlosOverRooftopPropagationLossModel;
class ItuR1238PropagationLossModel;
class Kun2600MhzPropagationLossModel;

/**
 * \ingroup propagation
 *
 *  \brief The HybridBuildingsPropagationModel is a compound of different models able to evaluate the pathloss from 200 to 2600 MHz, in different environments and with buildings (i.e., indoor and outdoor communications).
 *
 *  This model includes Hata model, COST231, ITU-R P.1411 (short range
 *  communications), ITU-R P.1238 (indoor communications), which are combined in order
 *  to be able to evaluate the pathloss under different scenarios, in detail:
 *  - Environments: urban, suburban, open-areas;
 *  - frequency: from 200 uo to 2600 MHz
 *  - short range communications vs long range communications
 *  - Node position respect to buildings: indoor, outdoor and hybrid (indoor <-> outdoor)
 *  - Building penetretation loss
 *  - floors, etc...
 *
 *  \warning This model works only with BuildingsMobilityModel
 *
 */

class HybridBuildingsPropagationLossModel : public BuildingsPropagationLossModel
{

public:
  static TypeId GetTypeId (void);
  HybridBuildingsPropagationLossModel ();
  ~HybridBuildingsPropagationLossModel ();


  /** 
   * set the environment type
   * 
   * \param env 
   */
  void SetEnvironment (EnvironmentType env);

  /** 
   * set the size of the city
   * 
   * \param size 
   */
  void SetCitySize (CitySize size);

  /** 
   * set the propagation frequency
   * 
   * \param freq 
   */
  void SetFrequency (double freq);

  /** 
   * set the rooftop height
   * 
   * \param rooftopHeight
   */
  void SetRooftopHeight (double rooftopHeight);

  /**
   * \param a the mobility model of the source
   * \param b the mobility model of the destination
   * \returns the propagation loss (in dBm)
   */
  virtual double GetLoss (Ptr<MobilityModel> a, Ptr<MobilityModel> b) const;

  
private:

  double OkumuraHata (Ptr<BuildingsMobilityModel> a, Ptr<BuildingsMobilityModel> b) const;
  double ItuR1411 (Ptr<BuildingsMobilityModel> a, Ptr<BuildingsMobilityModel> b) const;
  double ItuR1238 (Ptr<BuildingsMobilityModel> a, Ptr<BuildingsMobilityModel> b) const;

  Ptr<OkumuraHataPropagationLossModel> m_okumuraHata;
  Ptr<ItuR1411LosPropagationLossModel> m_ituR1411Los;
  Ptr<ItuR1411NlosOverRooftopPropagationLossModel> m_ituR1411NlosOverRooftop;
  Ptr<ItuR1238PropagationLossModel> m_ituR1238;
  Ptr<Kun2600MhzPropagationLossModel> m_kun2600Mhz;

  double m_itu1411NlosThreshold; ///< in meters (switch Los -> NLoS)
  double m_rooftopHeight;
  double m_frequency;

};

}

#endif /* HYBRID_BUILDINGS_PROPAGATION_LOSS_MODEL_H_ */
