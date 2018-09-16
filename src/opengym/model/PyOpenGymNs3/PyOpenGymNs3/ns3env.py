import os
import sys
import zmq
import time

import numpy as np

import gym
from gym import spaces
from gym.utils import seeding
from enum import IntEnum

from PyOpenGymNs3.start_sim import start_sim_script

import PyOpenGymNs3.messages_pb2 as pb
from google.protobuf.any_pb2 import Any


__author__ = "Piotr Gawlowicz"
__copyright__ = "Copyright (c) 2018, Technische Universität Berlin"
__version__ = "0.1.0"
__email__ = "gawlowicz@tkn.tu-berlin.de"


class Ns3ZmqBridge(object):
    """docstring for Ns3ZmqBridge"""
    def __init__(self, port=5555, startSim=True, simTime=0, simSeed=-1, simArgs={}, debug=False):
        super(Ns3ZmqBridge, self).__init__()
        self.port = port
        self.startSim = startSim
        self.simTime = simTime
        self.simSeed = simSeed
        self.simArgs = simArgs
        self.envStopped = False
        self.simPid = None
        self.wafPid = None
        self.ns3Process = None

        if (startSim == True and simSeed == -1):
            maxSeed = np.iinfo(np.uint32).max
            simSeed = np.random.randint(0, maxSeed)
            self.simSeed = simSeed

        if self.startSim:
            self.ns3Process = start_sim_script(port, simTime, simSeed, simArgs, debug)
        else:
            print("Waiting for simulation script to connect")

        time.sleep(1)

        context = zmq.Context()
        self.socket = context.socket(zmq.REQ)
        self.socket.connect ("tcp://localhost:%s" % str(port))

        self._action_space = None
        self._observation_space = None

    def close(self):
        try:
            if not self.envStopped:
                self.envStopped = True
                self.send_stop_env_request()
                self.ns3Process.kill()
                if self.simPid:
                    os.kill(self.simPid, signal.SIGTERM)
                    self.simPid = None
                if self.wafPid:
                    os.kill(self.wafPid, signal.SIGTERM)
                    self.wafPid = None
        except Exception as e:
            pass

    def send_init_request(self, stepInterval):
        # print ("Sending INIT request ")
        msg = pb.InitializeRequest()
        msg.timeStep = stepInterval

        requestMsg = pb.RequestMsg()
        requestMsg.type = pb.Init
        requestMsg.msg.Pack(msg)
        requestMsg = requestMsg.SerializeToString()

        self.socket.send(requestMsg)
        reply = self.socket.recv()
        replyPbMsg = pb.ReplyMsg()
        initReplyPbMsg = pb.InitializeReply()
        replyPbMsg.ParseFromString(reply)
        replyPbMsg.msg.Unpack(initReplyPbMsg)
        errCode = 0
        value = int(initReplyPbMsg.done)
        self.simPid = int(initReplyPbMsg.simProcessId)
        self.wafPid = int(initReplyPbMsg.wafShellProcessId)
        return [errCode, value]

    def initialize_env(self, stepInterval):
        [errCode, value] = self.send_init_request(stepInterval)
        return value

    def _create_space(self, spaceReplyPb):
        space = None
        if (spaceReplyPb.type == pb.Discrete):
            discreteSpacePb = pb.DiscreteSpace()
            spaceReplyPb.space.Unpack(discreteSpacePb)
            space = spaces.Discrete(discreteSpacePb.n)

        elif (spaceReplyPb.type == pb.Box):
            boxSpacePb = pb.BoxSpace()
            spaceReplyPb.space.Unpack(boxSpacePb)
            low = boxSpacePb.low
            high = boxSpacePb.high
            shape = tuple(boxSpacePb.shape)
            mtype = boxSpacePb.dtype

            if mtype == pb.INT:
                mtype = np.int
            elif mtype == pb.UINT:
                mtype = np.uint
            elif mtype == pb.DOUBLE:
                mtype = np.float
            else:
                mtype = np.float

            space = spaces.Box(low=low, high=high, shape=shape, dtype=mtype)
        return space

    def send_get_action_space_request(self):
        msg = pb.GetActionSpaceRequest()
        requestMsg = pb.RequestMsg()
        requestMsg.type = pb.ActionSpace
        requestMsg.msg.Pack(msg)
        requestMsg = requestMsg.SerializeToString()
        self.socket.send(requestMsg)

        reply = self.socket.recv()
        replyPbMsg = pb.ReplyMsg()
        actionApaceReplyPbMsg = pb.GetSpaceReply()
        replyPbMsg.ParseFromString(reply)
        replyPbMsg.msg.Unpack(actionApaceReplyPbMsg)
        return actionApaceReplyPbMsg

    def get_action_space(self):
        spaceReplyPb = self.send_get_action_space_request()
        actionSpace = self._create_space(spaceReplyPb)
        self._action_space = actionSpace
        return actionSpace

    def send_get_obs_space_request(self):
        msg = pb.GetObservationSpaceRequest()
        requestMsg = pb.RequestMsg()
        requestMsg.type = pb.ObservationSpace
        requestMsg.msg.Pack(msg)
        requestMsg = requestMsg.SerializeToString()
        self.socket.send(requestMsg)

        reply = self.socket.recv()
        replyPbMsg = pb.ReplyMsg()
        obsApaceReplyPbMsg = pb.GetSpaceReply()
        replyPbMsg.ParseFromString(reply)
        replyPbMsg.msg.Unpack(obsApaceReplyPbMsg)
        return obsApaceReplyPbMsg

    def get_observation_space(self):
        spaceReplyPb = self.send_get_obs_space_request()
        obsSpace = self._create_space(spaceReplyPb)
        self._observation_space = obsSpace
        return obsSpace

    def send_stop_env_request(self, timeout=-1):
        msg = pb.StopEnvRequest()
        requestMsg = pb.RequestMsg()
        requestMsg.type = pb.StopEnv
        requestMsg.msg.Pack(msg)
        requestMsg = requestMsg.SerializeToString()
        self.socket.send(requestMsg)

        reply = self.socket.recv()
        replyPbMsg = pb.ReplyMsg()
        innerReplyPbMsg = pb.StopEnvReply()
        replyPbMsg.ParseFromString(reply)
        replyPbMsg.msg.Unpack(innerReplyPbMsg)
        return innerReplyPbMsg.done

    def send_is_game_over_request(self):
        msg = pb.GetIsGameOverRequest()
        requestMsg = pb.RequestMsg()
        requestMsg.type = pb.IsGameOver
        requestMsg.msg.Pack(msg)
        requestMsg = requestMsg.SerializeToString()
        self.socket.send(requestMsg)

        reply = self.socket.recv()
        replyPbMsg = pb.ReplyMsg()
        innerReplyPbMsg = pb.GetIsGameOverReply()
        replyPbMsg.ParseFromString(reply)
        replyPbMsg.msg.Unpack(innerReplyPbMsg)
        return innerReplyPbMsg

    def is_game_over(self):
        msg = self.send_is_game_over_request()

        if msg.isGameOver:
            self.envStopped = True
            if (msg.reason == pb.GetIsGameOverReply.GameOver):
                done = self.send_stop_env_request()

        return msg.isGameOver

    def send_get_state_request(self):
        msg = pb.GetObservationRequest()
        requestMsg = pb.RequestMsg()
        requestMsg.type = pb.Observation
        requestMsg.msg.Pack(msg)
        requestMsg = requestMsg.SerializeToString()
        self.socket.send(requestMsg)

        reply = self.socket.recv()
        replyPbMsg = pb.ReplyMsg()
        innerReplyPbMsg = pb.GetObservationReply()
        replyPbMsg.ParseFromString(reply)
        replyPbMsg.msg.Unpack(innerReplyPbMsg)
        return innerReplyPbMsg

    def get_obs(self):
        obsMsg = self.send_get_state_request()
        dataContainer = obsMsg.container

        data = None
        if (dataContainer.type == pb.Box):
            boxContainerPb = pb.BoxDataContainer()
            dataContainer.data.Unpack(boxContainerPb)
            # print(boxContainerPb.shape, boxContainerPb.dtype, boxContainerPb.uintData)

            if boxContainerPb.dtype == pb.INT:
                data = boxContainerPb.intData
            elif boxContainerPb.dtype == pb.UINT:
                data = boxContainerPb.uintData
            elif boxContainerPb.dtype == pb.DOUBLE:
                data = boxContainerPb.doubleData
            else:
                data = boxContainerPb.floatData

            # TODO: reshape using shape info

        return data

    def send_get_reward_request(self):
        msg = pb.GetRewardRequest()
        requestMsg = pb.RequestMsg()
        requestMsg.type = pb.Reward
        requestMsg.msg.Pack(msg)
        requestMsg = requestMsg.SerializeToString()
        self.socket.send(requestMsg)

        reply = self.socket.recv()
        replyPbMsg = pb.ReplyMsg()
        innerReplyPbMsg = pb.GetRewardReply()
        replyPbMsg.ParseFromString(reply)
        replyPbMsg.msg.Unpack(innerReplyPbMsg)
        return innerReplyPbMsg

    def get_reward(self):
        rewardMsg = self.send_get_reward_request()
        return rewardMsg.reward

    def send_execute_action_request(self, actions):
        dataContainer = pb.DataContainer()
        dataContainer.type = pb.Box

        boxContainerPb = pb.BoxDataContainer()
        shape = [len(actions)]
        boxContainerPb.shape.extend(shape)

        if (self._action_space.dtype in ['int', 'int8', 'int16', 'int32', 'int64']):
            boxContainerPb.dtype = pb.INT
            boxContainerPb.intData.extend(actions)

        elif (self._action_space.dtype in ['uint', 'uint8', 'uint16', 'uint32', 'uint64']):
            boxContainerPb.dtype = pb.UINT
            boxContainerPb.uintData.extend(actions)

        elif (self._action_space.dtype in ['float', 'float32', 'float64']):
            boxContainerPb.dtype = pb.FLOAT
            boxContainerPb.floatData.extend(actions)

        elif (self._action_space.dtype in ['double']):
            boxContainerPb.dtype = pb.DOUBLE
            boxContainerPb.doubleData.extend(actions)

        else:
            boxContainerPb.dtype = pb.FLOAT
            boxContainerPb.floatData.extend(actions)

        dataContainer.data.Pack(boxContainerPb)

        msg = pb.SetActionRequest()
        msg.container.CopyFrom(dataContainer)

        requestMsg = pb.RequestMsg()
        requestMsg.type = pb.Action
        requestMsg.msg.Pack(msg)
        requestMsg = requestMsg.SerializeToString()
        self.socket.send(requestMsg)

        reply = self.socket.recv()
        replyPbMsg = pb.ReplyMsg()
        innerReplyPbMsg = pb.SetActionReply()
        replyPbMsg.ParseFromString(reply)
        replyPbMsg.msg.Unpack(innerReplyPbMsg)
        return innerReplyPbMsg

    def execute_action(self, actions):
        replyMsg = self.send_execute_action_request(actions)
        return replyMsg.done


