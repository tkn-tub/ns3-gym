# python lib modules
import os
import sys
import shutil
import pproc as subprocess
import urllib

# WAF modules
import Build
import Options
import Utils
import Task

# local modules
import wutils


def dev_null():
    if sys.platform == 'win32':
        return open("NUL:", "w")
    else:
        return open("/dev/null", "w")


def _find_tests(testdir):
    """Return a list of test modules in the test directory

    Arguments:
    testdir -- the directory to look in for tests
    """
    names = os.listdir(testdir)
    tests = []
    for name in names:
        if name[:5] == "test-" and name[-3:] == ".py":
            testname = name[:-3]
            tests.append(testname)
    tests.sort()
    return tests


class regression_test_task(Task.TaskBase):
    after = 'cc cxx cc_link cxx_link'
    color = 'BLUE'

    def __init__(self, env, test_name, test_scripts_dir, build_traces_dir, reference_traces):
        super(regression_test_task, self).__init__()
        self.env = env
        self.test_name = test_name
        self.test_scripts_dir = test_scripts_dir
        self.build_traces_dir = build_traces_dir
        self.reference_traces_dir = reference_traces

    def __str__(self):
        return 'regression-test (%s)\n' % self.test_name

    def run(self):
        """Run a single test"""
        sys.path.insert(0, self.test_scripts_dir)
        try:
            mod = __import__(self.test_name, globals(), locals(), [])
        finally:
            sys.path.remove(self.test_scripts_dir)

        assert self.test_name.startswith('test-')
        short_name = self.test_name[len('test-'):]

        trace_dir_name = getattr(mod, "trace_dir_name", None)
        if trace_dir_name is None:
            trace_dir_name = "%s.ref" % short_name
        trace_output_path = os.path.join(self.build_traces_dir, trace_dir_name)
        reference_traces_path = os.path.join(self.reference_traces_dir, trace_dir_name)

        if hasattr(mod, 'get_arguments'):
            arguments = mod.get_arguments(self.env, '..')
        else:
            arguments = getattr(mod, "arguments", [])

        pyscript = getattr(mod, "pyscript", None)
        if pyscript:
            is_pyscript = True
            program = pyscript
        else:
            is_pyscript = False
            program = getattr(mod, "program", short_name)

        if hasattr(mod, 'may_run'):
            reason_cannot_run = mod.may_run(self.env)
        else:
            reason_cannot_run = None
        if reason_cannot_run:
            print "SKIP %s (%s)" % (self.test_name, reason_cannot_run)
            self.result = None
            return 0

        if Options.options.regression_generate:
            # clean the target dir
            shutil.rmtree(reference_traces_path, ignore_errors=True)
            os.makedirs(reference_traces_path)
            result = self.run_reference_generate(reference_traces_path, program, arguments, is_pyscript)
            if result == 0:
                print "GENERATE " + self.test_name
            else:
                print "GENERATE FAIL " + self.test_name
        else:
            # clean the target dir
            shutil.rmtree(trace_output_path, ignore_errors=True)
            os.makedirs(trace_output_path)
            # run it
            result = self.run_reference_test(reference_traces_path, trace_output_path, program, arguments, is_pyscript)
            if result == 0:
                print "PASS " + self.test_name
            else:
                print "FAIL " + self.test_name
        self.result = result
        return 0

    def run_reference_test(self, reference_traces_path, trace_output_path, program, arguments, is_pyscript):
        if not os.path.exists(reference_traces_path):
            print "Cannot locate reference traces in " + reference_traces_path
            return 1

        if is_pyscript:
            script = os.path.abspath(os.path.join('..', *os.path.split(program)))
            argv = [self.env['PYTHON'], script] + arguments
            wutils.run_argv(argv, cwd=trace_output_path)
        else:
            wutils.run_program(program,
                               command_template=wutils.get_command_template(self.env, arguments),
                               cwd=trace_output_path)

        if Options.options.verbose:
            #diffCmd = "diff traces " + refTestDirName + " | head"
            diffCmd = subprocess.Popen([self.env['DIFF'], trace_output_path, reference_traces_path],
                                       stdout=subprocess.PIPE)
            headCmd = subprocess.Popen("head", stdin=diffCmd.stdout)
            rc2 = headCmd.wait()
            diffCmd.stdout.close()
            rc1 = diffCmd.wait()
            rc = rc1 or rc2
        else:
            rc = subprocess.Popen([self.env['DIFF'], trace_output_path, reference_traces_path], stdout=dev_null()).wait()
        if rc:
            print "----------"
            print "Traces differ in test: ", self.test_name
            print "Reference traces in directory: " + reference_traces_path
            print "Traces in directory: " + trace_output_path
            print "Run the following command for details:"
            print "\tdiff -u %s %s" % (reference_traces_path, trace_output_path)
            print "----------"
        return rc


    def run_reference_generate(self, trace_output_path, program, arguments, is_pyscript):
        if is_pyscript:
            script = os.path.abspath(os.path.join('..', *os.path.split(program)))
            argv = [self.env['PYTHON'], script] + arguments
            retval = wutils.run_argv(argv, cwd=trace_output_path)
        else:
            retval = wutils.run_program(program,
                                        command_template=wutils.get_command_template(self.env, arguments),
                                        cwd=trace_output_path)
        return retval


class regression_test_collector_task(Task.TaskBase):
    after = 'regression_test_task'
    color = 'BLUE'

    def __init__(self, test_tasks):
        super(regression_test_collector_task, self).__init__()
        self.test_tasks = test_tasks

    def __str__(self):
        return 'regression-test-collector\n'

    def run(self):
        failed_tests = [test for test in self.test_tasks if test.result is not None and test.result != 0]
        skipped_tests = [test for test in self.test_tasks if test.result is None]
        print "Regression testing summary:"
        if skipped_tests:
            print "SKIP: %i of %i tests have been skipped (%s)" % (
                len(skipped_tests), len(self.test_tasks),
                ', '.join([test.test_name for test in skipped_tests]))
        if failed_tests:
            print "FAIL: %i of %i tests have failed (%s)" % (
                len(failed_tests), len(self.test_tasks),
                ', '.join([test.test_name for test in failed_tests]))
            return 1
        else:
            print "PASS: %i of %i tests passed" % (len(self.test_tasks) - len(skipped_tests),
                                                   len(self.test_tasks))
            return 0

def run_regression(bld, reference_traces):
    """Execute regression tests.  Called with cwd set to the 'regression' subdir of ns-3.

    @param reference_traces: reference traces directory.

    """

    testdir = os.path.join("regression", "tests")
    if not os.path.exists(testdir):
        print "Tests directory does not exist"
        sys.exit(3)

    if Options.options.regression_tests:
        tests = Options.options.regression_tests.split(',')
    else:
        tests = _find_tests(testdir)

    if not os.path.exists(reference_traces):
        print "Reference traces directory (%s) does not exist" % reference_traces
        return 3
    
    test_scripts_dir = bld.path.find_dir('regression/tests').abspath()
    build_traces_dir = bld.path.find_or_declare('regression/traces').abspath(bld.env)
    tasks = []
    for test in tests:
        tasks.append(regression_test_task(bld.env, test, test_scripts_dir, build_traces_dir, reference_traces))
    regression_test_collector_task(tasks)
