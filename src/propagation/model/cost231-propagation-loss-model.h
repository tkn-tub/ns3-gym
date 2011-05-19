/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007,2008, 2009 INRIA, UDcast
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
 * Author: Mohamed Amine Ismail <amine.ismail@sophia.inria.fr>
 *                              <amine.ismail@udcast.com>
 */

#ifndef COST231_PROPAGATION_LOSS_MODEL_H_
#define COST231_PROPAGATION_LOSS_MODEL_H_

#include "ns3/nstime.h"
#include "ns3/propagation-loss-model.h"

namespace ns3 {

/**
 * \ingroup propagation
 *
 *  \brief The COST-Hata-Model is the most often cited of the COST 231 models.
 *
 *  Also called the Hata Model PCS Extension, it is a radio propagation model
 *  that extends the Hata Model (which in turn is based on the Okumura Model)
 *  to cover a more elaborated range of frequencies. COST (COperation
 *  europ�enne dans le domaine de la recherche Scientifique et Technique)
 *  is a European Union Forum for cooperative scientific research which has
 *  developed this model accordingly to various experiments and researches.
 *  This model is applicable to urban areas. To further evaluate Path Loss
 *  in Suburban or Rural Quasi-open/Open Areas.
 *  Frequency: 1500 MHz to 2000 MHz
 *  Mobile Station Antenna Height: 1 up to 10m
 *  Base station Antenna Height: 30m to 200m
 *  Link Distance:up to 20 km
 *
 */

class Cost231PropagationLossModel : public PropagationLossModel
{

public:
  static TypeId GetTypeId (void);
  Cost231PropagationLossModel ();
  enum Environment
  {
    SubUrban, MediumCity, Metropolitan
  };

  /**
   * \param a the mobility model of the source
   * \param b the mobility model of the destination
   * \returns the propagation loss (in dBm)
   */
  double GetLoss (Ptr<MobilityModel> a, Ptr<MobilityModel> b) const;
  void SetBSAntennaHeight (double height);
  void SetSSAntennaHeight (double height);
  void SetEnvironment (Environment env);
  void SetLambda (double lambda);
  void SetMinDistance (double minDistance);
  double GetBSAntennaHeight (void) const;
  double GetSSAntennaHeight (void) const;
  Environment GetEnvironment (void) const;
  double GetMinDistance (void) const;
  double GetLambda (void) const;
  void SetLambda (double frequency, double speed);
  double GetShadowing (void);
  void SetShadowing (double shadowing);
private:
  virtual double DoCalcRxPower (double txPowerDbm, Ptr<MobilityModel> a, Ptr<MobilityModel> b) const;
  double m_BSAntennaHeight; // in meter
  double m_SSAntennaHeight; // in meter
  double C;
  double m_lambda;
  Environment m_environment;
  double m_minDistance; // in meter
  double m_frequency; // frequency in MHz
  double m_shadowing;

};

}

#endif /* COST231PROPAGATIONMODEL_H_ */
