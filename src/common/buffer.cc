/* -*-    Mode:C++; c-basic-offset:4; tab-width:4; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
#include "buffer.h"
#include <cassert>

#include <iostream>
//#define TRACE(x) std::cout << x << std::endl;
#define TRACE(x)

namespace ns3 {

Buffer::BufferDataList  Buffer::m_freeList;
uint32_t Buffer::m_maxTotalAddStart = 0;
uint32_t Buffer::m_maxTotalAddEnd = 0;

struct Buffer::BufferData *
Buffer::allocate (uint32_t reqSize, uint32_t reqStart)
{
    if (reqSize == 0) {
        reqSize = 1;
    }
    assert (reqSize >= 1);
    uint32_t size = reqSize - 1 + sizeof (struct Buffer::BufferData);
    uint8_t *b = new uint8_t [size];
    struct BufferData *data = reinterpret_cast<struct Buffer::BufferData*>(b);
    data->m_size = reqSize;
    data->m_initialStart = reqStart;
    data->m_dirtyStart = reqStart;
    data->m_dirtySize = 0;
    data->m_count = 1;
    return data;
}

void
Buffer::deallocate (struct Buffer::BufferData *data)
{
    uint8_t *buf = reinterpret_cast<uint8_t *> (data);
    delete [] buf;
}
#ifdef USE_FREE_LIST
void
Buffer::recycle (struct Buffer::BufferData *data)
{
    assert (data->m_count == 0);
    /* get rid of it if it is too small for later reuse. */
    if (data->m_size < (Buffer::m_maxTotalAddStart + Buffer::m_maxTotalAddEnd)) {
        Buffer::deallocate (data);
        return; 
    }
    /* feed into free list */
    if (Buffer::m_freeList.size () > 1000) {
        Buffer::deallocate (data);
    } else {
        Buffer::m_freeList.push_back (data);
    }
}

Buffer::BufferData *
Buffer::create (void)
{
    /* try to find a buffer correctly sized. */
    while (!Buffer::m_freeList.empty ()) {
        struct Buffer::BufferData *data = Buffer::m_freeList.back ();
        Buffer::m_freeList.pop_back ();
        if (data->m_size >= (m_maxTotalAddStart + m_maxTotalAddEnd)) {
            data->m_initialStart = m_maxTotalAddStart;
            data->m_dirtyStart = m_maxTotalAddStart;
            data->m_dirtySize = 0;
            data->m_count = 1;
            return data;
        }
        Buffer::deallocate (data);
    }
    struct Buffer::BufferData *data = Buffer::allocate (m_maxTotalAddStart+m_maxTotalAddEnd,
                                m_maxTotalAddStart);
    assert (data->m_count == 1);
    return data;
}
#else
void
Buffer::recycle (struct Buffer::BufferData *data)
{
    Buffer::deallocate (data);
}

Buffer::BufferData *
Buffer::create (void)
{
    return Buffer::allocate (m_maxTotalAddStart+m_maxTotalAddEnd,
                 m_maxTotalAddStart);
}
#endif

}; // namespace ns3


#include <cassert>

