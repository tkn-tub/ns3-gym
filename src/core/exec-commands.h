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
#ifndef EXEC_COMMANDS_H
#define EXEC_COMMANDS_H

#include <stdint.h>
#include <string>
#include <vector>
#include "callback.h"

namespace ns3 {

class ExecCommandsPrivate;

class Command {
public:
	void reset (void);
	void append (std::string arg);
	uint32_t get_n (void);
	char const *get (uint32_t i);
private:
	typedef std::vector<std::string> Args;
	Args m_args;
};

class ExecCommands {
public:
	typedef Callback<void,char const *,uint32_t> CommandCallback;
	ExecCommands (uint32_t pool_size);
	void enable_log (char const *main_log);
	void add (Command command, char const *id);
	void start (void);
	uint32_t get_size (void);
private:
	ExecCommandsPrivate *m_priv;
};

};

#endif /* EXEC_COMMANDS_H */
