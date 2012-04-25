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
 * Author: Marco Miozzo <marco.miozzo@cttc.es>
 */
#ifndef LTE_COMMON_H
#define LTE_COMMON_H

#include "ns3/uinteger.h"
#include <math.h>

// see 36.213 section 8
#define UL_PUSCH_TTIS_DELAY 4

namespace ns3 {


struct LteFlowId_t
{
  uint16_t  m_rnti;
  uint8_t   m_lcId;

public:
  LteFlowId_t ();
  LteFlowId_t (const uint16_t a, const uint8_t b);

  friend bool operator == (const LteFlowId_t &a, const LteFlowId_t &b);
  friend bool operator < (const LteFlowId_t &a, const LteFlowId_t &b);
};

struct ImsiLcidPair_t
{
  uint64_t  m_imsi;
  uint8_t   m_lcId;

public:
  ImsiLcidPair_t ();
  ImsiLcidPair_t (const uint64_t a, const uint8_t b);

  friend bool operator == (const ImsiLcidPair_t &a, const ImsiLcidPair_t &b);
  friend bool operator < (const ImsiLcidPair_t &a, const ImsiLcidPair_t &b);
};

struct LteUeConfig_t
{
  uint16_t  m_rnti;
  uint8_t   m_transmissionMode;
  
  public:
    LteUeConfig_t ();
    
    friend bool operator == (const LteUeConfig_t &a, const LteUeConfig_t &b);
    friend bool operator < (const LteUeConfig_t &a, const LteUeConfig_t &b);
};


class LteFfConverter
{

public:
  static uint16_t double2fpS11dot3 (double val);
  static double fpS11dot3toDouble (uint16_t val);
  static double getMinFpS11dot3Value ();

  //static const double MIN_FP_S11DOT3_VALUE = -4096;


};

class BufferSizeLevelBsr
{
  
  public:
    static uint32_t BsrId2BufferSize (uint8_t val);
    static uint8_t BufferSize2BsrId (uint32_t val);
    
    static int  m_bufferSizeLevelBsr[64];
    
    
};

class TransmissionModesLayers
{
  public:
  static uint8_t TxMode2LayerNum (uint8_t txMode);
};


}; // namespace ns3


#endif /* LTE_COMMON_H_ */
