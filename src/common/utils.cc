/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
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

#include "utils.h"

namespace yans {

uint16_t 
utils_hton_16 (uint16_t v)
{
	uint8_t array[2];
	array[0] = (v >> 8) & 0xff;
	array[1] = (v >> 0) & 0xff;
	return *((uint16_t *)array);
}
uint32_t 
utils_hton_32 (uint32_t v)
{
	uint8_t array[4];
	array[0] = (v >> 24) & 0xff;
	array[1] = (v >> 16) & 0xff;
	array[2] = (v >>  8) & 0xff;
	array[3] = (v >>  0) & 0xff;
	return *((uint32_t *)array);
}
uint16_t 
utils_ntoh_16 (uint16_t v)
{
	uint16_t val;
	uint8_t *array;
	array = (uint8_t *)&v;
	val = (array[0] << 8) | (array[1] << 0);
	return val;
}
uint32_t 
utils_ntoh_32 (uint32_t v)
{
	uint32_t val = 0;
	uint8_t *array = (uint8_t *)&v;
	val |= array[0] << 24;
	val |= array[1] << 16;
	val |= array[2] << 8;
	val |= array[3] << 0;
	return val;
}

#define ASCII_DOT (0x2e)
#define ASCII_ZERO (0x30)
#define ASCII_A (0x41)
#define ASCII_Z (0x5a)
#define ASCII_a (0x61)
#define ASCII_z (0x7a)
#define ASCII_COLON (0x3a)


uint32_t 
ascii_to_ipv4_host (char const *address)
{
	uint32_t host = 0;
	while (true) {
		uint8_t byte = 0;
		while (*address != ASCII_DOT &&
		       *address != 0) {
			byte *= 10;
			byte += *address - ASCII_ZERO;
			address++;
		}
		host <<= 8;
		host |= byte;
		if (*address == 0) {
			break;
		}
		address++;
	}
	return host;
}


char
ascii_to_low_case (char c)
{
	if (c >= ASCII_a && c <= ASCII_z) {
		return c;
	} else if (c >= ASCII_A && c <= ASCII_Z) {
		return c + (ASCII_a - ASCII_A);
	} else {
		return c;
	}
}
void 
ascii_to_mac_network (char const *str, uint8_t address[6])
{
	uint8_t i = 0;
	while (*str != 0 && i < 6) {
		uint8_t byte = 0;
		while (*str != ASCII_COLON && *str != 0) {
			byte <<= 4;
			char low = ascii_to_low_case (*str);
			if (low >= ASCII_a) {
				byte |= low - ASCII_a + 10;
			} else {
				byte |= low - ASCII_ZERO;
			}
			str++;
		}
		address[i] = byte;
		str++;
		i++;
	}
}

uint16_t 
utils_checksum_calculate (uint16_t checksum, uint8_t *buffer, uint16_t size)
{
	/* see RFC 1071 to understand this code. */
	uint32_t sum = checksum;
	uint16_t *data = (uint16_t *) buffer;
	for (uint16_t i = 0; i < (size/2); i++) {
		sum += data[i];
	}
	if ((size % 2) != 0) {
		uint8_t tmp_buf[2];
		tmp_buf[0] = buffer[size-1];
		tmp_buf[1] = 0;
		data = (uint16_t *)tmp_buf;
		sum += *data;
	}
	while (sum >> 16) {
		sum = (sum & 0xffff) + (sum >> 16);
	}
	return sum;
}

uint16_t 
utils_checksum_complete (uint16_t checksum)
{
	return ~checksum;
}


}; // namespace yans


