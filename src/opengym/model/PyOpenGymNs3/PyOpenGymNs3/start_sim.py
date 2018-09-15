#!/usr/bin/python
import os
import time
import subprocess


def find_waf_path(cwd):
	wafPath = cwd

	found = False
	myDir = cwd
	while (not found):
		for fname in os.listdir(myDir):
			if fname == "waf":
				found = True
				wafPath = os.path.join(myDir, fname)
				break

		myDir = os.path.dirname(myDir)

	return wafPath


def start_sim_script(port=5555, simTime=0, simSeed=0, simArgs={}):
	cwd = os.getcwd()
	simScriptName = os.path.basename(cwd)
	wafPath = find_waf_path(cwd)

	wafString = wafPath + ' --run "' + simScriptName

	if port:
		wafString += ' --openGymPort=' + str(port)

	if simTime:
		wafString += ' --simTime=' + str(simTime)

	if simSeed:
		wafString += ' --simSeed=' + str(simSeed)

	for k,v in simArgs.items():
		wafString += " "
		wafString += str(k)
		wafString += "="
		wafString += str(v)

	wafString += '"'

	ns3Proc = subprocess.Popen(wafString, shell=True)

	print(wafString)
	print("Started ns3 simulation script, Process Id: ", ns3Proc.pid)
	return ns3Proc