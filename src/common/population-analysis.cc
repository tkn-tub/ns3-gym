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

#include <cassert>
#include <math.h>

#include "population-analysis.h"

/* This code is a C++ translation of Java code released under the GPLv2
   copyright Mathieu Lacage in treegrowth-stable:
   http://cutebugs.net/code/treegrowth-stable.
 */

namespace ns3 {

PopulationAnalysis::PopulationAnalysis ()
{
	reset ();
}
PopulationAnalysis::~PopulationAnalysis ()
{}

void 
PopulationAnalysis::reset (void)
{
	m_n = 0;
	m_square_sum = 0.0;
	m_mean = 0.0;
	m_sum = 0.0;
}

void 
PopulationAnalysis::add_term (double term)
{
	double d = (term - m_mean);
        m_n++;
        m_mean += d / m_n;
        m_square_sum += d * (term - m_mean);
	m_sum += term;
}

uint32_t
PopulationAnalysis::get_n (void)
{
	return m_n;
}
double 
PopulationAnalysis::get_total (void)
{
	return m_sum;
}
double 
PopulationAnalysis::get_mean (void)
{
	return m_mean;
}
double 
PopulationAnalysis::get_standard_deviation (void)
{
	if (m_n == 0) {
		return 0.0;
	}
	assert (get_unbiased_variance () >= 0);
        double deviation = sqrt (get_unbiased_variance ());
        return deviation;
}
double 
PopulationAnalysis::get_unbiased_variance (void)
{
        if (m_n == 1 || m_n == 0) {
		return 0.0;
        }
        return m_square_sum / (m_n - 1);
}

}; // namespace ns3
