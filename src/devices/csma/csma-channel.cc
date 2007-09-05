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

#include "csma-channel.h"
#include "csma-net-device.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/debug.h"

NS_DEBUG_COMPONENT_DEFINE ("CsmaChannel");

namespace ns3 {

CsmaDeviceRec::CsmaDeviceRec()
{
  active = false;
}

CsmaDeviceRec::CsmaDeviceRec(Ptr<CsmaNetDevice> device)
{
  devicePtr = device; 
  active = true;
}

bool
CsmaDeviceRec::IsActive() {
  return active;
}


//
// By default, you get a channel with the name "Csma Channel" that 
// has an "infitely" fast transmission speed and zero delay.
CsmaChannel::CsmaChannel()
: 
  Channel ("Csma Channel"), 
  m_bps (DataRate(0xffffffff)),
  m_delay (Seconds(0))
{
  NS_DEBUG("CsmaChannel::CsmaChannel ()");
  Init();
}

CsmaChannel::CsmaChannel(
  const DataRate& bps, 
  const Time& delay)
: 
  Channel ("Csma Channel"), 
  m_bps (bps),
  m_delay (delay)
{
  NS_DEBUG("CsmaChannel::CsmaChannel (" << Channel::GetName() 
    << ", " << bps.GetBitRate() << ", " << delay << ")");
  Init();
}

CsmaChannel::CsmaChannel(
  const std::string& name,
  const DataRate& bps, 
  const Time& delay)
: 
  Channel (name),
  m_bps (bps), 
  m_delay (delay)
{
  NS_DEBUG("CsmaChannel::CsmaChannel (" << name << ", " << 
           bps.GetBitRate() << ", " << delay << ")");
  Init();
}

void CsmaChannel::Init() {
  m_state = IDLE;
  m_deviceList.clear();
}

int32_t
CsmaChannel::Attach(Ptr<CsmaNetDevice> device)
{
  NS_DEBUG("CsmaChannel::Attach (" << device << ")");
  NS_ASSERT(device != 0);

  CsmaDeviceRec rec(device);
  
  m_deviceList.push_back(rec);
  return (m_deviceList.size() - 1);
}

bool
CsmaChannel::Reattach(Ptr<CsmaNetDevice> device)
{
  NS_DEBUG("CsmaChannel::Reattach (" << device << ")");
  NS_ASSERT(device != 0);

  std::vector<CsmaDeviceRec>::iterator it;
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
CsmaChannel::Reattach(uint32_t deviceId)
{
  NS_DEBUG("CsmaChannel::Reattach (" << deviceId << ")");
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
CsmaChannel::Detach(uint32_t deviceId)
{
  NS_DEBUG("CsmaChannel::Detach (" << deviceId << ")");

  if (deviceId < m_deviceList.size())
    {
      if (!m_deviceList[deviceId].active)
        {
          NS_DEBUG("CsmaChannel::Detach Device is already detached (" 
                   << deviceId << ")");
          return false;
        }

      m_deviceList[deviceId].active = false;
      if ((m_state == TRANSMITTING) && (m_currentSrc == deviceId))
        {
          NS_DEBUG("CsmaChannel::Detach Device is currently"
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
CsmaChannel::Detach(Ptr<CsmaNetDevice> device)
{
  NS_DEBUG("CsmaChannel::Detach (" << device << ")");
  NS_ASSERT(device != 0);

  std::vector<CsmaDeviceRec>::iterator it;
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
CsmaChannel::TransmitStart(Packet& p, uint32_t srcId)
{
  NS_DEBUG ("CsmaChannel::TransmitStart (" << &p << ", " << srcId 
            << ")");
  NS_DEBUG ("CsmaChannel::TransmitStart (): UID is " << 
            p.GetUid () << ")");

  if (m_state != IDLE)
    {
      NS_DEBUG("CsmaChannel::TransmitStart (): state is not IDLE");
      return false;
    }

  if (!IsActive(srcId))
    {
      NS_DEBUG("CsmaChannel::TransmitStart (): ERROR: Seclected "
               << "source is not currently attached to network");
      return false;
    }

  NS_DEBUG("CsmaChannel::TransmitStart (): switch to TRANSMITTING");
  m_currentPkt = p;
  m_currentSrc = srcId;
  m_state = TRANSMITTING;
  return true;
}

bool
CsmaChannel::IsActive(uint32_t deviceId) 
{
    return (m_deviceList[deviceId].active);
}

bool
CsmaChannel::TransmitEnd()
{
  NS_DEBUG("CsmaChannel::TransmitEnd (" << &m_currentPkt << ", " 
           << m_currentSrc << ")");
  NS_DEBUG("CsmaChannel::TransmitEnd (): UID is " << 
            m_currentPkt.GetUid () << ")");

  NS_ASSERT(m_state == TRANSMITTING);
  m_state = PROPAGATING;

  bool retVal = true;

  if (!IsActive(m_currentSrc)) {
    NS_DEBUG("CsmaChannel::TransmitEnd (): ERROR: Seclected source "
             << "was detached before the end of the transmission");
    retVal = false;
  }

  NS_DEBUG ("CsmaChannel::TransmitEnd (): Schedule event in " << 
            m_delay.GetSeconds () << "sec");

  Simulator::Schedule (m_delay,
                       &CsmaChannel::PropagationCompleteEvent,
                       this);
  return retVal;
}

void
CsmaChannel::PropagationCompleteEvent()
{
  NS_DEBUG("CsmaChannel::PropagationCompleteEvent (" 
           << &m_currentPkt << ")");
  NS_DEBUG ("CsmaChannel::PropagationCompleteEvent (): UID is " << 
            m_currentPkt.GetUid () << ")");

  NS_ASSERT(m_state == PROPAGATING);

  NS_DEBUG ("CsmaChannel::PropagationCompleteEvent (): Receive");
  
  std::vector<CsmaDeviceRec>::iterator it;
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
CsmaChannel::GetNumActDevices (void)
{
  int numActDevices = 0;
  std::vector<CsmaDeviceRec>::iterator it;
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
CsmaChannel::GetNDevices (void) const
{
  return (m_deviceList.size());
}

Ptr<NetDevice>
CsmaChannel::GetDevice (uint32_t i) const
{
  Ptr< CsmaNetDevice > netDevice;

  netDevice = m_deviceList[i].devicePtr;
  return netDevice;
}

int32_t
CsmaChannel::GetDeviceNum (Ptr<CsmaNetDevice> device)
{
  std::vector<CsmaDeviceRec>::iterator it;
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
CsmaChannel::IsBusy (void)
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
CsmaChannel::GetDataRate (void)
{
  return m_bps;
}

Time
CsmaChannel::GetDelay (void)
{
  return m_delay;
}

WireState
CsmaChannel::GetState(void)
{
  return m_state;
}

} // namespace ns3
