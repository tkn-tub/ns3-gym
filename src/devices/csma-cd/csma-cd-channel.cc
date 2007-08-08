/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 Emmanuelle Laprise
 * All rights reserved.
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
 * Author: Emmanuelle Laprise <emmanuelle.laprise@bluekazoo.ca>
 */

#include "csma-cd-channel.h"
#include "csma-cd-net-device.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/debug.h"

NS_DEBUG_COMPONENT_DEFINE ("CsmaCdChannel");

namespace ns3 {

CsmaCdDeviceRec::CsmaCdDeviceRec()
{
  active = false;
}

CsmaCdDeviceRec::CsmaCdDeviceRec(Ptr<CsmaCdNetDevice> device)
{
  devicePtr = device; 
  active = true;
}

bool
CsmaCdDeviceRec::IsActive() {
  return active;
}


//
// By default, you get a channel with the name "CsmaCd Channel" that 
// has an "infitely" fast transmission speed and zero delay.
CsmaCdChannel::CsmaCdChannel()
: 
  Channel ("CsmaCd Channel"), 
  m_bps (DataRate(0xffffffff)),
  m_delay (Seconds(0))
{
  NS_DEBUG("CsmaCdChannel::CsmaCdChannel ()");
  Init();
}

CsmaCdChannel::CsmaCdChannel(
  const DataRate& bps, 
  const Time& delay)
: 
  Channel ("CsmaCd Channel"), 
  m_bps (bps),
  m_delay (delay)
{
  NS_DEBUG("CsmaCdChannel::CsmaCdChannel (" << Channel::GetName() 
    << ", " << bps.GetBitRate() << ", " << delay << ")");
  Init();
}

CsmaCdChannel::CsmaCdChannel(
  const std::string& name,
  const DataRate& bps, 
  const Time& delay)
: 
  Channel (name),
  m_bps (bps), 
  m_delay (delay)
{
  NS_DEBUG("CsmaCdChannel::CsmaCdChannel (" << name << ", " << 
           bps.GetBitRate() << ", " << delay << ")");
  Init();
}

void CsmaCdChannel::Init() {
  m_state = IDLE;
  m_deviceList.clear();
}

int32_t
CsmaCdChannel::Attach(Ptr<CsmaCdNetDevice> device)
{
  NS_DEBUG("CsmaCdChannel::Attach (" << device << ")");
  NS_ASSERT(device != 0);

  CsmaCdDeviceRec rec(device);
  
  m_deviceList.push_back(rec);
  return (m_deviceList.size() - 1);
}

bool
CsmaCdChannel::Reattach(Ptr<CsmaCdNetDevice> device)
{
  NS_DEBUG("CsmaCdChannel::Reattach (" << device << ")");
  NS_ASSERT(device != 0);

  std::vector<CsmaCdDeviceRec>::iterator it;
  for (it = m_deviceList.begin(); it < m_deviceList.end(); it++) 
    {
      if (it->devicePtr == device) 
        {
          if (!it->active) 
            {
              it->active = true;
              return true;
            } 
          else 
            {
              return false;
            }
        }
    }
  return false;
}

bool
CsmaCdChannel::Reattach(uint32_t deviceId)
{
  NS_DEBUG("CsmaCdChannel::Reattach (" << deviceId << ")");
  if (deviceId < m_deviceList.size())
    {
      return false;
    }

  if (m_deviceList[deviceId].active)
    {
      return false;
    } 
  else 
    {
      m_deviceList[deviceId].active = true;
      return true;
    }
}

bool
CsmaCdChannel::Detach(uint32_t deviceId)
{
  NS_DEBUG("CsmaCdChannel::Detach (" << deviceId << ")");

  if (deviceId < m_deviceList.size())
    {
      if (!m_deviceList[deviceId].active)
        {
          NS_DEBUG("CsmaCdChannel::Detach Device is already detached (" 
                   << deviceId << ")");
          return false;
        }

      m_deviceList[deviceId].active = false;
      if ((m_state == TRANSMITTING) && (m_currentSrc == deviceId))
        {
          NS_DEBUG("CsmaCdChannel::Detach Device is currently"
                   << "transmitting (" << deviceId << ")");
          // Here we will need to place a warning in the packet
        }

      return true;
    } 
  else 
    {
      return false;
    }
}

bool
CsmaCdChannel::Detach(Ptr<CsmaCdNetDevice> device)
{
  NS_DEBUG("CsmaCdChannel::Detach (" << device << ")");
  NS_ASSERT(device != 0);

  std::vector<CsmaCdDeviceRec>::iterator it;
  for (it = m_deviceList.begin(); it < m_deviceList.end(); it++) 
    {
      if ((it->devicePtr == device) && (it->active)) 
        {
          it->active = false;
          return true;
        }
    }
  return false;
}

bool
CsmaCdChannel::TransmitStart(Packet& p, uint32_t srcId)
{
  NS_DEBUG ("CsmaCdChannel::TransmitStart (" << &p << ", " << srcId 
            << ")");
  NS_DEBUG ("CsmaCdChannel::TransmitStart (): UID is " << 
            p.GetUid () << ")");

  if (m_state != IDLE)
    {
      NS_DEBUG("CsmaCdChannel::TransmitStart (): state is not IDLE");
      return false;
    }

  if (!IsActive(srcId))
    {
      NS_DEBUG("CsmaCdChannel::TransmitStart (): ERROR: Seclected "
               << "source is not currently attached to network");
      return false;
    }

  NS_DEBUG("CsmaCdChannel::TransmitStart (): switch to TRANSMITTING");
  m_currentPkt = p;
  m_currentSrc = srcId;
  m_state = TRANSMITTING;
  return true;
}

bool
CsmaCdChannel::IsActive(uint32_t deviceId) 
{
    return (m_deviceList[deviceId].active);
}

bool
CsmaCdChannel::TransmitEnd()
{
  NS_DEBUG("CsmaCdChannel::TransmitEnd (" << &m_currentPkt << ", " 
           << m_currentSrc << ")");
  NS_DEBUG("CsmaCdChannel::TransmitEnd (): UID is " << 
            m_currentPkt.GetUid () << ")");

  NS_ASSERT(m_state == TRANSMITTING);
  m_state = PROPAGATING;

  bool retVal = true;

  if (!IsActive(m_currentSrc)) {
    NS_DEBUG("CsmaCdChannel::TransmitEnd (): ERROR: Seclected source "
             << "was detached before the end of the transmission");
    retVal = false;
  }

  NS_DEBUG ("CsmaCdChannel::TransmitEnd (): Schedule event in " << 
            m_delay.GetSeconds () << "sec");

  Simulator::Schedule (m_delay,
                       &CsmaCdChannel::PropagationCompleteEvent,
                       this);
  return retVal;
}

void
CsmaCdChannel::PropagationCompleteEvent()
{
  NS_DEBUG("CsmaCdChannel::PropagationCompleteEvent (" 
           << &m_currentPkt << ")");
  NS_DEBUG ("CsmaCdChannel::PropagationCompleteEvent (): UID is " << 
            m_currentPkt.GetUid () << ")");

  NS_ASSERT(m_state == PROPAGATING);

  NS_DEBUG ("CsmaCdChannel::PropagationCompleteEvent (): Receive");
  
  std::vector<CsmaCdDeviceRec>::iterator it;
  for (it = m_deviceList.begin(); it < m_deviceList.end(); it++) 
    {
      if (it->IsActive())
      {
        it->devicePtr->Receive (m_currentPkt);
      }
    }
  m_state = IDLE;
}


uint32_t 
CsmaCdChannel::GetNumActDevices (void)
{
  int numActDevices = 0;
  std::vector<CsmaCdDeviceRec>::iterator it;
  for (it = m_deviceList.begin(); it < m_deviceList.end(); it++) 
    {
      if (it->active)
        {
          numActDevices++;
        }
    }
  return numActDevices;
}

// This is not the number of active devices. This is the total number
// of devices even if some were detached after.
uint32_t 
CsmaCdChannel::GetNDevices (void) const
{
  return (m_deviceList.size());
}

Ptr<NetDevice>
CsmaCdChannel::GetDevice (uint32_t i) const
{
  Ptr< CsmaCdNetDevice > netDevice;

  netDevice = m_deviceList[i].devicePtr;
  return netDevice;
}

int32_t
CsmaCdChannel::GetDeviceNum (Ptr<CsmaCdNetDevice> device)
{
  std::vector<CsmaCdDeviceRec>::iterator it;
  int i = 0;
  for (it = m_deviceList.begin(); it < m_deviceList.end(); it++) 
    {
      if (it->devicePtr == device)
        {
          if (it->active) 
            {
              return i;
            } 
          else 
            {
              return -2;
            }
        }
      i++;
    }
  return -1;
}

bool 
CsmaCdChannel::IsBusy (void)
{
  if (m_state == IDLE) 
    {
      return false;
    } 
  else 
    {
      return true;
    }
}

DataRate
CsmaCdChannel::GetDataRate (void)
{
  return m_bps;
}

Time
CsmaCdChannel::GetDelay (void)
{
  return m_delay;
}

WireState
CsmaCdChannel::GetState(void)
{
  return m_state;
}

} // namespace ns3
