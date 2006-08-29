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
#ifndef RANDOM_UNIFORM_H
#define RANDOM_UNIFORM_H

#include <stdint.h>

namespace yans {

class RandomUniformPrivate;

class RandomUniform {
public:
	RandomUniform ();
	~RandomUniform ();

	void reset (uint32_t seed);

	uint32_t get_max (void) const;
	uint32_t get_min (void) const;

	/* return an integer in the range [min,max] */
	uint32_t get_uint (void);
	/* return an integer in the range [0,n] */
	uint32_t get_uint (uint32_t n);
	/* return an integer in the range [a,b] */
	uint32_t get_uint (uint32_t a, uint32_t b);
	/* return a floating-point number in the 
	 * range [0,1)
	 */
	double get_double (void);
private:
	RandomUniformPrivate *m_priv;
};

}; // namespace yans


#endif /* RANDOM_UNIFORM_H */
