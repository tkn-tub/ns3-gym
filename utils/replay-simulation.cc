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

#include "ns3/simulator.h"
#include "ns3/event.h"
#include "ns3/event.tcc"
#include "ns3/wall-clock-ms.h"
#include <vector>
#include <deque>
#include <fstream>
#include <iostream>

using namespace yans;

class LogReader {
public:
	void read_from_filename (char const *filename);
	void run (void);
	void print_stats (void);
private:
	struct Command {
		enum {
			REMOVE,
			INSERT,
			INSERT_LATER,
			INSERT_REMOVE
		} m_type;
		// uid at which this command is supposed to be executed.
		uint32_t m_uid;
		union {
			struct {
				// time at which the event is supposed to expire
				uint64_t m_ev_us;
			} insert;
			struct {
				// location in the array of events to remove where
				// to insert this event once it is inserted in 
				// the scheduler.
				uint32_t m_ev_loc; 
				// time at which the event is supposed to expire
				uint64_t m_ev_us;
			} insert_remove;
		};
	};
	void execute_log_commands (uint32_t uid);

	typedef std::deque<struct Command> Commands;
	typedef std::deque<struct Command>::iterator CommandsI;
	typedef std::deque<Event > RemoveEvents;
	

	Commands m_commands;
	CommandsI m_command;
	RemoveEvents m_remove_events;
	uint32_t m_uid;
};

typedef std::vector<std::pair<uint32_t, uint32_t> > Removes;
typedef std::vector<std::pair<uint32_t, uint32_t> >::iterator RemovesI;

void
LogReader::read_from_filename (char const *filename)
{
	std::ifstream log;
	std::cout << "read log..." << std::endl;
	Removes removes;
	log.open (filename);
	while (!log.eof ()) {
		std::string type;
		log >> type;
		if (type == "i") {
			uint32_t now_uid, ev_uid;
			uint64_t now_us, ev_us;
			log >> now_uid >> now_us >> ev_uid >> ev_us;
			struct Command cmd;
			cmd.m_type = Command::INSERT;
			cmd.m_uid = now_uid;
			cmd.insert.m_ev_us = ev_us;
			m_commands.push_back (cmd);
		} else if (type == "r") {
			uint32_t now_uid, ev_uid;
			uint64_t now_us, ev_us;
			log >> now_uid >> now_us >> ev_uid >> ev_us;
			struct Command cmd;
			cmd.m_type = Command::REMOVE;
			cmd.m_uid = now_uid;
			m_commands.push_back (cmd);
			removes.push_back (std::make_pair (now_uid, ev_uid));
		} else if (type == "il") {
			uint32_t now_uid, ev_uid;
			uint64_t now_us, ev_us;
			log >> now_uid >> now_us >> ev_uid >> ev_us;
			struct Command cmd;
			cmd.m_type = Command::INSERT_LATER;
			cmd.m_uid = now_uid;
			m_commands.push_back (cmd);
		}
	}
	log.close ();

	std::cout << "gather insert removes..." << std::endl;
	for (CommandsI i = m_commands.begin (); i != m_commands.end (); i++) {
		if (i->m_type == Command::INSERT) {
			for (RemovesI j = removes.begin (); j != removes.end (); j++) {
				if (j->second == i->m_uid) {
					// this insert will be removed later.
					uint64_t us = i->insert.m_ev_us;
					uint32_t uid = i->m_uid;
					i->m_type = Command::INSERT_REMOVE;
					i->m_uid = uid;
					i->insert_remove.m_ev_us = us;
					i->insert_remove.m_ev_loc = j->first;
					break;
				}
			}
		}
	}
	std::cout << "calculate remove locations..." << std::endl;
	// calculate the final insert/remove location.
	for (CommandsI i = m_commands.begin (); i != m_commands.end (); i++) {
		if (i->m_type == Command::INSERT_REMOVE) {
			uint32_t loc = 0;
			for (CommandsI tmp = i; tmp != m_commands.end (); tmp++) {
				if (tmp->m_type == Command::REMOVE &&
				    tmp->m_uid == i->insert_remove.m_ev_loc) {
					i->insert_remove.m_ev_loc = loc;
					break;
				}
				loc++;
			}
		}
	}
}
void
LogReader::execute_log_commands (uint32_t uid)
{
	if (m_command == m_commands.end ()) {
		return;
	}
	//std::cout << "one event, uid=" <<m_uid<< std::endl;
	struct Command cmd = *m_command;
	//std::cout << "cmd uid=" <<cmd.m_uid<< std::endl;
	while (cmd.m_uid == uid) {
		m_command++;
		switch (cmd.m_type) {
		case Command::INSERT:
			//std::cout << "exec insert now=" << Simulator::now_us ()
			//<< ", time=" << cmd.insert.m_ev_us << std::endl;
			Simulator::schedule_abs_us (cmd.insert.m_ev_us, 
						 make_event (&LogReader::execute_log_commands, this, m_uid));
			m_uid++;
			break;
		case Command::INSERT_LATER:
			//std::cout << "exec insert later" << std::endl;
			Simulator::schedule_now (make_event (&LogReader::execute_log_commands, this, m_uid));
			m_uid++;
			break;
		case Command::REMOVE: {
			//std::cout << "exec remove" << std::endl;
			Event ev = m_remove_events.front ();
			m_remove_events.pop_front ();
			Simulator::remove (ev);
		} break;
		case Command::INSERT_REMOVE: {
			//std::cout << "exec insert remove" << std::endl;
			Event ev = make_event (&LogReader::execute_log_commands, this, m_uid);
			Simulator::schedule_abs_us (cmd.insert_remove.m_ev_us, ev);
			m_remove_events[cmd.insert_remove.m_ev_loc] = ev;
			m_uid++;
		} break;
		}
		cmd = *m_command;
	}
}

