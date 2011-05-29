/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 The Boeing Company
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
 * Authors:
 *  Gary Pei <guangyu.pei@boeing.com>
 */

#include "lr-wpan-helper.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("LrWpanHelper");

namespace ns3 {

LrWpanHelper::LrWpanHelper (void)
{
  m_channel = CreateObject<SingleModelSpectrumChannel> ();
  Ptr<WpanSpectrumPropagationLossModel> model = CreateObject<WpanSpectrumPropagationLossModel> ();
  m_channel->AddSpectrumPropagationLossModel (model);
}

LrWpanHelper::~LrWpanHelper (void)
{
  m_channel = 0;
}

void
LrWpanHelper::AddMobility (Ptr<LrWpanPhy> phy, Ptr<MobilityModel> m)
{
  phy->SetMobility (m);
}


NetDeviceContainer
LrWpanHelper::Install (NodeContainer c)
{
  NetDeviceContainer devices;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); i++)
    {
      Ptr<Node> node = *i;

      Ptr<LrWpanMac> mac = CreateObject<LrWpanMac> ();
      Ptr<LrWpanPhy> phy = CreateObject<LrWpanPhy> ();
      Ptr<LrWpanCsmaCa> csmaca = CreateObject<LrWpanCsmaCa> ();
      // Set MAC-PHY SAPs
      phy->SetPdDataIndicationCallback (MakeCallback (&LrWpanMac::PdDataIndication, mac));
      phy->SetPdDataConfirmCallback (MakeCallback (&LrWpanMac::PdDataConfirm, mac));
      phy->SetPlmeEdConfirmCallback (MakeCallback (&LrWpanMac::PlmeEdConfirm, mac));
      phy->SetPlmeGetAttributeConfirmCallback (MakeCallback (&LrWpanMac::PlmeGetAttributeConfirm, mac));
      phy->SetPlmeSetTRXStateConfirmCallback (MakeCallback (&LrWpanMac::PlmeSetTRXStateConfirm, mac));
      phy->SetPlmeSetAttributeConfirmCallback (MakeCallback (&LrWpanMac::PlmeSetAttributeConfirm, mac));

      mac->SetCsmaCa (csmaca);
      csmaca->SetMac (mac);
      csmaca->SetLrWpanMacStateCallback (MakeCallback (&LrWpanMac::SetLrWpanMacState, mac));
      phy->SetPlmeCcaConfirmCallback (MakeCallback (&LrWpanCsmaCa::PlmeCcaConfirm, csmaca));

      // Set Channel
      phy->SetChannel (m_channel);
      m_channel->AddRx (phy);
    }
  return devices;
}

void
LrWpanHelper::EnableLogComponents (void)
{
  LogComponentEnableAll (LOG_PREFIX_TIME);
  LogComponentEnableAll (LOG_PREFIX_FUNC);
  LogComponentEnable ("LrWpanCsmaCa", LOG_LEVEL_ALL);
  LogComponentEnable ("LrWpanPhy", LOG_LEVEL_ALL);
  LogComponentEnable ("LrWpanMac", LOG_LEVEL_ALL);
  LogComponentEnable ("LrWpanErrorRateModel", LOG_LEVEL_ALL);
}


} // namespace ns3

