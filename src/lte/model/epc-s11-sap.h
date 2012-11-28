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
 * Author: Nicola Baldo <nbaldo@cttc.cat>
 */

#ifndef EPC_S11_SAP_H
#define EPC_S11_SAP_H

#include <ns3/address.h>
#include <ns3/ptr.h>
#include <ns3/object.h>
#include <ns3/eps-bearer.h>
#include <ns3/epc-tft.h>
#include <list>

namespace ns3 {

class EpcS11Sap
{
public:

  virtual ~EpcS11Sap ();

  /**
   * Fully-qualified TEID, see 3GPP TS 29.274 section 8.22
   * 
   */
  struct Fteid 
  {
    uint32_t teid;
    Ipv4Address address;
  };

  /**
   * TS 29.274 8.21  User Location Information (ULI)
   * 
   */
  struct Uli
  {
    uint16_t gci;
  };
  
 
};

/**
 * \ingroup lte
 *
 * MME side of the S11 Service Access Point (SAP), provides the MME
 * methods to be called when an S11 message is received by the MME. 
 */
  class EpcS11SapMme : public EpcS11Sap
{
public:
  
 /**
   * 3GPP TS 29.274 version 8.3.1 Release 8 section 8.28
   * 
   */
  struct BearerContext
  {

    EpcS11Sap::Fteid sgwFteid;
    uint8_t epsBearerId; 
    EpsBearer bearerLevelQos; 
    Ptr<EpcTft> tft;
  };

  virtual void RecvCreateSessionResponse (uint64_t imsi, std::list<BearerContext> bearerContextList) = 0;

};

/**
 * \ingroup lte
 *
 * SGW side of the S11 Service Access Point (SAP), provides the SGW
 * methods to be called when an S11 message is received by the SGW. 
 */
class EpcS11SapSgw : public EpcS11Sap
{
public:

  struct BearerContext
  {
    
    EpcS11Sap::Fteid sgwFteid;
    uint8_t epsBearerId; 
    EpsBearer bearerLevelQos; 
    Ptr<EpcTft> tft;
  };

  /** 
   * 
   * 
   * \param imsi
   * \param uli theoretically, the User Location Information (ULI) which
   * includes the EGCI. In practice, we use the Cell Id. 
   * \param bearersToBeSetup default bearer + eventual other bearers
   * to be setup
   */
  virtual void RecvCreateSessionRequest (uint64_t imsi, Uli uli, std::list<BearerContext> bearersToBeSetup) = 0;


  /** 
   * 
   * 
   * \param imsi not included in the specs, we add it for simplicity.
   * \param uli theoretically, the User Location Information (ULI) which
   * includes the EGCI. In practice, we use the Cell Id. 
   * \param bearersToBeSetup 
   */
  virtual void ModifyBearerRequest (uint64_t mei, Uli uli, std::list<BearerContext> bearersToBeSetup) = 0;

};







/**
 * Template for the implementation of the EpcS11SapMme as a member
 * of an owner class of type C to which all methods are forwarded
 * 
 */
template <class C>
class MemberEpcS11SapMme : public EpcS11SapMme
{
public:
  MemberEpcS11SapMme (C* owner);

  // inherited from EpcS11SapMme
  void RecvCreateSessionResponse (uint64_t imsi, std::list<BearerContext>);

private:
  MemberEpcS11SapMme ();
  C* m_owner;
};

template <class C>
MemberEpcS11SapMme<C>::MemberEpcS11SapMme (C* owner)
  : m_owner (owner)
{
}

template <class C>
MemberEpcS11SapMme<C>::MemberEpcS11SapMme ()
{
}

template <class C>
void MemberEpcS11SapMme<C>::RecvCreateSessionResponse (uint64_t imsi, std::list<BearerContext> bearerContextList)
{
  m_owner->DoRecvCreateSessionResponse (imsi, bearerContextList);
}





/**
 * Template for the implementation of the EpcS11SapSgw as a member
 * of an owner class of type C to which all methods are forwarded
 * 
 */
template <class C>
class MemberEpcS11SapSgw : public EpcS11SapSgw
{
public:
  MemberEpcS11SapSgw (C* owner);

  // inherited from EpcS11SapSgw
  virtual void RecvCreateSessionRequest (uint64_t imsi, Uli uli, std::list<BearerContext> bearersToBeSetup);
  virtual void ModifyBearerRequest (uint64_t mei, Uli uli, std::list<BearerContext> bearersToBeSetup);

private:
  MemberEpcS11SapSgw ();
  C* m_owner;
};

template <class C>
MemberEpcS11SapSgw<C>::MemberEpcS11SapSgw (C* owner)
  : m_owner (owner)
{
}

template <class C>
MemberEpcS11SapSgw<C>::MemberEpcS11SapSgw ()
{
}

template <class C>
void MemberEpcS11SapSgw<C>::RecvCreateSessionRequest (uint64_t imsi, Uli uli, std::list<BearerContext> bearersToBeSetup)
{
  m_owner->DoRecvCreateSessionRequest (imsi, uli, bearersToBeSetup);
}

template <class C>
void MemberEpcS11SapSgw<C>::ModifyBearerRequest (uint64_t mei, Uli uli, std::list<BearerContext> bearersToBeSetup)
{
  m_owner->DoModifyBearerRequest (mei, uli, bearersToBeSetup);
}









} //namespace ns3

#endif /* EPC_S11_SAP_H */

