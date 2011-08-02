/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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

#ifndef UE_MANAGER_H
#define UE_MANAGER_H

#include <stdint.h>
#include "ue-record.h"

namespace ns3 {

class UeNetDevice;
class EnbNetDevice;

/**
 * \ingroup lte
 *
 * \brief this class allow the eNodeb to manage in a very simple way all registered UE.
 * In particular, to each registered UE, a new UeRecord is created.
 * \see UeRecord
 */
class UeManager : public Object
{
public:
  UeManager (void);
  ~UeManager (void);

  /**
   * \brief Create a new UE Record to store into the eNB
   * \param ue the pointer of the UE device
   * \param enb the pointer of the enb device
   */
  void CreateUeRecord (Ptr<UeNetDevice> ue, Ptr<EnbNetDevice> enb);

  /**
   * \brief Get a UE Record stored into the eNB
   * \param ue the pointer of the UE device
   * \return a pointer ot the UE record
   */
  Ptr<UeRecord>  GetUeRecord (Ptr<UeNetDevice> ue);

  /**
   * \brief Get a UE Record stored into the eNB
   * \param macAddress the mac address of the UE device
   * \return a pointer ot the UE record
   */
  Ptr<UeRecord> GetUeRecord (const Mac48Address macAddress);

  /**
   * \brief Verify if the UE is registered into this eNB
   * \param ue the pointer of the UE device
   * \return true if the UE is registered, false otherwise
   */
  bool IsRegistered (Ptr<UeNetDevice> ue) const;

  /**
   * \brief Verify if the UE is registered into this eNB
   * \param macAddress the mac address of the UE device
   * \return true if the UE is registered, false otherwise
   */
  bool IsRegistered (const Mac48Address &macAddress) const;

  /**
   * \brief Delete an UE Record stored into the eNB
   * \param ue the pointer of the ue device
   */
  void DeleteUeRecord (Ptr<UeNetDevice> ue);

  /**
   * \brief Delete an UE Record stored into the eNB
   * \param macAddress the mac address of the UE device
   */
  void DeleteUeRecord (const Mac48Address &macAddress);

  /**
   * \brief Get a list of UE records
   * \return a list of UE records
   */
  std::vector< Ptr<UeRecord> >*  GetUeRecords (void);

  /**
   * \brief Get the number of registered UE
   * \return the number of registered UE
   */
  uint32_t GetNRegisteredUes (void) const;

private:
  std::vector< Ptr<UeRecord> > *m_ueRecords;
};

} // namespace ns3

#endif /* UE_MANAGER_H */
