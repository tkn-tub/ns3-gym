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

  struct GtpcMessage
  {
    uint32_t teid;
  };

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
  struct BearerContextCreated
  {

    EpcS11Sap::Fteid sgwFteid;
    uint8_t epsBearerId; 
    EpsBearer bearerLevelQos; 
    Ptr<EpcTft> tft;
  };


  /**     
   * Create Session Response message, see 3GPP TS 29.274 7.2.2
   */
  struct CreateSessionResponseMessage : public GtpcMessage
  {
    std::list<BearerContextCreated> bearerContextsCreated;
  };


  /** 
   * send a Create Session Response message
   * 
   * \param msg the message
   */
  virtual void CreateSessionResponse (CreateSessionResponseMessage msg) = 0;

  struct BearerContextRemoved
  {
    uint8_t epsBearerId;
  };

  /**
   * Delete Bearer Request message, see 3GPP TS 29.274 Release 9 V9.3.0 section 7.2.9.2
   */
  struct DeleteBearerRequestMessage : public GtpcMessage
  {
    std::list<BearerContextRemoved> bearerContextsRemoved;
  };

  /**
    * \brief As per 3GPP TS 29.274 Release 9 V9.3.0, a Delete Bearer Request message shall be sent on the S11 interface by PGW to SGW and from SGW to MME
    * \param msg the message
    */
  virtual void DeleteBearerRequest (DeleteBearerRequestMessage msg) = 0;




  /**     
   * Modify Bearer Response message, see 3GPP TS 29.274 7.2.7
   */
  struct ModifyBearerResponseMessage : public GtpcMessage
  {
    enum Cause {
      REQUEST_ACCEPTED = 0,
      REQUEST_ACCEPTED_PARTIALLY,
      REQUEST_REJECTED,
      CONTEXT_NOT_FOUND
    } cause;
  };

  /** 
   * send a Modify Bearer Response message
   * 
   * \param msg the message
   */
  virtual void ModifyBearerResponse (ModifyBearerResponseMessage msg) = 0;

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

  struct BearerContextToBeCreated
  {    
    EpcS11Sap::Fteid sgwFteid;
    uint8_t epsBearerId; 
    EpsBearer bearerLevelQos; 
    Ptr<EpcTft> tft;
  };

  
  /**     
   * Create Session Request message, see 3GPP TS 29.274 7.2.1
   */
  struct CreateSessionRequestMessage : public GtpcMessage
  {
    uint64_t imsi; 
    Uli uli; 
    std::list<BearerContextToBeCreated> bearerContextsToBeCreated;    
  };

  /** 
   * send a Create Session Request message
   * 
   * \param msg the message
   */
  virtual void CreateSessionRequest (CreateSessionRequestMessage msg) = 0;

  struct BearerContextToBeRemoved
  {
    uint8_t epsBearerId;
  };

  /**
   * Delete Bearer Command message, see 3GPP TS 29.274 Release 9 V9.3.0 section 7.2.17.1
   */
  struct DeleteBearerCommandMessage : public GtpcMessage
  {
    std::list<BearerContextToBeRemoved> bearerContextsToBeRemoved;
  };

  /**
    * \brief As per 3GPP TS 29.274 Release 9 V9.3.0, a Delete Bearer Command message shall be sent on the S11 interface by the MME to the SGW
    */
  virtual void DeleteBearerCommand (DeleteBearerCommandMessage msg) = 0;


  struct BearerContextRemovedSgwPgw
  {
    uint8_t epsBearerId;
  };

  /**
   * Delete Bearer Response message, see 3GPP TS 29.274 Release 9 V9.3.0 section 7.2.10.2
   */
  struct DeleteBearerResponseMessage : public GtpcMessage
  {
    std::list<BearerContextRemovedSgwPgw> bearerContextsRemoved;
  };

  /**
    * \brief As per 3GPP TS 29.274 Release 9 V9.3.0, a Delete Bearer Command message shall be sent on the S11 interface by the MME to the SGW
    * \param msg the message
    */
  virtual void DeleteBearerResponse (DeleteBearerResponseMessage msg) = 0;

  /**     
   * Modify Bearer Request message, see 3GPP TS 29.274 7.2.7
   */
  struct ModifyBearerRequestMessage : public GtpcMessage
  {
    Uli uli;
  };

  /** 
   * send a Modify Bearer Request message
   * 
   * \param msg the message
   */
  virtual void ModifyBearerRequest (ModifyBearerRequestMessage msg) = 0;

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
  virtual void CreateSessionResponse (CreateSessionResponseMessage msg);
  virtual void ModifyBearerResponse (ModifyBearerResponseMessage msg);
  virtual void DeleteBearerRequest (DeleteBearerRequestMessage msg);

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
void MemberEpcS11SapMme<C>::CreateSessionResponse (CreateSessionResponseMessage msg)
{
  m_owner->DoCreateSessionResponse (msg);
}

template <class C>
void MemberEpcS11SapMme<C>::DeleteBearerRequest (DeleteBearerRequestMessage msg)
{
  m_owner->DoDeleteBearerRequest (msg);
}

template <class C>
void MemberEpcS11SapMme<C>::ModifyBearerResponse (ModifyBearerResponseMessage msg)
{
  m_owner->DoModifyBearerResponse (msg);
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
  virtual void CreateSessionRequest (CreateSessionRequestMessage msg);
  virtual void ModifyBearerRequest (ModifyBearerRequestMessage msg);
  virtual void DeleteBearerCommand (DeleteBearerCommandMessage msg);
  virtual void DeleteBearerResponse (DeleteBearerResponseMessage msg);

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
void MemberEpcS11SapSgw<C>::CreateSessionRequest (CreateSessionRequestMessage msg)
{
  m_owner->DoCreateSessionRequest (msg);
}

template <class C>
void MemberEpcS11SapSgw<C>::ModifyBearerRequest (ModifyBearerRequestMessage msg)
{
  m_owner->DoModifyBearerRequest (msg);
}

template <class C>
void MemberEpcS11SapSgw<C>::DeleteBearerCommand (DeleteBearerCommandMessage msg)
{
  m_owner->DoDeleteBearerCommand (msg);
}

template <class C>
void MemberEpcS11SapSgw<C>::DeleteBearerResponse (DeleteBearerResponseMessage msg)
{
  m_owner->DoDeleteBearerResponse (msg);
}




} //namespace ns3

#endif /* EPC_S11_SAP_H */

