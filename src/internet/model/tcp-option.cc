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

#include "tcp-option.h"
#include "tcp-option-rfc793.h"
#include "tcp-option-winscale.h"
#include "tcp-option-ts.h"

#include "ns3/type-id.h"

#include <vector>

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (TcpOption);


TcpOption::TcpOption ()
{
}

TcpOption::~TcpOption ()
{
}

TypeId
TcpOption::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TcpOption")
    .SetParent<Object> ()
  ;
  return tid;
}

TypeId
TcpOption::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

Ptr<TcpOption>
TcpOption::CreateOption (uint8_t kind)
{
  struct kindToTid
  {
    TcpOption::Kind kind;
    TypeId tid;
  };

  static ObjectFactory objectFactory;
  static kindToTid toTid[] =
  {
    { TcpOption::END,       TcpOptionEnd::GetTypeId () },
    { TcpOption::MSS,       TcpOptionMSS::GetTypeId () },
    { TcpOption::NOP,       TcpOptionNOP::GetTypeId () },
    { TcpOption::TS,        TcpOptionTS::GetTypeId () },
    { TcpOption::WINSCALE,  TcpOptionWinScale::GetTypeId () }
  };

  for (unsigned int i = 0; i < sizeof (toTid) / sizeof (kindToTid); ++i)
    {
      if (toTid[i].kind == kind)
        {
          objectFactory.SetTypeId (toTid[i].tid);
          return objectFactory.Create<TcpOption> ();
        }
    }

  return CreateObject<TcpOptionUnknown> ();
}

bool
TcpOption::IsKindKnown (uint8_t kind)
{
  switch (kind)
    {
    case END:
    case NOP:
    case MSS:
    case WINSCALE:
    case TS:
      return true;
    }

  return false;
}

} // namespace ns3
