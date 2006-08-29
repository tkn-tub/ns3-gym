/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
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

Buffer::BufferDataList  Buffer::m_free_list;
uint32_t Buffer::m_max_total_add_start = 0;
uint32_t Buffer::m_max_total_add_end = 0;

struct Buffer::BufferData *
Buffer::allocate (uint32_t req_size, uint32_t req_start)
{
	if (req_size == 0) {
		req_size = 1;
	}
	assert (req_size >= 1);
	uint32_t size = req_size - 1 + sizeof (struct Buffer::BufferData);
	uint8_t *b = new uint8_t [size];
	struct BufferData *data = reinterpret_cast<struct Buffer::BufferData*>(b);
	data->m_size = req_size;
	data->m_initial_start = req_start;
	data->m_dirty_start = req_start;
	data->m_dirty_size = 0;
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
	if (data->m_size < (Buffer::m_max_total_add_start + Buffer::m_max_total_add_end)) {
		Buffer::deallocate (data);
		return; 
	}
	/* feed into free list */
	if (Buffer::m_free_list.size () > 1000) {
		Buffer::deallocate (data);
	} else {
		Buffer::m_free_list.push_back (data);
	}
}

Buffer::BufferData *
Buffer::create (void)
{
	/* try to find a buffer correctly sized. */
	while (!Buffer::m_free_list.empty ()) {
		struct Buffer::BufferData *data = Buffer::m_free_list.back ();
		Buffer::m_free_list.pop_back ();
		if (data->m_size >= (m_max_total_add_start + m_max_total_add_end)) {
			data->m_initial_start = m_max_total_add_start;
			data->m_dirty_start = m_max_total_add_start;
			data->m_dirty_size = 0;
			data->m_count = 1;
			return data;
		}
		Buffer::deallocate (data);
	}
	struct Buffer::BufferData *data = Buffer::allocate (m_max_total_add_start+m_max_total_add_end,
							    m_max_total_add_start);
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
	return Buffer::allocate (m_max_total_add_start+m_max_total_add_end,
				 m_max_total_add_start);
}
#endif

}; // namespace ns3


#include <cassert>

