/* -*-    Mode:C++; c-basic-offset:4; tab-width:4; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2006 INRIA
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
#ifndef STREAM_TRACER_H
#define STREAM_TRACER_H

#include <ostream>

namespace ns3 {

/**
 * \brief log arbitrary data to std::ostreams
 * 
 * Whenever operator << is invoked on this class,
 * it is forwarded to the stored std::ostream output
 * stream (if there is one).
 */
class StreamTracer {
public:
    StreamTracer ()
        : m_os (0) {}
    template <typename T>
    StreamTracer &operator << (T const&v) {
        if (m_os != 0) {
            (*m_os) << v;
        }
        return *this;
    }
    template <typename T>
    StreamTracer &operator << (T &v) {
        if (m_os != 0) {
            (*m_os) << v;
        }
        return *this;
    }
    StreamTracer &operator << (std::ostream &(*v) (std::ostream &)) {
        if (m_os != 0) {
            (*m_os) << v;
        }
        return *this;
    }

    /**
     * \param os the output stream to store
     */
    void setStream (std::ostream * os) {
        m_os = os;
    }
private:
    std::ostream *m_os;
};

}; // namespace ns3


#endif /* TRACER_STREAM_H */
