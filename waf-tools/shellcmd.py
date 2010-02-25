# Copyright (C) 2008 Gustavo J. A. M. Carneiro  <gjcarneiro@gmail.com>

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

import shlex
import subprocess
import sys
import re
import os

env_var_rx = re.compile(r"^([a-zA-Z0-9_]+)=(\S+)$")

def debug(message):
    print >> sys.stderr, message


if sys.platform == 'win32':
    dev_null = open("NUL:", "w")
else:
    dev_null = open("/dev/null", "w")

fcntl = fd = fl = None
try:
    import fcntl
except ImportError:
    pass
else:
    fd = dev_null.fileno()
    fl = fcntl.fcntl(fd, fcntl.F_GETFD)
    fcntl.fcntl(fd, fcntl.F_SETFD, fl | fcntl.FD_CLOEXEC)
del fcntl, fd, fl

def _open_out_file(filename):
    if filename in ['NUL:', '/dev/null']:
        return dev_null
    else:
        return open(filename, 'wb')


class Node(object):
    pass

class Op(Node):
    pass

class Pipe(Op):
    pass

class And(Op):
    pass

class Or(Op):
    pass

class Command(Node):
    class PIPE(object):
        pass # PIPE is a constant
    class STDOUT(object):
        pass # PIPE is a constant

    def __init__(self, name):
        super(Command, self).__init__()
        self.name = name # command name
        self.argv = [name] # command argv
        self.stdin = None
        self.stdout = None
        self.stderr = None
        self.env_vars = None

    def __repr__(self):
        return "Command(%r, argv=%r, stdin=%r, stdout=%r, stderr=%r)" \
            % (self.name, self.argv, self.stdin, self.stdout, self.stderr)

class Chdir(Node):
    def __init__(self):
        super(Chdir, self).__init__()
        self.dir = None

    def __repr__(self):
        return "Chdir(%r)" \
            % (self.dir)

