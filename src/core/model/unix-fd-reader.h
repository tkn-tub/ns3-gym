/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 The Boeing Company
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
 * Author: Tom Goff <thomas.goff@boeing.com>
 */

#ifndef UNIX_FD_READER_H
#define UNIX_FD_READER_H

#include <stdint.h>

#include "callback.h"
#include "system-thread.h"
#include "event-id.h"

namespace ns3 {

/**
 * \brief A class that asynchronously reads from a file descriptor.
 *
 * This class can be used to start a system thread that reads from a
 * given file descriptor and invokes a given callback when data is
 * received.  This class handles thread management automatically but
 * the \p DoRead() method must be implemented by a subclass.
 */
class FdReader : public SimpleRefCount<FdReader>
{
public:
  FdReader();
  virtual ~FdReader();

  /**
   * Start a new read thread.
   *
   * \param fd A valid file descriptor open for reading.
   *
   * \param readCallback A callback to invoke when new data is
   * available.
   */
  void Start (int fd, Callback<void, uint8_t *, ssize_t> readCallback);

  /**
   * Stop the read thread and reset internal state.  This does not
   * close the file descriptor used for reading.
   */
  void Stop (void);

protected:

  /**
   * \internal
   * \brief A structure representing data read.
   */
  struct Data
  {
    Data () : m_buf (0), m_len (0) {}
    Data (uint8_t *buf, ssize_t len) : m_buf (buf), m_len (len) {}
    uint8_t *m_buf;
    ssize_t m_len;
  };

  /**
   * \internal
   * \brief The read implementation.
   *
   * The value of \p m_len returned controls further processing.  The
   * callback function is only invoked when \p m_len is positive; any
   * data read is not processed when \p m_len is negative; reading
   * stops when \p m_len is zero.
   *
   * The management of memory associated with \p m_buf must be
   * compatible with the read callback.
   *
   * \return A structure representing what was read.
   */
  virtual FdReader::Data DoRead (void) = 0;

  /**
   * \internal
   * \brief The file descriptor to read from.
   */
  int m_fd;

private:

  void Run (void);
  void DestroyEvent (void);

  Callback<void, uint8_t *, ssize_t> m_readCallback;
  Ptr<SystemThread> m_readThread;
  int m_evpipe[2];           // pipe used to signal events between threads
  bool m_stop;               // true means the read thread should stop
  EventId m_destroyEvent;
};

} // namespace ns3

#endif /* UNIX_FD_READER_H */