namespace ns3 {


void 
Buffer::add_at_start (uint32_t start)
{
	assert (m_start <= m_data->m_initial_start);
	bool is_dirty = m_data->m_count > 1 && m_start > m_data->m_dirty_start;
	if (m_start >= start && !is_dirty) {
		/* enough space in the buffer and not dirty. */
		m_start -= start;
		m_size += start;
	} else if (m_size + start <= m_data->m_size && !is_dirty) {
		/* enough space but need to move data around to fit new data */
                memmove (m_data->m_data + start, get_start (), m_size);
		assert (start > m_start);
		m_data->m_initial_start += start;
                m_start = 0;
                m_size += start;
	} else if (m_start < start) {
		/* not enough space in buffer */
		uint32_t new_size = m_size + start;
		struct Buffer::BufferData *new_data = Buffer::allocate (new_size, 0);
		memcpy (new_data->m_data + start, get_start (), m_size);
		new_data->m_initial_start = m_data->m_initial_start + start;
		m_data->m_count--;
		if (m_data->m_count == 0) {
			Buffer::deallocate (m_data);
		}
		m_data = new_data;
		m_start = 0;
		m_size = new_size;
	} else {
		/* enough space in the buffer but it is dirty ! */
		assert (is_dirty);
		struct Buffer::BufferData *new_data = Buffer::create ();
		memcpy (new_data->m_data + m_start, get_start (), m_size);
		new_data->m_initial_start = m_data->m_initial_start;
		m_data->m_count--;
		if (m_data->m_count == 0) {
			recycle (m_data);
		}
		m_data = new_data;
		m_start -= start;
		m_size += start;
	} 
	// update dirty area
	m_data->m_dirty_start = m_start;
	m_data->m_dirty_size = m_size;
	// update m_max_total_add_start
	uint32_t added_at_start;
	if (m_data->m_initial_start > m_start) {
		added_at_start = m_data->m_initial_start - m_start;
	} else {
		added_at_start = 0;
	}
	if (added_at_start > m_max_total_add_start) {
		m_max_total_add_start = added_at_start;
	}
	TRACE ("start add="<<start<<", start="<<m_start<<", size="<<m_size<<", zero="<<m_zero_area_size<<
	       ", real size="<<m_data->m_size<<", ini start="<<m_data->m_initial_start<<
	       ", dirty start="<<m_data->m_dirty_start<<", dirty size="<<m_data->m_dirty_size); 
}
void 
Buffer::add_at_end (uint32_t end)
{
	assert (m_start <= m_data->m_initial_start);
	bool is_dirty = m_data->m_count > 1 &&
		m_start + m_size < m_data->m_dirty_start + m_data->m_dirty_size;
	if (m_start + m_size + end <= m_data->m_size && !is_dirty) {
		/* enough space in buffer and not dirty */
		m_size += end;
	} else if (m_size + end <= m_data->m_size && !is_dirty) {
		/* enough space but need to move data around to fit the extra data */
                uint32_t new_start = m_data->m_size - (m_size + end);
                memmove (m_data->m_data + new_start, get_start (), m_size);
		assert (new_start < m_start);
		m_data->m_initial_start -= m_start - new_start;
                m_start = new_start;
                m_size += end;
	} else if (m_start + m_size + end > m_data->m_size) {
		/* not enough space in buffer */
		uint32_t new_size = m_size + end;
		struct Buffer::BufferData *new_data = Buffer::allocate (new_size, 0);
		memcpy (new_data->m_data, get_start (), m_size);
		new_data->m_initial_start = m_data->m_initial_start;
		m_data->m_count--;
		if (m_data->m_count == 0) {
			Buffer::deallocate (m_data);
		}
		m_data = new_data;
		m_size = new_size;
		m_start = 0;
	} else {
		/* enough space in the buffer but it is dirty ! */
		assert (is_dirty);
		struct Buffer::BufferData *new_data = Buffer::create ();
		memcpy (new_data->m_data + m_start, get_start (), m_size);
		new_data->m_initial_start = m_data->m_initial_start;
		m_data->m_count--;
		if (m_data->m_count == 0) {
			recycle (m_data);
		}
		m_data = new_data;
		m_size += end;
	} 
	// update dirty area
	m_data->m_dirty_start = m_start;
	m_data->m_dirty_size = m_size;
	// update m_max_total_add_end
	uint32_t end_loc = m_start + m_size;
	uint32_t added_at_end;
	if (m_data->m_initial_start < end_loc) {
		added_at_end = end_loc - m_data->m_initial_start;
	} else {
		added_at_end = 0;
	}
	if (added_at_end > m_max_total_add_end) {
		m_max_total_add_end = added_at_end;
	}
	TRACE ("end add="<<end<<", start="<<m_start<<", size="<<m_size<<", zero="<<m_zero_area_size<<
	       ", real size="<<m_data->m_size<<", ini start="<<m_data->m_initial_start<<
	       ", dirty start="<<m_data->m_dirty_start<<", dirty size="<<m_data->m_dirty_size); 
}

void 
Buffer::remove_at_start (uint32_t start)
{
	if (m_zero_area_size == 0) {
		if (m_size <= start) {
			m_start += m_size;
			m_size = 0;
		} else {
			m_start += start;
			m_size -= start;
		}
	} else {
		assert (m_data->m_initial_start >= m_start);
		uint32_t zero_start = m_data->m_initial_start - m_start;
		uint32_t zero_end = zero_start + m_zero_area_size;
		uint32_t data_end = m_size + m_zero_area_size;
		if (start <= zero_start) {
			/* only remove start of buffer */
			m_start += start;
			m_size -= start;
		} else if (start <= zero_end) {
			/* remove start of buffer _and_ start of zero area */
			m_start += zero_start;
			uint32_t zero_delta = start - zero_start;
			m_zero_area_size -= zero_delta;
			assert (zero_delta <= start);
			m_size -= zero_start;
		} else if (start <= data_end) {
			/* remove start of buffer, complete zero area, and part
			 * of end of buffer */
			m_start += start - m_zero_area_size;
			m_size -= start - m_zero_area_size;
			m_zero_area_size = 0;
		} else {
			/* remove all buffer */
			m_start += m_size;
			m_size = 0;
			m_zero_area_size = 0;
		}
	}
	TRACE ("start remove="<<start<<", start="<<m_start<<", size="<<m_size<<", zero="<<m_zero_area_size<<
	       ", real size="<<m_data->m_size<<", ini start="<<m_data->m_initial_start<<
	       ", dirty start="<<m_data->m_dirty_start<<", dirty size="<<m_data->m_dirty_size); 
}
void 
Buffer::remove_at_end (uint32_t end)
{
	if (m_zero_area_size == 0) {
		if (m_size <= end) {
			m_size = 0;
		} else {
			m_size -= end;
		} 
	} else {
		assert (m_data->m_initial_start >= m_start);
		uint32_t zero_start = m_data->m_initial_start - m_start;
		uint32_t zero_end = zero_start + m_zero_area_size;
		uint32_t data_end = m_size + m_zero_area_size;
		assert (zero_start <= m_size);
		assert (zero_end <= m_size + m_zero_area_size);
		if (data_end <= end) {
			/* remove all buffer */
			m_zero_area_size = 0;
			m_start += m_size;
			m_size = 0;
		} else if (data_end - zero_start <= end) {
			/* remove end of buffer, zero area, part of start of buffer */
			assert (end >= m_zero_area_size);
			m_size -= end - m_zero_area_size;
			m_zero_area_size = 0;
		} else if (data_end - zero_end <= end) {
			/* remove end of buffer, part of zero area */
			uint32_t zero_delta = end - (data_end - zero_end);
			m_zero_area_size -= zero_delta;
			m_size -= end - zero_delta;
		} else {
			/* remove part of end of buffer */
			m_size -= end;
		}
	}
	TRACE ("end remove="<<end<<", start="<<m_start<<", size="<<m_size<<", zero="<<m_zero_area_size<<
	       ", real size="<<m_data->m_size<<", ini start="<<m_data->m_initial_start<<
	       ", dirty start="<<m_data->m_dirty_start<<", dirty size="<<m_data->m_dirty_size); 
}

Buffer 
Buffer::create_fragment (uint32_t start, uint32_t length) const
{
	uint32_t zero_start = m_data->m_initial_start - m_start;
	uint32_t zero_end = zero_start + m_zero_area_size;
	if (m_zero_area_size != 0 &&
	    start + length > zero_start &&
	    start <= zero_end) {
		transform_into_real_buffer ();
	}
	Buffer tmp = *this;
	tmp.remove_at_start (start);
	tmp.remove_at_end (get_size () - (start + length));
	return tmp;
}

void
Buffer::transform_into_real_buffer (void) const
{
	if (m_zero_area_size != 0) {
		assert (m_data->m_initial_start >= m_start);
		assert (m_size >= (m_data->m_initial_start - m_start));
		Buffer tmp;
		tmp.add_at_start (m_zero_area_size);
		tmp.begin ().write_u8 (0, m_zero_area_size);
		uint32_t data_start = m_data->m_initial_start - m_start;
		tmp.add_at_start (data_start);
		tmp.begin ().write (m_data->m_data+m_start, data_start);
		uint32_t data_end = m_size - (m_data->m_initial_start - m_start);
		tmp.add_at_end (data_end);
		Buffer::Iterator i = tmp.end ();
		i.prev (data_end);
		i.write (m_data->m_data+m_data->m_initial_start,data_end);
		*const_cast<Buffer *> (this) = tmp;
	}
}


uint8_t *
Buffer::peek_data (void) const
{
	transform_into_real_buffer ();
	return m_data->m_data + m_start;
}




}; // namespace ns3


