/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
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
#include "seed-generator.h"
#include "rng-mrg32k3a.h"

namespace yans {

class SeedGeneratorPrivate {
public:
	static void destroy (void);
	static void reset (uint32_t seed);
	static uint32_t get (void);
private:
	static RngMrg32k3a m_generator;
};

RngMrg32k3a SeedGeneratorPrivate::m_generator;

void 
SeedGeneratorPrivate::reset (uint32_t seed)
{
	m_generator.reset (seed);
}
uint32_t 
SeedGeneratorPrivate::get (void)
{
	return m_generator.get_uint ();
}



void 
SeedGenerator::reset (uint32_t seed)
{
	SeedGeneratorPrivate::reset (seed);
}
uint32_t 
SeedGenerator::get (void)
{
	return SeedGeneratorPrivate::get ();
}

}; // namespace yans
