#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import time
import gym
import os
import numpy as np
from ns3gym import ns3env
import matplotlib.pyplot as plt

# Environment initialization
port = 5551
simTime = 10 # seconds
startSim = True
stepTime = 0.1 # seconds
seed = 0
simArgs = {"--simTime": simTime,
           "--testArg": 123,
           "--nodeNum": 5,
           "--distance": 500}
debug = False

env = ns3env.Ns3Env(port=port, stepTime=stepTime, startSim=startSim, simSeed=seed, simArgs=simArgs, debug=debug)

rewards = []
iterations = []
episodes = 10

def get_no_op_action(obs):
    # cwValue 0 is not applied, so no_op
    action = np.zeros(shape=len(obs), dtype=np.uint32)
    return action

# Episodes
for episode in range(episodes):
    # Refresh state
    state = env.reset()
    done = False
    t_reward = 0

    i = 0
    # Run episode
    while True:
        if done:
            break

        i += 1
        action = get_no_op_action(state)
        state, reward, done, info = env.step(action)
        t_reward += reward

    print(episode, " Total reward:", t_reward)
    if t_reward:
        rewards.append(t_reward)
        iterations.append(i)

# Close environment
env.close()

# Plot results
def chunks_func(l, n):
    n = max(1, n)
    return (l[i:i+n] for i in xrange(0, len(l), n))

size = episodes
#chunks = list(chunk_list(rewards, size))
rewards = np.array(rewards)
print("mean value: ", np.mean(rewards), " std:", np.std(rewards),)
#chunks = np.array_split(rewards, size)
#chunks = chunks_func(rewards, size)
#averages = [sum(chunk) / len(chunk) for chunk in chunks]


plt.plot(rewards)
plt.xlabel('Episode')
plt.ylabel('Average Reward')
plt.show()