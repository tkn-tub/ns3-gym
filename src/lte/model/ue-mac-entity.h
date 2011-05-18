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

#ifndef UE_MAC_ENTITY_H
#define UE_MAC_ENTITY_H

#include "ns3/object.h"
#include <list>
#include <vector>
#include "mac-entity.h"

namespace ns3 {

class CqiIdealControlMessage;

/**
 * \ingroup lte
 *
 * This class implements the MAC layer of the UE device
 */
class UeMacEntity : public MacEntity
{
public:
  static TypeId GetTypeId (void);

  UeMacEntity (void);
  virtual ~UeMacEntity (void);

  /**
   * \brief Create CQI feedbacks from SINR values. SINR values are
   * computed at the physical layer when is received a signal from the eNB
   * \param sinr list of SINR values
   */
  Ptr<CqiIdealControlMessage> CreateCqiFeedbacks (std::vector<double> sinr);

private:
};

}


#endif /* UE_MAC_ENTITY_H */
