/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
 * Author: Manuel Requena <manuel.requena@cttc.es>
 */

#ifndef EPC_X2_SAP_H
#define EPC_X2_SAP_H

#include "ns3/packet.h"

namespace ns3 {


class Node;
class Packet;


class EpcX2SapProvider
{
public:
  virtual ~EpcX2SapProvider ();
  
  /**
   * Parameters of the API primitives
   */
  
  struct HandoverRequestParams
  {
    Ptr<Node> ueNode;
    Ptr<Node> sourceEnbNode;
    Ptr<Node> targetEnbNode;

    uint32_t            targetCellId;
    std::list<uint32_t> bearers;
    Ptr<Packet>         rrcContext;
  };
  
  struct HandoverRequestAckParams
  {
    Ptr<Node> ueNode;

    std::list<uint32_t> bearers;
    Ptr<Packet>         rrcContext;
  };
  
  /**
   * SAP primitives
   */

  virtual void SendHandoverRequest (HandoverRequestParams params) = 0;

  virtual void SendHandoverRequestAck (HandoverRequestAckParams params) = 0;
  
// TODO
//   virtual void SendSnStatusTransfer (const struct SnStatusTransfer& params) = 0;
// 
//   virtual void SendUeContextRelease (const struct UeContextRelease& params) = 0;
};


class EpcX2SapUser
{
public:
  virtual ~EpcX2SapUser ();

  /**
   * Parameters of the API primitives
   */
  
  struct HandoverRequestParams
  {
    uint32_t            targetCellId;
    std::list<uint32_t> bearers;
    Ptr<Packet>         rrcContext;
  };

  struct HandoverRequestAckParams
  {
    std::list<uint32_t> bearers;
    Ptr<Packet>         rrcContext;
  };

  /**
   * SAP primitives
   */

  virtual void RecvHandoverRequest (HandoverRequestParams params) = 0;
  
  virtual void RecvHandoverRequestAck (HandoverRequestAckParams params) = 0;

// TODO
//   virtual void RecvSnStatusTransfer (const struct SnStatusTransfer& params) = 0;
// 
//   virtual void RecvUeContextRelease (const struct UeContextRelease& params) = 0;
};

///////////////////////////////////////

template <class C>
class EpcX2SpecificEpcX2SapProvider : public EpcX2SapProvider
{
public:
  EpcX2SpecificEpcX2SapProvider (C* x2);

  //
  // Interface implemented from EpcX2SapProvider
  //

  virtual void SendHandoverRequest (HandoverRequestParams params);

  virtual void SendHandoverRequestAck (HandoverRequestAckParams params);
  
private:
  EpcX2SpecificEpcX2SapProvider ();
  C* m_x2;
};

template <class C>
EpcX2SpecificEpcX2SapProvider<C>::EpcX2SpecificEpcX2SapProvider (C* x2)
  : m_x2 (x2)
{
}

template <class C>
EpcX2SpecificEpcX2SapProvider<C>::EpcX2SpecificEpcX2SapProvider ()
{
}

template <class C>
void
EpcX2SpecificEpcX2SapProvider<C>::SendHandoverRequest (HandoverRequestParams params)
{
  m_x2->DoSendHandoverRequest (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapProvider<C>::SendHandoverRequestAck (HandoverRequestAckParams params)
{
  m_x2->DoSendHandoverRequestAck (params);
}

///////////////////////////////////////

template <class C>
class EpcX2SpecificEpcX2SapUser : public EpcX2SapUser
{
public:
  EpcX2SpecificEpcX2SapUser (C* rrc);

  //
  // Interface implemented from EpcX2SapUser
  //

  virtual void RecvHandoverRequest (HandoverRequestParams params);

  virtual void RecvHandoverRequestAck (HandoverRequestAckParams params);
  
private:
  EpcX2SpecificEpcX2SapUser ();
  C* m_rrc;
};

template <class C>
EpcX2SpecificEpcX2SapUser<C>::EpcX2SpecificEpcX2SapUser (C* rrc)
  : m_rrc (rrc)
{
}

template <class C>
EpcX2SpecificEpcX2SapUser<C>::EpcX2SpecificEpcX2SapUser ()
{
}

template <class C>
void
EpcX2SpecificEpcX2SapUser<C>::RecvHandoverRequest (HandoverRequestParams params)
{
  m_rrc->DoRecvHandoverRequest (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapUser<C>::RecvHandoverRequestAck (HandoverRequestAckParams params)
{
  m_rrc->DoRecvHandoverRequestAck (params);
}


} // namespace ns3

#endif // EPC_X2_SAP_H
