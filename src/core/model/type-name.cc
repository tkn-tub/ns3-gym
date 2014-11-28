/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 Georgia Tech Research Corporation
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
 */

#include "type-name.h"

namespace ns3 {

#define DEF_TYPE(x)                             \
  template <>                                   \
  std::string TypeNameGet<x> (void)             \
  {                                             \
    return # x;                                  \
  }

DEF_TYPE (uint8_t);
DEF_TYPE (uint16_t);
DEF_TYPE (uint32_t);
DEF_TYPE (uint64_t);
DEF_TYPE (int8_t);
DEF_TYPE (int16_t);
DEF_TYPE (int32_t);
DEF_TYPE (int64_t);
DEF_TYPE (float);
DEF_TYPE (double);


} // namespace ns3
