/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007,2008, 2009 INRIA, UDcast
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
 * Author: Jahanzeb Farooq <jahanzeb.farooq@sophia.inria.fr>
 *         Mohamed Amine Ismail <amine.ismail@sophia.inria.fr>
 */

#ifndef SERVICE_FLOW_H
#define SERVICE_FLOW_H

#include <stdint.h>
#include "wimax-phy.h"
#include "wimax-connection.h"
#include "cs-parameters.h"

namespace ns3 {

class ServiceFlowRecord;
class WimaxConnection;

/**
 * \ingroup wimax
 * This class implements service flows as described by the IEEE-802.16 standard
 */
class ServiceFlow
{
public:
  enum Direction
  {
    SF_DIRECTION_DOWN, SF_DIRECTION_UP
  };

  enum Type
  {
    SF_TYPE_PROVISIONED, SF_TYPE_ADMITTED, SF_TYPE_ACTIVE
  };

  // section 11.13.11 Service flow scheduling type, page 701
  enum SchedulingType
  {
    SF_TYPE_NONE = 0, SF_TYPE_UNDEF = 1, SF_TYPE_BE = 2, SF_TYPE_NRTPS = 3,
    SF_TYPE_RTPS = 4, SF_TYPE_UGS = 6, SF_TYPE_ALL = 255
  };
  // section 11.13.19.2 CS parameter encoding rules, page 707
  enum CsSpecification
  {
    ATM = 99, IPV4 = 100, IPV6 = 101, ETHERNET = 102, VLAN = 103,
    IPV4_OVER_ETHERNET = 104, IPV6_OVER_ETHERNET = 105, IPV4_OVER_VLAN = 106,
    IPV6_OVER_VLAN = 107
  };
  enum ModulationType // Table 356 and 362
  {
    MODULATION_TYPE_BPSK_12, MODULATION_TYPE_QPSK_12, MODULATION_TYPE_QPSK_34, MODULATION_TYPE_QAM16_12,
    MODULATION_TYPE_QAM16_34, MODULATION_TYPE_QAM64_23, MODULATION_TYPE_QAM64_34
  };

  /**
   * \brief creates a TLV from this service flow
   * \return the created tlv
   *
   */
  Tlv ToTlv (void) const;
  /**
   * \brief creates a service flow from a TLV
   * \param tlv the tlv from which the service flow will be created
   */
  ServiceFlow (Tlv tlv);
  /**
   * \return true if the passed parameters match the classifier of the service flow, false otherwise
   * \param srcAddress the source ip address
   * \param dstAddress the destination ip address
   * \param srcPort the source port
   * \param dstPort the destination port
   * \param proto the layer 4 protocol
   */
  bool CheckClassifierMatch (Ipv4Address srcAddress,
                             Ipv4Address dstAddress,
                             uint16_t srcPort,
                             uint16_t dstPort,
                             uint8_t proto) const;
  ServiceFlow (enum Direction direction);
  ServiceFlow ();
  ServiceFlow (const ServiceFlow & sf);
  ServiceFlow (uint32_t sfid,
               enum Direction direction,
               Ptr<WimaxConnection> connection);
  ~ServiceFlow (void);
  ServiceFlow & operator = (ServiceFlow const& o);

  void InitValues (void);
  void SetDirection (enum Direction direction);
  enum Direction GetDirection (void) const;
  void CopyParametersFrom (ServiceFlow sf);

  void SetType (enum Type type);
  enum Type GetType (void) const;
  void SetConnection (Ptr<WimaxConnection> connection);
  // Can return a null connection is this service flow has not
  // been associated yet to a connection.
  Ptr<WimaxConnection> GetConnection (void) const;

  void SetIsEnabled (bool isEnabled);
  bool GetIsEnabled (void) const;

  void SetRecord (ServiceFlowRecord *record);
  ServiceFlowRecord* GetRecord (void) const;

  // wrapper functions
  Ptr<WimaxMacQueue> GetQueue (void) const;
  enum ServiceFlow::SchedulingType GetSchedulingType (void) const;
  bool HasPackets (void) const;
  bool HasPackets (MacHeaderType::HeaderType packetType) const;

  // shall be called only by BS
  void CleanUpQueue (void);

  void PrintQoSParameters (void) const;

  char* GetSchedulingTypeStr (void) const;

