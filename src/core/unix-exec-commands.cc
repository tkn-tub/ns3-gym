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
#include "exec-commands.h"
#include "system-thread.h"
#include "system-semaphore.h"
#include "system-mutex.h"
#include "callback.h"
#include <cassert>
#include <vector>

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define noTRACE_COMMAND 1

#ifdef TRACE_COMMAND
#include <iostream>
# define TRACE(x) \
std::cout << "COMMAND TRACE " << x << std::endl;
#else /* TRACE_COMMAND */
# define TRACE(format,...)
#endif /* TRACE_COMMAND */


namespace ns3 {

class LogThread : public SystemThread {
public:
	LogThread (int main_log);
	virtual ~LogThread ();
	void add (int in, int out);
private:
	virtual void real_run (void);
	void write_to (int fd, char *buffer, int size);
	typedef std::vector<int> Fds;
	typedef std::vector<int>::iterator FdsI;

	int m_main_log;
	Fds m_input_fds;
	Fds m_output_fds;
	SystemMutex m_fds_mutex;
};

LogThread::LogThread (int main_log)
	: m_main_log (main_log)
{}

LogThread::~LogThread ()
{}

void
LogThread::add (int in, int out)
{
	m_fds_mutex.lock ();
	m_input_fds.push_back (in);
	m_output_fds.push_back (out);
	m_fds_mutex.unlock ();
}
void
LogThread::write_to (int fd, char *buffer, int to_write)
{
	while (to_write > 0) {
		int written = write (fd, buffer, to_write);
		to_write -= written;
		buffer += written;
		assert (written >= 0);
	}
}

void
LogThread::real_run (void)
{
	char buffer[1024];
	while (true) {
		fd_set in;
		struct timeval tv;
		int retval;

		TRACE ("prepare fd set");
		m_fds_mutex.lock ();
		assert (m_input_fds.size () == m_output_fds.size ());
		FD_ZERO (&in);
		int max_fd = 0;
		for (uint32_t i = 0; i < m_input_fds.size (); i++) {
			if (m_input_fds[i] > max_fd) {
				max_fd = m_input_fds[i];
			}
			FD_SET (m_input_fds[i], &in);
		}
		m_fds_mutex.unlock ();
		TRACE ("fd set prepared");

		tv.tv_sec = 1;
		tv.tv_usec = 0;

		retval = select (max_fd+1, &in, NULL, NULL, &tv);
		TRACE ("selected");
		for (uint32_t i = 0; i < m_input_fds.size (); i++) {
			if (FD_ISSET (m_input_fds[i], &in)) {
				retval = read (m_input_fds[i], buffer, 1024);
				if (retval == 0) {
					close (m_output_fds[i]);
					TRACE ("closed input");
					m_fds_mutex.lock ();
					FdsI cur = m_input_fds.begin ();
					cur += i;
					m_input_fds.erase (cur);
					cur = m_output_fds.begin ();
					cur += i;
					m_output_fds.erase (cur);
					m_fds_mutex.unlock ();
					continue;
				}
				assert (retval > 0);
				write_to (m_output_fds[i], buffer, retval);
				write_to (m_main_log, buffer, retval);
			}
		}
		TRACE ("written");
	}
}


class CommandSystemThread : public SystemThread {
public:
	typedef Callback<void, CommandSystemThread *, int, std::string> DoneLogCallback;
	typedef Callback<void, CommandSystemThread *> DoneNoLogCallback;
	CommandSystemThread ();
	void start_log (Command command, std::string id, DoneLogCallback callback);
	void start_no_log (Command command, DoneNoLogCallback callback);
	void stop (void);
private:
	virtual void real_run (void);
	void run_exec (void);
	SystemSemaphore m_sem;
	bool m_stop;
	Command m_command;
	std::string m_id;
	DoneLogCallback m_done_log;
	DoneNoLogCallback m_done_no_log;
	bool m_no_log;
};

CommandSystemThread::CommandSystemThread ()
	: m_sem (0),
	  m_stop (false)
{}

void
CommandSystemThread::start_no_log (Command command, DoneNoLogCallback done)
{
	m_no_log = true;
	m_command = command;
	m_done_no_log = done;
	m_sem.post ();
}

void
CommandSystemThread::start_log (Command command, std::string id, DoneLogCallback done)
{
	m_no_log = false;
	m_command = command;
	m_id = id;
	m_done_log = done;
	m_sem.post ();
}

void
CommandSystemThread::stop (void)
{
	m_stop = true;
	m_sem.post ();
}

void
CommandSystemThread::run_exec (void)
{
	char ** args = (char **)malloc (sizeof (char *) * (m_command.get_n ()+1));
	char const *file = m_command.get (0);
	args[0] = strdup (m_command.get (0));
	for (uint32_t i = 1; i < m_command.get_n (); i++) {
		args[i] = strdup (m_command.get (i));
		//std::cout << "arg " << args[i-1] << std::endl;
	}
	args[m_command.get_n ()] = (char *)NULL;
	int retval = execvp (file, args);
	if (retval != 0) {
		std::cout << strerror (errno) << std::endl;
		exit (1);
	}
	// NOTREACHED
	assert (false);
}

void
CommandSystemThread::real_run (void)
{
	while (true) {
		m_sem.wait ();
		if (m_stop) {
			break;
		}
		int retval;
		int filedes[2];
		if (m_no_log) {
			pid_t pid = fork ();
			if (pid == -1) {
				assert (false);
				// error in parent fork.
			} else if (pid == 0) {
				retval = open ("/dev/null", O_WRONLY);
				dup2 (retval, 1);
				close (retval);
				// success, child.
				run_exec ();
			} else {
				// success, parent.
				m_done_no_log (this);
			}
		} else {
			retval = pipe (filedes);
			assert (retval == 0);
			pid_t pid = fork ();
			if (pid == -1) {
				assert (false);
				// error in parent fork.
			} else if (pid == 0) {
				// success, child.
				TRACE ("child");
				close (1);
				close (filedes[0]);
				dup2 (filedes[1], 1);
				run_exec ();
			} else {
				TRACE ("parent");
				// success, parent.
				close (filedes[1]);
				m_done_log (this, filedes[0], m_id);
			}
		}
	}
}

class ExecCommandsPrivate {
public:
	ExecCommandsPrivate (uint32_t pool_size);
	~ExecCommandsPrivate ();
	void enable_log (char const *main_log);
	void add (Command command, std::string id);
	void start (void);
	uint32_t get_size (void);
private:
	struct CommandRequest {
		Command m_command;
		std::string m_id;
	};
	typedef std::vector<CommandSystemThread *> Threads;
	typedef std::vector<CommandSystemThread *>::iterator ThreadsI;
	typedef std::vector<CommandRequest> Requests;
	typedef std::vector<CommandRequest>::iterator RequestsI;
	void command_done_log (CommandSystemThread *thread, int fd, std::string id);
	void command_done_no_log (CommandSystemThread *thread);