namespace ns3 {


void 
Buffer::addAtStart (uint32_t start)
{
    assert (m_start <= m_data->m_initialStart);
    bool isDirty = m_data->m_count > 1 && m_start > m_data->m_dirtyStart;
    if (m_start >= start && !isDirty) {
        /* enough space in the buffer and not dirty. */
        m_start -= start;
        m_size += start;
    } else if (m_size + start <= m_data->m_size && !isDirty) {
        /* enough space but need to move data around to fit new data */
    memmove (m_data->m_data + start, getStart (), m_size);
        assert (start > m_start);
        m_data->m_initialStart += start;
    m_start = 0;
    m_size += start;
    } else if (m_start < start) {
        /* not enough space in buffer */
        uint32_t newSize = m_size + start;
        struct Buffer::BufferData *newData = Buffer::allocate (newSize, 0);
        memcpy (newData->m_data + start, getStart (), m_size);
        newData->m_initialStart = m_data->m_initialStart + start;
        m_data->m_count--;
        if (m_data->m_count == 0) {
            Buffer::deallocate (m_data);
        }
        m_data = newData;
        m_start = 0;
        m_size = newSize;
    } else {
        /* enough space in the buffer but it is dirty ! */
        assert (isDirty);
        struct Buffer::BufferData *newData = Buffer::create ();
        memcpy (newData->m_data + m_start, getStart (), m_size);
        newData->m_initialStart = m_data->m_initialStart;
        m_data->m_count--;
        if (m_data->m_count == 0) {
            recycle (m_data);
        }
        m_data = newData;
        m_start -= start;
        m_size += start;
    } 
    // update dirty area
    m_data->m_dirtyStart = m_start;
    m_data->m_dirtySize = m_size;
    // update m_maxTotalAddStart
    uint32_t addedAtStart;
    if (m_data->m_initialStart > m_start) {
        addedAtStart = m_data->m_initialStart - m_start;
    } else {
        addedAtStart = 0;
    }
    if (addedAtStart > m_maxTotalAddStart) {
        m_maxTotalAddStart = addedAtStart;
    }
    TRACE ("start add="<<start<<", start="<<m_start<<", size="<<m_size<<", zero="<<m_zeroAreaSize<<
           ", real size="<<m_data->m_size<<", ini start="<<m_data->m_initialStart<<
           ", dirty start="<<m_data->m_dirtyStart<<", dirty size="<<m_data->m_dirtySize); 
}
void 
Buffer::addAtEnd (uint32_t end)
{
    assert (m_start <= m_data->m_initialStart);
    bool isDirty = m_data->m_count > 1 &&
        m_start + m_size < m_data->m_dirtyStart + m_data->m_dirtySize;
    if (m_start + m_size + end <= m_data->m_size && !isDirty) {
        /* enough space in buffer and not dirty */
        m_size += end;
    } else if (m_size + end <= m_data->m_size && !isDirty) {
        /* enough space but need to move data around to fit the extra data */
    uint32_t newStart = m_data->m_size - (m_size + end);
    memmove (m_data->m_data + newStart, getStart (), m_size);
        assert (newStart < m_start);
        m_data->m_initialStart -= m_start - newStart;
    m_start = newStart;
    m_size += end;
    } else if (m_start + m_size + end > m_data->m_size) {
        /* not enough space in buffer */
        uint32_t newSize = m_size + end;
        struct Buffer::BufferData *newData = Buffer::allocate (newSize, 0);
        memcpy (newData->m_data, getStart (), m_size);
        newData->m_initialStart = m_data->m_initialStart;
        m_data->m_count--;
        if (m_data->m_count == 0) {
            Buffer::deallocate (m_data);
        }
        m_data = newData;
        m_size = newSize;
        m_start = 0;
    } else {
        /* enough space in the buffer but it is dirty ! */
        assert (isDirty);
        struct Buffer::BufferData *newData = Buffer::create ();
        memcpy (newData->m_data + m_start, getStart (), m_size);
        newData->m_initialStart = m_data->m_initialStart;
        m_data->m_count--;
        if (m_data->m_count == 0) {
            recycle (m_data);
        }
        m_data = newData;
        m_size += end;
    } 
    // update dirty area
    m_data->m_dirtyStart = m_start;
    m_data->m_dirtySize = m_size;
    // update m_maxTotalAddEnd
    uint32_t endLoc = m_start + m_size;
    uint32_t addedAtEnd;
    if (m_data->m_initialStart < endLoc) {
        addedAtEnd = endLoc - m_data->m_initialStart;
    } else {
        addedAtEnd = 0;
    }
    if (addedAtEnd > m_maxTotalAddEnd) {
        m_maxTotalAddEnd = addedAtEnd;
    }
    TRACE ("end add="<<end<<", start="<<m_start<<", size="<<m_size<<", zero="<<m_zeroAreaSize<<
           ", real size="<<m_data->m_size<<", ini start="<<m_data->m_initialStart<<
           ", dirty start="<<m_data->m_dirtyStart<<", dirty size="<<m_data->m_dirtySize); 
}

void 
Buffer::removeAtStart (uint32_t start)
{
    if (m_zeroAreaSize == 0) {
        if (m_size <= start) {
            m_start += m_size;
            m_size = 0;
        } else {
            m_start += start;
            m_size -= start;
        }
    } else {
        assert (m_data->m_initialStart >= m_start);
        uint32_t zeroStart = m_data->m_initialStart - m_start;
        uint32_t zeroEnd = zeroStart + m_zeroAreaSize;
        uint32_t dataEnd = m_size + m_zeroAreaSize;
        if (start <= zeroStart) {
            /* only remove start of buffer */
            m_start += start;
            m_size -= start;
        } else if (start <= zeroEnd) {
            /* remove start of buffer _and_ start of zero area */
            m_start += zeroStart;
            uint32_t zeroDelta = start - zeroStart;
            m_zeroAreaSize -= zeroDelta;
            assert (zeroDelta <= start);
            m_size -= zeroStart;
        } else if (start <= dataEnd) {
            /* remove start of buffer, complete zero area, and part
             * of end of buffer */
            m_start += start - m_zeroAreaSize;
            m_size -= start - m_zeroAreaSize;
            m_zeroAreaSize = 0;
        } else {
            /* remove all buffer */
            m_start += m_size;
            m_size = 0;
            m_zeroAreaSize = 0;
        }
    }
    TRACE ("start remove="<<start<<", start="<<m_start<<", size="<<m_size<<", zero="<<m_zeroAreaSize<<
           ", real size="<<m_data->m_size<<", ini start="<<m_data->m_initialStart<<
           ", dirty start="<<m_data->m_dirtyStart<<", dirty size="<<m_data->m_dirtySize); 
}
void 
Buffer::removeAtEnd (uint32_t end)
{
    if (m_zeroAreaSize == 0) {
        if (m_size <= end) {
            m_size = 0;
        } else {
            m_size -= end;
        } 
    } else {
        assert (m_data->m_initialStart >= m_start);
        uint32_t zeroStart = m_data->m_initialStart - m_start;
        uint32_t zeroEnd = zeroStart + m_zeroAreaSize;
        uint32_t dataEnd = m_size + m_zeroAreaSize;
        assert (zeroStart <= m_size);
        assert (zeroEnd <= m_size + m_zeroAreaSize);
        if (dataEnd <= end) {
            /* remove all buffer */
            m_zeroAreaSize = 0;
            m_start += m_size;
            m_size = 0;
        } else if (dataEnd - zeroStart <= end) {
            /* remove end of buffer, zero area, part of start of buffer */
            assert (end >= m_zeroAreaSize);
            m_size -= end - m_zeroAreaSize;
            m_zeroAreaSize = 0;
        } else if (dataEnd - zeroEnd <= end) {
            /* remove end of buffer, part of zero area */
            uint32_t zeroDelta = end - (dataEnd - zeroEnd);
            m_zeroAreaSize -= zeroDelta;
            m_size -= end - zeroDelta;
        } else {
            /* remove part of end of buffer */
            m_size -= end;
        }
    }
    TRACE ("end remove="<<end<<", start="<<m_start<<", size="<<m_size<<", zero="<<m_zeroAreaSize<<
           ", real size="<<m_data->m_size<<", ini start="<<m_data->m_initialStart<<
           ", dirty start="<<m_data->m_dirtyStart<<", dirty size="<<m_data->m_dirtySize); 
}

Buffer 
Buffer::createFragment (uint32_t start, uint32_t length) const
{
    uint32_t zeroStart = m_data->m_initialStart - m_start;
    uint32_t zeroEnd = zeroStart + m_zeroAreaSize;
    if (m_zeroAreaSize != 0 &&
        start + length > zeroStart &&
        start <= zeroEnd) {
        transformIntoRealBuffer ();
    }
    Buffer tmp = *this;
    tmp.removeAtStart (start);
    tmp.removeAtEnd (getSize () - (start + length));
    return tmp;
}

void
Buffer::transformIntoRealBuffer (void) const
{
    if (m_zeroAreaSize != 0) {
        assert (m_data->m_initialStart >= m_start);
        assert (m_size >= (m_data->m_initialStart - m_start));
        Buffer tmp;
        tmp.addAtStart (m_zeroAreaSize);
        tmp.begin ().writeU8 (0, m_zeroAreaSize);
        uint32_t dataStart = m_data->m_initialStart - m_start;
        tmp.addAtStart (dataStart);
        tmp.begin ().write (m_data->m_data+m_start, dataStart);
        uint32_t dataEnd = m_size - (m_data->m_initialStart - m_start);
        tmp.addAtEnd (dataEnd);
        Buffer::Iterator i = tmp.end ();
        i.prev (dataEnd);
        i.write (m_data->m_data+m_data->m_initialStart,dataEnd);
        *const_cast<Buffer *> (this) = tmp;
    }
}


uint8_t const*
Buffer::peekData (void) const
{
    transformIntoRealBuffer ();
    return m_data->m_data + m_start;
}




}; // namespace ns3


