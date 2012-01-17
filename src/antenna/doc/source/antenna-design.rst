.. include:: replace.txt


++++++++++++++++++++++++++++++++++++++
Design documentation
++++++++++++++++++++++++++++++++++++++

--------
Overview
--------

The Antenna module provides:

 #. a new base class (AntennaModel) that provides an interface for the modeling of the radiation pattern of an antenna;
 #. a set of classes derived from this base class that each models the radiation pattern of different types of antennas.


------------
AntennaModel
------------

The AntennaModel uses the coordinate system adopted in [Balanis]_ and
depicted in Figure :ref:`fig-antenna-coordinate-system`. This system
is obtained by traslating the cartesian coordinate system used by the
ns-3 MobilityModel into the new origin :math:`o` which is the location
of the antenna, and then transforming the coordinates of every generic
point :math:`p` of the space from cartesian coordinates
:math:`(x,y,z)` into spherical coordinates 
:math:`(r, \theta,\phi)`. 
The antenna model neglects the radial component :math:`r`, and
only considers the angle components :math:`(\theta, \phi)`. An antenna
radiation pattern is then expressed as a mathematical function
:math:`g(\theta, \phi) \longrightarrow \mathcal{R}` that returns the
gain (in dB) for each possible direction of
transmission/reception. All angles are expressed in radians.


.. _fig-antenna-coordinate-system:
   
.. figure:: figures/antenna-coordinate-system.*
   :align: center

   Coordinate system of the AntennaModel

---------------
Provided models
---------------

In this section we describe the antenna radiation pattern models that
are included within the antenna module.


IsotropicAntennaModel
+++++++++++++++++++++

This antenna radiation pattern model provides a unitary gain (0 dB)
for all direction.



CosineAntennaModel
++++++++++++++++++

This is the cosine model described in [Chunjan]_: the antenna gain is
determined as:

.. math::
 
  g(\phi, \theta) = \cos^{n} \left(\frac{\phi - \phi_{0}}{2}  \right)

where :math:`\phi_{0}` is the azimuthal orientation of the antenna
(i.e., its direction of maximum gain) and the exponential

.. math::
 
  n = -\frac{3}{20 \log_{10} \left( \cos \frac{\phi_{3dB}}{4} \right)}

determines the desired 3dB beamwidth :math:`\phi_{3dB}`.

A major difference between the model of [Chunjian]_ and the one
implemented in the class CosineAntennaModel is that only the element
factor (i.e., what described by the above formulas) is considered. In
fact, [Chunjian]_ also considered an additional antenna array
factor. The reason why the latter is excluded is that we expect that
the average user would desire to specify a given beamwidth exactly,
without adding an array factor at a latter stage which would in
practice alter the effective beamwidth of the resulting radiation
pattern.


References
++++++++++

.. [Balanis] C.A. Balanis, "Antenna Theory - Analysis and Design",  Wiley, 2nd Ed.

.. [Chunjian] Li Chunjian, "Efficient Antenna Patterns for
   Three-Sector WCDMA Systems", Master of Science Thesis, Chalmers
   University of Technology, Göteborg, Sweden, 2003
