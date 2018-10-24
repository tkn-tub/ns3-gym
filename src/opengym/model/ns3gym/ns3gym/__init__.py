from gym.envs.registration import register

register(
    id='ns3-v0',
    entry_point='ns3gym.ns3env:Ns3Env',
)