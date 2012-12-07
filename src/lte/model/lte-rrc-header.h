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
 * Author: Lluis Parcerisa <lparcerisa@cttc.cat>
 */

#ifndef RRC_HEADER_H
#define RRC_HEADER_H

#include "ns3/header.h"

#include <bitset>
#include <string>

#include "ns3/lte-rrc-sap.h"
#include "ns3/lte-asn1-header.h"

namespace ns3 {

/**
 * This class extends Asn1Header functions, adding serialization/deserialization
 * of some Information elements defined in 3GPP TS 36.331
 */
class RrcAsn1Header : public Asn1Header
{
public:
  RrcAsn1Header ();
  int GetMessageType ();

protected:
  // Serialization functions
  void SerializeSrbToAddModList (std::list<LteRrcSap::SrbToAddMod> srbToAddModList) const;
  void SerializeDrbToAddModList (std::list<LteRrcSap::DrbToAddMod> drbToAddModList) const;
  void SerializeLogicalChannelConfig (LteRrcSap::LogicalChannelConfig logicalChannelConfig) const;
  void SerializeRadioResourceConfigDedicated (LteRrcSap::RadioResourceConfigDedicated radioResourceConfigDedicated) const;
  void SerializePhysicalConfigDedicated (LteRrcSap::PhysicalConfigDedicated physicalConfigDedicated) const;
  void SerializeSystemInformationBlockType1 (LteRrcSap::SystemInformationBlockType1 systemInformationBlockType1) const;
  void SerializeSystemInformationBlockType2 () const;
  void SerializeRadioResourceConfigCommonSIB () const;

  // Deserialization functions
  Buffer::Iterator DeserializeDrbToAddModList (std::list<LteRrcSap::DrbToAddMod> *drbToAddModLis, Buffer::Iterator bIterator);
  Buffer::Iterator DeserializeSrbToAddModList (std::list<LteRrcSap::SrbToAddMod> *srbToAddModList, Buffer::Iterator bIterator);
  Buffer::Iterator DeserializeLogicalChannelConfig (LteRrcSap::LogicalChannelConfig *logicalChannelConfig, Buffer::Iterator bIterator);
  Buffer::Iterator DeserializeRadioResourceConfigDedicated (LteRrcSap::RadioResourceConfigDedicated *radioResourceConfigDedicated, Buffer::Iterator bIterator);
  Buffer::Iterator DeserializePhysicalConfigDedicated (LteRrcSap::PhysicalConfigDedicated *physicalConfigDedicated, Buffer::Iterator bIterator);
  Buffer::Iterator DeserializeSystemInformationBlockType1 (LteRrcSap::SystemInformationBlockType1 *systemInformationBlockType1, Buffer::Iterator bIterator);
  Buffer::Iterator DeserializeSystemInformationBlockType2 (Buffer::Iterator bIterator);
  Buffer::Iterator DeserializeRadioResourceConfigCommon (Buffer::Iterator bIterator);
  Buffer::Iterator DeserializeRadioResourceConfigCommonSib (Buffer::Iterator bIterator);

  void Print (std::ostream &os, LteRrcSap::RadioResourceConfigDedicated radioResourceConfigDedicated) const;

