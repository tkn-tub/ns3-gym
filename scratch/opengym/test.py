import ns3env

__author__ = "Piotr Gawlowicz"
__copyright__ = "Copyright (c) 2018, Technische Universität Berlin"
__version__ = "0.1.0"
__email__ = "gawlowicz@tkn.tu-berlin.de"


# TODO: integrate with openGym lib, like:
# import gym
# env = gym.make('ns3-linear-wireless-mesh')
# env.reset()
# env.render()

stepTime = 0.5  # seconds
env = ns3env.Ns3Env(stepTime=stepTime)
env.reset()

ob_space = env.observation_space
ac_space = env.action_space
print("Observation space: ", ob_space)
print("Action space: ", ac_space)

stepIdx = 0

try:
    while True:
        stepIdx += 1
        print("Step: ", stepIdx)

        action = env.get_random_action()
        print("---action: ", action)
        obs, reward, done, extra = env.step(action)
        print("---obs, reward, done, extra: ", obs, reward, done, extra)

        if done:
            break

except KeyboardInterrupt:
    print("Ctrl-C -> Exit")
finally:
    pass

print("Done")