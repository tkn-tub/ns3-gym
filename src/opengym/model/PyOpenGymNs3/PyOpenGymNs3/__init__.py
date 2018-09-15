from gym.envs.registration import register

register(
    id='ns3-v0',
    entry_point='PyOpenGymNs3.ns3env:Ns3Env',
    kwargs={'port' : '5555'},
)