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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef TAG_BUFFER_H
#define TAG_BUFFER_H

#include <stdint.h>

namespace ns3 {

/**
 * \brief read and write tag data
 *
 * This class allows subclasses of the ns3::Tag base class
 * to serialize and deserialize their data.
 */
class TagBuffer
{
public:
  TagBuffer (uint8_t *start, uint8_t *end);
  void TrimAtEnd (uint32_t trim);

  void WriteU8 (uint8_t v);
  void WriteU16 (uint16_t v);
  void WriteU32 (uint32_t v);
  void WriteU64 (uint64_t v);
  void WriteDouble (double v);
  void Write (const uint8_t *buffer, uint32_t size);
  uint8_t  ReadU8 (void);
  uint16_t ReadU16 (void);
  uint32_t ReadU32 (void);
  uint64_t ReadU64 (void);
  double ReadDouble (void);
  void Read (uint8_t *buffer, uint32_t size);

  void CopyFrom (TagBuffer o);
private:
  
  uint8_t *m_current;
  uint8_t *m_end;
};

} // namespace ns3

#endif /* TAG_BUFFER_H */
