.. include:: replace.txt

Propagation
-----------

The |ns3| propagation module defines two generic interfaces, namely ``PropagationLossModel`` and ``PropagationDelayModel``, for the modeling of respectively propagation loss and propagation delay.

The following propagation loss models are implemented:

* ``JakesPropagationLossModel``
* ``PropagationLossModel``
* ``RandomPropagationLossModel``
* ``FriisPropagationLossModel``
* ``TwoRayGroundPropagationLossModel``
* ``LogDistancePropagationLossModel``
* ``ThreeLogDistancePropagationLossModel``
* ``NakagamiPropagationLossModel``
* ``FixedRssLossModel``
* ``MatrixPropagationLossModel``
* ``RangePropagationLossModel``


The following propagation delay models are implemented:

* ``PropagationDelayModel``
* ``RandomPropagationDelayModel``
* ``ConstantSpeedPropagationDelayModel``

All the above models can be used within any model using the ``PropagationLossModel`` and ``PropagationDelayModel`` interfaces, such as Wifi and Wimax.



