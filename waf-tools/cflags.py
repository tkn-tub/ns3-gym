from waflib import Logs, Options, Utils


class CompilerTraits(object):
	def get_warnings_flags(self, level):
		"""get_warnings_flags(level) -> list of cflags"""
		raise NotImplementedError

	def get_optimization_flags(self, level):
		"""get_optimization_flags(level) -> list of cflags"""
		raise NotImplementedError

	def get_debug_flags(self, level):
		"""get_debug_flags(level) -> (list of cflags, list of cppdefines)"""
		raise NotImplementedError


class GccTraits(CompilerTraits):
	def __init__(self):
		super(GccTraits, self).__init__()
		# cumulative list of warnings per level
		self.warnings_flags = [['-Wall'], ['-Werror'], ['-Wextra']]

	def get_warnings_flags(self, level):
		warnings = []
		for l in range(level):
			if l < len(self.warnings_flags):
				warnings.extend(self.warnings_flags[l])
			else:
				break
		return warnings

	def get_optimization_flags(self, level):
		if level == 0:
			return ['-O0']
		elif level == 1:
			return ['-O']
		elif level == 2:
			return ['-O2']
		elif level == 3:
			return ['-O3']

	def get_debug_flags(self, level):
		if level == 0:
			return (['-g0'], ['NDEBUG'])
		elif level == 1:
			return (['-g'], [])
		elif level >= 2:
			return (['-ggdb', '-g3'], ['_DEBUG'])
		

class IccTraits(CompilerTraits):
	def __init__(self):
		super(IccTraits, self).__init__()
		# cumulative list of warnings per level
		# icc is _very_ verbose with -Wall, -Werror is barely achievable
		self.warnings_flags = [[], [], ['-Wall']]
		
	def get_warnings_flags(self, level):
		warnings = []
		for l in range(level):
			if l < len(self.warnings_flags):
				warnings.extend(self.warnings_flags[l])
			else:
				break
		return warnings

	def get_optimization_flags(self, level):
		if level == 0:
			return ['-O0']
		elif level == 1:
			return ['-O']
		elif level == 2:
			return ['-O2']
		elif level == 3:
			return ['-O3']

	def get_debug_flags(self, level):
		if level == 0:
			return (['-g0'], ['NDEBUG'])
		elif level == 1:
			return (['-g'], [])
		elif level >= 2:
			return (['-ggdb', '-g3'], ['_DEBUG'])
		


class MsvcTraits(CompilerTraits):
	def __init__(self):
		super(MsvcTraits, self).__init__()
		# cumulative list of warnings per level
		self.warnings_flags = [['/W2'], ['/WX'], ['/Wall']]

	def get_warnings_flags(self, level):
		warnings = []
		for l in range(level):
			if l < len(self.warnings_flags):
				warnings.extend(self.warnings_flags[l])
			else:
				break
		return warnings

	def get_optimization_flags(self, level):
		if level == 0:
			return ['/Od']
		elif level == 1:
			return []
		elif level == 2:
			return ['/O2']
		elif level == 3:
			return ['/Ox']

	def get_debug_flags(self, level):
		if level == 0:
			return ([], ['NDEBUG'])
		elif level == 1:
			return (['/ZI', '/RTC1'], [])
		elif level >= 2:
			return (['/ZI', '/RTC1'], ['_DEBUG'])



gcc = GccTraits()
icc = IccTraits()
msvc = MsvcTraits()

# how to map env['COMPILER_CC'] or env['COMPILER_CXX'] into a traits object
compiler_mapping = {
	'gcc': gcc,
	'g++': gcc,
	'msvc': msvc,
	'icc': icc,
	'icpc': icc,
	'clang': gcc,
	'clang++': gcc,
}

profiles = {
	# profile name: [optimization_level, warnings_level, debug_level]
	'default': [2, 1, 1],
	'debug': [0, 2, 3],
	'release': [3, 1, 0],
	}

default_profile = 'default'

def options(opt):
	assert default_profile in profiles
	opt.add_option('-d', '--build-profile',
		       action='store',
		       default=default_profile,
		       help=("Specify the build profile.  "
			     "Build profiles control the default compilation flags"
			     " used for C/C++ programs, if CCFLAGS/CXXFLAGS are not"
			     " set set in the environment. [Allowed Values: %s]"
			     % ", ".join([repr(p) for p in profiles.keys()])),
		       choices=profiles.keys(),
		       dest='build_profile')

def configure(conf):
	cc = conf.env['COMPILER_CC'] or None
	cxx = conf.env['COMPILER_CXX'] or None
	if not (cc or cxx):
		raise Utils.WafError("neither COMPILER_CC nor COMPILER_CXX are defined; "
				     "maybe the compiler_cc or compiler_cxx tool has not been configured yet?")
	
	try:
		compiler = compiler_mapping[cc]
	except KeyError:
		try:
			compiler = compiler_mapping[cxx]
		except KeyError:
			Logs.warn("No compiler flags support for compiler %r or %r"
				  % (cc, cxx))
			return

	opt_level, warn_level, dbg_level = profiles[Options.options.build_profile]

	optimizations = compiler.get_optimization_flags(opt_level)
	debug, debug_defs = compiler.get_debug_flags(dbg_level)
	warnings = compiler.get_warnings_flags(warn_level)
	
	if cc and not conf.env['CCFLAGS']:
		conf.env.append_value('CCFLAGS', optimizations)
		conf.env.append_value('CCFLAGS', debug)
		conf.env.append_value('CCFLAGS', warnings)
		conf.env.append_value('CCDEFINES', debug_defs)
	if cxx and not conf.env['CXXFLAGS']:
		conf.env.append_value('CXXFLAGS', optimizations)
		conf.env.append_value('CXXFLAGS', debug)
		conf.env.append_value('CXXFLAGS', warnings)
		conf.env.append_value('CXXDEFINES', debug_defs)
