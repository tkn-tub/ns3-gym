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

template <> std::string TypeNameGet< int8_t  > (void) { return "int8_t"  ; }
template <> std::string TypeNameGet< int16_t > (void) { return "int16_t" ; }
template <> std::string TypeNameGet< int32_t > (void) { return "int32_t" ; }
template <> std::string TypeNameGet< int64_t > (void) { return "int64_t" ; }
template <> std::string TypeNameGet< uint8_t > (void) { return "uint8_t" ; }
template <> std::string TypeNameGet< uint16_t> (void) { return "uint16_t"; }
template <> std::string TypeNameGet< uint32_t> (void) { return "uint32_t"; }
template <> std::string TypeNameGet< uint64_t> (void) { return "uint64_t"; }
template <> std::string TypeNameGet< float   > (void) { return "float"   ; }
template <> std::string TypeNameGet< double  > (void) { return "double"  ; }
  
} // namespace ns3
