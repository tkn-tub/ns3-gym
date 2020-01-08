### Description
This ns3 module implements a bidirectional coupling to the road traffic simulator [SUMO](https://software.dlr.de/p/sumo/home/). It dynamically synchronizes the positions of SUMO vehicles with corresponding ns3 nodes. Additionally, the state of SUMO vehicles can be controlled via ns3, e.g. for changing the speed. The module is built up on the [TraCI API](http://sumo.dlr.de/wiki) of the SUMO simulator. The module prerequisites a SUMO installation of version 1.1.0, but no additional sources.

### Usage
Copy the `traci/` and `traci-applications/` directory into your `<ns3-root>/src/` directory. If you copy the folders in the new `<ns3-root>/contrib/` directory of ns-3.29, then you have to adapt the path for the example in `traci-applications/examples/ns3-sumo-coupling-simple.cc`.

Build/rebuild your ns3 simulator with the new module and run the example.
```sh
$ cd <ns3-root>/
$ ./waf configure --enable-examples
$ ./waf build
$ ./waf --run ns3-sumo-coupling-simple
```
To visualize the scenario in ns3 use
```sh
$ ./waf --run ns3-sumo-coupling-simple --vis
```
To visualize the scenario additionally with `sumo-gui` set the attribute in the example `ns3-sumo-coupling-simple.cc` to 
```
client->SetAttribute("SumoGUI", BooleanValue(true));
```
The example can be found in `traci-applications/examples/ns3-sumo-coupling-simple.cc`.

### Remarks
ns3 is not considered to support dynamic node generation and destruction; everything should be defined BEFORE the simulation starts. Hence, for all SUMO scenarios with a fixed number of vehicles, created at the beginning of the simulation, no dynamic ns3 node generation/destruction is necessary. However, most SUMO scenarios include and exlude vehicles during the simulation, which requires ns3 to define a "node pool" before simulation starts (see example `ns3-sumo-coupling-simple.cc`). It is crucial to ensure an appropriate functionality for node inclusion and exclusion in ns3 to avoid unwanted packet transmissions within the "node pool". Therefore, additional functions in the application and other layers should be implemented. 

### Update SUMO source code of the module
The module uses the source code of SUMO (version 1.1.0) for compiling the TraCI API. The following steps are necessary for updating the used SUMO sources e.g. if there are changes in the TraCI API.
Unpack the SUMO sources and copy the required headers to the ns3 traci module and rename them to avoid name conflicts.
```sh
$ cp <sumo-root>/src/config.h <ns3-root>/src/traci/model/sumo-config.h
$ cp <sumo-root>/src/foreign/tcpip/socket.h <ns3-root>/src/traci/model/sumo-socket.h
$ cp <sumo-root>/src/foreign/tcpip/socket.cpp <ns3-root>/src/traci/model/sumo-socket.cpp
$ cp <sumo-root>/src/foreign/tcpip/storage.h <ns3-root>/src/traci/model/sumo-storage.h
$ cp <sumo-root>/src/foreign/tcpip/storage.cpp <ns3-root>/src/traci/model/sumo-storage.cpp
$ cp <sumo-root>/src/utils/traci/TraCIAPI.h <ns3-root>/src/traci/model/sumo-TraCIAPI.h
$ cp <sumo-root>/src/utils/traci/TraCIAPI.cpp <ns3-root>/src/traci/model/sumo-TraCIAPI.cpp
$ cp <sumo-root>/src/traci-server/TraCIConstants.h <ns3-root>/src/traci/model/sumo-TraCIConstants.h
$ cp <sumo-root>/src/libsumo/TraCIDefs.h <ns3-root>/src/traci/model/sumo-TraCIDefs.h
```

Adapt the corresponding lines in the files to the new file names.

* sumo-socket.h:
```
#include "storage.h" → #include "sumo-storage.h"
```
* sumo-socket.cc:
```
#include "socket.h" → #include "sumo-socket.h"
```
* sumo-storage.cc:
```
#include "storage.h" → #include "sumo-storage.h"
```
* sumo-TraCIAPI.h:
```
#include <config.h> → #inlcude “sumo-config.h”
#include <foreign/tcpip/socket.h> → #include "sumo-socket.h"
#include <traci-server/TraCIConstants.h> → #include "sumo-TraCIConstants.h"
#include <libsumo/TraCIDefs.h> → #include "sumo-TraCIDefs.h"
```
* sumo-TraCIAPI.cc:
```
#include <config.h> → #inlcude “sumo-config.h”
#include "TraCIAPI.h" → #include "sumo-TraCIAPI.h"
```

### System specification
This module was tested on the following system:
* ns3: 3.29
* SUMO: 1.1.0
* Ubuntu: 18.04 LTS
* gcc: 7.3.0

### References
[1] Y. Pigné, G. Danoy & P. Bouvry, "A platform for realistic online vehicular network management," 2010 IEEE Globecom Workshops, Miami, FL, 2010, pp. 595-599.

[2] Wegener, Axel & Piorkowski, Michal & Raya, Maxim & Hellbrück, Horst & Fischer, Stefan & Hubaux, Jean-Pierre. (2008). TraCI: An Interface for Coupling Road Traffic and Network Simulators. Proceedings of the 11th Communications and Networking Simulation Symposium, CNS'08.


### Authors
Patrick Schmager, TU Dresden, Vodafone Chair, <patrick.schmager@tu-dresden.de>

Sebastian Kühlmorgen, TU Dresden, Vodafone Chair, <sebastian.kuehlmorgen@tu-dresden.de>
