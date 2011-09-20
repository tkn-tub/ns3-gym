# -*- mode: python; encoding: utf-8 -*-
# Gustavo Carneiro (gjamc) 2008

import Options
import Configure
import subprocess
import config_c
import sys

def configure(conf):
	pkg_config = conf.find_program('pkg-config', var='PKG_CONFIG')
	if not pkg_config: return

@Configure.conf
def pkg_check_modules(conf, uselib_name, expression, mandatory=True):
	pkg_config = conf.env['PKG_CONFIG']
	if not pkg_config:
		if mandatory:
			conf.fatal("pkg-config is not available")
		else:
			return False

	if Options.options.verbose:
		extra_msg = ' (%s)' % expression
	else:
		extra_msg = ''

	conf.start_msg('Checking for pkg-config flags for %s%s' % (uselib_name, extra_msg))

	argv = [pkg_config, '--cflags', '--libs', expression]
	cmd = subprocess.Popen(argv, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	out, err = cmd.communicate()
	retval = cmd.wait()

	conf.to_log('%r: %r (exit code %i)\n%s' % (argv, out, retval, err))

	if retval != 0:
		conf.end_msg(False)
		sys.stderr.write(err)
	else:
		if Options.options.verbose:
			conf.end_msg(out)
		else:
			conf.end_msg(True)

	if retval == 0:
		conf.parse_flags(out, uselib_name, conf.env)
		conf.env[uselib_name] = True
		return True

	else:

		conf.env[uselib_name] = False
		if mandatory:
			raise Configure.ConfigurationError('pkg-config check failed')
		else:
			return False

@Configure.conf
def pkg_check_module_variable(conf, module, variable):
	pkg_config = conf.env['PKG_CONFIG']
	if not pkg_config:
		conf.fatal("pkg-config is not available")

	argv = [pkg_config, '--variable', variable, module]
	cmd = subprocess.Popen(argv, stdout=subprocess.PIPE)
	out, dummy = cmd.communicate()
	retval = cmd.wait()
	out = out.rstrip() # strip the trailing newline

	msg_checking = ("Checking for pkg-config variable %r in %s" % (variable, module,))
	conf.check_message_custom(msg_checking, '', out)
	conf.log.write('%r: %r (exit code %i)\n' % (argv, out, retval))

	if retval == 0:
		return out
	else:
		raise Configure.ConfigurationError('pkg-config check failed')
