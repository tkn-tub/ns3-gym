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


#include <ns3/log.h>
#include <ns3/simulator.h>
#include "channel-realization.h"


NS_LOG_COMPONENT_DEFINE ("ChannelRealization");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (ChannelRealization);

ChannelRealization::ChannelRealization ()
{
  m_shadowing = CreateObject<ShadowingLossModel> ();
  m_pathloss = CreateObject<PathLossModel> ();
  m_multipath = CreateObject<JakesFadingLossModel> ();
  m_penetration = CreateObject<PenetrationLossModel> ();
}


TypeId
ChannelRealization::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ChannelRealization")
    .SetParent<Object> ()
    .AddConstructor<ChannelRealization> ()
  ;
  return tid;
}


ChannelRealization::~ChannelRealization ()
{
}


void
ChannelRealization::SetJakesFadingLossModel (Ptr<JakesFadingLossModel> l)
{
  NS_LOG_FUNCTION (this);
  m_multipath = l;
}


void
ChannelRealization::SetPathLossModel (Ptr<PathLossModel> l)
{
  NS_LOG_FUNCTION (this << l);
  m_pathloss = l;
}


void
ChannelRealization::SetShadowingLossModel (Ptr<ShadowingLossModel> l)
{
  NS_LOG_FUNCTION (this << l);
  m_shadowing = l;
}


void
ChannelRealization::SetPenetrationLossModel (Ptr<PenetrationLossModel> l)
{
  NS_LOG_FUNCTION (this << l);
  m_penetration = l;
}


Ptr<JakesFadingLossModel>
ChannelRealization::GetJakesFadingLossModel (void)
{
  NS_LOG_FUNCTION (this);
  return m_multipath;
}


Ptr<PathLossModel>
ChannelRealization::GetPathLossModel (void)
{
  NS_LOG_FUNCTION (this);
  return m_pathloss;
}


Ptr<ShadowingLossModel>
ChannelRealization::GetShadowingLossModel (void)
{
  NS_LOG_FUNCTION (this);
  return m_shadowing;
}


Ptr<PenetrationLossModel>
ChannelRealization::GetPenetrationLossModel (void)
{
  NS_LOG_FUNCTION (this);
  return m_penetration;
}

} // namespace ns3
