import zmq
import sys

import gym
from gym import spaces
from gym.utils import seeding
from enum import IntEnum


__author__ = "Piotr Gawlowicz"
__copyright__ = "Copyright (c) 2018, Technische Universität Berlin"
__version__ = "0.1.0"
__email__ = "gawlowicz@tkn.tu-berlin.de"


class Ns3ZmqBridge(object):
    """docstring for Ns3ZmqBridge"""
    def __init__(self, port="5555"):
        super(Ns3ZmqBridge, self).__init__()
        self.port = port
        context = zmq.Context()
        self.socket = context.socket(zmq.REQ)
        self.socket.connect ("tcp://localhost:%s" % port)

    def send_init_request(self, stepInterval):
        # print ("Sending INIT request ")
        msgType = "0|" + str(stepInterval)
        self.socket.send_string(msgType)
        message = self.socket.recv_string()
        # print ("Received reply: ", message)
        [errCode, value] = message.split("|")
        errCode = int(errCode)
        value = int(value)
        return [errCode, value]

    def send_get_action_space_request(self):
        # print ("Sending INIT request ")
        msgType = "1|0"
        self.socket.send_string(msgType)
        message = self.socket.recv_string()
        # print ("Received reply: ", message)
        [errCode, value] = message.split("|")
        errCode = int(errCode)
        return [errCode, value]

    def send_is_game_over_request(self):
        # print ("Sending IsGameOver request ")
        msgType = "2|0"
        self.socket.send_string(msgType)
        message = self.socket.recv_string()
        # print ("Received reply: ", message)
        [errCode, value] = message.split("|")
        errCode = int(errCode)
        value = int(value)
        return [errCode, value]

    def send_get_state_request(self):
        #print ("Sending GetState request ")
        msgType = "3|0"
        self.socket.send_string(msgType)
        message = self.socket.recv_string()
        #print ("Received reply: ", message)
        [errCode, value] = message.split("|")
        errCode = int(errCode)
        #value = int(value)
        return [errCode, value]

    def send_get_reward_request(self):
        # print ("Sending GetReward request ")
        msgType = "4|0"
        self.socket.send_string(msgType)
        message = self.socket.recv_string()
        # print ("Received reply: ", message)
        [errCode, value] = message.split("|")
        errCode = int(errCode)
        #value = int(value)
        return [errCode, value]

    def send_execute_action_request(self, actions):
        #print ("Sending ExecuteAction request ")
        msgType = "5|" + actions
        self.socket.send_string(msgType)
        message = self.socket.recv_string()
        #print ("Received reply: ", message)
        [errCode, value] = message.split("|")
        errCode = int(errCode)
        #value = int(value)
        return [errCode, value]


class Ns3Env(gym.Env):
    def __init__(self, stepTime):
        self.stepTime = stepTime
        self.port = "5555"

        # TODO: start ns3 script from here
        self.ns3ZmqBridge = Ns3ZmqBridge(self.port)
        self.ns3ZmqBridge.send_init_request(stepTime)
        [errCode, actionSpace] = self.ns3ZmqBridge.send_get_action_space_request()
        # TODO: get actions and observation space from ns3

        self.actions = ['0,0,2,3,3', '1,2,3,4,5', '0,1,3,2,1']
        self.action_size = len(self.actions)
        self.action_space = spaces.Discrete(self.action_size)
        self.observation_space = spaces.Discrete(self.action_size)

        self._seed()
        self.viewer = None
        self.state = None
        self.steps_beyond_done = None

    def _seed(self, seed=None):
        self.np_random, seed = seeding.np_random(seed)
        return [seed]

    def _get_obs(self):
        [errCode, done] = self.ns3ZmqBridge.send_is_game_over_request()
        [errCode, obs] = self.ns3ZmqBridge.send_get_state_request()
        [errCode, reward] = self.ns3ZmqBridge.send_get_reward_request()
        return (obs, reward, done, {})

    def step(self, action):
        action = self.actions[action]
        [errCode, response] = self.ns3ZmqBridge.send_execute_action_request(action)
        return self._get_obs()

    def reset(self):
        # TODO: add reset function
        return self._get_obs()

    def render(self, mode='human'):
        return

    def get_random_action(self):
        act = self.np_random.randint(self.action_size)
        return act

    def close(self):
        if self.viewer:
            self.viewer.close()