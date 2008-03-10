/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "trace-source-accessor.h"

namespace ns3 {

TraceSourceAccessor::TraceSourceAccessor ()
  : m_count (1)
{}
TraceSourceAccessor::~TraceSourceAccessor ()
{}
void 
TraceSourceAccessor::Ref (void) const
{
  m_count++;
}
void 
TraceSourceAccessor::Unref (void) const
{
  m_count--;
  if (m_count == 0)
    {
      delete this;
    }
}

} // namespace ns3
