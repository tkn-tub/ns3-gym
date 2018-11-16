#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import time
import gym
import os
import numpy as np
from ns3gym import ns3env
import matplotlib.pyplot as plt

# Environment initialization
port = 5556
simTime = 10 # seconds
startSim = True
stepTime = 0.005  # seconds
seed = 0
simArgs = {"--simTime": simTime,
           "--testArg": 123,
           "--distance": 20}
debug = False

env = ns3env.Ns3Env(port=port, stepTime=stepTime, startSim=startSim, simSeed=seed, simArgs=simArgs, debug=debug)

# Q and rewards
Q = np.zeros(shape=(5, 101, 100), dtype=np.float)
action = np.zeros(shape=(5), dtype=np.uint)

rewards = []
iterations = []

# Parameters
alpha = 0.75
discount = 0.95
episodes = 1000

def calculate_q_diff(obs):
    obs = np.array(obs)
    diff = -np.diff(obs)
    obs[:-1] = diff
    obs[-1] = 0
    obs[obs<0] = 0
    return obs

# Episodes
for episode in range(episodes):
    # Refresh state
    state = env.reset()

    #state = calculate_q_diff(state)
    done = False
    t_reward = 0

    i = 0
    # Run episode
    while True:
        if done:
            break

        i += 1
        current = state
        for n in range(5):
            action[n] = np.argmax(Q[n, current[n], :] + np.random.randn(1, 100) * (1 / float(episode + 1)))

        state, reward, done, info = env.step(action)
        #state = calculate_q_diff(state)
        #print('state:', state, reward, done)
        #print("action:", action)

        t_reward += reward
        for n in range(5):
            Q[n, current[n], action[n]] += alpha * (reward + discount * np.max(Q[n, state[n], :]) - Q[n, current[n], action[n]])

    print("Total reward:", t_reward)
    rewards.append(t_reward)
    iterations.append(i)

# Close environment
env.close()

# Plot results
def chunks_func(l, n):
    n = max(1, n)
    return (l[i:i+n] for i in xrange(0, len(l), n))

size = 5
#chunks = list(chunk_list(rewards, size))
rewards = np.array(rewards)
chunks = np.array_split(rewards, size)
#chunks = chunks_func(rewards, size)
averages = [sum(chunk) / len(chunk) for chunk in chunks]

plt.plot(averages)
plt.xlabel('Episode')
plt.ylabel('Average Reward')
plt.show()

print(Q)