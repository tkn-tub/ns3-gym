/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 TELEMATICS LAB, DEE - Politecnico di Bari
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
 * Author: Giuseppe Piro  <g.piro@poliba.it>
 */


#ifndef LTE_SPECTRUM_PROPAGATION_LOSS_MODEL_H
#define LTE_SPECTRUM_PROPAGATION_LOSS_MODEL_H


#include <ns3/object.h>
#include <ns3/spectrum-propagation-loss-model.h>
#include <map>
#include "channel-realization.h"

namespace ns3 {


class MobilityModel;

/**
 * \ingroup lte
 *
 * \brief propagation loss model for LTE
 */
class LtePropagationLossModel : public SpectrumPropagationLossModel
{
public:
  LtePropagationLossModel ();
  virtual ~LtePropagationLossModel ();

  static TypeId GetTypeId ();

  /**
   * \brief The couple of mobility mnode that form a channel realization
   */
  typedef std::pair<Ptr<const MobilityModel>, Ptr<const MobilityModel> > ChannelRealizationId_t;

  /**
   * \brief Create a channel realization among two device
   * \param enbMobility mobility object of the enb
   * \param ueMobility mobility object of the ue
   */
  void CreateChannelRealization (Ptr<const MobilityModel> enbMobility, Ptr<const MobilityModel> ueMobility);
  /**
   * \brief Get a channel realization among two device
   * \param a the mobility object of the enb
   * \param b the mobility object of the ue
   * \return the pointer to the channel realization
   */
  Ptr<ChannelRealization> GetChannelRealization (Ptr<const MobilityModel> a, Ptr<const MobilityModel> b) const;

private:
  /**
   * @param txPower set of values vs frequency representing the
   * transmission power. See SpectrumChannel for details.
   *
   * @param a sender mobility
   * @param b receiver mobility
   *
   * @return set of values vs frequency representing the received
   * power in the same units used for the txPower parameter.
   */
  Ptr<SpectrumValue> DoCalcRxPowerSpectralDensity (Ptr<const SpectrumValue> txPsd,
                                                   Ptr<const MobilityModel> a,
                                                   Ptr<const MobilityModel> b) const;

  std::map <ChannelRealizationId_t, Ptr<ChannelRealization> > m_channelRealizationMap;
};






} // namespace ns3

#endif /* LTE_SPECTRUM_PROPAGATION_LOSS_MODEL_H */
