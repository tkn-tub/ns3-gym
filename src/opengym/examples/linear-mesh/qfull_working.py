#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import time
import gym
import os
import numpy as np
from ns3gym import ns3env
import matplotlib.pyplot as plt

# Environment initialization
port = 5553
simTime = 10 # seconds
startSim = True
stepTime = 0.1 # seconds
seed = 122
simArgs = {"--simTime": simTime,
           "--testArg": 123,
           "--nodeNum": 5,
           "--distance": 500}
debug = False

env = ns3env.Ns3Env(port=port, stepTime=stepTime, startSim=startSim, simSeed=seed, simArgs=simArgs, debug=debug)

# Q and rewards
Q0 = np.zeros(shape=(5, 5, 5, 4), dtype=np.float)
Q1 = np.zeros(shape=(5, 5, 5, 4), dtype=np.float)
Q2 = np.zeros(shape=(5, 5, 5, 4), dtype=np.float)
Q3 = np.zeros(shape=(5, 5, 5, 4), dtype=np.float)

rewards = []
iterations = []

# Parameters
alpha = 0.75
discount = 0.95
episodes = 10

def calculate_q_diff(obs):
    obs = np.array(obs)
    diff = -np.diff(obs)
    diff[diff<0] = 0
    return diff

# Episodes
for episode in range(episodes):
    # Refresh state
    state = env.reset()
    state = np.uint(np.array(state, dtype=np.uint32) / 25)
    state = state[1:-1]
    done = False
    t_reward = 0

    i = 0
    # Run episode
    while True:
        if done:
            break

        i += 1
        current = state
        divider = episode
        [a, b, c] = current
        action0 = np.argmax(Q0[a,b,c, :] + np.random.randn(1, 4) * (1 / float(divider + 1)))
        action1 = np.argmax(Q1[a,b,c, :] + np.random.randn(1, 4) * (1 / float(divider + 1)))
        action2 = np.argmax(Q2[a,b,c, :] + np.random.randn(1, 4) * (1 / float(divider + 1)))
        action3 = np.argmax(Q3[a,b,c, :] + np.random.randn(1, 4) * (1 / float(divider + 1)))

        action0 = np.unravel_index(action0, Q0.shape)[-1]
        action1 = np.unravel_index(action1, Q1.shape)[-1]
        action2 = np.unravel_index(action2, Q2.shape)[-1]
        action3 = np.unravel_index(action3, Q3.shape)[-1]

        action = np.array([action0, action1, action2, action3, 0], dtype=np.uint)
        action = action * 25 + 5
        #print(action)

        state, reward, done, info = env.step(action)
        state = np.uint(np.array(state, dtype=np.uint32) / 25)
        #state = calculate_q_diff(state)
        state = state[1:-1]

        t_reward += reward
        [x,y,z] = state
        Q0[a,b,c, action0] += alpha * (reward + discount * np.max(Q0[x,y,z, :]) - Q0[a,b,c, action0])
        Q1[a,b,c, action1] += alpha * (reward + discount * np.max(Q1[x,y,z, :]) - Q1[a,b,c, action1])
        Q2[a,b,c, action2] += alpha * (reward + discount * np.max(Q2[x,y,z, :]) - Q2[a,b,c, action2])
        Q3[a,b,c, action3] += alpha * (reward + discount * np.max(Q3[x,y,z, :]) - Q3[a,b,c, action3])


    print("Total reward:", t_reward)
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
chunks = np.array_split(rewards, size)
#chunks = chunks_func(rewards, size)
averages = [sum(chunk) / len(chunk) for chunk in chunks]

plt.plot(averages)
plt.xlabel('Episode')
plt.ylabel('Average Reward')
plt.show()

print("Q0",Q0)
print("Q1",Q1)
print("Q2",Q2)
print("Q3",Q3)