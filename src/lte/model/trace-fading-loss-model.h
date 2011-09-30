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
 * Author: Marco Miozzo <mmiozzo@cttc.es>
 */


#ifndef TRACE_FADING_LOSS_MODEL_H
#define TRACE_FADING_LOSS_MODEL_H


#include <ns3/object.h>
#include <ns3/spectrum-propagation-loss-model.h>
#include <map>
//#include "channel-realization.h"
#include <ns3/jakes-fading-loss-model.h>

namespace ns3 {


class MobilityModel;

/**
 * \ingroup lte
 *
 * \brief fading loss model based on precalculated fading traces
 */
class TraceFadingLossModel : public SpectrumPropagationLossModel
{
public:
  TraceFadingLossModel ();
  virtual ~TraceFadingLossModel ();

  static TypeId GetTypeId ();

  /**
   * \brief The couple of mobility mnode that form a fading channel realization
   */
  typedef std::pair<Ptr<const MobilityModel>, Ptr<const MobilityModel> > ChannelRealizationId_t;

  /**
   * \brief Create a fading channel realization among two device
   * \param enbMobility mobility object of the enb
   * \param ueMobility mobility object of the ue
   */
  void CreateFadingChannelRealization (Ptr<const MobilityModel> enbMobility, Ptr<const MobilityModel> ueMobility);
  /**
   * \brief Get a fading channel realization among two device
   * \param a the mobility object of the enb
   * \param b the mobility object of the ue
   * \return the pointer to the channel realization
   */
  Ptr<JakesFadingLossModel> GetFadingChannelRealization (Ptr<const MobilityModel> a, Ptr<const MobilityModel> b) const;

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

  std::map <ChannelRealizationId_t, Ptr<JakesFadingLossModel> > m_channelRealizationMap;
};






} // namespace ns3

#endif /* TRACE_FADING_LOSS_MODEL_H */
