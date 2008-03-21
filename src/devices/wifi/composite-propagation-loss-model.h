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
 * Author: Federico Maguolo <maguolof@dei.unipd.it>
 */
#ifndef PROPAGATION_COMP_MODEL_H
#define PROPAGATION_COMP_MODEL_H

#include "ns3/nstime.h"
#include "propagation-loss-model.h"

namespace ns3 {

/**
 * \brief a Composite propagation loss model
 *
 * This model is use to compute the receivng power
 * using more than one propagation loss model (e.g. distance loss 
 * and jakes).
 *
 * The received power is computed considering the cascade of the models
 */
class CompositePropagationLossModel : public PropagationLossModel {
public:
  static TypeId GetTypeId (void);
  CompositePropagationLossModel ();
  virtual ~CompositePropagationLossModel ();

  virtual double GetLoss (Ptr<MobilityModel> a,
			  Ptr<MobilityModel> b) const;
  /**
   * \param pl the propagation loss model to add
   *
   * Insert a propagation loss model to the composite.
   */
  void AddPropagationLossModel (Ptr<PropagationLossModel> pl);
protected:
  virtual void AddDefaults (void);
private:
  typedef std::vector<Ptr<PropagationLossModel> > PropagationModelList;
  PropagationModelList m_propagationModels;
};


} // namespace ns3

#endif /* PROPAGATION_COMP_MODEL_H */


