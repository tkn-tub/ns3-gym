/* -*- Mode:NS3; -*- */
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
#include "data-writer.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <cassert>
#include <string.h>
#include <list>

#define noTRACE_DATA_WRITER 1

#ifdef TRACE_DATA_WRITER
#include <iostream>
# define TRACE(x) \
std::cout << "DATA WRITER TRACE " << this << " " << x << std::endl;
#else /* TRACE_DATA_WRITER */
# define TRACE(format,...)
#endif /* TRACE_DATA_WRITER */

#define BUFFER_SIZE (4096)


namespace ns3 {

class DataWriterPrivate {
public:
    DataWriterPrivate ();
    ~DataWriterPrivate ();

    void open (char const *filename);
    void write (uint8_t *buffer, uint32_t size);
private:
    uint8_t m_data[BUFFER_SIZE];
    uint32_t m_current;
    int m_fd;
};

DataWriterPrivate::DataWriterPrivate ()
    : m_current (0)
{}
DataWriterPrivate::~DataWriterPrivate ()
{
    ::Write (m_fd, m_data, m_current);
    ::Close (m_fd);
}


void
DataWriterPrivate::Open (char const *filename)
{
    m_fd = ::Open (filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    assert (m_fd != -1);
}

#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif /* min */

void
DataWriterPrivate::Write (uint8_t *buffer, uint32_t size)
{
    while (size > 0) {
        uint32_t toCopy = min (BUFFER_SIZE - m_current, size);
        memcpy (m_data + m_current, buffer, toCopy);
        size -= toCopy;
        m_current += toCopy;
        buffer += toCopy;
        if (m_current == BUFFER_SIZE) {
            ssize_t written = 0;
            written = ::Write (m_fd, m_data, BUFFER_SIZE);
            assert (written == BUFFER_SIZE);
            m_current = 0;
        }
    }
}

DataWriter::DataWriter ()
    : m_priv (new DataWriterPrivate ())
{}
DataWriter::~DataWriter ()
{
    delete m_priv;
    m_priv = 0;
}

void 
DataWriter::Open (char const *filename)
{
    m_priv->Open (filename);
}
void 
DataWriter::Write (uint8_t *buffer, uint32_t size)
{
    m_priv->Write (buffer, size);
}

}; // namespace
