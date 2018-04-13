/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2018
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
 * Author: Sébastien Deronne <sebastien.deronne@gmail.com>
 */

#ifndef BLOCK_ACK_TYPE_H
#define BLOCK_ACK_TYPE_H

namespace ns3 {

/**
 * \ingroup wifi
 * The different block ACK policies.
 */
enum BlockAckType
{
  BASIC_BLOCK_ACK,
  COMPRESSED_BLOCK_ACK,
  MULTI_TID_BLOCK_ACK
};

} //namespace ns3

#endif /* BLOCK_ACK_TYPE_H */
