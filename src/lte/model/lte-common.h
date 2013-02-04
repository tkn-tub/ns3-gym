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
#include <cmath>

// see 36.213 section 8
#define UL_PUSCH_TTIS_DELAY 4

#define HARQ_PERIOD 7

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

/**
* \brief Parameters for configuring the UE 
*/
struct LteUeConfig_t
{
  uint16_t  m_rnti;
  /**
  * When false means that the message is inteded foro configuring a new UE
  */
  bool      m_reconfigureFlag;
  /**
  * Transmission mode [1..7] (i.e., SISO, MIMO, etc.)
  */
  uint8_t   m_transmissionMode;
  /**
  * Srs Configuration index for UE specific SRS, see section 8.2 of TS 36.213
  */
  uint16_t  m_srsConfigurationIndex;
  
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


struct PhyTransmissionStatParameters
{
  int64_t  m_timestamp; // in millisecond
  uint16_t m_cellId;
  uint64_t m_imsi;
  uint16_t m_rnti;
  uint8_t  m_txMode;
  uint8_t  m_layer;
  uint8_t  m_mcs;
  uint16_t m_size;
  uint8_t  m_rv;
  uint8_t  m_ndi;
};


struct PhyReceptionStatParameters
{
  int64_t  m_timestamp; // in millisecond
  uint16_t m_cellId;
  uint64_t m_imsi;
  uint16_t m_rnti;
  uint8_t  m_txMode;
  uint8_t  m_layer;
  uint8_t  m_mcs;
  uint16_t m_size;
  uint8_t  m_rv;
  uint8_t  m_ndi;
  uint8_t  m_correctness;
};


}; // namespace ns3


#endif /* LTE_COMMON_H_ */
