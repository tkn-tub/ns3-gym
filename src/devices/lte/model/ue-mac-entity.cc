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


#include "ue-mac-entity.h"
#include <ns3/log.h>
#include <ns3/pointer.h>
#include <ns3/packet.h>
#include "packet-scheduler.h"
#include "amc-module.h"
#include "ideal-control-messages.h"
#include "lte-net-device.h"
#include "ue-net-device.h"
#include "enb-net-device.h"
#include "ue-phy.h"

NS_LOG_COMPONENT_DEFINE ("UeMacEntity");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (UeMacEntity);

TypeId UeMacEntity::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UeMacEntity")
    .SetParent<MacEntity> ();
  return tid;
}


UeMacEntity::UeMacEntity ()
{
  SetAmcModule (CreateObject<AmcModule> ());
}


UeMacEntity::~UeMacEntity ()
{
}

Ptr<CqiIdealControlMessage> 
UeMacEntity::CreateCqiFeedbacks (std::vector<double> sinr)
{
  NS_LOG_FUNCTION (this);

  Ptr<UeNetDevice> thisDevice = GetDevice ()->GetObject<UeNetDevice> ();
  Ptr<EnbNetDevice> remoteDevice = thisDevice->GetTargetEnb ();
  Ptr<UeLtePhy> phy = thisDevice->GetPhy ()->GetObject<UeLtePhy> ();

  Ptr<AmcModule> amc = GetAmcModule ();
  std::vector<int> cqi = amc->CreateCqiFeedbacks (sinr);


  // CREATE CqiIdealControlMessage
  Ptr<CqiIdealControlMessage> msg = Create<CqiIdealControlMessage> ();

  msg->SetSourceDevice (thisDevice);
  msg->SetDestinationDevice (remoteDevice);

  int nbSubChannels = cqi.size ();
  for (int i = 0; i < nbSubChannels; i++)
    {
      msg->AddNewRecord (phy->GetDownlinkSubChannels ().at (i), cqi.at (i));
    }

  return msg;
}


} // namespace ns3
