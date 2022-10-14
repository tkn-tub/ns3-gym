#!/usr/bin/python
import sys
import os
import subprocess


def find_ns3_path(cwd):
	"""
	Find the executable ns3 for building and running ns3 scenarios
	"""
	ns3_path = cwd

	found = False
	my_dir = cwd
	while (not found):
		for fname in os.listdir(my_dir):
			if fname == "ns3":
				found = True
				ns3_path = os.path.join(my_dir, fname)
				break

		my_dir = os.path.dirname(my_dir)

		if str(my_dir) == "/": #root folder
			print("ns3 file not found. Quitting...")
			sys.exit()
	return ns3_path


def build_ns3_project(debug=True):
	"""
	Actually build the ns3 scenario before running.
	"""
	cwd = os.getcwd()
	sim_script_name = os.path.basename(cwd)
	ns3_path = find_ns3_path(cwd)
	base_ns3_dir = os.path.dirname(ns3_path)

	os.chdir(base_ns3_dir)

	ns3_string = ns3_path + ' build'

	output = subprocess.DEVNULL
	if debug:
		output = None

	build_required = False
	ns3_proc = subprocess.Popen(ns3_string, shell=True, stdout=subprocess.PIPE, stderr=None, universal_newlines=True)

	line_history = []
	for line in ns3_proc.stdout:
		if (True or "Compiling" in line or "Linking" in line) and not build_required:
			build_required = True
			print("Build ns-3 project if required")
			for l in line_history:
				sys.stdout.write(l)
				line_history = []

		if build_required:
			sys.stdout.write(line)
		else:
			line_history.append(line)

	p_status = ns3_proc.wait()
	if build_required:
		print("(Re-)Build of ns-3 finished with status: ", p_status)
	os.chdir(cwd)


def start_sim_script(port=5555, sim_seed=0, sim_args={}, debug=False):
	"""
	Actually run the ns3 scenario
	"""
	cwd = os.getcwd()
	sim_script_name = os.path.basename(cwd)
	ns3_path = find_ns3_path(cwd)
	base_ns3_dir = os.path.dirname(ns3_path)

	os.chdir(base_ns3_dir)

	ns3_string = ns3_path + ' run "' + sim_script_name

	if port:
		ns3_string += ' --openGymPort=' + str(port)

	if sim_seed:
		ns3_string += ' --simSeed=' + str(sim_seed)

	for key, value in sim_args.items():
		ns3_string += " "
		ns3_string += str(key)
		ns3_string += "="
		ns3_string += str(value)

	ns3_string += '"'

	debug = True
	ns3_proc = None
	if debug:
		ns3_proc = subprocess.Popen(ns3_string, shell=True, stdout=None, stderr=None)
	else:
		# users were complaining that when they start example they have to wait 10 min for initialization.
		# simply ns3 is being built during this time, so now the output of the build will be put to stdout
		# but sometimes build is not required and I would like to avoid unnecessary output on the screen
		# it is not easy to get tell before start ./waf whether the build is required or not
		# here, I use simple trick, i.e. if output of build contains {"Compiling","Linking"}
		# then the build is required and, hence, i put the output to the stdout
		error_output = subprocess.DEVNULL
		print(ns3_string)
		ns3_proc = subprocess.Popen(ns3_string, shell=True, stdout=subprocess.PIPE, stderr=error_output, universal_newlines=True)

		build_required = False
		line_history = []
		for line in ns3_proc.stdout:
			print(line)
			if ("Compiling" in line or "Linking" in line) and not build_required:
				build_required = True
				print("Build ns-3 project if required")
				for subline in line_history:
					sys.stdout.write(subline)
					line_history = []

			if build_required:
				sys.stdout.write(line)
			else:
				line_history.append(line)

			if "Waf: Leaving directory" in line:
				break

	if debug:
		print("Start command: ", ns3_string)
		print("Started ns3 simulation script, Process Id: ", ns3_proc.pid)

	# go back to my dir
	os.chdir(cwd)
	return ns3_proc
