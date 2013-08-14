/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 CTTC
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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 *       : Ghada Badawy <gbadawy@gmail.com>
 */

#include "ns3/wifi-tx-vector.h"

namespace ns3 {

WifiTxVector::WifiTxVector ()
{
}

WifiTxVector::WifiTxVector (WifiMode m, uint8_t l, uint8_t r, bool sg, uint8_t ns, uint8_t ne, bool Stbc)
  : m_mode (m),
    m_txPowerLevel (l),
    m_retries (r),
    m_shortGuardInterval(sg),
    m_nss(ns),
    m_ness(ne),
    m_stbc(Stbc)
{
}

WifiMode
WifiTxVector::GetMode (void) const
{
  return m_mode;
}
uint8_t 
WifiTxVector::GetTxPowerLevel (void) const
{
  return m_txPowerLevel;
}
uint8_t 
WifiTxVector::GetRetries (void) const
{
  return m_retries;
}
bool 
WifiTxVector::IsShortGuardInterval (void) const
{
 return m_shortGuardInterval;
}
uint8_t 
WifiTxVector::GetNss (void) const
{
  return m_nss;
}
uint8_t 
WifiTxVector::GetNess (void) const
{
  return m_ness;
}
bool 
WifiTxVector::IsStbc (void) const
{
  return m_stbc;
}

void 
WifiTxVector::SetMode (WifiMode mode)
{
  m_mode=mode;
}
void 
WifiTxVector::SetTxPowerLevel (uint8_t powerlevel)
{
  m_txPowerLevel=powerlevel;
}
void 
WifiTxVector::SetRetries (uint8_t retries)
{
  m_retries = retries;
}
void 
WifiTxVector::SetShortGuardInterval (bool guardinterval)
{
  m_shortGuardInterval=guardinterval;
}
void 
WifiTxVector::SetNss (uint8_t nss)
{
  m_nss= nss;
}
void 
WifiTxVector::SetNess (uint8_t ness)
{
  m_ness=ness;
}
void 
WifiTxVector::SetStbc (bool stbc)
{
  m_stbc=stbc;
}

std::ostream & operator << ( std::ostream &os, const WifiTxVector &v)
{ 
  os << "mode:" << v.GetMode() << " txpwrlvl:" << v.GetTxPowerLevel() << " retries:" << v.GetRetries() << " Short GI: " << v.IsShortGuardInterval() << " Nss: " << v.GetNss() << " Ness: " << v.GetNess() << " STBC: " << v.IsStbc();
  return os;
}

} // namespace ns3