#ifdef RUN_SELF_TESTS

#include "ns3/test.h"
#include <iomanip>

namespace ns3 {

class BufferTest: public Test {
private:
  bool ensure_written_bytes (Buffer b, uint32_t n, uint8_t array[]);
public:
  virtual bool run_tests (void);
  BufferTest ();
};


BufferTest::BufferTest ()
	: Test ("Buffer") {}

bool
BufferTest::ensure_written_bytes (Buffer b, uint32_t n, uint8_t array[])
{
	bool success = true;
	uint8_t *expected = array;
	uint8_t *got;
	got = b.peek_data ();
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
	if (!ensure_written_bytes (buffer, n , bytes)) { \
		ok = false; \
	} \
}

bool
BufferTest::run_tests (void)
{
	bool ok = true;
	Buffer buffer;
	Buffer::Iterator i;
	buffer.add_at_start (6);
	i = buffer.begin ();
	i.write_u8 (0x66);
	ENSURE_WRITTEN_BYTES (buffer, 1, 0x66);
	i = buffer.begin ();
	i.write_u8 (0x67);
	ENSURE_WRITTEN_BYTES (buffer, 1, 0x67);
	i.write_hton_u16 (0x6568);
	i = buffer.begin ();
	ENSURE_WRITTEN_BYTES (buffer, 3, 0x67, 0x65, 0x68);
	i.write_hton_u16 (0x6369);
	ENSURE_WRITTEN_BYTES (buffer, 3, 0x63, 0x69, 0x68);
	i.write_hton_u32 (0xdeadbeaf);
	ENSURE_WRITTEN_BYTES (buffer, 6, 0x63, 0x69, 0xde, 0xad, 0xbe, 0xaf);
	buffer.add_at_start (2);
	i = buffer.begin ();
	i.write_u16 (0);
	ENSURE_WRITTEN_BYTES (buffer, 8, 0, 0, 0x63, 0x69, 0xde, 0xad, 0xbe, 0xaf);
	buffer.add_at_end (2);
	i = buffer.begin ();
	i.next (8);
	i.write_u16 (0);
	ENSURE_WRITTEN_BYTES (buffer, 10, 0, 0, 0x63, 0x69, 0xde, 0xad, 0xbe, 0xaf, 0, 0);
	buffer.remove_at_start (3);
	i = buffer.begin ();
	ENSURE_WRITTEN_BYTES (buffer, 7, 0x69, 0xde, 0xad, 0xbe, 0xaf, 0, 0);
	buffer.remove_at_end (4);
	i = buffer.begin ();
	ENSURE_WRITTEN_BYTES (buffer, 3, 0x69, 0xde, 0xad);
	buffer.add_at_start (1);
	i = buffer.begin ();
	i.write_u8 (0xff);
	ENSURE_WRITTEN_BYTES (buffer, 4, 0xff, 0x69, 0xde, 0xad);
	buffer.add_at_end (1);
	i = buffer.begin ();
	i.next (4);
	i.write_u8 (0xff);
	i.prev (2);
	uint16_t saved = i.read_u16 ();
	i.prev (2);
	i.write_hton_u16 (0xff00);
	i.prev (2);
	if (i.read_ntoh_u16 () != 0xff00) {
		ok = false;
	}
	i.prev (2);
	i.write_u16 (saved);
	ENSURE_WRITTEN_BYTES (buffer, 5, 0xff, 0x69, 0xde, 0xad, 0xff);
	Buffer o = buffer;
	ENSURE_WRITTEN_BYTES (o, 5, 0xff, 0x69, 0xde, 0xad, 0xff);
	o.add_at_start (1);
	i = o.begin ();
	i.write_u8 (0xfe);
	ENSURE_WRITTEN_BYTES (o, 6, 0xfe, 0xff, 0x69, 0xde, 0xad, 0xff);
	buffer.add_at_start (2);
	i = buffer.begin ();
	i.write_u8 (0xfd);
	i.write_u8 (0xfd);
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
	buffer.remove_at_start (1);
	ENSURE_WRITTEN_BYTES (buffer, 4, 0, 0, 0, 0);
	buffer.add_at_start (1);
	buffer.begin ().write_u8 (0xff);
	ENSURE_WRITTEN_BYTES (buffer, 5, 0xff, 0, 0, 0, 0);
	buffer.remove_at_start(3);
	ENSURE_WRITTEN_BYTES (buffer, 2, 0, 0);
	buffer.add_at_start (4);
	buffer.begin ().write_hton_u32 (0xdeadbeaf);
	ENSURE_WRITTEN_BYTES (buffer, 6,  0xde, 0xad, 0xbe, 0xaf, 0, 0);
	buffer.remove_at_start (2);
	ENSURE_WRITTEN_BYTES (buffer, 4,  0xbe, 0xaf, 0, 0);
	buffer.add_at_end (4);
	i = buffer.begin ();
	i.next (4);
	i.write_hton_u32 (0xdeadbeaf);
	ENSURE_WRITTEN_BYTES (buffer, 8,  0xbe, 0xaf, 0, 0, 0xde, 0xad, 0xbe, 0xaf);
	buffer.remove_at_start (5);
	ENSURE_WRITTEN_BYTES (buffer, 3,  0xad, 0xbe, 0xaf);
	// test remove end
	buffer = Buffer (5);
	ENSURE_WRITTEN_BYTES (buffer, 5, 0, 0, 0, 0, 0);
	buffer.remove_at_end (1);
	ENSURE_WRITTEN_BYTES (buffer, 4, 0, 0, 0, 0);
	buffer.add_at_end (2);
	i = buffer.begin ();
	i.next (4);
	i.write_u8 (0xab);
	i.write_u8 (0xac);
	ENSURE_WRITTEN_BYTES (buffer, 6, 0, 0, 0, 0, 0xab, 0xac);
	buffer.remove_at_end (1);
	ENSURE_WRITTEN_BYTES (buffer, 5, 0, 0, 0, 0, 0xab);
	buffer.remove_at_end (3);
	ENSURE_WRITTEN_BYTES (buffer, 2, 0, 0);
	buffer.add_at_end (6);
	i = buffer.begin ();
	i.next (2);
	i.write_u8 (0xac);
	i.write_u8 (0xad);
	i.write_u8 (0xae);
	i.write_u8 (0xaf);
	i.write_u8 (0xba);
	i.write_u8 (0xbb);
	ENSURE_WRITTEN_BYTES (buffer, 8, 0, 0, 0xac, 0xad, 0xae, 0xaf, 0xba, 0xbb);
	buffer.add_at_start (3);
	i = buffer.begin ();
	i.write_u8 (0x30);
	i.write_u8 (0x31);
	i.write_u8 (0x32);
	ENSURE_WRITTEN_BYTES (buffer, 11, 0x30, 0x31, 0x32, 0, 0, 0xac, 0xad, 0xae, 0xaf, 0xba, 0xbb);
	buffer.remove_at_end (9);
	ENSURE_WRITTEN_BYTES (buffer, 2, 0x30, 0x31);
	buffer = Buffer (3);
	buffer.add_at_end (2);
	i = buffer.begin ();
	i.next (3);
	i.write_hton_u16 (0xabcd);
	buffer.add_at_start (1);
	buffer.begin ().write_u8 (0x21);
	ENSURE_WRITTEN_BYTES (buffer, 6, 0x21, 0, 0, 0, 0xab, 0xcd);
	buffer.remove_at_end (8);
	if (buffer.get_size () != 0) {
		ok = false;
	}
	
	
	

	return ok;
}



static BufferTest g_buffer_test;

}; // namespace ns3

#endif /* RUN_SELF_TESTS */


