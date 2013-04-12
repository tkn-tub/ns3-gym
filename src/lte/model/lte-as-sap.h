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


#ifndef LTE_AS_SAP_H
#define LTE_AS_SAP_H

#include <stdint.h>
#include <ns3/ptr.h>
#include <ns3/packet.h>

namespace ns3 {

class LteEnbNetDevice;

/**
 * This class implements the Access Stratum (AS) Service Access Point
 * (SAP), i.e., the interface between the EpcUeNas and the LteUeRrc.
 * In particular, this class implements the  
 * Provider part of the SAP, i.e., the methods exported by the
 * LteUeRrc and called by the EpcUeNas.
 * 
 */
class LteAsSapProvider
{
public:
  virtual ~LteAsSapProvider ();

  /** 
   * Force the RRC to stay camped on a certain eNB
   * 
   * \param enbDevice the eNB device (wild hack, might go away in
   * future versions)
   * \param cellId the Cell ID identifying the eNB
   */
  virtual void ForceCampedOnEnb (uint16_t cellId, uint16_t earfcn) = 0;
  
  /** 
   * Tell the RRC to go into Connected Mode
   * 
   */
  virtual void Connect (void) = 0;

  /** 
   * Send a data packet
   * 
   * \param packet the packet
   * \param bid the EPS bearer ID
   */
  virtual void SendData (Ptr<Packet> packet, uint8_t bid) = 0;


  /** 
   * Tell the RRC to release the connection
   * 
   */
  virtual void Disconnect () = 0;

};
  

/**
 * This class implements the Access Stratum (AS) Service Access Point
 * (SAP), i.e., the interface between the EpcUeNas and the LteUeRrc
 * In particular, this class implements the 
 * User part of the SAP, i.e., the methods exported by the
 * EpcUeNas and called by the LteUeRrc.
 * 
 */
class LteAsSapUser
{
public:
  virtual ~LteAsSapUser ();
  
  /** 
   * Notify the NAS that RRC Connection Establishment was successful
   * 
   */
  virtual void NotifyConnectionSuccessful () = 0;

  /** 
   * Notify the NAS that RRC Connection Establishment failed
   * 
   */
  virtual void NotifyConnectionFailed () = 0;


  /** 
   * Notify the NAS that RRC Connection was released
   * 
   */
  virtual void NotifyConnectionReleased () = 0;

  /** 
   * receive a data packet
   * 
   * \param packet the packet
   */
  virtual void RecvData (Ptr<Packet> packet) = 0;
  
};
  



/**
 * Template for the implementation of the LteAsSapProvider as a member
 * of an owner class of type C to which all methods are forwarded
 * 
 */
template <class C>
class MemberLteAsSapProvider : public LteAsSapProvider
{
public:
  MemberLteAsSapProvider (C* owner);

  // inherited from LteAsSapProvider
  virtual void Connect (void);
  virtual void ForceCampedOnEnb (uint16_t cellId, uint16_t earfcn);
  virtual void SendData (Ptr<Packet> packet, uint8_t bid);
  virtual void Disconnect ();

private:
  MemberLteAsSapProvider ();
  C* m_owner;
};

template <class C>
MemberLteAsSapProvider<C>::MemberLteAsSapProvider (C* owner)
  : m_owner (owner)
{
}

template <class C>
MemberLteAsSapProvider<C>::MemberLteAsSapProvider ()
{
}

template <class C>
void 
MemberLteAsSapProvider<C>::ForceCampedOnEnb (uint16_t cellId, uint16_t earfcn)
{
  m_owner->DoForceCampedOnEnb (cellId, earfcn);
}


template <class C>
void 
MemberLteAsSapProvider<C>::Connect ()
{
  m_owner->DoConnect ();
}

template <class C>
void
MemberLteAsSapProvider<C>::SendData (Ptr<Packet> packet, uint8_t bid)
{
  m_owner->DoSendData (packet, bid);
}

template <class C>
void 
MemberLteAsSapProvider<C>::Disconnect ()
{
  m_owner->DoDisconnect ();
}


/**
 * Template for the implementation of the LteAsSapUser as a member
 * of an owner class of type C to which all methods are forwarded
 * 
 */
template <class C>
class MemberLteAsSapUser : public LteAsSapUser
{
public:
  MemberLteAsSapUser (C* owner);

  // inherited from LteAsSapUser
  virtual void NotifyConnectionSuccessful ();
  virtual void NotifyConnectionFailed ();
  virtual void RecvData (Ptr<Packet> packet);
  virtual void NotifyConnectionReleased ();

private:
  MemberLteAsSapUser ();
  C* m_owner;
};

template <class C>
MemberLteAsSapUser<C>::MemberLteAsSapUser (C* owner)
  : m_owner (owner)
{
}

template <class C>
MemberLteAsSapUser<C>::MemberLteAsSapUser ()
{
}

template <class C>
void 
MemberLteAsSapUser<C>::NotifyConnectionSuccessful ()
{
  m_owner->DoNotifyConnectionSuccessful ();
}

template <class C>
void 
MemberLteAsSapUser<C>::NotifyConnectionFailed ()
{
  m_owner->DoNotifyConnectionFailed ();
}

template <class C>
void 
MemberLteAsSapUser<C>::RecvData (Ptr<Packet> packet)
{
  m_owner->DoRecvData (packet);
}

template <class C>
void 
MemberLteAsSapUser<C>::NotifyConnectionReleased ()
{
  m_owner->DoNotifyConnectionReleased ();
}


} // namespace ns3

#endif // LTE_AS_SAP_H
