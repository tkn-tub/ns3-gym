.. include:: replace.txt


++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  User Documentation
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



Main configurable parameters
----------------------------

The ``Building`` class has the following configurable parameters:

* building type: Residential, Office and Commercial.
* external walls type: Wood, ConcreteWithWindows, ConcreteWithoutWindows and StoneBlocks.
* building bounds: a ``Box`` class with the building bounds.
* number of floors.
* number of rooms in x-axis and y-axis (rooms can be placed only in a grid way).

The ``BuildingMobilityLossModel`` parameter configurable with the ns3 attribute system is represented by the bound (string ``Bounds``) of the simulation area by providing a ``Box`` class with the area bounds. Moreover, by means of its methos the following parameters can be configured:

* the number of floor the node is placed (default 0).
* the position in the rooms grid.


The ``BuildingPropagationLossModel`` class has the following configurable parameters configurable with the attribute system:

* ``Frequency``: reference frequency (default 2160 MHz), note that by setting the frequency the wavelength is set accordingly automatically and viceversa).
* ``Lambda``: the wavelength (0.139 meters, considering the above frequency).
* ``ShadowSigmaOutdoor``: the standard deviation of the shadowing for outdoor nodes (defaul 7.0).
* ``ShadowSigmaIndoor``: the standard deviation of the shadowing for indoor nodes (default 8.0).
* ``ShadowSigmaExtWalls``: the standard deviation of the shadowing due to external walls penetration for outdoor to indoor communications (default 5.0).
* ``RooftopLevel``: the level of the rooftop of the building in meters (default 20 meters).
* ``Los2NlosThr``: the value of distance of the switching point between line-of-sigth and non-line-of-sight propagation model in meters (default 200 meters).
* ``ITU1411DistanceThr``: the value of distance of the switching point between short range (ITU 1211) communications and long range (Okumura Hata) in meters (default 200 meters).
* ``MinDistance``: the minimum distance in meters between two nodes for evaluating the pathloss (considered neglictible before this threshold) (default 0.5 meters).
* ``Environment``: the environment scenario among Urban, SubUrban and OpenAreas (default Urban).
* ``CitySize``: the dimension of the city among Small, Medium, Large (default Large).

In order to use the hybrid mode, the class to be used is the ``HybridBuildingMobilityLossModel``, which allows the selection of the proper pathloss model according to the pathloss logic presented in the design chapter. However, this solution has the problem that the pathloss model switching points might present discontinuities due to the different characteristics of the model. This implies that according to the specific scenario, the threshold used for switching have to be properly tuned.
The simple ``OhBuildingMobilityLossModel`` overcome this problem by using only the Okumura Hata model and the wall penetration losses.









