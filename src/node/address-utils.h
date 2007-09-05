/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
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
#ifndef ADDRESS_UTILS_H
#define ADDRESS_UTILS_H

#include "ns3/buffer.h"
#include "ipv4-address.h"
#include "address.h"
#include "eui48-address.h"

namespace ns3 {

void WriteTo (Buffer::Iterator &i, Ipv4Address ad);
void WriteTo (Buffer::Iterator &i, const Address &ad);
void WriteTo (Buffer::Iterator &i, Eui48Address ad);

void ReadFrom (Buffer::Iterator &i, Ipv4Address &ad);
void ReadFrom (Buffer::Iterator &i, Address &ad, uint32_t len);
void ReadFrom (Buffer::Iterator &i, Eui48Address &ad);

};

#endif /* ADDRESS_UTILS_H */
