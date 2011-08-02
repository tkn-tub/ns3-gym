/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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


#include "lte-propagation-loss-model.h"
#include <ns3/mobility-model.h>
#include <ns3/spectrum-value.h>
#include <ns3/log.h>

NS_LOG_COMPONENT_DEFINE ("LtePropagationLossModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LtePropagationLossModel);

LtePropagationLossModel::LtePropagationLossModel ()
{
  SetNext (NULL);
}


LtePropagationLossModel::~LtePropagationLossModel ()
{
}


TypeId
LtePropagationLossModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LtePropagationLossModel")
    .SetParent<SpectrumPropagationLossModel> ()
  ;
  return tid;
}



Ptr<SpectrumValue>
LtePropagationLossModel::DoCalcRxPowerSpectralDensity (Ptr<const SpectrumValue> txPsd,
                                                       Ptr<const MobilityModel> a,
                                                       Ptr<const MobilityModel> b) const
{
  NS_LOG_FUNCTION (this << *txPsd << a << b);
  /*
   * The loss propagation model for LTE networks is based on
   * a on a combination of four different models:
   * - the path loss
   * - the penetration loss
   * - the shadowind
   * - the multipath
   *
   * The rxPsd will be obtained considering, for each sub channel, the following
   * relations:
   * rxPsd (i) = txPsd (i) + m(i,t) - sh(i,t) - pnl(i,t) - pl (a,b);
   * where i is the i-th sub-channel and t is the current time (Simulator::Now()).
   */


  Ptr<ChannelRealization> c = GetChannelRealization (a,b);

  double multipath; // its value is different for each sub channels
  double pathLoss = c->GetPathLossModel ()->GetValue (a,b);
  double shadowind = c->GetShadowingLossModel ()->GetValue ();
  double penetration = c->GetPenetrationLossModel ()->GetValue ();


  Ptr<SpectrumValue> rxPsd = Copy<SpectrumValue> (txPsd);
  Values::iterator vit = rxPsd->ValuesBegin ();

  NS_LOG_FUNCTION (this << *rxPsd);

  int subChannel = 0;
  while (vit != rxPsd->ValuesEnd ())
    {
      NS_ASSERT (subChannel < 100);
      if (*vit != 0.)
        {
          multipath = c->GetJakesFadingLossModel ()->GetValue (subChannel);

          // computei PROPRAGATION LOSS:
          double loss = multipath - pathLoss - shadowind - penetration; // in dB

          double power = *vit; // in Watt/Hz
          power = 10 * log10 (180000 * power); // in dB

          NS_LOG_FUNCTION (this << subChannel << *vit  << power << multipath
                                << pathLoss << shadowind  << penetration);

          *vit = pow (10., ((power + loss) / 10)) / 180000; // in Watt

          NS_LOG_FUNCTION (this << subChannel << *vit);

        }

      ++vit;
      ++subChannel;

    }

  NS_LOG_FUNCTION (this << *rxPsd);
  return rxPsd;
}


void
LtePropagationLossModel::CreateChannelRealization (Ptr<const MobilityModel> enbMobility, Ptr<const MobilityModel> ueMobility)
{
  NS_LOG_FUNCTION (this << enbMobility << ueMobility);

  Ptr<ChannelRealization> c = CreateObject<ChannelRealization> ();
  ChannelRealizationId_t mobilities = std::make_pair (enbMobility, ueMobility);

  NS_LOG_FUNCTION (this <<
                   "insert new channel realization, m_channelRealizationMap.size () = "
                        << m_channelRealizationMap.size ());

  m_channelRealizationMap.insert ( std::pair<ChannelRealizationId_t,Ptr<ChannelRealization> > (mobilities, c) );

  NS_LOG_FUNCTION (this <<
                   "m_channelRealizationMap.size () = "
                        << m_channelRealizationMap.size ());

}


Ptr<ChannelRealization>
LtePropagationLossModel::GetChannelRealization (Ptr<const MobilityModel> a, Ptr<const MobilityModel> b) const
{

  NS_LOG_FUNCTION (this << a << b);

  std::map <ChannelRealizationId_t, Ptr<ChannelRealization> >::const_iterator it;

  ChannelRealizationId_t mobilities = std::make_pair (a,b);
  it = m_channelRealizationMap.find (mobilities);

  return it->second;
}

} // namespace ns3
