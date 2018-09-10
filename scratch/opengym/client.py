import zmq
import sys


__author__ = "Piotr Gawlowicz"
__copyright__ = "Copyright (c) 2018, Technische Universität Berlin"
__version__ = "0.1.0"
__email__ = "gawlowicz@tkn.tu-berlin.de"


class Ns3GymEnv(object):
    """docstring for Ns3GymEnv"""
    def __init__(self, port="5555"):
        super(Ns3GymEnv, self).__init__()
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


stepInterval = 0.5
ns3Env = Ns3GymEnv()
ns3Env.send_init_request(stepInterval)

[errCode, actionSpace] = ns3Env.send_get_action_space_request()
print("Action Space: ", actionSpace)

i = 0
while True:
    i += 1

    [errCode, value] = ns3Env.send_is_game_over_request()
    if errCode == 0 and value == 1:
        break

    print("Step: ", i)
    [errCode, state] = ns3Env.send_get_state_request()
    print("---State: ", state)

    [errCode, reward] = ns3Env.send_get_reward_request()
    print("---Reward: ", reward)

    actions = '0,12,0,1'
    print("---Actions: ", actions)
    [errCode, value] = ns3Env.send_execute_action_request(actions)

print ("GAME OVER")