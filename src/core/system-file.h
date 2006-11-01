/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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

#ifndef SYSTEM_FILE_H
#define SYSTEM_FILE_H

#include <stdint.h>

namespace ns3 {

class SystemFilePrivate;

/**
 * \brief os-independent file creation and writing
 *
 * Create a file and write data to this file.
 */
class SystemFile {
public:
  /**
   * This method does not create or open any
   * file on disk.
   */
  SystemFile ();
  /**
   * If a file has been opened, it is closed by
   * this destructor.
   */
  ~SystemFile ();

  /**
   * \param filename name of file to open
   *
   * Open a file for writing. If the file does not
   * exist, it is created. If it exists, it is 
   * emptied first.
   */
  void Open (char const *filename);
  /**
   * \param buffer data to write
   * \param size size of data to write
   *
   * Write data in file on disk. This method cannot fail:
   * it will write _all_ the data to disk. This method does not
   * perform any data caching and forwards the data
   * to the OS through a direct syscall. However, 
   * it is not possible to rely on the data being
   * effectively written to disk after this method returns.
   * To make sure the data is written to disk, destroy 
   * this object.
   */
  void Write (uint8_t const*buffer, uint32_t size);
private:
  SystemFilePrivate *m_priv;
};

}; //namespace ns3

#endif /* SYSTEM_FILE_H */