void
LogReader::print_stats (void)
{
	uint32_t n_inserts = 0;
	uint32_t n_removes = 0;
	for (CommandsI i = m_commands.begin (); i != m_commands.end (); i++) {
		switch (i->m_type) {
		case Command::INSERT:
			n_inserts++;
			break;
		case Command::INSERT_LATER:
			n_inserts++;
			break;
		case Command::INSERT_REMOVE:
			n_inserts++;
			break;
		case Command::REMOVE:
			n_removes++;
			break;
		}
	}
	std::cout << "inserts="<<n_inserts<<", removes="<<n_removes<<std::endl;
	std::cout << "run simulation..."<<std::endl;
}

void
LogReader::run (void)
{
	m_uid = 0;
	WallClockMs time;
	time.start ();
	m_command = m_commands.begin ();
	execute_log_commands (m_uid);
	Simulator::run ();
	unsigned long long delta = time.end ();
	double delay = ((double)delta)/1000;
	std::cout << "runtime="<<delay<<"s"<<std::endl;
}


int main (int argc, char *argv[])
{
	char const *input = 0;
	uint32_t n = 1;
	while (argc > 0) {
		if (strcmp ("--list", argv[0]) == 0) {
			Simulator::set_linked_list ();
		} else if (strcmp ("--heap", argv[0]) == 0) {
			Simulator::set_binary_heap ();
		} else if (strcmp ("--map", argv[0]) == 0) {
			Simulator::set_std_map ();
		} else if (strncmp ("--n=", argv[0], strlen("--n=")) == 0) {
			n = atoi (argv[0]+strlen ("--n="));
		} else if (strncmp ("--input=", argv[0],strlen ("--input=")) == 0) {
			input = argv[0] + strlen ("--input=");
		} else if (strncmp ("--log=", argv[0],strlen ("--log=")) == 0) {
			char const *filename = argv[0] + strlen ("--log=");
			Simulator::enable_log_to (filename);
		}
		argc--;
		argv++;
	}
	if (input == 0) {
		std::cerr << "need --input=[filename] option" << std::endl;
		return 1;
	}
	LogReader log;
	log.read_from_filename (input);
	for (uint32_t i = 0; i < n; i++) {
		log.run ();
	}
}
