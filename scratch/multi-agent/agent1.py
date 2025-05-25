#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import argparse
from ns3gym import ns3env

__author__ = "Piotr Gawlowicz"
__copyright__ = "Copyright (c) 2020, Technische Universität Berlin"
__version__ = "0.1.0"
__email__ = "gawlowicz@tkn.tu-berlin.de"


port = 5555
env = ns3env.Ns3Env(port=port, startSim=False)
env.reset()

ob_space = env.observation_space
ac_space = env.action_space
print("Observation space: ", ob_space,  ob_space.dtype)
print("Action space: ", ac_space, ac_space.dtype)


stepIdx = 0
currIt = 0
iterationNum = 3

try:
    while True:
        obs = env.reset()
        print("Step: ", stepIdx)
        print("---obs: ", obs)

        while True:
            stepIdx += 1
            action = env.action_space.sample()
            print("---action: ", action)

            print("Step: ", stepIdx)
            obs, reward, done, info = env.step(action)
            print("---obs, reward, done, info: ", obs, reward, done, info)

            input("press enter....")

            if done:
                break

        currIt += 1
        if currIt == iterationNum:
            break


except KeyboardInterrupt:
    print("Ctrl-C -> Exit")
finally:
    env.close()
    print("Done")