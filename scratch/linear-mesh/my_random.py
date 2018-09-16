import argparse
from PyOpenGymNs3 import ns3env

__author__ = "Piotr Gawlowicz"
__copyright__ = "Copyright (c) 2018, Technische Universität Berlin"
__version__ = "0.1.0"
__email__ = "gawlowicz@tkn.tu-berlin.de"


parser = argparse.ArgumentParser(description='Start simulation script on/off')
parser.add_argument('--start',
                    type=int,
                    default=1,
                    help='Start simulation script 0/1, Default: 1')
parser.add_argument('--iterations',
                    type=int,
                    default=2,
                    help='Number of iterations, Default: 2')
args = parser.parse_args()
startSim = bool(args.start)
iterationNum = int(args.iterations)

port = 5555
simTime = 20 # seconds
stepTime = 0.01  # seconds
seed = 0
simArgs = {"--testArg": 123}
debug = False

env = ns3env.Ns3Env(port=port, stepTime=stepTime, startSim=startSim, simTime=simTime, simSeed=seed, simArgs=simArgs, debug=debug)
env.reset()

ob_space = env.observation_space
ac_space = env.action_space
print("Observation space: ", ob_space,  ob_space.dtype)
print("Action space: ", ac_space, ac_space.dtype)

stepIdx = 0
currIt = 0
allRxPkts = 0

try:
    while True:
        print("Start iteration: ", currIt)
        obs, reward, done, info = env.reset()
        print("Step: ", stepIdx)
        print("---obs, reward, done, info: ", obs, reward, done, info)

        while True:
            stepIdx += 1

            allRxPkts += reward
            action = env.action_space.sample()
            print("---action: ", action)

            obs, reward, done, info = env.step(action)
            print("Step: ", stepIdx)
            print("---obs, reward, done, info: ", obs, reward, done, info)

            if done:
                stepIdx = 0
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
    print("All rx pkts num: ", allRxPkts)
    print("Done")