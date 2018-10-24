import os
import sys
import zmq
import time

import numpy as np

import gym
from gym import spaces
from gym.utils import seeding
from enum import IntEnum

from ns3gym.start_sim import start_sim_script, build_ns3_project

import ns3gym.messages_pb2 as pb
from google.protobuf.any_pb2 import Any


__author__ = "Piotr Gawlowicz"
__copyright__ = "Copyright (c) 2018, Technische Universität Berlin"
__version__ = "0.1.0"
__email__ = "gawlowicz@tkn.tu-berlin.de"


class Ns3ZmqBridge(object):
    """docstring for Ns3ZmqBridge"""
    def __init__(self, port=5555, startSim=True, simSeed=0, simArgs={}, debug=False):
        super(Ns3ZmqBridge, self).__init__()
        self.port = port
        self.startSim = startSim
        self.simSeed = simSeed
        self.simArgs = simArgs
        self.envStopped = False
        self.simPid = None
        self.wafPid = None
        self.ns3Process = None

        context = zmq.Context()
        self.socket = context.socket(zmq.REQ)
        try:
            if port == 0 and self.startSim:
                port = self.socket.bind_to_random_port('tcp://*', min_port=5001, max_port=10000, max_tries=100)
                print("Got new port for ns3gm interface: ", port)

            elif port == 0 and not self.startSim:
                print("Cannot use port %s to bind" % str(port) )
                print("Please specify correct port" )
                sys.exit()

            else:
                self.socket.bind ("tcp://*:%s" % str(port))

        except Exception as e:
            print("Cannot bind to tcp://*:%s as port is already in use" % str(port) )
            print("Please specify different port or use 0 to get free port" )
            sys.exit()

        if (startSim == True and simSeed == 0):
            maxSeed = np.iinfo(np.uint32).max
            simSeed = np.random.randint(0, maxSeed)
            self.simSeed = simSeed

        if self.startSim:
            # run simulation script
            self.ns3Process = start_sim_script(port, simSeed, simArgs, debug)
        else:
            print("Waiting for simulation script to connect on port: tcp://localhost:{}".format(port))

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

        if self.startSim:
            msg.simSeed = self.simSeed

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

    def _create_space(self, spaceDesc):
        space = None
        if (spaceDesc.type == pb.Discrete):
            discreteSpacePb = pb.DiscreteSpace()
            spaceDesc.space.Unpack(discreteSpacePb)
            space = spaces.Discrete(discreteSpacePb.n)

        elif (spaceDesc.type == pb.Box):
            boxSpacePb = pb.BoxSpace()
            spaceDesc.space.Unpack(boxSpacePb)
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

        elif (spaceDesc.type == pb.Tuple):
            mySpaceList = []
            tupleSpacePb = pb.TupleSpace()
            spaceDesc.space.Unpack(tupleSpacePb)

            for pbSubSpaceDesc in tupleSpacePb.element:
                subSpace = self._create_space(pbSubSpaceDesc)
                mySpaceList.append(subSpace)

            mySpaceTuple = tuple(mySpaceList)
            space = spaces.Tuple(mySpaceTuple)

        elif (spaceDesc.type == pb.Dict):
            mySpaceDict = {}
            dictSpacePb = pb.DictSpace()
            spaceDesc.space.Unpack(dictSpacePb)

            for pbSubSpaceDesc in dictSpacePb.element:
                subSpace = self._create_space(pbSubSpaceDesc)
                mySpaceDict[pbSubSpaceDesc.name] = subSpace

            space = spaces.Dict(mySpaceDict)

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
        actionSpace = self._create_space(spaceReplyPb.space)
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
        obsSpace = self._create_space(spaceReplyPb.space)
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

    def _create_data(self, dataContainerPb):
        if (dataContainerPb.type == pb.Discrete):
            discreteContainerPb = pb.DiscreteDataContainer()
            dataContainerPb.data.Unpack(discreteContainerPb)
            data = discreteContainerPb.data
            return data

        if (dataContainerPb.type == pb.Box):
            boxContainerPb = pb.BoxDataContainer()
            dataContainerPb.data.Unpack(boxContainerPb)
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

        elif (dataContainerPb.type == pb.Tuple):
            tupleDataPb = pb.TupleDataContainer()
            dataContainerPb.data.Unpack(tupleDataPb)

            myDataList = []
            for pbSubData in tupleDataPb.element:
                subData = self._create_data(pbSubData)
                myDataList.append(subData)

            data = tuple(myDataList)
            return data

        elif (dataContainerPb.type == pb.Dict):
            dictDataPb = pb.DictDataContainer()
            dataContainerPb.data.Unpack(dictDataPb)

            myDataDict = {}
            for pbSubData in dictDataPb.element:
                subData = self._create_data(pbSubData)
                myDataDict[pbSubData.name] = subData

            data = myDataDict
            return data

    def get_obs(self):
        obsMsg = self.send_get_state_request()
        data = self._create_data(obsMsg.container)
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

    def send_get_extra_info(self):
        msg = pb.GetExtraInfoRequest()
        requestMsg = pb.RequestMsg()
        requestMsg.type = pb.ExtraInfo
        requestMsg.msg.Pack(msg)
        requestMsg = requestMsg.SerializeToString()
        self.socket.send(requestMsg)

        reply = self.socket.recv()
        replyPbMsg = pb.ReplyMsg()
        innerReplyPbMsg = pb.GetExtraInfoReply()
        replyPbMsg.ParseFromString(reply)
        replyPbMsg.msg.Unpack(innerReplyPbMsg)
        return innerReplyPbMsg

    def get_extra_info(self):
        msg = self.send_get_extra_info()
        info = msg.info
        if not info:
            info = {}
        return info

    def _pack_data(self, actions, spaceDesc):
        dataContainer = pb.DataContainer()

        spaceType = spaceDesc.__class__

        if spaceType == spaces.Discrete:
            dataContainer.type = pb.Discrete
            discreteContainerPb = pb.DiscreteDataContainer()
            discreteContainerPb.data = actions
            dataContainer.data.Pack(discreteContainerPb)

        elif spaceType == spaces.Box:
            dataContainer.type = pb.Box
            boxContainerPb = pb.BoxDataContainer()
            shape = [len(actions)]
            boxContainerPb.shape.extend(shape)

            if (spaceDesc.dtype in ['int', 'int8', 'int16', 'int32', 'int64']):
                boxContainerPb.dtype = pb.INT
                boxContainerPb.intData.extend(actions)

            elif (spaceDesc.dtype in ['uint', 'uint8', 'uint16', 'uint32', 'uint64']):
                boxContainerPb.dtype = pb.UINT
                boxContainerPb.uintData.extend(actions)

            elif (spaceDesc.dtype in ['float', 'float32', 'float64']):
                boxContainerPb.dtype = pb.FLOAT
                boxContainerPb.floatData.extend(actions)

            elif (spaceDesc.dtype in ['double']):
                boxContainerPb.dtype = pb.DOUBLE
                boxContainerPb.doubleData.extend(actions)

            else:
                boxContainerPb.dtype = pb.FLOAT
                boxContainerPb.floatData.extend(actions)

            dataContainer.data.Pack(boxContainerPb)

        elif spaceType == spaces.Tuple:
            dataContainer.type = pb.Tuple
            tupleDataPb = pb.TupleDataContainer()

            spaceList = list(self._action_space.spaces)
            subDataList = []
            for subAction, subActSpaceType in zip(actions, spaceList):
                subData = self._pack_data(subAction, subActSpaceType)
                subDataList.append(subData)

            tupleDataPb.element.extend(subDataList)
            dataContainer.data.Pack(tupleDataPb)

        elif spaceType == spaces.Dict:
            dataContainer.type = pb.Dict
            dictDataPb = pb.DictDataContainer()

            subDataList = []
            for sName, subAction in actions.items():
                subActSpaceType = self._action_space.spaces[sName]
                subData = self._pack_data(subAction, subActSpaceType)
                subData.name = sName
                subDataList.append(subData)

            dictDataPb.element.extend(subDataList)
            dataContainer.data.Pack(dictDataPb)

        return dataContainer

    def send_execute_action_request(self, actions):
        dataContainer = self._pack_data(actions, self._action_space)
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
    def __init__(self, stepTime=0, port=5555, startSim=True, simSeed=0, simArgs={}, debug=False):
        self.stepTime = stepTime
        self.port = port
        self.startSim = startSim
        self.simSeed = simSeed
        self.simArgs = simArgs
        self.debug = debug

        # Filled in reset function
        self.ns3ZmqBridge = None
        self.action_space = None
        self.observation_space = None

        self.viewer = None
        self.state = None
        self.steps_beyond_done = None

        self.ns3ZmqBridge = Ns3ZmqBridge(self.port, self.startSim, self.simSeed, self.simArgs, self.debug)
        self.ns3ZmqBridge.initialize_env(self.stepTime)
        self.action_space = self.ns3ZmqBridge.get_action_space()
        self.observation_space = self.ns3ZmqBridge.get_observation_space()
        self.envDirty = False
        self.seed()

    def seed(self, seed=None):
        self.np_random, seed = seeding.np_random(seed)
        return [seed]

    def get_obs(self):
        # get_extra info is optional, so execute first, otherwise env will move to next step
        extraInfo = self.ns3ZmqBridge.get_extra_info()
        obs = self.ns3ZmqBridge.get_obs()
        reward = self.ns3ZmqBridge.get_reward()
        done = self.ns3ZmqBridge.is_game_over()
        return (obs, reward, done, extraInfo)

    def step(self, action):
        response = self.ns3ZmqBridge.execute_action(action)
        self.envDirty = True
        return self.get_obs()

    def reset(self):
        if not self.envDirty:
            state = self.get_obs()
            obs = state[0]
            return obs

        if self.ns3ZmqBridge:
            self.ns3ZmqBridge.close()
            self.ns3ZmqBridge = None

        self.ns3ZmqBridge = Ns3ZmqBridge(self.port, self.startSim, self.simSeed, self.simArgs, self.debug)
        self.ns3ZmqBridge.initialize_env(self.stepTime)
        self.action_space = self.ns3ZmqBridge.get_action_space()
        self.observation_space = self.ns3ZmqBridge.get_observation_space()
        self.envDirty = False
        state = self.get_obs()
        obs = state[0]
        return obs

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