class Ns3Env(gym.Env):
    def __init__(self, stepTime=0, port=5555, startSim=True, simTime=0, simSeed=-1, simArgs={}, debug=True):
        self.stepTime = stepTime
        self.port = port
        self.startSim = startSim
        self.simTime = simTime
        self.simSeed = simSeed
        self.simArgs = simArgs
        self.debug = debug

        # Filled in reset function
        self.ns3ZmqBridge = None
        self.action_space = None
        self.observation_space = None

        self._seed()
        self.viewer = None
        self.state = None
        self.steps_beyond_done = None

    def _seed(self, seed=None):
        self.np_random, seed = seeding.np_random(seed)
        return [seed]

    def _get_obs(self):
        obs = self.ns3ZmqBridge.get_obs()
        reward = self.ns3ZmqBridge.get_reward()
        done = self.ns3ZmqBridge.is_game_over()
        return (obs, reward, done, {})

    def step(self, action):
        response = self.ns3ZmqBridge.execute_action(action)
        return self._get_obs()

    def reset(self):
        self.ns3ZmqBridge = Ns3ZmqBridge(self.port, self.startSim, self.simTime, self.simSeed, self.simArgs, self.debug)
        self.ns3ZmqBridge.initialize_env(self.stepTime)
        self.action_space = self.ns3ZmqBridge.get_action_space()
        self.observation_space = self.ns3ZmqBridge.get_observation_space()
        return self._get_obs()

    def render(self, mode='human'):
        return

    def get_random_action(self):
        act = self.action_space.sample()
        return act

    def close(self):
        if self.ns3ZmqBridge:
            self.ns3ZmqBridge.close()
            self.ns3ZmqBridge = None

        if self.viewer:
            self.viewer.close()