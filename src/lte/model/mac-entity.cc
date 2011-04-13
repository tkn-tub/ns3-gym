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


#include "mac-entity.h"
#include <ns3/log.h>
#include <ns3/pointer.h>
#include <ns3/packet.h>
#include "amc-module.h"
#include "lte-net-device.h"

NS_LOG_COMPONENT_DEFINE ("MacEntity");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (MacEntity);

TypeId MacEntity::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MacEntity")
    .SetParent<Object> ();
  return tid;
}


MacEntity::MacEntity ()
{
  m_amcModule = CreateObject<AmcModule> ();
  m_device = 0;
}


MacEntity::~MacEntity ()
{
  m_amcModule = 0;
  m_device = 0;
}

void 
MacEntity::DoDispose (void)
{
  m_amcModule = 0;
  m_device = 0;
}

void
MacEntity::SetDevice (Ptr<LteNetDevice> d)
{
  NS_LOG_FUNCTION (this << d);
  m_device = d;
}


Ptr<LteNetDevice>
MacEntity::GetDevice ()
{
  NS_LOG_FUNCTION (this);
  return m_device;
}

void
MacEntity::SetAmcModule (Ptr<AmcModule> amc)
{
  NS_LOG_FUNCTION (this << amc);
  m_amcModule = amc;
}


Ptr<AmcModule>
MacEntity::GetAmcModule (void) const
{
  NS_LOG_FUNCTION (this);
  return m_amcModule;
}


} // namespace ns3
