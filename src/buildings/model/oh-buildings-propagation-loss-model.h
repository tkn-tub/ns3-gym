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

#ifndef OH_BUILDINGS_PROPAGATION_LOSS_MODEL_H_
#define OH_BUILDINGS_PROPAGATION_LOSS_MODEL_H_

#include <ns3/buildings-propagation-loss-model.h>

namespace ns3 {


/**
 * \ingroup propagation
 *
 *  \brief The Okumura Hata (OH)-Building-Propagation-Model is propagation module
 *  using only the Okumura Hata model from the one provided by
 *   BuildingsPropagationLossModel, in different environments and with 
 *   buildings (i.e., indoor and outdoor communications).
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
 *  \warning This model works with BuildingsMobilityModel
 *
 */

class OhBuildingsPropagationLossModel : public BuildingsPropagationLossModel
{

public:
  static TypeId GetTypeId (void);
  OhBuildingsPropagationLossModel ();
  ~OhBuildingsPropagationLossModel ();

  /**
   * \param a the mobility model of the source
   * \param b the mobility model of the destination
   * \returns the propagation loss (in dBm)
   */
  virtual double GetLoss (Ptr<MobilityModel> a, Ptr<MobilityModel> b) const;
  

};

}

#endif /* OH_BUILDINGS_PROPAGATION_LOSS_MODEL_H_ */
