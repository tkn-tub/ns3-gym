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


#ifndef POPULATION_ANALYSIS_H
#define POPULATION_ANALYSIS_H

#include <stdint.h>

namespace yans {

class PopulationAnalysis {
public:
	PopulationAnalysis ();
	~PopulationAnalysis ();

	void reset (void);

	void add_term (double term);

	uint32_t get_n (void);
	double get_total (void);
	double get_mean (void);
	double get_standard_deviation (void);
	double get_unbiased_variance (void);

private:
	double m_mean;
	double m_square_sum;
	double m_sum;
	uint32_t m_n;
};

}; // namespace yans


#endif /* POPULATION_ANALYSIS_H */
