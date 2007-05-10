/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
 * All rights reserved.
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
#include "object.h"
#include "debug.h"
#include "assert.h"

NS_DEBUG_COMPONENT_DEFINE ("Object");

namespace ns3 {

Object::Object ()
  : m_count (0),
    m_disposed (false)
{
  NS_DEBUG ("Object::Object: m_count=0");
}

Object::~Object ()
{}

void 
Object::Ref (void) const
{
  m_count++;
  NS_DEBUG("Object::Ref (): this == 0x" << this << " m_count=" << m_count);
}

void 
Object::Unref (void) const
{
  NS_ASSERT (m_count > 0);
  m_count--;
  NS_DEBUG("Object::Unref (): this == 0x" << this << " m_count=" << m_count);

  if (m_count == 0)
    {
      NS_DEBUG("Object::Unref (): delete");
      delete this;
    }
}

bool 
Object::IsSingle (void) const
{
  NS_DEBUG("Object::IsSingle (): m_count == " << m_count);
  return m_count == 1;
}

void
Object::Dispose (void)
{
  NS_ASSERT (!m_disposed);
  DoDispose ();
}

void 
Object::DoDispose (void)
{}

}//namespace ns3