#ifdef RUN_SELF_TESTS
#include "yans/test.h"
namespace yans {

class UtilsTest : public Test {
public:
	UtilsTest ();
	virtual bool run_tests (void);
private:
	bool test_ipv4_ascii_to_host (char const *str, uint32_t expected);
	bool test_mac_ascii (char const *str, uint8_t expected[6]);
	bool test_hton_16 (uint16_t v, uint8_t expected[2]);
};
bool
UtilsTest::test_ipv4_ascii_to_host (char const *str, uint32_t expected)
{
	if (ascii_to_ipv4_host (str) != expected) {
		failure () << "Utils ascii to host --"
			   << " for: \"" << str << "\""
			   << " expected: " << expected
			   << " got: " << ascii_to_ipv4_host (str)
			   << std::endl;
		return false;
	}
	return true;
}

bool
UtilsTest::test_mac_ascii (char const *str, uint8_t expected[6])
{
	uint8_t got[6];
	ascii_to_mac_network (str, got);
	for (uint8_t i = 0; i < 6; i++) {
		if (got[i] != expected[i]) {
			failure () << "Utils ascii to mac --"
				   << " for: \"" << str << "\""
				   << " expected: ";
			failure ().setf (std::ios::hex, std::ios::basefield);
			failure () << (uint32_t)expected[0] << ":"
				   << (uint32_t)expected[1] << ":"
				   << (uint32_t)expected[2] << ":"
				   << (uint32_t)expected[3] << ":"
				   << (uint32_t)expected[4] << ":"
				   << (uint32_t)expected[5];
			failure ().setf (std::ios::dec, std::ios::basefield);
			failure () << " got: ";
			failure ().setf (std::ios::hex, std::ios::basefield);
			failure () << (uint32_t)got[0] << ":"
				   << (uint32_t)got[1] << ":"
				   << (uint32_t)got[2] << ":"
				   << (uint32_t)got[3] << ":"
				   << (uint32_t)got[4] << ":"
				   << (uint32_t)got[5];
			failure ().setf (std::ios::dec, std::ios::basefield);
			failure () << std::endl;
			return false;
		}
	}
	return true;
}

bool
UtilsTest::test_hton_16 (uint16_t v, uint8_t expected[2])
{
	uint16_t result = utils_hton_16 (v);
	uint8_t *got = (uint8_t *)&result;
	if (got[0] != expected[0] ||
	    got[1] != expected[1]) {
		failure () << "Utils hton 16 --"
			   << " for: \"" << v << "\""
			   << " expected: ";
		failure ().setf (std::ios::hex, std::ios::basefield);
		failure () << (uint32_t)expected[0] << ":"
			   << (uint32_t)expected[1];
		failure () << " got: ";
		failure () << (uint32_t)got[0] << ":"
			   << (uint32_t)got[1];
		failure ().setf (std::ios::dec, std::ios::basefield);
		failure () << std::endl;
		return false;
	}
	return true;
}

#define TEST_IPV4_ASCII_TO_HOST(a,b)  \
if (!test_ipv4_ascii_to_host (a,b)) { \
	ok = false;                   \
}

#define TEST_MAC_ASCII(ad, a, b, c, d, e, f)       \
{                                                  \
	uint8_t expected[6] = {a, b, c, d, e, f};  \
	if (!test_mac_ascii (ad, expected)) {      \
		ok = false;                        \
	}                                          \
}

#define TEST_HTON_16(v, a, b)               \
{                                           \
	uint8_t expected[2] = {a, b};       \
	if (!test_hton_16 (v, expected)) {  \
		ok = false;                 \
	}                                   \
}


UtilsTest::UtilsTest ()
	: Test ("Utils") {}

bool 
UtilsTest::run_tests (void)
{
	bool ok = true;
	TEST_IPV4_ASCII_TO_HOST ("255.255.255.255", 0xffffffff);
	TEST_IPV4_ASCII_TO_HOST ("255.255.255.0", 0xffffff00);
	TEST_IPV4_ASCII_TO_HOST ("255.255.255.00", 0xffffff00);
	TEST_IPV4_ASCII_TO_HOST ("255.255.255.000", 0xffffff00);
	TEST_IPV4_ASCII_TO_HOST ("255.255.255.0000", 0xffffff00);
	TEST_IPV4_ASCII_TO_HOST ("255.255.0.255", 0xffff00ff);
	TEST_IPV4_ASCII_TO_HOST ("192.168.0.1", 0xc0a80001);
	TEST_IPV4_ASCII_TO_HOST ("0.168.0.1", 0x00a80001);
	TEST_MAC_ASCII ("00:00:00:00:00:00", 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
	TEST_MAC_ASCII ("00:00:00:00:00:01", 0x00, 0x00, 0x00, 0x00, 0x00, 0x01);
	TEST_MAC_ASCII ("01:00:00:00:00:01", 0x01, 0x00, 0x00, 0x00, 0x00, 0x01);
	TEST_MAC_ASCII ("ff:00:00:ff:00:01", 0xff, 0x00, 0x00, 0xff, 0x00, 0x01);
	TEST_MAC_ASCII ("f0:00:00:00:5d:01", 0xf0, 0x00, 0x00, 0x00, 0x5d, 0x01);
	TEST_HTON_16 (0xf00f, 0xf0, 0x0f);
	return ok;
}
static UtilsTest g_utils_test;
}; //namespace yans
#endif /* RUN_SELF_TESTS */
