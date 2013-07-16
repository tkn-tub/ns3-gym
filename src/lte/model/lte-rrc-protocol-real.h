/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Authors: Nicola Baldo <nbaldo@cttc.es>
 *          Lluis Parcerisa <lparcerisa@cttc.cat>
 */


#ifndef LTE_RRC_PROTOCOL_REAL_H
#define LTE_RRC_PROTOCOL_REAL_H

#include <stdint.h>
#include <map>

#include <ns3/ptr.h>
#include <ns3/object.h>
#include <ns3/lte-rrc-sap.h>
#include <ns3/lte-pdcp-sap.h>
#include <ns3/lte-rlc-sap.h>
#include <ns3/lte-rrc-header.h>

namespace ns3 {

class LteUeRrcSapProvider;
class LteUeRrcSapUser;
class LteEnbRrcSapProvider;
class LteUeRrc;


/**
 * Models the transmission of RRC messages from the UE to the eNB in
 * a real fashion, by creating real RRC PDUs and transmitting them
 * over Signaling Radio Bearers using radio resources allocated by the
 * LTE MAC scheduler.
 * 
 */
class LteUeRrcProtocolReal : public Object
{
  friend class MemberLteUeRrcSapUser<LteUeRrcProtocolReal>;
  friend class LteRlcSpecificLteRlcSapUser<LteUeRrcProtocolReal>;
  friend class LtePdcpSpecificLtePdcpSapUser<LteUeRrcProtocolReal>;

public:
  LteUeRrcProtocolReal ();
  virtual ~LteUeRrcProtocolReal ();

  // inherited from Object
  virtual void DoDispose (void);
  static TypeId GetTypeId (void);

  void SetLteUeRrcSapProvider (LteUeRrcSapProvider* p);
  LteUeRrcSapUser* GetLteUeRrcSapUser ();

  void SetUeRrc (Ptr<LteUeRrc> rrc);


private:
  // methods forwarded from LteUeRrcSapUser
  void DoSetup (LteUeRrcSapUser::SetupParameters params);
  void DoSendRrcConnectionRequest (LteRrcSap::RrcConnectionRequest msg);
  void DoSendRrcConnectionSetupCompleted (LteRrcSap::RrcConnectionSetupCompleted msg);
  void DoSendRrcConnectionReconfigurationCompleted (LteRrcSap::RrcConnectionReconfigurationCompleted msg);
  void DoSendRrcConnectionReestablishmentRequest (LteRrcSap::RrcConnectionReestablishmentRequest msg);
  void DoSendRrcConnectionReestablishmentComplete (LteRrcSap::RrcConnectionReestablishmentComplete msg);
  void DoSendMeasurementReport (LteRrcSap::MeasurementReport msg);

  void SetEnbRrcSapProvider ();
  void DoReceivePdcpPdu (Ptr<Packet> p);
  void DoReceivePdcpSdu (LtePdcpSapUser::ReceivePdcpSduParameters params);

  Ptr<LteUeRrc> m_rrc;
  uint16_t m_rnti;
  LteUeRrcSapProvider* m_ueRrcSapProvider;
  LteUeRrcSapUser* m_ueRrcSapUser;
  LteEnbRrcSapProvider* m_enbRrcSapProvider;

  LteUeRrcSapUser::SetupParameters m_setupParameters;
  LteUeRrcSapProvider::CompleteSetupParameters m_completeSetupParameters;

};


/**
 * Models the transmission of RRC messages from the UE to the eNB in
 * a real fashion, by creating real RRC PDUs and transmitting them
 * over Signaling Radio Bearers using radio resources allocated by the
 * LTE MAC scheduler.
 *
 */
class LteEnbRrcProtocolReal : public Object
{
  friend class MemberLteEnbRrcSapUser<LteEnbRrcProtocolReal>;
  friend class LtePdcpSpecificLtePdcpSapUser<LteEnbRrcProtocolReal>;
  friend class LteRlcSpecificLteRlcSapUser<LteEnbRrcProtocolReal>;
  friend class RealProtocolRlcSapUser;

public:
  LteEnbRrcProtocolReal ();
  virtual ~LteEnbRrcProtocolReal ();

  // inherited from Object
  virtual void DoDispose (void);
  static TypeId GetTypeId (void);

  void SetLteEnbRrcSapProvider (LteEnbRrcSapProvider* p);
  LteEnbRrcSapUser* GetLteEnbRrcSapUser ();

  void SetCellId (uint16_t cellId);

  LteUeRrcSapProvider* GetUeRrcSapProvider (uint16_t rnti);
  void SetUeRrcSapProvider (uint16_t rnti, LteUeRrcSapProvider* p);

private:
  // methods forwarded from LteEnbRrcSapUser
  void DoSetupUe (uint16_t rnti, LteEnbRrcSapUser::SetupUeParameters params);
  void DoRemoveUe (uint16_t rnti);
  void DoSendSystemInformation (LteRrcSap::SystemInformation msg);
  void SendSystemInformation (LteRrcSap::SystemInformation msg);
  void DoSendRrcConnectionSetup (uint16_t rnti, LteRrcSap::RrcConnectionSetup msg);
  void DoSendRrcConnectionReconfiguration (uint16_t rnti, LteRrcSap::RrcConnectionReconfiguration msg);
  void DoSendRrcConnectionReestablishment (uint16_t rnti, LteRrcSap::RrcConnectionReestablishment msg);
  void DoSendRrcConnectionReestablishmentReject (uint16_t rnti, LteRrcSap::RrcConnectionReestablishmentReject msg);
  void DoSendRrcConnectionRelease (uint16_t rnti, LteRrcSap::RrcConnectionRelease msg);
  void DoSendRrcConnectionReject (uint16_t rnti, LteRrcSap::RrcConnectionReject msg);
  Ptr<Packet> DoEncodeHandoverPreparationInformation (LteRrcSap::HandoverPreparationInfo msg);
  LteRrcSap::HandoverPreparationInfo DoDecodeHandoverPreparationInformation (Ptr<Packet> p);
  Ptr<Packet> DoEncodeHandoverCommand (LteRrcSap::RrcConnectionReconfiguration msg);
  LteRrcSap::RrcConnectionReconfiguration DoDecodeHandoverCommand (Ptr<Packet> p);

  void DoReceivePdcpSdu (LtePdcpSapUser::ReceivePdcpSduParameters params);
  void DoReceivePdcpPdu (uint16_t rnti, Ptr<Packet> p);

  uint16_t m_rnti;
  uint16_t m_cellId;
  LteEnbRrcSapProvider* m_enbRrcSapProvider;
  LteEnbRrcSapUser* m_enbRrcSapUser;
  std::map<uint16_t, LteUeRrcSapProvider*> m_enbRrcSapProviderMap;
  std::map<uint16_t, LteEnbRrcSapUser::SetupUeParameters> m_setupUeParametersMap;
  std::map<uint16_t, LteEnbRrcSapProvider::CompleteSetupUeParameters> m_completeSetupUeParametersMap;

};

///////////////////////////////////////

class RealProtocolRlcSapUser : public LteRlcSapUser
{
public:
  RealProtocolRlcSapUser (LteEnbRrcProtocolReal* pdcp, uint16_t rnti);

  // Interface implemented from LteRlcSapUser
  virtual void ReceivePdcpPdu (Ptr<Packet> p);

private:
  RealProtocolRlcSapUser ();
  LteEnbRrcProtocolReal* m_pdcp;
  uint16_t m_rnti;
};


}


#endif // LTE_RRC_PROTOCOL_REAL_H
