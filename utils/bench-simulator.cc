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

#include "yans/simulator.h"
#include "yans/event.h"
#include "yans/event.tcc"
#include "yans/wall-clock-ms.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace yans;


bool g_debug = false;

class Bench {
public:
	void read_distribution (std::istream &istream);
	void set_total (uint32_t total);
	void bench (void);
private:
	void cb (void);
	std::vector<uint64_t> m_distribution;
	std::vector<uint64_t>::const_iterator m_current;
	uint32_t m_n;
	uint32_t m_total;
};

void 
Bench::set_total (uint32_t total)
{
	m_total = total;
}

void
Bench::read_distribution (std::istream &input)
{
	double data;
	while (!input.eof ()) {
		if (input >> data) {
			uint64_t us = (uint64_t) (data * 1000000);
			m_distribution.push_back (us);
		} else {
			input.clear ();
			std::string line;
			input >> line;
		}
	}
}

void
Bench::bench (void) 
{
	WallClockMs time;
	double init, simu;
	time.start ();
	for (std::vector<uint64_t>::const_iterator i = m_distribution.begin ();
	     i != m_distribution.end (); i++) {
		Simulator::schedule_rel_us (*i, make_event (&Bench::cb, this));
	}
	init = time.end ();

	m_current = m_distribution.begin ();

	time.start ();
	Simulator::run ();
	simu = time.end ();

	std::cout <<
		"init n=" << m_distribution.size () << ", time=" << init << "s" << std::endl <<
		"simu n=" << m_n << ", time=" <<simu << "s" << std::endl <<
		"init " << ((double)m_distribution.size ()) / init << " insert/s, avg insert=" <<
		init / ((double)m_distribution.size ())<< "s" << std::endl <<
		"simu " << ((double)m_n) / simu<< " hold/s, avg hold=" << 
		simu / ((double)m_n) << "s" << std::endl
		;
}

void
Bench::cb (void)
{
	if (m_n > m_total) {
		return;
	}
	if (m_current == m_distribution.end ()) {
		m_current = m_distribution.begin ();
	}
	if (g_debug) {
		std::cerr << "event at " << Simulator::now_s () << std::endl;
	}
	Simulator::schedule_rel_us (*m_current, make_event (&Bench::cb, this));
	m_current++;
	m_n++;
}

int main (int argc, char *argv[])
{
	char const *filename = argv[1];
	std::istream *input;
	argc-=2;
	argv+= 2;
	if (strcmp (filename, "-") == 0) {
		input = &std::cin;
	} else {
		input = new std::ifstream (filename);
	}
	while (argc > 0) {
		if (strcmp ("--list", argv[0]) == 0) {
			Simulator::set_linked_list ();
		} else if (strcmp ("--heap", argv[0]) == 0) {
			Simulator::set_binary_heap ();
		} else if (strcmp ("--map", argv[0]) == 0) {
			Simulator::set_std_map ();
		} else if (strcmp ("--debug", argv[0]) == 0) {
			g_debug = true;
		} else if (strncmp ("--log=", argv[0],strlen ("--log=")) == 0) {
			char const *filename = argv[0] + strlen ("--log=");
			Simulator::enable_log_to (filename);
		}
		argc--;
		argv++;
	}
	Bench *bench = new Bench ();
	bench->read_distribution (*input);
	bench->set_total (20000);
	bench->bench ();

	return 0;
}
