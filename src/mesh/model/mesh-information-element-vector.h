/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 IITP RAS
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
 * Authors: Kirill Andreev <andreev@iitp.ru>
 *          Pavel Boyko <boyko.iitp.ru>
 */

#ifndef MESH_INFORMATION_ELEMENT_VECTOR_H
#define MESH_INFORMATION_ELEMENT_VECTOR_H

#include "ns3/wifi-information-element-vector.h"
#include "ns3/mesh-information-element.h"

namespace ns3 {

class MeshInformationElementVector : public WifiInformationElementVector
{
public:
  virtual uint32_t DeserializeSingleIe (Buffer::Iterator start);
};

}

#endif
