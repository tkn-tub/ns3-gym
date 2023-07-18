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
    def __init__(self, port=0, startSim=True, simSeed=0, simArgs={}, debug=False):
        super(Ns3ZmqBridge, self).__init__()
        port = int(port)
        self.port = port
        self.startSim = startSim
        self.simSeed = simSeed
        self.simArgs = simArgs
        self.envStopped = False
        self.simPid = None
        self.wafPid = None
        self.ns3Process = None

        context = zmq.Context()
        self.socket = context.socket(zmq.REP)
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
            print('Please start proper ns-3 simulation script using ./waf --run "..."')

        self._action_space = None
        self._observation_space = None

        self.forceEnvStop = False
        self.obsData = None
        self.reward = 0
        self.gameOver = False
        self.gameOverReason = None
        self.extraInfo = None
        self.newStateRx = False

    def close(self):
        try:
            if not self.envStopped:
                self.envStopped = True
                self.force_env_stop()
                self.rx_env_state()
                self.send_close_command()
                self.ns3Process.kill()
                if self.simPid:
                    os.kill(self.simPid, signal.SIGTERM)
                    self.simPid = None
                if self.wafPid:
                    os.kill(self.wafPid, signal.SIGTERM)
                    self.wafPid = None
        except Exception as e:
            pass

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

    def initialize_env(self, stepInterval):
        request = self.socket.recv()
        simInitMsg = pb.SimInitMsg()
        simInitMsg.ParseFromString(request)

        self.simPid = int(simInitMsg.simProcessId)
        self.wafPid = int(simInitMsg.wafShellProcessId)
        self._action_space = self._create_space(simInitMsg.actSpace)
        self._observation_space = self._create_space(simInitMsg.obsSpace)

        reply = pb.SimInitAck()
        reply.done = True
        reply.stopSimReq = False
        replyMsg = reply.SerializeToString()
        self.socket.send(replyMsg)
        return True

    def get_action_space(self):
        return self._action_space

    def get_observation_space(self):
        return self._observation_space

    def force_env_stop(self):
        self.forceEnvStop = True

    def rx_env_state(self):
        if self.newStateRx:
            return

        request = self.socket.recv()
        envStateMsg = pb.EnvStateMsg()
        envStateMsg.ParseFromString(request)

        self.obsData = self._create_data(envStateMsg.obsData)
        self.reward = envStateMsg.reward
        self.gameOver = envStateMsg.isGameOver
        self.gameOverReason = envStateMsg.reason

        if self.gameOver:
            if self.gameOverReason == pb.EnvStateMsg.SimulationEnd:
                self.envStopped = True
                self.send_close_command()
            else:
                self.forceEnvStop = True
                self.send_close_command()

        self.extraInfo = envStateMsg.info
        if not self.extraInfo:
            self.extraInfo = {}

        self.newStateRx = True

    def send_close_command(self):
        reply = pb.EnvActMsg()
        reply.stopSimReq = True

        replyMsg = reply.SerializeToString()
        self.socket.send(replyMsg)
        self.newStateRx = False
        return True

    def send_actions(self, actions):
        reply = pb.EnvActMsg()

        actionMsg = self._pack_data(actions, self._action_space)
        reply.actData.CopyFrom(actionMsg)

        reply.stopSimReq = False
        if self.forceEnvStop:
            reply.stopSimReq = True

        replyMsg = reply.SerializeToString()
        self.socket.send(replyMsg)
        self.newStateRx = False
        return True

    def step(self, actions):
        # exec actions for current state
        self.send_actions(actions)
        # get result of above actions
        self.rx_env_state()

    def is_game_over(self):
        return self.gameOver

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
            data = np.array(data)
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
        return self.obsData

    def get_reward(self):
        return self.reward

    def get_extra_info(self):
        return self.extraInfo

    # add a context for the nested action, an Optional[Space]
    def _pack_data(self, actions, spaceDesc, context=None):
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
                # if there is a context, retrieve its type
                if context is None:
                    subActSpaceType = self._action_space.spaces[sName]
                else:
                    subActSpaceType = context.spaces[sName]
                # if there there is a nested dict, create a subcontext
                if type(subActSpaceType) == spaces.Dict and context is None:
                    subcontext = self._action_space.spaces[sName]
                elif type(subActSpaceType) == spaces.Dict:
                    subcontext = context.spaces[sName]
                else:
                    subcontext = None
                # create the data within the new context
                subData = self._pack_data(subAction, subActSpaceType, context=subcontext)

                subData.name = sName
                subDataList.append(subData)

            dictDataPb.element.extend(subDataList)
            dataContainer.data.Pack(dictDataPb)

        return dataContainer


class Ns3Env(gym.Env):
    def __init__(self, stepTime=0, port=0, startSim=True, simSeed=0, simArgs={}, debug=False):
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
        # get first observations
        self.ns3ZmqBridge.rx_env_state()
        self.envDirty = False
        self.seed()

    def seed(self, seed=None):
        self.np_random, seed = seeding.np_random(seed)
        return [seed]

    def get_state(self):
        obs = self.ns3ZmqBridge.get_obs()
        reward = self.ns3ZmqBridge.get_reward()
        done = self.ns3ZmqBridge.is_game_over()
        extraInfo = self.ns3ZmqBridge.get_extra_info()
        return (obs, reward, done, extraInfo)

    def step(self, action):
        response = self.ns3ZmqBridge.step(action)
        self.envDirty = True
        return self.get_state()

    def reset(self):
        if not self.envDirty:
            obs = self.ns3ZmqBridge.get_obs()
            return obs

        if self.ns3ZmqBridge:
            self.ns3ZmqBridge.close()
            self.ns3ZmqBridge = None

        self.envDirty = False
        self.ns3ZmqBridge = Ns3ZmqBridge(self.port, self.startSim, self.simSeed, self.simArgs, self.debug)
        self.ns3ZmqBridge.initialize_env(self.stepTime)
        self.action_space = self.ns3ZmqBridge.get_action_space()
        self.observation_space = self.ns3ZmqBridge.get_observation_space()
        # get first observations
        self.ns3ZmqBridge.rx_env_state()
        obs = self.ns3ZmqBridge.get_obs()
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