  int m_messageType;
};


/**
 * This class only serves to discriminate which message type has been received
 * in uplink (ue to eNb) for channel DCCH
 */
class RrcUlDcchMessage : public RrcAsn1Header
{
public:
  uint32_t Deserialize (Buffer::Iterator bIterator);
  void Print (std::ostream &os) const;
  void PreSerialize () const;
  void SerializeUlDcchMessage (int msgType) const;
  Buffer::Iterator DeserializeUlDcchMessage (Buffer::Iterator bIterator);
};

/**
 * This class only serves to discriminate which message type has been received
 * in downlink (eNb to ue) for channel DCCH
 */
class RrcDlDcchMessage : public RrcAsn1Header
{
public:
  uint32_t Deserialize (Buffer::Iterator bIterator);
  void Print (std::ostream &os) const;
  void PreSerialize () const;
  void SerializeDlDcchMessage (int msgType) const;
  Buffer::Iterator DeserializeDlDcchMessage (Buffer::Iterator bIterator);
};

/**
 * This class only serves to discriminate which message type has been received
 * in uplink (ue to eNb) for channel CCCH
 */
class RrcUlCcchMessage : public RrcAsn1Header
{
public:
  uint32_t Deserialize (Buffer::Iterator bIterator);
  void Print (std::ostream &os) const;
  void PreSerialize () const;
  void SerializeUlCcchMessage (int msgType) const;
  Buffer::Iterator DeserializeUlCcchMessage (Buffer::Iterator bIterator);
};

/**
 * This class only serves to discriminate which message type has been received
 * in downlink (eNb to ue) for channel CCCH
 */
class RrcDlCcchMessage : public RrcAsn1Header
{
public:
  uint32_t Deserialize (Buffer::Iterator bIterator);
  void Print (std::ostream &os) const;
  void PreSerialize () const;
  void SerializeDlCcchMessage (int msgType) const;
  Buffer::Iterator DeserializeDlCcchMessage (Buffer::Iterator bIterator);
};

/**
* This class manages the serialization/deserialization of RrcConnectionRequest IE
*/
class RrcConnectionRequestHeader : public RrcUlCcchMessage,
                                   LteRrcSap
{
public:
  RrcConnectionRequestHeader ();
  void PreSerialize () const;
  uint32_t Deserialize (Buffer::Iterator bIterator);
  void Print (std::ostream &os) const;
  void SetMessage (RrcConnectionRequest msg);
  RrcConnectionRequest GetMessage () const;
  std::bitset<8> getMmec () const;
  std::bitset<32> getMtmsi () const;

private:
  std::bitset<8> m_mmec;
  std::bitset<32> m_mTmsi;
  enum
  {
    EMERGENCY = 0, HIGHPRIORITYACCESS, MT_ACCESS,
    MO_SIGNALLING, MO_DATA, SPARE3, SPARE2, SPARE1
  } m_establishmentCause;
  std::bitset<1> m_spare;
};

/**
* This class manages the serialization/deserialization of RrcConnectionSetup IE
*/
class RrcConnectionSetupHeader : public RrcDlCcchMessage,
                                 LteRrcSap
{
public:
  RrcConnectionSetupHeader ();
  void PreSerialize () const;
  uint32_t Deserialize (Buffer::Iterator bIterator);
  void Print (std::ostream &os) const;
  void SetMessage (RrcConnectionSetup msg);
  RrcConnectionSetup GetMessage () const;
  uint8_t GetRrcTransactionIdentifier () const;
  bool HavePhysicalConfigDedicated () const;
  std::list<LteRrcSap::SrbToAddMod> GetSrbToAddModList () const;
  std::list<LteRrcSap::DrbToAddMod> GetDrbToAddModList () const;
  std::list<uint8_t> GetDrbToReleaseList () const;
  LteRrcSap::PhysicalConfigDedicated GetPhysicalConfigDedicated () const;
  LteRrcSap::RadioResourceConfigDedicated GetRadioResourceConfigDedicated () const;

private:
  uint8_t rrcTransactionIdentifier;
  mutable RadioResourceConfigDedicated radioResourceConfigDedicated;
};

/**
* This class manages the serialization/deserialization of RrcConnectionSetupComplete IE
*/
class RrcConnectionSetupCompleteHeader : public RrcUlDcchMessage,
                                         LteRrcSap
{
public:
  RrcConnectionSetupCompleteHeader ();
  void PreSerialize () const;
  uint32_t Deserialize (Buffer::Iterator bIterator);
  void Print (std::ostream &os) const;
  void SetMessage (RrcConnectionSetupCompleted msg);

  uint8_t GetRrcTransactionIdentifier () const;
  RrcConnectionSetupCompleted GetMessage () const;

private:
  uint8_t m_rrcTransactionIdentifier;

};

/**
* This class manages the serialization/deserialization of RrcConnectionSetupComplete IE
*/
class RrcConnectionReconfigurationCompleteHeader : public RrcUlDcchMessage,
                                                   LteRrcSap
{
public:
  RrcConnectionReconfigurationCompleteHeader ();
  void PreSerialize () const;
  uint32_t Deserialize (Buffer::Iterator bIterator);
  void Print (std::ostream &os) const;
  void SetMessage (RrcConnectionReconfigurationCompleted msg);
  RrcConnectionReconfigurationCompleted GetMessage () const;
  uint8_t GetRrcTransactionIdentifier () const;

private:
  uint8_t m_rrcTransactionIdentifier;

};

/**
* This class manages the serialization/deserialization of RrcConnectionReconfiguration IE
*/
class RrcConnectionReconfigurationHeader : public RrcDlDcchMessage,
                                           LteRrcSap
{
public:
  RrcConnectionReconfigurationHeader ();
  void PreSerialize () const;
  uint32_t Deserialize (Buffer::Iterator bIterator);
  void Print (std::ostream &os) const;
  void SetMessage (RrcConnectionReconfiguration msg);

  RrcConnectionReconfiguration GetMessage () const;
  uint8_t GetRrcTransactionIdentifier () const;
  bool GetHaveMeasConfig ();
  MeasConfig GetMeasConfig ();
  bool GetHaveMobilityControlInfo ();
  MobilityControlInfo GetMobilityControlInfo ();
  bool GetHaveRadioResourceConfigDedicated ();
  RadioResourceConfigDedicated GetRadioResourceConfigDedicated ();

  // RadioResourceConfigDedicated functions
  bool HavePhysicalConfigDedicated () const;
  std::list<LteRrcSap::SrbToAddMod> GetSrbToAddModList () const;
  std::list<LteRrcSap::DrbToAddMod> GetDrbToAddModList () const;
  std::list<uint8_t> GetDrbToReleaseList () const;
  LteRrcSap::PhysicalConfigDedicated GetPhysicalConfigDedicated () const;

private:
  uint8_t m_rrcTransactionIdentifier;
  bool m_haveMeasConfig;
  MeasConfig m_measConfig;
  bool m_haveMobilityControlInfo;
  MobilityControlInfo m_mobilityControlInfo;
  bool m_haveRadioResourceConfigDedicated;
  RadioResourceConfigDedicated m_radioResourceConfigDedicated;
};

/**
* This class manages the serialization/deserialization of HandoverPreparationInfo IE
*/
class HandoverPreparationInfoHeader : public RrcAsn1Header,
                                      LteRrcSap
{
public:
  HandoverPreparationInfoHeader ();
  void PreSerialize () const;
  uint32_t Deserialize (Buffer::Iterator bIterator);
  void Print (std::ostream &os) const;
  void SetMessage (HandoverPreparationInfo msg);

  HandoverPreparationInfo GetMessage () const;
  AsConfig GetAsConfig () const;

private:
  AsConfig m_asConfig;
};

/**
* This class manages the serialization/deserialization of RRCConnectionReestablishmentRequest IE
*/
class RrcConnectionReestablishmentRequestHeader : public RrcUlCcchMessage,
                                                  LteRrcSap
{
public:
  RrcConnectionReestablishmentRequestHeader ();
  void PreSerialize () const;
  uint32_t Deserialize (Buffer::Iterator bIterator);
  void Print (std::ostream &os) const;
  void SetMessage (RrcConnectionReestablishmentRequest msg);
  RrcConnectionReestablishmentRequest GetMessage () const;

  ReestabUeIdentity GetUeIdentity () const;
  ReestablishmentCause GetReestablishmentCause () const;

private:
  ReestabUeIdentity m_ueIdentity;
  ReestablishmentCause m_reestablishmentCause;
};

/**
* This class manages the serialization/deserialization of RrcConnectionReestablishment IE
*/
class RrcConnectionReestablishmentHeader : public RrcDlCcchMessage,
                                           LteRrcSap
{
public:
  RrcConnectionReestablishmentHeader ();
  void PreSerialize () const;
  uint32_t Deserialize (Buffer::Iterator bIterator);
  void Print (std::ostream &os) const;
  void SetMessage (RrcConnectionReestablishment msg);
  RrcConnectionReestablishment GetMessage () const;
  uint8_t GetRrcTransactionIdentifier () const;
  RadioResourceConfigDedicated GetRadioResourceConfigDedicated () const;

private:
  uint8_t m_rrcTransactionIdentifier;
  RadioResourceConfigDedicated m_radioResourceConfigDedicated;
};

/**
* This class manages the serialization/deserialization of RrcConnectionReestablishmentComplete IE
*/
class RrcConnectionReestablishmentCompleteHeader : public RrcUlDcchMessage,
                                                   LteRrcSap
{
public:
  RrcConnectionReestablishmentCompleteHeader ();
  void PreSerialize () const;
  uint32_t Deserialize (Buffer::Iterator bIterator);
  void Print (std::ostream &os) const;
  void SetMessage (RrcConnectionReestablishmentComplete msg);
  RrcConnectionReestablishmentComplete GetMessage () const;
  uint8_t GetRrcTransactionIdentifier () const;

private:
  uint8_t m_rrcTransactionIdentifier;
};

/**
* This class manages the serialization/deserialization of RrcConnectionReestablishmentReject IE
*/
class RrcConnectionReestablishmentRejectHeader : public RrcDlCcchMessage,
                                                 LteRrcSap
{
public:
  RrcConnectionReestablishmentRejectHeader ();
  void PreSerialize () const;
  uint32_t Deserialize (Buffer::Iterator bIterator);
  void Print (std::ostream &os) const;
  void SetMessage (RrcConnectionReestablishmentReject msg);
  RrcConnectionReestablishmentReject GetMessage () const;

private:
  RrcConnectionReestablishmentReject m_rrcConnectionReestablishmentReject;
};

/**
* This class manages the serialization/deserialization of RrcConnectionRelease IE
*/
class RrcConnectionReleaseHeader : public RrcDlDcchMessage,
                                   LteRrcSap
{
public:
  RrcConnectionReleaseHeader ();
  void PreSerialize () const;
  uint32_t Deserialize (Buffer::Iterator bIterator);
  void Print (std::ostream &os) const;
  void SetMessage (RrcConnectionRelease msg);
  RrcConnectionRelease GetMessage () const;
  uint8_t GetRrcTransactionIdentifier () const;

private:
  RrcConnectionRelease m_rrcConnectionRelease;
};

} // namespace ns3

#endif // EPC_ASN1_HEADER_H

