#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import argparse
import time
import numpy as np
from ns3gym import ns3env

__author__ = "Piotr Gawlowicz"
__copyright__ = "Copyright (c) 2018, Technische Universität Berlin"
__version__ = "0.1.0"
__email__ = "gawlowicz@tkn.tu-berlin.de"


parser = argparse.ArgumentParser(description='Start simulation script on/off')
parser.add_argument('--start',
                    type=int,
                    default=0,
                    help='Start simulation script 0/1, Default: 0')
parser.add_argument('--iterations',
                    type=int,
                    default=1,
                    help='Number of iterations, Default: 1')
args = parser.parse_args()
startSim = bool(args.start)
iterationNum = int(args.iterations)

port = 5555
simTime = 10 # seconds
stepTime = 0.01  # seconds
seed = 0
simArgs = {"--simTime": simTime,
           "--testArg": 123,
           "--distance": 500}
debug = False

env = ns3env.Ns3Env(port=port, stepTime=stepTime, startSim=startSim, simSeed=seed, simArgs=simArgs, debug=debug)
env.reset()

ob_space = env.observation_space
ac_space = env.action_space
print("Observation space: ", ob_space,  ob_space.dtype)
print("Action space: ", ac_space, ac_space.dtype)

stepIdx = 0
currIt = 0
allRxPkts = 0

def calculate_cw_window(obs):
    diff = -np.diff(obs)
    maxDiff = np.argmax(diff)

    maxCw = 50
    action = np.ones(shape=len(obs), dtype=np.uint32) * maxCw
    action[maxDiff] = 5
    return action

try:
    while True:
        obs = env.reset()
        reward = 0
        print("Start iteration: ", currIt)
        print("Step: ", stepIdx)
        print("---obs: ", obs)

        while True:
            stepIdx += 1

            allRxPkts += reward
            action = calculate_cw_window(obs)
            print("---action: ", action)

            obs, reward, done, info = env.step(action)
            print("Step: ", stepIdx)
            print("---obs, reward, done, info: ", obs, reward, done, info)

            if done:
                stepIdx = 0
                print("All rx pkts num: ", allRxPkts)
                allRxPkts = 0

                if currIt + 1 < iterationNum:
                    env.reset()
                break

        currIt += 1
        if currIt == iterationNum:
            break

except KeyboardInterrupt:
    print("Ctrl-C -> Exit")
finally:
    env.close()
    print("Done")