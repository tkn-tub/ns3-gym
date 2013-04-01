import re
import subprocess

# import feature, taskgen_method, before_method, task_gen
from waflib import TaskGen, Node, Task, Utils, Build, Options, Logs, Task
debug = Logs.debug
error = Logs.error

import shellcmd
#shellcmd.subprocess = pproc # the WAF version of the subprocess module is supposedly less buggy
shellcmd.debug = debug


arg_rx = re.compile(r"(?P<dollar>\$\$)|(?P<subst>\$\{(?P<var>\w+)(?P<code>.*?)\})", re.M)

class command_task(Task.Task):
	color = "BLUE"
	def __init__(self, env, generator):
		Task.Task.__init__(self, env=env, normal=1, generator=generator)

	def __str__(self):
		"string to display to the user"
		env = self.env
		src_str = ' '.join([a.nice_path(env) for a in self.inputs])
		tgt_str = ' '.join([a.nice_path(env) for a in self.outputs])
		if self.outputs:
			sep = ' -> '
		else:
			sep = ''

		pipeline = shellcmd.Pipeline()
		pipeline.parse(self.generator.command)
		cmd = pipeline.get_abbreviated_command()
		return 'command (%s): %s%s%s\n' % (cmd, src_str, sep, tgt_str)

	def _subst_arg(self, arg, direction, namespace):
		"""
		@param arg: the command argument (or stdin/stdout/stderr) to substitute
		@param direction: direction of the argument: 'in', 'out', or None
		"""
		def repl(match):
			if match.group('dollar'):
				return "$"
			elif match.group('subst'):
				var = match.group('var')
				code = match.group('code')
				result = eval(var+code, namespace)
				if isinstance(result, Node.Node):
					if var == 'TGT':
						return result.get_bld().abspath()
					elif var == 'SRC':
						return result.srcpath()
					else:
						raise ValueError("Bad subst variable %r" % var)
				elif result is self.inputs:
					if len(self.inputs) == 1:
						return result[0].srcpath()
					else:
						raise ValueError("${SRC} requested but have multiple sources; which one?")
				elif result is self.outputs:
					if len(self.outputs) == 1:
						return result[0].get_bld().abspath()
					else:
						raise ValueError("${TGT} requested but have multiple targets; which one?")
				elif isinstance(result, list):
					assert len(result) == 1
					return result[0]
				else:
					return result
			return None

		return arg_rx.sub(repl, arg)

	def run(self):
		pipeline = shellcmd.Pipeline()
		pipeline.parse(self.generator.command)
		namespace = self.env.get_merged_dict()
		if self.generator.variables is not None:
			namespace.update(self.generator.variables)
		namespace.update(env=self.env, SRC=self.inputs, TGT=self.outputs)
		for cmd in pipeline.pipeline:
			if isinstance(cmd, shellcmd.Command):
				if isinstance(cmd.stdin, basestring):
					cmd.stdin = self._subst_arg(cmd.stdin, 'in', namespace)
				if isinstance(cmd.stdout, basestring):
					cmd.stdout = self._subst_arg(cmd.stdout, 'out', namespace)
				if isinstance(cmd.stderr, basestring):
					cmd.stderr = self._subst_arg(cmd.stderr, 'out', namespace)
				for argI in xrange(len(cmd.argv)):
					cmd.argv[argI] = self._subst_arg(cmd.argv[argI], None, namespace)
				if cmd.env_vars is not None:
					env_vars = dict()
					for name, value in cmd.env_vars.iteritems():
						env_vars[name] = self._subst_arg(value, None, namespace)
					cmd.env_vars = env_vars
			elif isinstance(cmd, shellcmd.Chdir):
				cmd.dir = self._subst_arg(cmd.dir, None, namespace)
		return pipeline.run(verbose=(Options.options.verbose > 0))

@TaskGen.taskgen_method
@TaskGen.feature('command')
def init_command(self):
	Utils.def_attrs(self,
					# other variables that can be used in the command: ${VARIABLE}
					variables = None,
					rule='')



@TaskGen.feature('command')
@TaskGen.after_method('process_rule')
def apply_command(self):
	#self.meths.remove('apply_core')
	# create the task
	task = self.create_task('command')
	setattr(task, "dep_vars", getattr(self, "dep_vars", None))
	# process the sources
	inputs = []
	for node in self.source:
		inputs.append(node)
	task.set_inputs(inputs)
	task.set_outputs([self.path.find_or_declare(tgt) for tgt in self.to_list(self.target)])
	self.source = ''
	#Task.file_deps = Task.extract_deps



# class command_taskgen(task_gen):
# 	def __init__(self, *k, **kw):
# 		task_gen.__init__(self, *k, **kw)
# 		self.features.append('command')