#ifdef RUN_SELF_TESTS

#include "ns3/test.h"
#include <iomanip>

namespace ns3 {

class BufferTest: public Test {
private:
  bool ensureWrittenBytes (Buffer b, uint32_t n, uint8_t array[]);
public:
  virtual bool runTests (void);
  BufferTest ();
};


BufferTest::BufferTest ()
    : Test ("Buffer") {}

bool
BufferTest::ensureWrittenBytes (Buffer b, uint32_t n, uint8_t array[])
{
    bool success = true;
    uint8_t *expected = array;
    uint8_t const*got;
    got = b.peekData ();
    for (uint32_t j = 0; j < n; j++) {
        if (got[j] != expected[j]) {
            success = false;
        }
    }
    if (!success) {
        failure () << "Buffer -- ";
        failure () << "expected: n=";
        failure () << n << ", ";
        failure ().setf (std::ios::hex, std::ios::basefield);
        for (uint32_t j = 0; j < n; j++) {
            failure () << (uint16_t)expected[j] << " ";
        }
        failure ().setf (std::ios::dec, std::ios::basefield);
        failure () << "got: ";
        failure ().setf (std::ios::hex, std::ios::basefield);
        for (uint32_t j = 0; j < n; j++) {
            failure () << (uint16_t)got[j] << " ";
        }
        failure () << std::endl;
    }
    return success;
}

/* Note: works only when variadic macros are
 * available which is the case for gcc.
 * XXX
 */
#define ENSURE_WRITTEN_BYTES(buffer, n, ...) \
{ \
    uint8_t bytes[] = {__VA_ARGS__}; \
    if (!ensureWrittenBytes (buffer, n , bytes)) { \
        ok = false; \
    } \
}

bool
BufferTest::runTests (void)
{
    bool ok = true;
    Buffer buffer;
    Buffer::Iterator i;
    buffer.addAtStart (6);
    i = buffer.begin ();
    i.writeU8 (0x66);
    ENSURE_WRITTEN_BYTES (buffer, 1, 0x66);
    i = buffer.begin ();
    i.writeU8 (0x67);
    ENSURE_WRITTEN_BYTES (buffer, 1, 0x67);
    i.writeHtonU16 (0x6568);
    i = buffer.begin ();
    ENSURE_WRITTEN_BYTES (buffer, 3, 0x67, 0x65, 0x68);
    i.writeHtonU16 (0x6369);
    ENSURE_WRITTEN_BYTES (buffer, 3, 0x63, 0x69, 0x68);
    i.writeHtonU32 (0xdeadbeaf);
    ENSURE_WRITTEN_BYTES (buffer, 6, 0x63, 0x69, 0xde, 0xad, 0xbe, 0xaf);
    buffer.addAtStart (2);
    i = buffer.begin ();
    i.writeU16 (0);
    ENSURE_WRITTEN_BYTES (buffer, 8, 0, 0, 0x63, 0x69, 0xde, 0xad, 0xbe, 0xaf);
    buffer.addAtEnd (2);
    i = buffer.begin ();
    i.next (8);
    i.writeU16 (0);
    ENSURE_WRITTEN_BYTES (buffer, 10, 0, 0, 0x63, 0x69, 0xde, 0xad, 0xbe, 0xaf, 0, 0);
    buffer.removeAtStart (3);
    i = buffer.begin ();
    ENSURE_WRITTEN_BYTES (buffer, 7, 0x69, 0xde, 0xad, 0xbe, 0xaf, 0, 0);
    buffer.removeAtEnd (4);
    i = buffer.begin ();
    ENSURE_WRITTEN_BYTES (buffer, 3, 0x69, 0xde, 0xad);
    buffer.addAtStart (1);
    i = buffer.begin ();
    i.writeU8 (0xff);
    ENSURE_WRITTEN_BYTES (buffer, 4, 0xff, 0x69, 0xde, 0xad);
    buffer.addAtEnd (1);
    i = buffer.begin ();
    i.next (4);
    i.writeU8 (0xff);
    i.prev (2);
    uint16_t saved = i.readU16 ();
    i.prev (2);
    i.writeHtonU16 (0xff00);
    i.prev (2);
    if (i.readNtohU16 () != 0xff00) {
        ok = false;
    }
    i.prev (2);
    i.writeU16 (saved);
    ENSURE_WRITTEN_BYTES (buffer, 5, 0xff, 0x69, 0xde, 0xad, 0xff);
    Buffer o = buffer;
    ENSURE_WRITTEN_BYTES (o, 5, 0xff, 0x69, 0xde, 0xad, 0xff);
    o.addAtStart (1);
    i = o.begin ();
    i.writeU8 (0xfe);
    ENSURE_WRITTEN_BYTES (o, 6, 0xfe, 0xff, 0x69, 0xde, 0xad, 0xff);
    buffer.addAtStart (2);
    i = buffer.begin ();
    i.writeU8 (0xfd);
    i.writeU8 (0xfd);
    ENSURE_WRITTEN_BYTES (o, 6, 0xfe, 0xff, 0x69, 0xde, 0xad, 0xff);
    ENSURE_WRITTEN_BYTES (buffer, 7, 0xfd, 0xfd, 0xff, 0x69, 0xde, 0xad, 0xff);

    // test self-assignment
    {
        Buffer a = o;
        a = a;
    }

    // test remove start.
    buffer = Buffer (5);
    ENSURE_WRITTEN_BYTES (buffer, 5, 0, 0, 0, 0, 0);
    buffer.removeAtStart (1);
    ENSURE_WRITTEN_BYTES (buffer, 4, 0, 0, 0, 0);
    buffer.addAtStart (1);
    buffer.begin ().writeU8 (0xff);
    ENSURE_WRITTEN_BYTES (buffer, 5, 0xff, 0, 0, 0, 0);
    buffer.removeAtStart(3);
    ENSURE_WRITTEN_BYTES (buffer, 2, 0, 0);
    buffer.addAtStart (4);
    buffer.begin ().writeHtonU32 (0xdeadbeaf);
    ENSURE_WRITTEN_BYTES (buffer, 6,  0xde, 0xad, 0xbe, 0xaf, 0, 0);
    buffer.removeAtStart (2);
    ENSURE_WRITTEN_BYTES (buffer, 4,  0xbe, 0xaf, 0, 0);
    buffer.addAtEnd (4);
    i = buffer.begin ();
    i.next (4);
    i.writeHtonU32 (0xdeadbeaf);
    ENSURE_WRITTEN_BYTES (buffer, 8,  0xbe, 0xaf, 0, 0, 0xde, 0xad, 0xbe, 0xaf);
    buffer.removeAtStart (5);
    ENSURE_WRITTEN_BYTES (buffer, 3,  0xad, 0xbe, 0xaf);
    // test remove end
    buffer = Buffer (5);
    ENSURE_WRITTEN_BYTES (buffer, 5, 0, 0, 0, 0, 0);
    buffer.removeAtEnd (1);
    ENSURE_WRITTEN_BYTES (buffer, 4, 0, 0, 0, 0);
    buffer.addAtEnd (2);
    i = buffer.begin ();
    i.next (4);
    i.writeU8 (0xab);
    i.writeU8 (0xac);
    ENSURE_WRITTEN_BYTES (buffer, 6, 0, 0, 0, 0, 0xab, 0xac);
    buffer.removeAtEnd (1);
    ENSURE_WRITTEN_BYTES (buffer, 5, 0, 0, 0, 0, 0xab);
    buffer.removeAtEnd (3);
    ENSURE_WRITTEN_BYTES (buffer, 2, 0, 0);
    buffer.addAtEnd (6);
    i = buffer.begin ();
    i.next (2);
    i.writeU8 (0xac);
    i.writeU8 (0xad);
    i.writeU8 (0xae);
    i.writeU8 (0xaf);
    i.writeU8 (0xba);
    i.writeU8 (0xbb);
    ENSURE_WRITTEN_BYTES (buffer, 8, 0, 0, 0xac, 0xad, 0xae, 0xaf, 0xba, 0xbb);
    buffer.addAtStart (3);
    i = buffer.begin ();
    i.writeU8 (0x30);
    i.writeU8 (0x31);
    i.writeU8 (0x32);
    ENSURE_WRITTEN_BYTES (buffer, 11, 0x30, 0x31, 0x32, 0, 0, 0xac, 0xad, 0xae, 0xaf, 0xba, 0xbb);
    buffer.removeAtEnd (9);
    ENSURE_WRITTEN_BYTES (buffer, 2, 0x30, 0x31);
    buffer = Buffer (3);
    buffer.addAtEnd (2);
    i = buffer.begin ();
    i.next (3);
    i.writeHtonU16 (0xabcd);
    buffer.addAtStart (1);
    buffer.begin ().writeU8 (0x21);
    ENSURE_WRITTEN_BYTES (buffer, 6, 0x21, 0, 0, 0, 0xab, 0xcd);
    buffer.removeAtEnd (8);
    if (buffer.getSize () != 0) {
        ok = false;
    }
    
    
    

    return ok;
}



static BufferTest gBufferTest;

}; // namespace ns3

#endif /* RUN_SELF_TESTS */


