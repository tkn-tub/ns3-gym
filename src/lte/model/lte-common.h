/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Manuel Requena <manuel.requena@cttc.es>
 */
#ifndef LTE_COMMON_H
#define LTE_COMMON_H

#include "ns3/uinteger.h"

namespace ns3 {


struct lteFlowId_t
{
  uint16_t  m_rnti;
  uint8_t   m_lcId;

public:
  lteFlowId_t ();
  lteFlowId_t (const uint16_t a, const uint8_t b);

  friend bool operator == (const lteFlowId_t &a, const lteFlowId_t &b);
  friend bool operator < (const lteFlowId_t &a, const lteFlowId_t &b);
};


}; // namespace ns3


#endif /* LTE_COMMON_H_ */