class Pipeline(object):
    def __init__(self):
        self.current_command = None
        self.pipeline = []

    def _commit_command(self):
        assert self.current_command is not None
        self.pipeline.append(self.current_command)
        self.current_command = None

    def get_abbreviated_command(self):
        l = []
        for node in self.pipeline:
            if isinstance(node, Command):
                l.append(node.name)
            if isinstance(node, Chdir):
                l.append('cd %s' % node.dir)
            elif isinstance(node, Pipe):
                l.append('|')
            elif isinstance(node, And):
                l.append('&&')
            elif isinstance(node, And):
                l.append('||')
        return ' '.join(l)

    def parse(self, command):
        self.current_command = None
        self.pipeline = []

        if isinstance(command, list):
            tokens = list(command)
        else:
            tokens = shlex.split(command)
        debug("command: shlex: %r" % (tokens,))

        BEGIN, COMMAND, CHDIR, STDERR, STDOUT, STDIN = range(6)
        state = BEGIN
        self.current_command = None
        env_vars = dict()

        while tokens:
            token = tokens.pop(0)
            if state == BEGIN:
                env_var_match = env_var_rx.match(token)
                if env_var_match is not None:
                    env_vars[env_var_match.group(1)] = env_var_match.group(2)
                else:
                    assert self.current_command is None
                    if token == 'cd':
                        self.current_command = Chdir()
                        assert not env_vars
                        state = CHDIR
                    else:
                        self.current_command = Command(token)
                        if env_vars:
                            self.current_command.env_vars = env_vars
                            env_vars = dict()
                        state = COMMAND
            elif state == COMMAND:
                if token == '>':
                    state = STDOUT
                elif token == '2>':
                    state = STDERR
                elif token == '2>&1':
                    assert self.current_command.stderr is None
                    self.current_command.stderr = Command.STDOUT
                elif token == '<':
                    state = STDIN
                elif token == '|':
                    assert self.current_command.stdout is None
                    self.current_command.stdout = Command.PIPE
                    self._commit_command()
                    self.pipeline.append(Pipe())
                    state = BEGIN
                elif token == '&&':
                    self._commit_command()
                    self.pipeline.append(And())
                    state = BEGIN
                elif token == '||':
                    self._commit_command()
                    self.pipeline.append(Or())
                    state = BEGIN
                else:
                    self.current_command.argv.append(token)
            elif state == CHDIR:
                if token == '&&':
                    self._commit_command()
                    self.pipeline.append(And())
                    state = BEGIN
                else:
                    assert self.current_command.dir is None
                    self.current_command.dir = token
            elif state == STDOUT:
                assert self.current_command.stdout is None
                self.current_command.stdout = token
                state = COMMAND
            elif state == STDERR:
                assert self.current_command.stderr is None
                self.current_command.stderr = token
                state = COMMAND
            elif state == STDIN:
                assert self.current_command.stdin is None
                self.current_command.stdin = token
                state = COMMAND
        self._commit_command()
        return self.pipeline

    def _exec_piped_commands(self, commands):
        retvals = []
        for cmd in commands:
            retvals.append(cmd.wait())
        retval = 0
        for r in retvals:
            if r:
                retval = retvals[-1]
                break
        return retval

    def run(self, verbose=False):
        pipeline = list(self.pipeline)
        files_to_close = []
        piped_commands = []
        piped_commands_display = []
        BEGIN, PIPE = range(2)
        state = BEGIN
        cwd = '.'
        while pipeline:
            node = pipeline.pop(0)

            if isinstance(node, Chdir):
                next_op = pipeline.pop(0)
                assert isinstance(next_op, And)
                cwd = os.path.join(cwd, node.dir)
                if verbose:
                    piped_commands_display.append("cd %s &&" % node.dir)
                continue
            
            assert isinstance(node, (Command, Chdir))
            cmd = node
            if verbose:
                if cmd.env_vars:
                    env_vars_str = ' '.join(['%s=%s' % (key, val) for key, val in cmd.env_vars.iteritems()])
                    piped_commands_display.append("%s %s" % (env_vars_str, ' '.join(cmd.argv)))
                else:
                    piped_commands_display.append(' '.join(cmd.argv))

            if state == PIPE:
                stdin = piped_commands[-1].stdout
            elif cmd.stdin is not None:
                stdin = open(cmd.stdin, "r")
                if verbose:
                    piped_commands_display.append('< %s' % cmd.stdin)
                files_to_close.append(stdin)
            else:
                stdin = None

            if cmd.stdout is None:
                stdout = None
            elif cmd.stdout is Command.PIPE:
                stdout = subprocess.PIPE
            else:
                stdout = _open_out_file(cmd.stdout)
                files_to_close.append(stdout)
                if verbose:
                    piped_commands_display.append('> %s' % cmd.stdout)

            if cmd.stderr is None:
                stderr = None
            elif cmd.stderr is Command.PIPE:
                stderr = subprocess.PIPE
            elif cmd.stderr is Command.STDOUT:
                stderr = subprocess.STDOUT
                if verbose:
                    piped_commands_display.append('2>&1')
            else:
                stderr = _open_out_file(cmd.stderr)
                files_to_close.append(stderr)
                if verbose:
                    piped_commands_display.append('2> %s' % cmd.stderr)

            if cmd.env_vars:
                env = dict(os.environ)
                env.update(cmd.env_vars)
            else:
                env = None

            if cwd == '.':
                proc_cwd = None
            else:
                proc_cwd = cwd

            debug("command: subprocess.Popen(argv=%r, stdin=%r, stdout=%r, stderr=%r, env_vars=%r, cwd=%r)"
                  % (cmd.argv, stdin, stdout, stderr, cmd.env_vars, proc_cwd))
            proc = subprocess.Popen(cmd.argv, stdin=stdin, stdout=stdout, stderr=stderr, env=env, cwd=proc_cwd)
            del stdin, stdout, stderr
            piped_commands.append(proc)

            try:
                next_node = pipeline.pop(0)
            except IndexError:
                try:
                    retval = self._exec_piped_commands(piped_commands)
                    if verbose:
                        print "%s: exit code %i" % (' '.join(piped_commands_display), retval)
                finally:
                    for f in files_to_close:
                        if f is not dev_null:
                            f.close()
                    files_to_close = []
                return retval
            else:

                if isinstance(next_node, Pipe):
                    state = PIPE
                    piped_commands_display.append('|')

                elif isinstance(next_node, Or):
                    try:
                        this_retval = self._exec_piped_commands(piped_commands)
                    finally:
                        for f in files_to_close:
                            if f is not dev_null:
                                f.close()
                        files_to_close = []
                    if this_retval == 0:
                        if verbose:
                            print "%s: exit code %i (|| is short-circuited)" % (' '.join(piped_commands_display), retval)
                        return this_retval
                    if verbose:
                        print "%s: exit code %i (|| proceeds)" % (' '.join(piped_commands_display), retval)
                    state = BEGIN
                    piped_commands = []
                    piped_commands_display = []

                elif isinstance(next_node, And):
                    try:
                        this_retval = self._exec_piped_commands(piped_commands)
                    finally:
                        for f in files_to_close:
                            if f is not dev_null:
                                f.close()
                        files_to_close = []
                    if this_retval != 0:
                        if verbose:
                            print "%s: exit code %i (&& is short-circuited)" % (' '.join(piped_commands_display), retval)
                        return this_retval
                    if verbose:
                        print "%s: exit code %i (&& proceeds)" % (' '.join(piped_commands_display), retval)
                    state = BEGIN
                    piped_commands = []
                    piped_commands_display = []



def _main():
    pipeline = Pipeline()
    pipeline.parse('./foo.py 2>&1 < xxx | cat && ls')
    print pipeline.run()

if __name__ == '__main__':
    _main()

