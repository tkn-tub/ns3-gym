/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Marco Miozzo <marco.miozzo@cttc.es>
 *         Nicola Baldo <nbaldo@cttc.es>
 */


#include "lte-mac-tag.h"
#include "ns3/tag.h"
#include "ns3/uinteger.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LtePhyTag);

TypeId
LtePhyTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LtePhyTag")
    .SetParent<Tag> ()
    .AddConstructor<LtePhyTag> ()    
  ;
  return tid;
}

TypeId
LtePhyTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

LtePhyTag::LtePhyTag (Ptr<LtePhy> enbPhy)
  : m_enbPhy (enbPhy.PeekPointer ())
{
}

uint32_t
LtePhyTag::GetSerializedSize (void) const
{
  return sizeof (void*);
}

void
LtePhyTag::Serialize (TagBuffer i) const
{
  switch (sizeof (void*))
    {
    case 4:
      i.WriteU32 ((uint32_t) m_enbPhy);
      break;      
    case 8:
      i.WriteU64 ((uint64_t) m_enbPhy);
      break;
    default:
      NS_FATAL ("unknown pointer size");
      break;
    }
}

void
LtePhyTag::Deserialize (TagBuffer i)
{
 switch (sizeof (void*))
    {
    case 4:
      m_enbPhy = (LtePhy*) i.ReadU32 ();
      break;      
    case 8:
      m_enbPhy = (LtePhy*) i.ReadU64 ();
      break;
    default:
      NS_FATAL ("unknown pointer size");
      break;
    }
}

void
LtePhyTag::Print (std::ostream &os) const
{
  os << "0x" << std::hex << m_enbPhy << std::dec;
}

} // namespace ns3
