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
#ifndef PROPAGATION_LOSS_MODEL_H
#define PROPAGATION_LOSS_MODEL_H

#include "ns3/object.h"
#include "ns3/random-variable.h"

namespace ns3 {

class MobilityModel;

/**
 * \brief Modelize the propagation loss through a transmission medium
 *
 * Calculate the receive power (dbm) from a transmit power (dbm),
 * and, a mobility model for the source and destination positions.
 */
class PropagationLossModel : public Object
{
public:
  static TypeId GetTypeId (void);

  virtual ~PropagationLossModel ();
  /**
   * \param a the mobility model of the source
   * \param b the mobility model of the destination
   * \returns the attenuation coefficient (dB)
   */
  virtual double GetLoss (Ptr<MobilityModel> a,
			  Ptr<MobilityModel> b) const = 0;
};

/**
 * \brief The propagation loss is random
 */ 
class RandomPropagationLossModel : public PropagationLossModel
{
public:
  static TypeId GetTypeId (void);

  RandomPropagationLossModel ();
  virtual ~RandomPropagationLossModel ();

  virtual double GetLoss (Ptr<MobilityModel> a,
			  Ptr<MobilityModel> b) const;
private:
  RandomVariable m_variable;
};

/**
 * \brief a Friis propagation loss model
 *
 * The Friis propagation loss model was first described in
 * "A Note on a Simple Transmission Formula", by 
 * "Harald T. Friis".
 * 
 * The original equation was described as:
 *  \f$ \frac{P_r}{P_t} = \frac{A_r A_t}{d^2\lambda^2} \f$
 *  with the following equation for the case of an
 *  isotropic antenna with no heat loss:
 *  \f$ A_{isotr.} = \frac{\lambda^2}{4\pi} \f$
 *
 * The final equation becomes:
 * \f$ \frac{P_r}{P_t} = \frac{\lambda^2}{(4 \pi d)^2} \f$
 *
 * Modern extensions to this original equation are:
 * \f$ P_r = \frac{P_t G_t G_r \lambda^2}{(4 \pi d)^2 L}\f$
 *
 * With:
 *  - \f$ P_r \f$ : reception power (W)
 *  - \f$ P_t \f$ : transmission power (W)
 *  - \f$ G_t \f$ : transmission gain (unit-less)
 *  - \f$ G_r \f$ : reception gain (unit-less)
 *  - \f$ \lambda \f$ : wavelength (m)
 *  - \f$ d \f$ : distance (m)
 *  - \f$ L \f$ : system loss (unit-less)
 *
 *
 * This model is invalid for small distance values.
 * The current implementation returns the txpower as the rxpower
 * for any distance smaller than MinDistance.
 */
class FriisPropagationLossModel : public PropagationLossModel
{
public:
  static TypeId GetTypeId (void);
  FriisPropagationLossModel ();
  /**
   * \param frequency (Hz)
   * \param speed (m/s)
   *
   * Set the main wavelength used in the Friis model 
   * calculation.
   */
  void SetLambda (double frequency, double speed);
  /**
   * \param lambda (m) the wavelength
   *
   * Set the main wavelength used in the Friis model 
   * calculation.
   */
  void SetLambda (double lambda);
  /**
   * \param systemLoss (dimension-less)
   *
   * Set the system loss used by the Friis propagation model.
   */
  void SetSystemLoss (double systemLoss);

  /**
   * \param minDistance the minimum distance
   *
   * Below this distance, the txpower is returned
   * unmodified as the rxpower.
   */
  void SetMinDistance (double minDistance);

  /**
   * \returns the minimum distance.
   */
  double GetMinDistance (void) const;

  /**
   * \returns the current wavelength (m)
   */
  double GetLambda (void) const;
  /**
   * \returns the current system loss (dimention-less)
   */
  double GetSystemLoss (void) const;

  virtual double GetLoss (Ptr<MobilityModel> a,
			  Ptr<MobilityModel> b) const;
private:
  double DbmToW (double dbm) const;
  double DbmFromW (double w) const;

  static const double PI;
  double m_lambda;
  double m_systemLoss;
  double m_minDistance;
};

/**
 * \brief a log distance propagation model.
 *
 * This model calculates the reception power with a so-called
 * log-distance propagation model:
 * \f$ L = L_0 + 10 n log_{10}(\frac{d}{d_0})\f$
 *
 * where:
 *  - \f$ n \f$ : the path loss distance exponent
 *  - \f$ d_0 \f$ : reference distance (m)
 *  - \f$ L_0 \f$ : path loss at reference distance (dB)
 *  - \f$ d \f$ : distance (m)
 *  - \f$ L \f$ : path loss (dB)
 *
 * When the path loss is requested at a distance smaller than
 * the reference distance, the tx power is returned.
 *
 */
class LogDistancePropagationLossModel : public PropagationLossModel
{
public:
  static TypeId GetTypeId (void);
  LogDistancePropagationLossModel ();

  /**
   * \param n the path loss exponent.
   * Set the path loss exponent.
   */
  void SetPathLossExponent (double n);
  /** 
   * \returns the current path loss exponent.
   */
  double GetPathLossExponent (void) const;

  /**
   * \param model the reference propagation model
   */
  void SetReferenceModel (Ptr<PropagationLossModel> model);

  void SetReferenceDistance (double referenceDistance);
  
  virtual double GetLoss (Ptr<MobilityModel> a,
			  Ptr<MobilityModel> b) const;
private:
  static Ptr<PropagationLossModel> CreateDefaultReference (void);

  double m_exponent;
  double m_referenceDistance;
  Ptr<PropagationLossModel> m_reference;
};

} // namespace ns3

#endif /* PROPAGATION_LOSS_MODEL_H */
