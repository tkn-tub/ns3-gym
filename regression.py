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

# local modules
import wutils


def dev_null():
    if sys.platform == 'win32':
        return open("NUL:", "w")
    else:
        return open("/dev/null", "w")


### Regression testing
class Regression(object):
    def __init__(self, testdir, reference_traces):
        self.testdir = testdir
        self.reference_traces = reference_traces
        self.env = Build.bld.env

    def run_test(self, verbose, generate, testName, arguments=[], pyscript=None, refTestName=None):
        """
        @param verbose: enable verbose execution

        @param generate: generate new traces instead of comparing with the reference

        @param testName: name of the test

        @arguments: list of extra parameters to pass to the program to be tested

        @pyscript: if not None, the test is written in Python and this
        parameter contains the path to the python script, relative to
        the project root dir

        @param refTestName: if not None, this is the name of the directory under refDirName
        that contains the reference traces. Otherwise "refDirname/testName + .ref" is used.

        """
        if not isinstance(arguments, list):
            raise TypeError
        
        if refTestName is None:
            refTestDirName = os.path.join(self.reference_traces, (testName + ".ref"))
        else:
            refTestDirName = os.path.join(self.reference_traces, refTestName)

        if not os.path.exists(self.reference_traces):
            print"No reference trace repository"
            return 1

        if generate:
            if not os.path.exists(refTestDirName):
                print "creating new " + refTestDirName
                os.mkdir(refTestDirName)

            if pyscript is None:
                Options.options.cwd_launch = refTestDirName
                tmpl = "%s"
                for arg in arguments:
                    tmpl = tmpl + " " + arg
                wutils.run_program(testName, tmpl)
            else:
                argv = [self.env['PYTHON'], os.path.join(Options.cwd_launch, *os.path.split(pyscript))] + arguments
                before = os.getcwd()
                os.chdir(refTestDirName)
                try:
                    wutils.run_argv(argv)
                finally:
                    os.chdir(before)
            print "Remember to commit " + refTestDirName
            return 0
        else:
            if not os.path.exists(refTestDirName):
                print "Cannot locate reference traces in " + refTestDirName
                return 1

            
            if refTestName is None:
                traceDirName = testName + ".ref"
            else:
                traceDirName = refTestName
            traceDirName = os.path.join ('traces', traceDirName)

            try:
                shutil.rmtree(traceDirName)
            except OSError:
                pass
            os.mkdir(traceDirName)

            #os.system("./waf --cwd regression/traces --run " +
            #  testName + " > /dev/null 2>&1")

            if pyscript is None:
                Options.options.cwd_launch = traceDirName
                wutils.run_program(testName, command_template=wutils.get_command_template(*arguments))
            else:
                argv = [self.env['PYTHON'], os.path.join('..', '..', '..', *os.path.split(pyscript))] + arguments
                before = os.getcwd()
                os.chdir(traceDirName)
                try:
                    wutils.run_argv(argv)
                finally:
                    os.chdir(before)

            if verbose:
                #diffCmd = "diff traces " + refTestDirName + " | head"
                diffCmd = subprocess.Popen([self.env['DIFF'], traceDirName, refTestDirName],
                                           stdout=subprocess.PIPE)
                headCmd = subprocess.Popen("head", stdin=diffCmd.stdout)
                rc2 = headCmd.wait()
                diffCmd.stdout.close()
                rc1 = diffCmd.wait()
                rc = rc1 or rc2
            else:
                rc = subprocess.Popen([self.env['DIFF'], traceDirName, refTestDirName], stdout=dev_null()).wait()
            if rc:
                print "----------"
                print "Traces differ in test: test-" + testName
                print "Reference traces in directory: regression/" + refTestDirName
                print "Traces in directory: traces"
                print "Rerun regression test as: " + \
                    "\"./waf --regression --regression-tests=test-" + testName + "\""
                print "Then do \"diff -u regression/" + refTestDirName + " regression/" + traceDirName +\
                    "\" for details"
                print "----------"
            return rc

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

def run_regression(reference_traces):
    """Execute regression tests.  Called with cwd set to the 'regression' subdir of ns-3.

    @param reference_traces: reference traces directory.

    """

    testdir = "tests"
    if not os.path.exists(testdir):
        print "Tests directory does not exist"
        sys.exit(3)

    sys.path.append(testdir)
    sys.modules['tracediff'] = Regression(testdir, reference_traces)

    if Options.options.regression_tests:
        tests = Options.options.regression_tests.split(',')
    else:
        tests = _find_tests(testdir)

    print "========== Running Regression Tests =========="
    env = Build.bld.env

    if not os.path.exists(reference_traces):
        print "Reference traces directory (%s) does not exist" % reference_traces
        return 3
    
    bad = []

    for test in tests:
        try:
            result = _run_regression_test(test)
            if result == 0:
                if Options.options.regression_generate:
                    print "GENERATE " + test
                else:
                    print "PASS " + test
            else:
                bad.append(test)
                print "FAIL " + test
        except NotImplementedError:
            print "SKIP " + test            

    return len(bad) > 0


def _run_regression_test(test):
    """Run a single test.

    Arguments:
    test -- the name of the test
    """

    if os.path.exists("traces"):
        files = os.listdir("traces")
        for file in files:
            if file == '.' or file == '..':
                continue
            shutil.rmtree(os.path.join("traces", file), ignore_errors=True)
    else:
        os.mkdir("traces")
    
    mod = __import__(test, globals(), locals(), [])
    return mod.run(verbose=(Options.options.verbose > 0),
                   generate=Options.options.regression_generate)

