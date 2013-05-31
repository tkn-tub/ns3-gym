/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "ns3/system-wall-clock-ms.h"
#include "ns3/packet.h"
#include "ns3/packet-metadata.h"
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h> // for exit ()

using namespace ns3;

template <int N>
class BenchHeader : public Header
{
public:
  BenchHeader ();
  bool IsOk (void) const;

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
private:
  static std::string GetTypeName (void);
  bool m_ok;
};

template <int N>
BenchHeader<N>::BenchHeader ()
  : m_ok (false)
{}

template <int N>
bool 
BenchHeader<N>::IsOk (void) const
{
  return m_ok;
}

template <int N>
std::string 
BenchHeader<N>::GetTypeName (void)
{
  std::ostringstream oss;
  oss << "ns3::BenchHeader<" << N << ">";
  return oss.str ();
}

template <int N>
TypeId 
BenchHeader<N>::GetTypeId (void)
{
  static TypeId tid = TypeId (GetTypeName ().c_str ())
    .SetParent<Header> ()
    ;
  return tid;
}
template <int N>
TypeId 
BenchHeader<N>::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

template <int N>
void 
BenchHeader<N>::Print (std::ostream &os) const
{
  NS_ASSERT (false);
}
template <int N>
uint32_t 
BenchHeader<N>::GetSerializedSize (void) const
{
  return N;
}
template <int N>
void 
BenchHeader<N>::Serialize (Buffer::Iterator start) const
{
  start.WriteU8 (N, N);
}
template <int N>
uint32_t
BenchHeader<N>::Deserialize (Buffer::Iterator start)
{
  m_ok = true;
  for (int i = 0; i < N; i++)
    {
      if (start.ReadU8 () != N)
        {
          m_ok = false;
        }
    }
  return N;
}

template <int N>
class BenchTag : public Tag
{
public:
  static std::string GetName (void) {
    std::ostringstream oss;
    oss << "anon::BenchTag<" << N << ">";
    return oss.str ();
  }
  static TypeId GetTypeId (void) {
    static TypeId tid = TypeId (GetName ().c_str ())
      .SetParent<Tag> ()
      .AddConstructor<BenchTag > ()
      .HideFromDocumentation ()
      ;
    return tid;
  }
  virtual TypeId GetInstanceTypeId (void) const {
    return GetTypeId ();
  }
  virtual uint32_t GetSerializedSize (void) const {
    return N;
  }
  virtual void Serialize (TagBuffer buf) const {
    for (uint32_t i = 0; i < N; ++i)
      {
        buf.WriteU8 (N);
      }
  }
  virtual void Deserialize (TagBuffer buf) {
    for (uint32_t i = 0; i < N; ++i)
      {
        buf.ReadU8 ();
      }
  }
  virtual void Print (std::ostream &os) const {
    os << "N=" << N;
  }
  BenchTag ()
    : Tag () {}
};


static void 
benchD (uint32_t n)
{
  BenchHeader<25> ipv4;
  BenchHeader<8> udp;
  BenchTag<16> tag1;
  BenchTag<17> tag2;

  for (uint32_t i = 0; i < n; i++) {
    Ptr<Packet> p = Create<Packet> (2000);
    p->AddPacketTag (tag1);
    p->AddHeader (udp);
    p->RemovePacketTag (tag1);
    p->AddPacketTag (tag2);
    p->AddHeader (ipv4);
    Ptr<Packet> o = p->Copy ();
    o->RemoveHeader (ipv4);
    p->RemovePacketTag (tag2);
    o->RemoveHeader (udp);
  }
}



static void 
benchA (uint32_t n)
{
  BenchHeader<25> ipv4;
  BenchHeader<8> udp;

  for (uint32_t i = 0; i < n; i++) {
    Ptr<Packet> p = Create<Packet> (2000);
    p->AddHeader (udp);
    p->AddHeader (ipv4);
    Ptr<Packet> o = p->Copy ();
    o->RemoveHeader (ipv4);
    o->RemoveHeader (udp);
  }
}

static void 
benchB (uint32_t n)
{
  BenchHeader<25> ipv4;
  BenchHeader<8> udp;

  for (uint32_t i = 0; i < n; i++) {
    Ptr<Packet> p = Create<Packet> (2000);
    p->AddHeader (udp);
    p->AddHeader (ipv4);
  }
}

static void
C2 (Ptr<Packet> p)
{
  BenchHeader<8> udp;

  p->RemoveHeader (udp);
}

static void 
C1 (Ptr<Packet> p)
{
  BenchHeader<25> ipv4;
  p->RemoveHeader (ipv4);
  C2 (p);
}

static void
benchC (uint32_t n)
{
  BenchHeader<25> ipv4;
  BenchHeader<8> udp;

  for (uint32_t i = 0; i < n; i++) {
    Ptr<Packet> p = Create<Packet> (2000);
    p->AddHeader (udp);
    p->AddHeader (ipv4);
    C1 (p);
  }
}


static void
runBench (void (*bench) (uint32_t), uint32_t n, char const *name)
{
  SystemWallClockMs time;
  time.Start ();
  (*bench) (n);
  uint64_t deltaMs = time.End ();
  double ps = n;
  ps *= 1000;
  ps /= deltaMs;
  std::cout << ps << " packets/s"
            << " (" << deltaMs << " ms elapsed)\t"
            << name
            << std::endl;
}

int main (int argc, char *argv[])
{
  uint32_t n = 0;
  while (argc > 0) {
      if (strncmp ("--n=", argv[0],strlen ("--n=")) == 0) 
        {
          char const *nAscii = argv[0] + strlen ("--n=");
          std::istringstream iss;
          iss.str (nAscii);
          iss >> n;
        }
      if (strncmp ("--enable-printing", argv[0], strlen ("--enable-printing")) == 0)
        {
          Packet::EnablePrinting ();
        }
      argc--;
      argv++;
  }
  if (n == 0)
    {
      std::cerr << "Error-- number of packets must be specified " <<
        "by command-line argument --n=(number of packets)" << std::endl;
      exit (1);
    }
  std::cout << "Running bench-packets with n=" << n << std::endl;
  std::cout << "All tests begin by adding UDP and IPv4 headers." << std::endl;

  runBench (&benchA, n, "Copy packet, remove headers");
  runBench (&benchB, n, "Just add headers");
  runBench (&benchC, n, "Remove by func call");
  runBench (&benchD, n, "Intermixed add/remove headers and tags");

  return 0;
}
