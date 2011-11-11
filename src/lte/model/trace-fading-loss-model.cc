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
 * Author: Marco Miozzo <marco.miozzo@cttc.es>
 */


#include "trace-fading-loss-model.h"
#include <ns3/mobility-model.h>
#include <ns3/spectrum-value.h>
#include <ns3/log.h>

NS_LOG_COMPONENT_DEFINE ("TraceFadingLossModel");

namespace ns3 {

  NS_OBJECT_ENSURE_REGISTERED (TraceFadingLossModel);

TraceFadingLossModel::TraceFadingLossModel ()
{
  SetNext (NULL);
}


TraceFadingLossModel::~TraceFadingLossModel ()
{
}


TypeId
TraceFadingLossModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TraceFadingLossModel")
    .SetParent<SpectrumPropagationLossModel> ()
  ;
  return tid;
}



Ptr<SpectrumValue>
TraceFadingLossModel::DoCalcRxPowerSpectralDensity (
  Ptr<const SpectrumValue> txPsd,
  Ptr<const MobilityModel> a,
  Ptr<const MobilityModel> b) const
{
  NS_LOG_FUNCTION (this << *txPsd << a << b);
  
  Ptr<JakesFadingLossModel> c = GetFadingChannelRealization (a,b);

  Ptr<SpectrumValue> rxPsd = Copy<SpectrumValue> (txPsd);
  Values::iterator vit = rxPsd->ValuesBegin ();
  
  Vector aSpeedVector = a->GetVelocity ();
  Vector bSpeedVector = b->GetVelocity ();
  
  double speed = sqrt (pow (aSpeedVector.x-bSpeedVector.x,2) +  pow (aSpeedVector.y-bSpeedVector.y,2));

  NS_LOG_FUNCTION (this << *rxPsd);

  int subChannel = 0;
  while (vit != rxPsd->ValuesEnd ())
    {
      NS_ASSERT (subChannel < 100);
      if (*vit != 0.)
        {
          double fading = c->GetValue (subChannel, speed);

          double power = *vit; // in Watt/Hz
          power = 10 * log10 (180000 * power); // in dB

          NS_LOG_FUNCTION (this << subChannel << *vit  << power << fading);

          *vit = pow (10., ((power + fading) / 10)) / 180000; // in Watt

          NS_LOG_FUNCTION (this << subChannel << *vit);

        }

      ++vit;
      ++subChannel;

    }

  NS_LOG_FUNCTION (this << *rxPsd);
  return rxPsd;
}


void
TraceFadingLossModel::CreateFadingChannelRealization (Ptr<const MobilityModel> enbMobility, Ptr<const MobilityModel> ueMobility)
{
  NS_LOG_FUNCTION (this << enbMobility << ueMobility);

  Ptr<JakesFadingLossModel> c = CreateObject<JakesFadingLossModel> ();
  ChannelRealizationId_t mobilities = std::make_pair (enbMobility, ueMobility);

  NS_LOG_FUNCTION (this <<
                   "insert new channel realization, m_channelRealizationMap.size () = "
                        << m_channelRealizationMap.size ());

  m_channelRealizationMap.insert ( std::pair<ChannelRealizationId_t,Ptr<JakesFadingLossModel> > (mobilities, c) );

  NS_LOG_FUNCTION (this <<
                   "m_channelRealizationMap.size () = "
                        << m_channelRealizationMap.size ());

}


Ptr<JakesFadingLossModel>
TraceFadingLossModel::GetFadingChannelRealization (Ptr<const MobilityModel> a, Ptr<const MobilityModel> b) const
{

  NS_LOG_FUNCTION (this << a << b);

  std::map <ChannelRealizationId_t, Ptr<JakesFadingLossModel> >::const_iterator it;

  ChannelRealizationId_t mobilities = std::make_pair (a,b);
  it = m_channelRealizationMap.find (mobilities);

  return it->second;
}

} // namespace ns3