  uint32_t GetSfid (void) const;
  uint16_t GetCid (void) const;
  std::string GetServiceClassName () const;
  uint8_t GetQosParamSetType (void) const;
  uint8_t GetTrafficPriority (void) const;
  uint32_t GetMaxSustainedTrafficRate (void) const;
  uint32_t GetMaxTrafficBurst (void) const;
  uint32_t GetMinReservedTrafficRate (void) const;
  uint32_t GetMinTolerableTrafficRate (void) const;
  enum ServiceFlow::SchedulingType GetServiceSchedulingType (void) const;
  uint32_t GetRequestTransmissionPolicy (void) const;
  uint32_t GetToleratedJitter (void) const;
  uint32_t GetMaximumLatency (void) const;
  uint8_t GetFixedversusVariableSduIndicator (void) const;
  uint8_t GetSduSize (void) const;
  uint16_t GetTargetSAID (void) const;
  uint8_t GetArqEnable (void) const;
  uint16_t GetArqWindowSize (void) const;
  uint16_t GetArqRetryTimeoutTx (void) const;
  uint16_t GetArqRetryTimeoutRx (void) const;
  uint16_t GetArqBlockLifeTime (void) const;
  uint16_t GetArqSyncLoss (void) const;
  uint8_t GetArqDeliverInOrder (void) const;
  uint16_t GetArqPurgeTimeout (void) const;
  uint16_t GetArqBlockSize (void) const;
  enum CsSpecification GetCsSpecification (void) const;
  CsParameters GetConvergenceSublayerParam (void) const;
  uint16_t GetUnsolicitedGrantInterval (void) const;
  uint16_t GetUnsolicitedPollingInterval (void) const;
  bool GetIsMulticast (void) const;
  enum WimaxPhy::ModulationType GetModulation (void) const;

  void SetSfid (uint32_t);
  void SetServiceClassName (std::string);
  void SetQosParamSetType (uint8_t);
  void SetTrafficPriority (uint8_t);
  void SetMaxSustainedTrafficRate (uint32_t);
  void SetMaxTrafficBurst (uint32_t);
  void SetMinReservedTrafficRate (uint32_t);
  void SetMinTolerableTrafficRate (uint32_t);
  void SetServiceSchedulingType (enum ServiceFlow::SchedulingType);
  void SetRequestTransmissionPolicy (uint32_t);
  void SetToleratedJitter (uint32_t);
  void SetMaximumLatency (uint32_t);
  void SetFixedversusVariableSduIndicator (uint8_t);
  void SetSduSize (uint8_t);
  void SetTargetSAID (uint16_t);
  void SetArqEnable (uint8_t);
  void SetArqWindowSize (uint16_t);
  void SetArqRetryTimeoutTx (uint16_t);
  void SetArqRetryTimeoutRx (uint16_t);
  void SetArqBlockLifeTime (uint16_t);
  void SetArqSyncLoss (uint16_t);
  void SetArqDeliverInOrder (uint8_t);
  void SetArqPurgeTimeout (uint16_t);
  void SetArqBlockSize (uint16_t);
  void SetCsSpecification (enum CsSpecification);
  void SetConvergenceSublayerParam (CsParameters);

  void SetUnsolicitedGrantInterval (uint16_t);
  void SetUnsolicitedPollingInterval (uint16_t);
  void SetIsMulticast (bool isMulticast);
  void SetModulation (enum WimaxPhy::ModulationType modulationType);



private:
  uint32_t m_sfid;
  std::string m_serviceClassName;
  uint8_t m_qosParamSetType;
  uint8_t m_trafficPriority;
  uint32_t m_maxSustainedTrafficRate;
  uint32_t m_maxTrafficBurst;
  uint32_t m_minReservedTrafficRate;
  uint32_t m_minTolerableTrafficRate;
  enum ServiceFlow::SchedulingType m_schedulingType;
  uint32_t m_requestTransmissionPolicy;
  uint32_t m_toleratedJitter;
  uint32_t m_maximumLatency;
  uint8_t m_fixedversusVariableSduIndicator;
  uint8_t m_sduSize;
  uint16_t m_targetSAID;
  uint8_t m_arqEnable;
  uint16_t m_arqWindowSize;
  uint16_t m_arqRetryTimeoutTx;
  uint16_t m_arqRetryTimeoutRx;
  uint16_t m_arqBlockLifeTime;
  uint16_t m_arqSyncLoss;
  uint8_t m_arqDeliverInOrder;
  uint16_t m_arqPurgeTimeout;
  uint16_t m_arqBlockSize;
  enum CsSpecification m_csSpecification;
  CsParameters m_convergenceSublayerParam;
  uint16_t m_unsolicitedGrantInterval;
  uint16_t m_unsolicitedPollingInterval;
  Direction m_direction;
  Type m_type;
  Ptr<WimaxConnection> m_connection;
  bool m_isEnabled;
  bool m_isMulticast;
  enum WimaxPhy::ModulationType m_modulationType;
  // will be used by the BS
  ServiceFlowRecord *m_record;
};

} // namespace ns3

#endif /* SERVICE_FLOW_H */
