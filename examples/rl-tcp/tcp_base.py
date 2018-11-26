__author__ = "Piotr Gawlowicz"
__copyright__ = "Copyright (c) 2018, Technische Universität Berlin"
__version__ = "0.1.0"
__email__ = "gawlowicz@tkn.tu-berlin.de"


class Tcp(object):
    """docstring for Tcp"""
    def __init__(self):
        super(Tcp, self).__init__()

    def set_spaces(self, obs, act):
        self.obsSpace = obs
        self.actSpace = act

    def get_action(self, obs, reward, done, info):
        pass


class TcpEventBased(Tcp):
    """docstring for TcpEventBased"""
    def __init__(self):
        super(TcpEventBased, self).__init__()

    def get_action(self, obs, reward, done, info):
        # unique socket ID
        socketUuid = obs[0]
        # TCP env type: event-based = 0 / time-based = 1
        envType = obs[1]
        # sim time in us
        simTime_us = obs[2]
        # unique node ID
        nodeId = obs[3]
        # current ssThreshold
        ssThresh = obs[4]
        # current contention window size
        cWnd = obs[5]
        # segment size
        segmentSize = obs[6]
        # number of acked segments
        segmentsAcked = obs[7]
        # estimated bytes in flight
        bytesInFlight  = obs[8]
        # last estimation of RTT
        lastRtt_us  = obs[9]
        # min value of RTT
        minRtt_us  = obs[10]
        # function from Congestion Algorithm (CA) interface:
        #  GET_SS_THRESH = 0 (packet loss),
        #  INCREASE_WINDOW (packet acked),
        #  PKTS_ACKED (unused),
        #  CONGESTION_STATE_SET (unused),
        #  CWND_EVENT (unused),
        calledFunc = obs[11]
        # Congetsion Algorithm (CA) state:
        #  CA_OPEN = 0,
        #  CA_DISORDER,
        #  CA_CWR,
        #  CA_RECOVERY,
        #  CA_LOSS,
        #  CA_LAST_STATE
        caState = obs[12]
        # Congetsion Algorithm (CA) event:
        #  CA_EVENT_TX_START = 0,
        #  CA_EVENT_CWND_RESTART,
        #  CA_EVENT_COMPLETE_CWR,
        #  CA_EVENT_LOSS,
        #  CA_EVENT_ECN_NO_CE,
        #  CA_EVENT_ECN_IS_CE,
        #  CA_EVENT_DELAYED_ACK,
        #  CA_EVENT_NON_DELAYED_ACK,
        caEvent = obs[13]
        # ECN state:
        #  ECN_DISABLED = 0,
        #  ECN_IDLE,
        #  ECN_CE_RCVD,
        #  ECN_SENDING_ECE,
        #  ECN_ECE_RCVD,
        #  ECN_CWR_SENT
        ecnState = obs[14]

        # compute new values
        new_cWnd = 10 * segmentSize
        new_ssThresh = 5 * segmentSize

        # return actions
        actions = [new_ssThresh, new_cWnd]

        return actions


class TcpTimeBased(Tcp):
    """docstring for TcpTimeBased"""
    def __init__(self):
        super(TcpTimeBased, self).__init__()

    def get_action(self, obs, reward, done, info):
        # unique socket ID
        socketUuid = obs[0]
        # TCP env type: event-based = 0 / time-based = 1
        envType = obs[1]
        # sim time in us
        simTime_us = obs[2]
        # unique node ID
        nodeId = obs[3]
        # current ssThreshold
        ssThresh = obs[4]
        # current contention window size
        cWnd = obs[5]
        # segment size
        segmentSize = obs[6]
        # bytesInFlightSum
        bytesInFlightSum = obs[7]
        # bytesInFlightAvg
        bytesInFlightAvg = obs[8]
        # segmentsAckedSum
        segmentsAckedSum = obs[9]
        # segmentsAckedAvg
        segmentsAckedAvg = obs[10]
        # avgRtt
        avgRtt = obs[11]
        # minRtt
        minRtt = obs[12]
        # avgInterTx
        avgInterTx = obs[13]
        # avgInterRx
        avgInterRx = obs[14]
        # throughput
        throughput = obs[15]

        # compute new values
        new_cWnd = 10 * segmentSize
        new_ssThresh = 5 * segmentSize

        # return actions
        actions = [new_ssThresh, new_cWnd]

        return actions