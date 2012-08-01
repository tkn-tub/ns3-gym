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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 */

#ifndef EPC_ENB_S1_SAP_H
#define EPC_ENB_S1_SAP_H

#include <stdint.h>
#include <ns3/eps-bearer.h>

namespace ns3 {


/**
 * This class implements the Service Access Point (SAP) between the
 * LteEnbRrc and the EpcEnbApplication. In particular, this class implements the
 * Provider part of the SAP, i.e., the methods exported by the
 * EpcEnbApplication and called by the LteEnbRrc.
 * 
 */
class EpcEnbS1SapProvider
{
public:
  virtual ~EpcEnbS1SapProvider ();
  
  /**
   * Parameters passed to S1BearerSetupRequest ()
   * 
   */
  struct S1BearerSetupRequestParameters
  {
    uint16_t rnti; /**< the RNTI corresponding to the IMSI for which
                      the radio bearer activation was requested */
    uint8_t lcid; /**< the LCID of the newly created radio bearer */

    uint32_t teid; /**< context information that was passed upon the
                      corresponding call to
                      EpcEnbS1SapUser::DataRadioBearerSetupRequest  */
  };

  /**
   * Request the setup of a S1 bearer
   * 
   */
  virtual void S1BearerSetupRequest (S1BearerSetupRequestParameters params) = 0;
  
  /** 
   * 
   * 
   * \param imsi 
   * \param rnti 
   */
  virtual void InitialUeMessage (uint64_t imsi, uint16_t rnti) = 0;
};
  


/**
 * This class implements the Service Access Point (SAP) between the
 * LteEnbRrc and the EpcEnbApplication. In particular, this class implements the
 * User part of the SAP, i.e., the methods exported by the LteEnbRrc
 * and called by the EpcEnbApplication.
 * 
 */
class EpcEnbS1SapUser
{
public:
  virtual ~EpcEnbS1SapUser ();
  
  /**
   * Parameters passed to DataRadioBearerSetupRequest ()
   * 
   */
  struct DataRadioBearerSetupRequestParameters
  {
    uint16_t rnti;   /**< the RNTI identifying the UE for which the
			DataRadioBearer is to be created */ 
    EpsBearer bearer; /**< the characteristics of the bearer to be set
                         up */
    uint32_t teid;   /**< context information that needs to be passed
                      to the corresponding call to
                      EpcEnbS1SapProvider::S1BearerSetupRequest */ 
  };

  /**
   * request the setup of a DataRadioBearer
   * 
   */
  virtual void DataRadioBearerSetupRequest (DataRadioBearerSetupRequestParameters params) = 0;
  
};
  



/**
 * Template for the implementation of the EpcEnbS1SapProvider as a member
 * of an owner class of type C to which all methods are forwarded
 * 
 */
template <class C>
class MemberEpcEnbS1SapProvider : public EpcEnbS1SapProvider
{
public:
  MemberEpcEnbS1SapProvider (C* owner);

  // inherited from EpcEnbS1SapProvider
  virtual void S1BearerSetupRequest (S1BearerSetupRequestParameters params);
  virtual void InitialUeMessage (uint64_t imsi, uint16_t rnti);

private:
  MemberEpcEnbS1SapProvider ();
  C* m_owner;
};

template <class C>
MemberEpcEnbS1SapProvider<C>::MemberEpcEnbS1SapProvider (C* owner)
  : m_owner (owner)
{
}

template <class C>
MemberEpcEnbS1SapProvider<C>::MemberEpcEnbS1SapProvider ()
{
}

template <class C>
void MemberEpcEnbS1SapProvider<C>::S1BearerSetupRequest (S1BearerSetupRequestParameters params)
{
  m_owner->DoS1BearerSetupRequest (params);
}


template <class C>
void MemberEpcEnbS1SapProvider<C>::InitialUeMessage (uint64_t imsi, uint16_t rnti)
{
  m_owner->DoInitialUeMessage (imsi, rnti);
}

/**
 * Template for the implementation of the EpcEnbS1SapUser as a member
 * of an owner class of type C to which all methods are forwarded
 * 
 */
template <class C>
class MemberEpcEnbS1SapUser : public EpcEnbS1SapUser
{
public:
  MemberEpcEnbS1SapUser (C* owner);

  // inherited from EpcEnbS1SapUser
  virtual void DataRadioBearerSetupRequest (DataRadioBearerSetupRequestParameters params);

private:
  MemberEpcEnbS1SapUser ();
  C* m_owner;
};

template <class C>
MemberEpcEnbS1SapUser<C>::MemberEpcEnbS1SapUser (C* owner)
  : m_owner (owner)
{
}

template <class C>
MemberEpcEnbS1SapUser<C>::MemberEpcEnbS1SapUser ()
{
}

template <class C>
void MemberEpcEnbS1SapUser<C>::DataRadioBearerSetupRequest (DataRadioBearerSetupRequestParameters params)
{
  m_owner->DoDataRadioBearerSetupRequest (params);
}


} // namespace ns3

#endif // EPC_ENB_S1_SAP_H
