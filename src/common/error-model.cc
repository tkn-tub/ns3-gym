/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 University of Washington
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
 * Author: Tom Henderson <tomhend@u.washington.edu>
 * This code has been ported from ns-2 (queue/errmodel.{cc,h}
 */

#include <math.h>

#include "error-model.h"

#include "ns3/packet.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/random-variable.h"
#include "ns3/boolean.h"
#include "ns3/enum.h"
#include "ns3/double.h"

NS_LOG_COMPONENT_DEFINE ("ErrorModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (ErrorModel);

TypeId ErrorModel::GetTypeId (void)
{ 
  static TypeId tid = TypeId ("ErrorModel")
    .SetParent<Object> ()
    .AddAttribute ("IsEnabled", "Whether this ErrorModel is enabled or not.",
                   Boolean (true),
                   MakeBooleanAccessor (&ErrorModel::m_enable),
                   MakeBooleanChecker ())
    ;
  return tid;
}

ErrorModel::ErrorModel () :
  m_enable (true) 
{
  NS_LOG_FUNCTION;  
}

ErrorModel::~ErrorModel ()
{
  NS_LOG_FUNCTION;  
}

bool
ErrorModel::IsCorrupt (Ptr<Packet> p)
{
  NS_LOG_FUNCTION;
  bool result;
  // Insert any pre-conditions here
  result = DoCorrupt (p);
  // Insert any post-conditions here
  return result;
}

void
ErrorModel::Reset (void)
{
  NS_LOG_FUNCTION;  
  DoReset ();
}

void
ErrorModel::Enable (void)
{
  NS_LOG_FUNCTION;  
  m_enable = true;
}

void
ErrorModel::Disable (void)
{
  NS_LOG_FUNCTION;  
  m_enable = false;
}

bool
ErrorModel::IsEnabled (void) const
{
  NS_LOG_FUNCTION;  
  return m_enable;
}

//
// RateErrorModel
//

NS_OBJECT_ENSURE_REGISTERED (RateErrorModel);

TypeId RateErrorModel::GetTypeId (void)
{ 
  static TypeId tid = TypeId ("RateErrorModel")
    .SetParent<ErrorModel> ()
    .AddConstructor<RateErrorModel> ()
    .AddAttribute ("Unit", "The error unit",
                   Enum (EU_BYTE),
                   MakeEnumAccessor (&RateErrorModel::m_unit),
                   MakeEnumChecker (EU_BYTE, "EU_BYTE",
                                    EU_PKT, "EU_PKT",
                                    EU_BIT, "EU_BIT"))
    .AddAttribute ("Rate", "The error rate.",
                   Double (0.0),
                   MakeDoubleAccessor (&RateErrorModel::m_rate),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("RanVar", "The decision variable attached to this error model.",
                   UniformVariable (0.0, 1.0),
                   MakeRandomVariableAccessor (&RateErrorModel::m_ranvar),
                   MakeRandomVariableChecker ())
    ;
  return tid;
}


RateErrorModel::RateErrorModel ()
{
  NS_LOG_FUNCTION;
}

RateErrorModel::~RateErrorModel () 
{
  NS_LOG_FUNCTION;
}

enum ErrorUnit 
RateErrorModel::GetUnit (void) const 
{ 
  NS_LOG_FUNCTION;
  return m_unit; 
}

void 
RateErrorModel::SetUnit (enum ErrorUnit error_unit) 
{ 
  NS_LOG_FUNCTION;
  m_unit = error_unit; 
}

double
RateErrorModel::GetRate (void) const 
{ 
  NS_LOG_FUNCTION;
  return m_rate; 
}

void 
RateErrorModel::SetRate (double rate)
{ 
  NS_LOG_FUNCTION;
  m_rate = rate;
}

void 
RateErrorModel::SetRandomVariable (const RandomVariable &ranvar)
{
  NS_LOG_FUNCTION;
  m_ranvar = ranvar;
}

bool 
RateErrorModel::DoCorrupt (Ptr<Packet> p) 
{ 
  NS_LOG_FUNCTION;
  if (!m_enable)
    {
      return false;  
    }
  switch (m_unit) 
    {
    case EU_PKT:
      return DoCorruptPkt (p);
    case EU_BYTE:
      return DoCorruptByte (p);
    case EU_BIT:
      return DoCorruptBit (p);
    default:
      NS_ASSERT_MSG (false, "m_unit not supported yet");
      break;
    }
  return false;
}

bool
RateErrorModel::DoCorruptPkt (Ptr<Packet> p)
{
  NS_LOG_FUNCTION;
  return (m_ranvar.GetValue () < m_rate);
}

bool
RateErrorModel::DoCorruptByte (Ptr<Packet> p)
{
  NS_LOG_FUNCTION;
  // compute pkt error rate, assume uniformly distributed byte error
  double per = 1 - pow (1.0 - m_rate, p->GetSize ());
  return (m_ranvar.GetValue () < per);
}

bool
RateErrorModel::DoCorruptBit(Ptr<Packet> p)
{
  NS_LOG_FUNCTION;
  // compute pkt error rate, assume uniformly distributed bit error
  double per = 1 - pow (1.0 - m_rate, (8 * p->GetSize ()) );
  return (m_ranvar.GetValue () < per);
}

void 
RateErrorModel::DoReset (void) 
{ 
  NS_LOG_FUNCTION;
  /* re-initialize any state; no-op for now */ 
}

//
// ListErrorModel
//

NS_OBJECT_ENSURE_REGISTERED (ListErrorModel);

TypeId ListErrorModel::GetTypeId (void)
{ 
  static TypeId tid = TypeId ("ListErrorModel")
    .SetParent<ErrorModel> ()
    .AddConstructor<ListErrorModel> ()
    ;
  return tid;
}

ListErrorModel::ListErrorModel ()  
{
  NS_LOG_FUNCTION;
}

ListErrorModel::~ListErrorModel () 
{
  NS_LOG_FUNCTION;
}

std::list<uint32_t> 
ListErrorModel::GetList (void) const 
{ 
  NS_LOG_FUNCTION;
  return m_packetList; 
}

void 
ListErrorModel::SetList (const std::list<uint32_t> &packetlist)
{ 
  NS_LOG_FUNCTION;
  m_packetList = packetlist;
}

// When performance becomes a concern, the list provided could be 
// converted to a dynamically-sized array of uint32_t to avoid 
// list iteration below.
bool 
ListErrorModel::DoCorrupt (Ptr<Packet> p) 
{ 
  NS_LOG_FUNCTION;
  if (!m_enable)
    {
      return false;  
    }
  uint32_t uid = p->GetUid ();
  for (PacketListCI i = m_packetList.begin (); 
    i != m_packetList.end (); i++) 
    {
      if (uid == *i)
      {
        return true;
      }
    }
  return false;
}

void 
ListErrorModel::DoReset (void) 
{ 
  NS_LOG_FUNCTION;
  m_packetList.clear();
}


} //namespace ns3
