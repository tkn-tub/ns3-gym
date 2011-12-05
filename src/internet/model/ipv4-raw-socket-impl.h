/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef IPV4_RAW_SOCKET_IMPL_H
#define IPV4_RAW_SOCKET_IMPL_H

#include "ns3/socket.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv4-route.h"
#include "ns3/ipv4-interface.h"
#include <list>

namespace ns3 {

class NetDevice;
class Node;

class Ipv4RawSocketImpl : public Socket
{
public:
  static TypeId GetTypeId (void);

  Ipv4RawSocketImpl ();

  void SetNode (Ptr<Node> node);

  virtual enum Socket::SocketErrno GetErrno (void) const;
  virtual enum Socket::SocketType GetSocketType (void) const;
  virtual Ptr<Node> GetNode (void) const;
  virtual int Bind (const Address &address);
  virtual int Bind ();
  virtual int GetSockName (Address &address) const; 
  virtual int Close (void);
  virtual int ShutdownSend (void);
  virtual int ShutdownRecv (void);
  virtual int Connect (const Address &address);
  virtual int Listen (void);
  virtual uint32_t GetTxAvailable (void) const;
  virtual int Send (Ptr<Packet> p, uint32_t flags);
  virtual int SendTo (Ptr<Packet> p, uint32_t flags, 
                      const Address &toAddress);
  virtual uint32_t GetRxAvailable (void) const;
  virtual Ptr<Packet> Recv (uint32_t maxSize, uint32_t flags);
  virtual Ptr<Packet> RecvFrom (uint32_t maxSize, uint32_t flags,
                                Address &fromAddress);

  void SetProtocol (uint16_t protocol);
  bool ForwardUp (Ptr<const Packet> p, Ipv4Header ipHeader, Ptr<Ipv4Interface> incomingInterface);
  virtual bool SetAllowBroadcast (bool allowBroadcast);
  virtual bool GetAllowBroadcast () const;

private:
  virtual void DoDispose (void);

  struct Data {
    Ptr<Packet> packet;
    Ipv4Address fromIp;
    uint16_t fromProtocol;
  };

  enum Socket::SocketErrno m_err;
  Ptr<Node> m_node;
  Ipv4Address m_src;
  Ipv4Address m_dst;
  uint16_t m_protocol;
  std::list<struct Data> m_recv;
  bool m_shutdownSend;
  bool m_shutdownRecv;
  uint32_t m_icmpFilter;
  bool m_iphdrincl;
};

} // namespace ns3

#endif /* IPV4_RAW_SOCKET_IMPL_H */
