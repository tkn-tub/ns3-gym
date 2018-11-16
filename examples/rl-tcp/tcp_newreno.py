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


class TcpNewReno(Tcp):
    """docstring for TcpNewReno"""
    def __init__(self):
        super(TcpNewReno, self).__init__()

    def get_action(self, obs, reward, done, info):
        ssThresh = obs[0]
        cWnd = obs[1]
        segmentSize = obs[2]
        segmentsAcked = obs[3]
        bytesInFlight  = obs[4]
        simTimeMs  = obs[5]

        new_cWnd = 1
        new_ssThresh = 1

        # IncreaseWindow
        if (cWnd < ssThresh):
            # slow start
            if (segmentsAcked >= 1):
                new_cWnd = cWnd + segmentSize

        if (cWnd >= ssThresh):
            # congestion avoidance
            if (segmentsAcked > 0):
                adder = 1.0 * (segmentSize * segmentSize) / cWnd;
                adder = int(max (1.0, adder))
                new_cWnd = cWnd + adder

        # GetSsThresh
        new_ssThresh = int(max (2 * segmentSize, bytesInFlight / 2))

        # return actions
        actions = [new_ssThresh, new_cWnd]

        return actions