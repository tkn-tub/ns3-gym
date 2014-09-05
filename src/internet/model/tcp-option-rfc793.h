/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Adrian Sai-wah Tam
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
 * Author: Adrian Sai-wah Tam <adrian.sw.tam@gmail.com>
 */
#ifndef TCPOPTIONRFC793_H
#define TCPOPTIONRFC793_H

#include "tcp-option.h"

namespace ns3 {

/**
 * Defines the TCP option of kind 0 (end of option list) as in RFC793
 */
class TcpOptionEnd : public TcpOption
{
public:
  TcpOptionEnd ();
  virtual ~TcpOptionEnd ();

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  virtual void Print (std::ostream &os) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);

  virtual uint8_t GetKind (void) const;
  virtual uint32_t GetSerializedSize (void) const;

};

/**
 * Defines the TCP option of kind 1 (no operation) as in RFC793
 */
class TcpOptionNOP : public TcpOption
{
public:
  TcpOptionNOP ();
  virtual ~TcpOptionNOP ();

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  virtual void Print (std::ostream &os) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);

  virtual uint8_t GetKind (void) const;
  virtual uint32_t GetSerializedSize (void) const;
};

/**
 * Defines the TCP option of kind 2 (maximum segment size) as in RFC793
 */
class TcpOptionMSS : public TcpOption
{
public:
  TcpOptionMSS ();
  virtual ~TcpOptionMSS ();

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  virtual void Print (std::ostream &os) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);

  virtual uint8_t GetKind (void) const;
  virtual uint32_t GetSerializedSize (void) const;

  uint16_t GetMSS (void) const;
  void SetMSS (uint16_t mss);
protected:
  uint16_t m_mss; // maximum segment size
};

/**
 * \brief An unknown tcp option
 */
class TcpOptionUnknown : public TcpOption
{
public:
  TcpOptionUnknown ();
  virtual ~TcpOptionUnknown ();

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  virtual void Print (std::ostream &os) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);

  virtual uint8_t GetKind (void) const;
  virtual uint32_t GetSerializedSize (void) const;
};

} // namespace ns3

#endif // TCPOPTIONRFC793_H
