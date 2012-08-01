/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011, 2012 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Nicola Baldo <nbaldo@cttc.es>,
 *         Marco Miozzo <mmiozzo@cttc.es>
 */

#ifndef LTE_UE_CPHY_SAP_H
#define LTE_UE_CPHY_SAP_H

#include <stdint.h>
#include <ns3/ptr.h>

namespace ns3 {


class LteEnbNetDevice;

/**
 * Service Access Point (SAP) offered by the UE PHY to the UE RRC for control purposes
 *
 * This is the PHY SAP Provider, i.e., the part of the SAP that contains
 * the PHY methods called by the MAC
 */
class LteUeCphySapProvider
{
public:

  /** 
   * destructor
   */
  virtual ~LteUeCphySapProvider ();

  /** 
   * tell the PHY to synchronize with a given eNB for communication purposes
   * 
   * \param enbPhy a pointer to the PHY of the eNB (wild hack, might go away in later versions)
   * \param cellId the ID of the eNB
   */
  virtual void SyncronizeWithEnb (Ptr<LteEnbNetDevice> enbDevice, uint16_t cellId) = 0;
  
  /**
   *
   * \param dlEarfcn the carrier frequency (EARFCN) in downlink
   * \param ulEarfcn the carrier frequency (EARFCN) in downlink
   */
  virtual void SetEarfcn (uint16_t dlEarfcn, uint16_t ulEarfcn) = 0;

  /**
   * \param ulBandwidth the UL bandwidth in PRBs
   * \param dlBandwidth the DL bandwidth in PRBs
   */
  virtual void SetBandwidth (uint8_t ulBandwidth, uint8_t dlBandwidth) = 0;
  
  /** 
   * 
   * \param rnti the cell-specific UE identifier
   */
  virtual void SetRnti (uint16_t rnti) = 0;

  /**
   * \param txMode the transmissionMode of the user
   */
  virtual void SetTransmissionMode (uint8_t txMode) = 0;

  /**
   * \param txMode the transmissionMode of the user
   */
  virtual void SetSrsConfigurationIndex (uint16_t   srcCi) = 0;

};


/**
 * Service Access Point (SAP) offered by the UE PHY to the UE RRC for control purposes
 *
 * This is the CPHY SAP User, i.e., the part of the SAP that contains the RRC
 * methods called by the PHY
*/
class LteUeCphySapUser
{
public:
  
  /** 
   * destructor
   */
  virtual ~LteUeCphySapUser ();
};




/**
 * Template for the implementation of the LteUeCphySapProvider as a member
 * of an owner class of type C to which all methods are forwarded
 * 
 */
template <class C>
class MemberLteUeCphySapProvider : public LteUeCphySapProvider
{
public:
  MemberLteUeCphySapProvider (C* owner);

  // inherited from LteUeCphySapProvider
  virtual void SyncronizeWithEnb (Ptr<LteEnbNetDevice> enbDevice, uint16_t cellId);
  virtual void SetEarfcn (uint16_t dlEarfcn, uint16_t ulEarfcn);
  virtual void SetBandwidth (uint8_t ulBandwidth, uint8_t dlBandwidth);
  virtual void SetRnti (uint16_t rnti);
  virtual void SetTransmissionMode (uint8_t txMode);
  virtual void SetSrsConfigurationIndex (uint16_t srcCi);

private:
  MemberLteUeCphySapProvider ();
  C* m_owner;
};

template <class C>
MemberLteUeCphySapProvider<C>::MemberLteUeCphySapProvider (C* owner)
  : m_owner (owner)
{
}

template <class C>
MemberLteUeCphySapProvider<C>::MemberLteUeCphySapProvider ()
{
}

template <class C>
void 
MemberLteUeCphySapProvider<C>::SyncronizeWithEnb (Ptr<LteEnbNetDevice> enbDevice, uint16_t cellId)
{
  m_owner->DoSyncronizeWithEnb (enbDevice, cellId);
}

template <class C>
void 
MemberLteUeCphySapProvider<C>::SetEarfcn (uint16_t dlEarfcn, uint16_t ulEarfcn)
{
  m_owner->DoSetEarfcn (dlEarfcn, ulEarfcn);
}

template <class C>
void 
MemberLteUeCphySapProvider<C>::SetBandwidth (uint8_t ulBandwidth, uint8_t dlBandwidth)
{
  m_owner->DoSetBandwidth (ulBandwidth, dlBandwidth);
}

template <class C>
void 
MemberLteUeCphySapProvider<C>::SetRnti (uint16_t rnti)
{
  m_owner->DoSetRnti (rnti);
}

template <class C>
void 
MemberLteUeCphySapProvider<C>::SetTransmissionMode (uint8_t txMode)
{
  m_owner->DoSetTransmissionMode (txMode);
}

template <class C>
void 
MemberLteUeCphySapProvider<C>::SetSrsConfigurationIndex (uint16_t srcCi)
{
  m_owner->DoSetSrsConfigurationIndex (srcCi);
}



/**
 * Template for the implementation of the LteUeCphySapUser as a member
 * of an owner class of type C to which all methods are forwarded
 * 
 */
template <class C>
class MemberLteUeCphySapUser : public LteUeCphySapUser
{
public:
  MemberLteUeCphySapUser (C* owner);

  // methods inherited from LteUeCphySapUser go here

private:
  MemberLteUeCphySapUser ();
  C* m_owner;
};

template <class C>
MemberLteUeCphySapUser<C>::MemberLteUeCphySapUser (C* owner)
  : m_owner (owner)
{
}

template <class C>
MemberLteUeCphySapUser<C>::MemberLteUeCphySapUser ()
{
}




} // namespace ns3


#endif // LTE_UE_CPHY_SAP_H
