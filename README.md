ns3-gym
============

OpenAI Gym is a toolkit for reinforcement learning widely used in research. The ns–3 network simulator is the de–facto standard for academic and industry research into networking protocols and communications technology. Ns3-gym is a framework which integrates OpenAI Gym into ns-3.

Installation
============

1. Install all required dependencies required by ns-3.
```
# minimal requirements for C++:
apt-get install gcc g++ python

see https://www.nsnam.org/wiki/Installation
```
2. Install ZMQ and Protocol Buffers libs:
```
# to install protobuf-3.6 on ubuntu 16.04:
sudo add-apt-repository ppa:maarten-fonville/protobuf
sudo apt-get update

apt-get install libzmq5 libzmq5-dev
apt-get install libprotobuf-dev
apt-get install protobuf-compiler
```
3. Install PyOpenGymNs3 located in src/opengym/model/PyOpenGymNs3 (Python3 required)
```
pip3 install ./src/opengym/model/PyOpenGymNs3
```
4. Configure and build ns-3 project:
```
# Opengym Protobuf messages are build during configure
./waf configure
./waf
```
5. (Optional) Install all libraries required by your agent (like tensorflow, keras, etc.).

6. Run example:
```
cd ./scratch/opengym
./simple_test.py
```
Examples
========

## Basic Interface

1. Example Python script. Note, that `gym.make('ns3-v0')` starts ns-3 simulation script located in current working directory.
```
import gym
import PyOpenGymNs3
import MyAgent

env = gym.make('ns3-v0')
obs = env.reset()
agent = MyAgent.Agent()

while True:
  action = agent.get_action(obs)
  obs, reward, done, info = env.step(action)

  if done:
    break
env.close()
```
2. Any ns-3 simulation script can be used as a Gym environment. This requires only to instantiate OpenGymInterface and implement the ns3-gym C++ interface consisting of the following functions:
```
Ptr<OpenGymSpace> GetObservationSpace();
Ptr<OpenGymSpace> GetActionSpace();
Ptr<OpenGymDataContainer> GetObservation();
float GetReward();
bool GetGameOver();
std::string GetExtraInfo();
bool ExecuteActions(Ptr<OpenGymDataContainer> action);
```
Note, that the generic ns3-gym interface allows to observe any variable or parameter in a simulation.

Contact
============
* Piotr Gawlowicz, TU-Berlin, gawlowicz@tkn
* Anatolij Zubow, TU-Berlin, zubow@tkn
* tkn = tkn.tu-berlin.de

How to reference ns3-gym?
============

Please use the following bibtex :

```
@article{ns3gym,
  author    = {Gawlowicz, Piotr and Zubow, Anatolij},
  title     = {{ns3-gym: Extending OpenAI Gym for Networking Research}},
  journal   = {CoRR},
  year      = {2018},
  url       = {https://arxiv.org/abs/1810.03943},
  archivePrefix = {arXiv},
}
```
