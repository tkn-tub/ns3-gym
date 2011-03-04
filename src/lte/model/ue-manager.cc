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

#include <stdint.h>
#include "ue-manager.h"
#include "ns3/log.h"
#include "ue-net-device.h"
#include "enb-net-device.h"

NS_LOG_COMPONENT_DEFINE ("UeManager");

namespace ns3 {
NS_OBJECT_ENSURE_REGISTERED ( UeManager);

UeManager::UeManager (void)
{
  NS_LOG_FUNCTION (this);
  m_ueRecords = new std::vector<Ptr<UeRecord> > ();
}


UeManager::~UeManager (void)
{
  delete m_ueRecords;
}


void
UeManager::CreateUeRecord (Ptr<UeNetDevice> ue, Ptr<EnbNetDevice> enb)
{
  NS_LOG_FUNCTION (this << ue << enb);
  Ptr<UeRecord> ueRecord = CreateObject<UeRecord> (ue, enb);
  m_ueRecords->push_back (ueRecord);
}


Ptr<UeRecord>
UeManager::GetUeRecord (Ptr<UeNetDevice> ue)
{
  NS_LOG_FUNCTION (this << ue);
  for (std::vector<Ptr<UeRecord> >::iterator iter = m_ueRecords->begin (); iter != m_ueRecords->end (); ++iter)
    {

      if ((*iter)->GetUe ()->GetAddress () == ue->GetAddress ())
        {
          return *iter;
        }
    }

  NS_LOG_DEBUG ("GetUeRecord: UeRecord not found!");
  return 0;
}


Ptr<UeRecord>
UeManager::GetUeRecord (const Mac48Address macAddress)
{
  NS_LOG_FUNCTION (this);

  for (std::vector<Ptr<UeRecord> >::iterator iter = m_ueRecords->begin (); iter != m_ueRecords->end (); ++iter)
    {

      NS_LOG_LOGIC ("find " << macAddress  << ", here:" << (*iter)->GetUe ()->GetAddress ());

      if ((*iter)->GetUe ()->GetAddress () == macAddress)
        {
          return *iter;
        }
    }
  NS_LOG_DEBUG ("GetUeRecord: UeRecord not found!");

  return 0;
}


bool
UeManager::IsRegistered (Ptr<UeNetDevice> ue) const
{
  NS_LOG_FUNCTION (this << ue);
  for (std::vector<Ptr<UeRecord> >::iterator iter = m_ueRecords->begin (); iter != m_ueRecords->end (); ++iter)
    {
      if ((*iter)->GetUe ()->GetAddress () == ue->GetAddress ())
        {
          return true;
        }
    }
  return false;
}


bool
UeManager::IsRegistered (const Mac48Address &macAddress) const
{
  NS_LOG_FUNCTION (this);
/*
  for (std::vector<UeRecord*>::iterator iter = m_ueRecords->begin (); iter != m_ueRecords->end (); ++iter)
    {
      if ((*iter)->GetUe ()->GetAddress ()  == macAddress)
        {
          return true;
        }
    }
*/
  return false;
}


void
UeManager::DeleteUeRecord (Ptr<UeNetDevice> ue)
{
  NS_LOG_FUNCTION (this << ue);
  for (std::vector<Ptr<UeRecord> >::iterator iter = m_ueRecords->begin (); iter != m_ueRecords->end (); ++iter)
    {
      if ((*iter)->GetUe ()->GetAddress () == ue->GetAddress ())
        {
          m_ueRecords->erase (iter);
          return;
        }
    }
}


void
UeManager::DeleteUeRecord (const Mac48Address &macAddress)
{
  NS_LOG_FUNCTION (this);
/*
  for (std::vector<UeRecord*>::iterator iter = m_ueRecords->begin (); iter != m_ueRecords->end (); ++iter)
    {
      if ((*iter)->GetUe ()->GetAddress () == macAddress)
        {
          m_ueRecords->erase (iter);
          return;
        }
    }
*/
}


std::vector<Ptr<UeRecord> >*
UeManager::GetUeRecords (void)
{
  NS_LOG_FUNCTION (this);
  return m_ueRecords;
}


uint32_t
UeManager::GetNRegisteredUes (void) const
{
  NS_LOG_FUNCTION (this);
  return m_ueRecords->size ();
}

} // namespace ns3


