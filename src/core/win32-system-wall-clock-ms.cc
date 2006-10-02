/* -*-    Mode:C++; c-basic-offset:4; tab-width:4; indent-tabs-mode:nil -*- */
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
 * Author: Mathieu Lacage <mathieu.lacage.inria.fr>
 */

#include "system-wall-clock-ms.h"

namespace ns3 {

class SystemWallClockMsPrivate {
public:
    void start (void);
    unsigned long long end (void);
private:
};

void 
SystemWallClockMsPrivate::start (void)
{
}

unsigned long long 
SystemWallClockMsPrivate::end (void)
{
    return 0;
}

SystemWallClockMs::SystemWallClockMs ()
    : m_priv (new SystemWallClockMsPrivate ())
{}

SystemWallClockMs::~SystemWallClockMs ()
{
    delete m_priv;
    m_priv = 0;
}

void
SystemWallClockMs::start (void)
{
    m_priv->start ();
}
unsigned long long
SystemWallClockMs::end (void)
{
    return m_priv->end ();
}

}; // namespace ns3
