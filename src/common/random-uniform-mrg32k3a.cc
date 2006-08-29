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
#include "random-uniform.h"
#include "seed-generator.h"
#include "rng-mrg32k3a.h"

namespace yans {

class RandomUniformPrivate {
public:
	RandomUniformPrivate ();
	~RandomUniformPrivate ();

	void reset (uint32_t seed);

	uint32_t get_max (void) const;
	uint32_t get_min (void) const;

	uint32_t get_uint (void);
	uint32_t get_uint (uint32_t n);
	uint32_t get_uint (uint32_t a, uint32_t b);
	double get_double (void);
private:
	RngMrg32k3a m_rng;
};

RandomUniformPrivate::RandomUniformPrivate ()
	: m_rng ()
{
	m_rng.reset (SeedGenerator::get ());
}
RandomUniformPrivate::~RandomUniformPrivate ()
{}

void 
RandomUniformPrivate::reset (uint32_t seed)
{
	m_rng.reset (seed);
}
uint32_t 
RandomUniformPrivate::get_max (void) const
{
	return m_rng.get_max ();
}
uint32_t 
RandomUniformPrivate::get_min (void) const
{
	return m_rng.get_min ();
}
uint32_t 
RandomUniformPrivate::get_uint (void)
{
	return m_rng.get_uint ();
}
uint32_t 
RandomUniformPrivate::get_uint (uint32_t n)
{
	return m_rng.get_uint (n);
}
uint32_t 
RandomUniformPrivate::get_uint (uint32_t a, uint32_t b)
{
	return m_rng.get_uint (a, b);
}
double
RandomUniformPrivate::get_double (void)
{
	return m_rng.get_double ();
}



RandomUniform::RandomUniform ()
	: m_priv (new RandomUniformPrivate ())
{}
RandomUniform::~RandomUniform ()
{
	delete m_priv;
	m_priv = reinterpret_cast<RandomUniformPrivate *> (0xdeadbeaf);
}

void 
RandomUniform::reset (uint32_t seed)
{
	m_priv->reset (seed);
}
uint32_t 
RandomUniform::get_max (void) const
{
	return m_priv->get_max ();
}
uint32_t 
RandomUniform::get_min (void) const
{
	return m_priv->get_min ();
}
uint32_t 
RandomUniform::get_uint (void)
{
	return m_priv->get_uint ();
}
uint32_t 
RandomUniform::get_uint (uint32_t n)
{
	return m_priv->get_uint (n);
}
uint32_t 
RandomUniform::get_uint (uint32_t a, uint32_t b)
{
	return m_priv->get_uint (a, b);
}
double 
RandomUniform::get_double (void)
{
	return m_priv->get_double ();
}

}; // namespace yans