	Requests m_requests;
	SystemSemaphore m_n_threads;
	uint32_t m_pool_size;
	Threads m_threads;
	SystemMutex m_threads_mutex;
	int m_main_log;
	LogThread *m_log;
};


ExecCommandsPrivate::ExecCommandsPrivate (uint32_t pool_size)
	: m_n_threads (0),
	  m_pool_size (pool_size),
	  m_main_log (0),
	  m_log (0)
{
	m_threads_mutex.lock ();
	for (uint32_t i = 0; i < pool_size; i++) {
		m_threads.push_back (new CommandSystemThread ());
	}
	m_threads_mutex.unlock ();
	m_n_threads.post (m_pool_size);
}

ExecCommandsPrivate::~ExecCommandsPrivate ()
{
	m_threads_mutex.lock ();
	for (ThreadsI i = m_threads.begin (); i != m_threads.end (); i++) {
		(*i)->stop ();
		delete *i;
	}
	m_threads.erase (m_threads.begin (), m_threads.end ());
	m_threads_mutex.unlock ();
	delete m_log;
}

void 
ExecCommandsPrivate::enable_log (char const *main_log)
{
	m_main_log = open (main_log, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	m_log = new LogThread (m_main_log);
	assert (m_main_log != -1);
}


void
ExecCommandsPrivate::add (Command command, std::string id)
{
	struct CommandRequest request;
	request.m_command = command;
	request.m_id = id;
	m_requests.push_back (request);
}
void 
ExecCommandsPrivate::start (void)
{
	for (RequestsI i = m_requests.begin (); i != m_requests.end (); i++) {
		TRACE ("wait for thread");
		m_n_threads.wait ();
		m_threads_mutex.lock ();
		CommandSystemThread *thread = m_threads.back ();
		m_threads.pop_back ();
		m_threads_mutex.unlock ();
		TRACE ("start command");
		if (m_main_log != 0) {
			thread->start_log (i->m_command, i->m_id, 
					   make_callback (&ExecCommandsPrivate::command_done_log, this));
		} else {
			thread->start_no_log (i->m_command, 
					      make_callback (&ExecCommandsPrivate::command_done_no_log, this));
		}
	}
}
void
ExecCommandsPrivate::command_done_log (CommandSystemThread *thread, int in, std::string id)
{
	TRACE ("command done");
	m_threads_mutex.lock ();
	m_threads.push_back (thread);
	m_threads_mutex.unlock ();
	m_n_threads.post ();

	std::string out_filename = id;
	out_filename.append (".log");
	int out_fd = open (out_filename.c_str (), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	assert (out_fd != -1);
	m_log->add (in, out_fd);
	TRACE ("notify command done");
}

void
ExecCommandsPrivate::command_done_no_log (CommandSystemThread *thread)
{
	TRACE ("command done");
	m_threads_mutex.lock ();
	m_threads.push_back (thread);
	m_threads_mutex.unlock ();
	m_n_threads.post ();
	TRACE ("notify command done");
}

uint32_t
ExecCommandsPrivate::get_size (void)
{
	return m_requests.size ();
}
				   
void 
Command::reset (void)
{
	m_args.erase (m_args.begin (), m_args.end ());
}

void 
Command::append (std::string arg)
{
	m_args.push_back (arg);
}


uint32_t
Command::get_n (void)
{
	return m_args.size ();
}
char const*
Command::get (uint32_t i)
{
	return m_args[i].c_str ();
}



ExecCommands::ExecCommands (uint32_t pool_size)
	: m_priv (new ExecCommandsPrivate (pool_size))
{
	assert (pool_size != 0);
}
void 
ExecCommands::enable_log (char const *main_log)
{
	m_priv->enable_log (main_log);
}
void 
ExecCommands::add (Command command, char const *id)
{
	m_priv->add (command, id);
}
void 
ExecCommands::start (void)
{
	m_priv->start ();
}

uint32_t 
ExecCommands::get_size (void)
{
	return m_priv->get_size ();
}

}; // namespace ns3

#ifdef RUN_SELF_TESTS

#include "test.h"
#include <iomanip>
#include <iostream>

namespace ns3 {

class ExecCommandsTest: public Test {
public:
	ExecCommandsTest ();
	virtual ~ExecCommandsTest ();
	virtual bool run_tests (void);
private:
	void command_output (char const *data, uint32_t size);
};

ExecCommandsTest::ExecCommandsTest ()
	: Test ("ExecCommands")
{}
ExecCommandsTest::~ExecCommandsTest ()
{}
void 
ExecCommandsTest::command_output (char const *data, uint32_t size)
{
	for (uint32_t i = 0; i < size; i++) {
		std::cout << data[i];
	};
}
bool 
ExecCommandsTest::run_tests (void)
{
	bool ok = true;
#if 0
	ExecCommands commands = ExecCommands (2);
	Command command;
	command.append ("ls");
	command.append ("-l");
	commands.add (command, "ls");

	command.reset ();
	command.append ("ls");
	command.append ("-l");
	command.append ("src");
	commands.add (command, "ls-src");

	command.reset ();
	command.append ("ls");
	command.append ("-l");
	command.append ("/usr/bin");
	commands.add (command, "ls-usr-bin");

	commands.enable_log ("main.log");
	commands.start ();
	sleep (5);
#endif
	return ok;
}

static ExecCommandsTest g_exec_commands_test;

}; // namespace ns3

#endif /* RUN_SELF_TESTS */


