/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 INRIA
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
 * Author: Guillaume Seguin <guilla...@segu.in>
 */

#ifndef ASCII_WRITER_H
#define ASCII_WRITER_H

#include <stdint.h>
#include <ostream>
#include "ns3/simple-ref-count.h"
#include "ns3/ptr.h"

namespace ns3 {

class Packet;

/**
 * \ingroup common
 *
 * \brief Ascii output
 */
class AsciiWriter : public SimpleRefCount<AsciiWriter>
{
public:
  static Ptr<AsciiWriter> Get (std::ostream &os);

  enum Type {
    ENQUEUE,
    DEQUEUE,
    DROP,
    TX,
    RX
  };

  ~AsciiWriter (void);

  /**
   * Writes a message in the output file, checking if the files will
   * need to be reordered.
   */
  void WritePacket (enum Type type, std::string message, Ptr<const Packet> p);

private:
  AsciiWriter (std::ostream *os);

  std::ostream *m_writer;
};

} // namespace ns3

#endif /* ASCII_WRITER_H */
