multi-agent example
===================

This example shows how to create an ns3-gym environment with multiple agents and connects them to multiple independent Python processes.
Note that for each agent an independent ns3-gym gateway is created. 
Each gateway binds its socket on different port number.
Here, agent 1 communicates over port number 5555, while agent 2 uses port number 5556.

In order to run the example:

```
# Terminal 1
./waf --run "multi-agent"

# Terminal 2
cd ./scratch/multi-agent
./agent1.py

# Terminal 3
cd ./scratch/multi-agent
./agent2.py
```