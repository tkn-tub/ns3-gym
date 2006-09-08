/* -*-    Mode:C++; c-basic-offset:4; tab-width:4; indent-tabs-mode:f -*- */
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
#include "system-file.h"


#define noTRACE_SYS_FILE 1

#ifdef TRACE_SYS_FILE
#include <iostream>
# define TRACE(x) \
std::cout << "SYS FILE TRACE " << this << " " << x << std::endl;
#else /* TRACE_SYS_FILE */
# define TRACE(format,...)
#endif /* TRACE_SYS_FILE */

#define BUFFER_SIZE (4096)


namespace ns3 {

class SystemFilePrivate {
public:
    SystemFilePrivate ();
    ~SystemFilePrivate ();

    void open (char const *filename);
    void write (uint8_t const*buffer, uint32_t size);
private:
};

SystemFilePrivate::SystemFilePrivate ()
{}
SystemFilePrivate::~SystemFilePrivate ()
{
}


void
SystemFilePrivate::open (char const *filename)
{
}

void
SystemFilePrivate::write (uint8_t const*buffer, uint32_t size)
{
}

SystemFile::SystemFile ()
    : m_priv (new SystemFilePrivate ())
{}
SystemFile::~SystemFile ()
{
    delete m_priv;
    m_priv = 0;
}

void 
SystemFile::open (char const *filename)
{
    m_priv->open (filename);
}
void 
SystemFile::write (uint8_t const*buffer, uint32_t size)
{
    m_priv->write (buffer, size);
}

}; // namespace
