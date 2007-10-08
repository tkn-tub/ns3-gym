/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "mac-stations.h"
#include "ns3/assert.h"

namespace ns3 {

class NonUnicastMacStation : public MacStation
{
public:
};

} // namespace ns3

namespace ns3 {

MacStations::MacStations (WifiMode defaultTxMode)
  : m_nonUnicast (0),
    m_defaultTxMode (defaultTxMode)
{}

MacStations::~MacStations ()
{
  for (StationsI i = m_stations.begin (); i != m_stations.end (); i++) 
    {
      delete (*i).second;
    }
  m_stations.clear ();
  delete m_nonUnicast;
}

MacStation *
MacStations::Lookup (Mac48Address address)
{
  if (address.IsBroadcast () ||
      address.IsMulticast ())
    {
      return m_nonUnicast;
    }
  for (StationsI i = m_stations.begin (); i != m_stations.end (); i++) 
    {
      if ((*i).first == address)
        {
          return (*i).second;
        }
    }
  MacStation *station = CreateStation (m_defaultTxMode);
  m_stations.push_back (std::make_pair (address, station));
  return station;
}

MacStation *
MacStations::LookupNonUnicast (void)
{
  return m_nonUnicast;
}

} // namespace ns3

/***************************************************************
 *           MacStation below.
 ***************************************************************/ 

namespace ns3 {

MacStation::WifiRate::WifiRate (WifiMode _mode, bool _isBasic)
  : mode (_mode), isBasic (_isBasic)
{}

MacStation::MacStation (WifiMode txMode)
  : m_state (DISASSOC),
    m_defaultTxMode (txMode)
{
  NS_ASSERT (m_defaultTxMode.IsMandatory ());
  ResetModes ();
}
MacStation::~MacStation ()
{}

bool 
MacStation::IsAssociated (void) const
{
  return m_state == GOT_ASSOC_TX_OK;
}
bool 
MacStation::IsWaitAssocTxOk (void) const
{
  return m_state == WAIT_ASSOC_TX_OK;
}
void 
MacStation::RecordWaitAssocTxOk (void)
{
  m_state = WAIT_ASSOC_TX_OK;
}
void 
MacStation::RecordGotAssocTxOk (void)
{
  m_state = GOT_ASSOC_TX_OK;
}
void 
MacStation::RecordGotAssocTxFailed (void)
{
  m_state = DISASSOC;
}
void 
MacStation::RecordDisassociated (void)
{
  m_state = DISASSOC;
}

void 
MacStation::ResetModes (void)
{
  m_rates.clear ();
  AddBasicMode (m_defaultTxMode);
}
void 
MacStation::AddBasicMode (WifiMode mode)
{
  if (IsIn (mode))
    {
      return;
    }
  m_rates.push_back (WifiRate (mode, true));
}
void 
MacStation::AddExtendedMode (WifiMode mode)
{
  if (IsIn (mode))
    {
      return;
    }
  m_rates.push_back (WifiRate (mode, false));
}

bool
MacStation::IsIn (WifiMode mode) const
{
  for (WifiRates::const_iterator i = m_rates.begin (); i != m_rates.end (); i++)
    {
      if (i->mode == mode)
        {
          return true;
        }
    }
  return false;
}

WifiMode
MacStation::GetControlAnswerMode (WifiMode reqMode)
{
  /**
   * see ieee 802.11e, section 9.6:
   * 
   * To allow the transmitting STA to calculate the contents of 
   * the Duration/ID field, a STA responding to a received frame 
   * shall transmit its Control Response frame (either CTS or ACK) 
   * frames, other than the Block-Ack control frame, at the highest 
   * rate in the BSSBasicRateSet parameter that is less than or equal 
   * to the rate of the immediately previous frame in the frame 
   * exchange sequence (as defined in 9.79.12) and that is of the
   * same modulation type as the received frame. If no rate in the 
   * basic rate set meets these conditions, then the control frame 
   * sent in response to a received frame shall be transmitted at 
   * the highest mandatory rate of the PHY that is less than or equal 
   * to the rate of the received frame, and that is of the same 
   * modulation type as the received frame. In addition, the Control 
   * Response frame shall be sent using the same PHY options as the
   * received frame, unless they conflict with the requirement to use 
   * the BSSBasicRateSet parameter.
   */
  WifiMode mode = m_defaultTxMode;
  bool found = false;

  // First, search the BSS Basic Rate set
  for (WifiRates::const_iterator i = m_rates.begin (); i != m_rates.end (); i++)
    {
      if (i->mode.GetPhyRate () > mode.GetPhyRate () &&
          i->mode.GetPhyRate () <= reqMode.GetPhyRate () &&
          i->mode.GetModulationType () == reqMode.GetModulationType () &&
          i->isBasic)
        {
          mode = i->mode;
          found = true;
        }
    }
  if (found)
    {
      return mode;
    }
  // Then, search the mandatory rates.
  for (WifiRates::const_iterator i = m_rates.begin (); i != m_rates.end (); i++)
    {
      if (i->mode.GetPhyRate () > mode.GetPhyRate () &&
          i->mode.GetPhyRate () <= reqMode.GetPhyRate () &&
          i->mode.GetModulationType () == reqMode.GetModulationType () &&
          i->mode.IsMandatory ())
        {
          mode = i->mode;
        }
    }
  return mode;
}

WifiMode 
MacStation::GetCtsMode (WifiMode rtsMode)
{
  return GetControlAnswerMode (rtsMode);
}
WifiMode 
MacStation::GetAckMode (WifiMode dataMode)
{
  return GetControlAnswerMode (dataMode);
}



} // namespace ns3

