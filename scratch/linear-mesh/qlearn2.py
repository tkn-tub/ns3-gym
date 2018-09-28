import gym
import numpy as np
#import gym_tic_tac_toe
import random
from pylab import *
import argparse
import time
import os

#A function to low pass filter a list, this is just to make the plots clearer at the end
#It serves no purpose with regard to solving the task.
def low_pass(data, alpha=0.99):
	low_pass = [data[0]]
	for i in range(1,len(data)):
		low_pass.append(alpha*low_pass[-1] + (1.0-alpha)*data[i] )
	return low_pass


parser = argparse.ArgumentParser(prog='qlearner')
parser.add_argument('--eight', action="store_true", help='Use 8x8 version of FrozenLake-v0')
parser.add_argument('-s', '--show', action="store_true", help="Show finished, trained attempt")
parser.add_argument('-e', '--episodes', action="store",default=2000,  help='Number of episodes used to train')
parser.add_argument('-ms', '--max_steps', action="store",default=100, type=int, help='Maximum number of steps in an episode')
parser.add_argument('-lr', '--learning_rate', action="store",default=0.8, type=float, help='Learning rate for Q learning')
parser.add_argument('-g', '--gamma', action="store", default=0.95, type=float, help='Gamma value for Bellman Equation')


args = parser.parse_args()
num_episodes = int(args.episodes)
max_steps = args.max_steps
learning_rate = args.learning_rate
gamma = args.gamma

#get the environment
env = None
if args.eight:
	env = gym.make('FrozenLake8x8-v0')
else:
	env = gym.make('FrozenLake-v0')

Q = np.zeros([env.observation_space.n,env.action_space.n])


reward_list = []
steps_list = []
for i in range(num_episodes):
	state = env.reset()
	steps = 0
	rewardSum = 0.0

	if (i%1000) == 0:
		sys.stdout.write("Training progress: %d / %d   \r" % (i, num_episodes) )
		sys.stdout.flush()

	while steps < max_steps:
		#if i%100 == 0:
		#	env.render()
		steps += 1 
		action_list = Q[state,:]
		fuzzy_actions = action_list + np.random.randn(1,env.action_space.n)*(1./((i+1)))
		
		selected_action = np.argmax(fuzzy_actions)

		new_state, reward, done, _ = env.step(selected_action)

		Q[state,selected_action] = Q[state,selected_action] + learning_rate*(reward + gamma * np.max(Q[new_state,:]) - Q[state,selected_action])
		state = new_state
		rewardSum += reward
		if done:
			break
	reward_list.append(rewardSum)
	steps_list.append(steps)
	
done = False
state = env.reset()
if args.show:
	while not done:
		env.render()
		action_list = Q[state,:]
		selected_action = np.argmax(action_list)
		new_state,reward,done, _ = env.step(selected_action)
		state = new_state
		time.sleep(0.5)
		os.system('clear')


subplot(211)
plot(low_pass(reward_list), 'b', label="Total Reward per Episode")
plot([0.78]*len(reward_list), 'g', label="Pass Mark")
legend(loc="best")

subplot(212)
plot(low_pass(steps_list), label="Total Number of Steps per Episode")
legend(loc="best")
show()

