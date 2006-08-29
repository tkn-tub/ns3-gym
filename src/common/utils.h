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

#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

namespace yans {

uint16_t utils_hton_16 (uint16_t v);
uint32_t utils_hton_32 (uint32_t v);
uint16_t utils_ntoh_16 (uint16_t v);
uint32_t utils_ntoh_32 (uint32_t v);

uint32_t ascii_to_ipv4_host (char const *address);
void ascii_to_mac_network (char const *str, uint8_t address[6]);

/* call with checksum = 0 the first time. */
uint16_t utils_checksum_calculate (uint16_t checksum, uint8_t *buffer, uint16_t size);
uint16_t utils_checksum_complete (uint16_t checksum);

}; // namespace yans

#endif /* UTILS_H */
