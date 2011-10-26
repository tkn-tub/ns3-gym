.. include:: replace.txt


++++++++++++++++++++++++++++++++++++++
Design documentation
++++++++++++++++++++++++++++++++++++++


Overview
++++++++

The Buildings module provides:
  
 #. a new mobility model (BuildingsMobilityModel) that allows to specify the location, size and characteristics of buildings present in the simulated area, and allows the placement of nodes inside those buildings;
 #. a new propagation model (BuildingsPropagationLossModel) working with the mobility model just introduced, that allows to model the phenomenon of indoor/outdoor propagation in the presence of buildings.

Both models have been designed with LTE in mind, though their implementation is in fact independent from any LTE-specific code, and can be used with other ns-3 wireless technologies as well (e.g., wifi). 

The pathloss model included is obtained through a combination of several well known pathloss models in order to mimic different environmental scenarios such as urban, suburban and open areas. Moreover, the model considers both outdoor and indoor indoor and outdoor communication has to be included since HeNB might be installed either within building and either outside. In case of indoor communication, the model has to consider also the type of building in outdoor <-> indoor communication according to some general criteria such as the wall penetration losses of the common materials; moreover it includes some general configuration for the internal walls in indoor communications. Finally, the frequency also represent an important parameter since it spans from 600 MHz up to 2600 MHz according to [TS36.101]_.

Description of the Included Models
++++++++++++++++++++++++++++++++++

For discriminate indoor and outdoor users, the model includes a specific class called ``Building`` which contains a ns3 ``Box`` class for defining the dimension of the building. In order to implements the characteristics of the pathloss models included, the ``Building`` class provides support for:

  * building type:

    * Residential (default value)
    * Office
    * Commercial
  * external walls type

    * Wood
    * ConcreteWithWindows (default value)
    * ConcreteWithoutWindows
    * StoneBlocks

  * number of floors (default value 1, which means only ground-floor)
  * number of rooms in x-axis (default value 1)
  * number of rooms in x-axis (default value 1)

By means of the number of rooms in x and y axis it is possible the definition of buildings where rooms are organized in grids, typical reference scenario for femto-cells in 3GPP called dual-strip.

The ``Building`` class is included in ``BuildingsMobilityModel`` class, which inherits from the ns3 class ``MobilityModel`` and it is in charge of managing the standard mobility functionalities plus the building ones (e.g., floor and room of the node).

The class ``BuildingsMobilityModel`` is used by ``BuildingsPropagationLossModel`` class, which inherits from the ns3 class ``PropagationLossModel`` and manages the pathloss computation of the single components and their composition according to the nodes' positions. Moreover, it implements also the shadowing, that is the loss due to obstacles in the main path (i.e., vegetation, buildings, etc.).

In the following we present the link pathloss models included.

Okumura Hata (OH)
-----------------

This model is used to model open area pathloss for long distance (i.e., > 1 Km). In order to include all the possible frequencies usable by LTE we need to consider several variant of the well known Okumura Hata model. In fact, the standard one is designed for frequencies ranging from 150 MHz to 1500 MHz, the COST231 [cost231]_ one for the 1500 MHz up to 2000 MHz and [pl26ghz]_ for the one at 2.6G Hz. Another important aspect is the scenarios considered by the models, in fact the all models are originally designed for urban scenario and then only the standard one and the COST231 are extended to suburban, while only the standard one has been extended to open areas. Therefore, the model cannot cover all scenarios at all frequencies. In the following we detail the models adopted.



The pathloss expression of the COST231 OH is:

.. math::

  L = 46.3 + 33.9\log{f} - 13.82 \log{h_\mathrm{b}} + (44.9 - 6.55\log{h_\mathrm{b}})\log{d} - F(h_\mathrm{M}) + C

where

.. math::

  F(h_\mathrm{M}) = \left\{\begin{array}{ll} (1.1\log(f))-0.7 \times h_\mathrm{M} - (1.56\times \log(f)-0.8) & \mbox{for medium and small size cities} \\ 3.2\times (\log{(11.75\times h_\mathrm{M}}))^2 & \mbox{for large cities}\end{array} \right.


.. math::

  C = \left\{\begin{array}{ll} 0dB & \mbox{for medium-size cities and suburban areas} \\ 3dB & \mbox{for large cities}\end{array} \right.

and

  :math:`f` : frequency [MHz]

  :math:`h_\mathrm{b}` : eNB height above the ground [m]

  :math:`h_\mathrm{M}` : UE height above the ground [m]

  :math:`d` : distance [km]
  
  :math:`log` : is a logarithm in base 10 (this for the whole document) 


This model is only for urban scenarios.

The pathloss expression of the standard OH in urban area is:

.. math::

  L = 69.55 + 26.16\log{f} - 13.82 \log{h_\mathrm{b}} + (44.9 - 6.55\log{h_\mathrm{b}})\log{d} - C_\mathrm{H}

where for small or medium sized city

.. math::

  C_\mathrm{H} = 0.8 + (1.1\log{f} - 0.7)h_\mathrm{M} -1.56\log{f}

and for large cities

.. math::
  C_\mathrm{H} = \left\{\begin{array}{ll} 8.29 (\log{(1.54h_M)})^2 -1.1 & \mbox{if } 150\leq f\leq 200 \\ 3.2(\log{(11.75h_M)})^2 -4.97 & \mbox{if } 200<f\leq 1500\end{array} \right.

There extension for the standard OH in suburban is

.. math::

  L_\mathrm{SU} = L_\mathrm{U} - 2 \left(\log{\frac{f}{28}}\right)^2 - 5.4

where

  :math:`L_\mathrm{U}` : pathloss in urban areas

The extension for the standard OH in open area is

.. math::

  L_\mathrm{O} = L_\mathrm{U} - 4.70 (\log{f})^2 + 18.33\log{f} - 40.94


The literature lacks of extensions of the COST231 to open area (for suburban it seems that we can just impose C = 0); therefore we consider it a special case fo the suburban one.


Regarding the pathloss at 2600 MHz, in literature we found a paper presenting a model coming from an empirical evaluation for urban area [pl26ghz]_:

.. math::

  L = 36 + 26\log{d}

Therefore, also in this case, the suburban and openareas environment scenarios are model as the urban one.


Short Range Communications ITU-R P.1411 (I1411)
---------------------------------------

This model is designed for short range outdoor communication in the frequency range 300 MHz to 100 GHz. It is divided in LOS and NLoS models and NLoS is split in roof-tops and canyons. The model implemented considers the LoS propagation for short distances according to a tunable threshold (``m_itu1411NlosThreshold``). In case on NLoS propagation, the over the roof-top model is taken in consideration for modeling both macro BS and SC. In case on NLoS several parameters scenario dependent have been included, such as average street width, orientation, etc. The values of such parameters have to be properly set according to the scenario implemented, the model does not calculate natively their values. In case any values is provided, the standard ones are used, apart for the height of the mobile and BS, which instead their integrity is tested directly in the code (i.e., they have to be greater then zero).  In the following we give the expressions of the components of the model.


LoS within street canyons
~~~~~~~~~~~~~~~~~~~~~~~~~

This model provides an upper and lower bound respectively according to the following formulas

.. math::

  L_\mathrm{LoS,l} = L_\mathrm{bp} + \left\{\begin{array}{ll} 20\log{\frac{d}{R_\mathrm{bp}}} & \mbox{for $d \le R_\mathrm{bp}$} \\ 40\log{\frac{d}{R_\mathrm{bp}}} & \mbox{for $d > R_\mathrm{bp}$}\end{array} \right.

.. math::

  L_\mathrm{LoS,u} = L_\mathrm{bp} + 20 + \left\{\begin{array}{ll} 25\log{\frac{d}{R_\mathrm{bp}}} & \mbox{for $d \le R_\mathrm{bp}$} \\ 40\log{\frac{d}{R_\mathrm{bp}}} & \mbox{for $d > R_\mathrm{bp}$}\end{array} \right.

where the breakpoint distance is given by

.. math::

  R_\mathrm{bp} \approx \frac{4h_\mathrm{b}h_\mathrm{m}}{\lambda}

and the above parameters are

  :math:`\lambda` : wavelength [m]

  :math:`h_\mathrm{b}` : eNB height above the ground [m]

  :math:`h_\mathrm{m}` : UE height above the ground [m]

  :math:`d` : distance [m]

and :math:`L_{bp}` is the value for the basic transmission loss at the break point, defined as:

.. math::

  L_{bp} = \left|20\log \left(\frac{\lambda^2}{8\pi h_\mathrm{b}h\mathrm{m}}\right)\right|

The value used by the simulator is the average one for modeling the median pathloss.


NLoS over the rooftops
~~~~~~~~~~~~~~~~~~~~~~

In this case the model is based on [walfisch]_ and [ikegami]_, where the loss is expressed as the sum of free-space loss (:math:`L_{bf}`), the diffraction loss from rooftop to street (:math:`L_{rts}`) and the reduction due to multiple screen diffraction past rows of building (:math:`L_{msd}`). The formula is:

.. math::

  L_{NLOS1} = \left\{ \begin{array}{ll} L_{bf} + L_{rts} + L_{msd} & \mbox{for } L_{rts} + L_{msd} > 0 \\ L_{bf} & \mbox{for } L_{rts} + L_{msd} \le 0\end{array}\right.

The free-space loss is given by:

.. math::

  L_{bf} = 32.4 + 20 \log {(d/1000)} + 20\log{(f)}

where:

  :math:`f` : frequency [MHz]

  :math:`d` : distance (where :math:`d > 1`) [m]

The term :math:`L_{rts}` takes into account the width of the street and its orientation, according to the formulas

.. math::

  L_{rts} = -8.2 - 10\log {(w)} + 10\log{(f)} + 20\log{(\Delta h_m)} + L_{ori}

  L_{ori} = \left\{ \begin{array}{lll} -10 + 0.354\varphi & \mbox{for } 0^{\circ} \le \varphi < 35^{\circ} \\ 2.5 + 0.075(\varphi-35) & \mbox{for } 35^{\circ} \le \varphi < 55^{\circ} \\ 4.0 -0.114(\varphi-55) & \mbox{for } 55^{\circ} \varphi \le 90^{\circ}\end{array}\right.

  \Delta h_m = h_r - h_m

where:

  :math:`h_r` : is the height of the rooftop [m]

  :math:`h_m` : is the height of the mobile [m]

  :math:`\varphi` : is the street orientation with respect to the direct path (degrees)


The multiple screen diffraction loss depends on the BS antenna height relative to the building height and on the incidence angle. The former is selected as the higher antenna in the communication link. Regarding the latter, the "settled field distance" is used for select the proper model; its value is given by

.. math::

  d_{s} = \frac{\lambda d^2}{\Delta h_{b}^2}

with

  :math:`\Delta h_b = h_b - h_m`

Therefore, in case of :math:`l > d_s` (where `l` is the distance over which the building extend), it can be evaluated according to

.. math::

  L_{msd} = L_{bsh} + k_{a} + k_{d}\log{(d/1000)} + k_{f}\log{(f)} - 9\log{(b)}

  L_{bsh} = \left\{ \begin{array}{ll} -18\log{(1+\Delta h_{b})} & \mbox{for } h_{b} > h_{r} \\ 0 & \mbox{for } h_{b} \le h_{hr} \end{array}\right.

  k_a = \left\{ \begin{array}{lll} 
      71.4 & \mbox{for } h_{b} > h_{r} \mbox{ and } f>2000 \mbox{ MHz} \\
      54 & \mbox{for } h_{b} > h_{r} \mbox{ and } f\le2000 \mbox{ MHz} \\
      54-0.8\Delta h_b & \mbox{for } h_{b} \le h_{r} \mbox{ and } d \ge 500 \mbox{ m} \\
      54-1.6\Delta h_b & \mbox{for } h_{b} \le h_{r} \mbox{ and } d < 500 \mbox{ m} \\
      \end{array} \right.

  k_d = \left\{ \begin{array}{ll}
        18 & \mbox{for } h_{b} > h_{r} \\
        18 -15\frac{\Delta h_b}{h_r} & \mbox{for } h_{b} \le h_{r}
        \end{array} \right.

  k_f = \left\{ \begin{array}{ll}
        -8 & \mbox{for } f>2000 \mbox{ MHz} \\
        -4 + 0.7(f/925 -1) & \mbox{for medium city and suburban centres and} f\le2000 \mbox{ MHz} \\
        -4 + 1.5(f/925 -1) & \mbox{for metropolitan centres and } f\le2000 \mbox{ MHz}
        \end{array}\right.


Alternatively, in case of :math:`l < d_s`, the formula is:

.. math::

  L_{msd} = -10\log{\left(Q_M^2\right)}

where

.. math::

  Q_M = \left\{ \begin{array}{lll}
        2.35\left(\frac{\Delta h_b}{d}\sqrt{\frac{b}{\lambda}}\right)^{0.9} & \mbox{for } h_{b} > h_{r} \\
        \frac{b}{d} &  \mbox{for } h_{b} \approx h_{r} \\
        \frac{b}{2\pi d}\sqrt{\frac{\lambda}{\rho}}\left(\frac{1}{\theta}-\frac{1}{2\pi + \theta}\right) & \mbox{for }  h_{b} < h_{r} 
        \end{array}\right.


where:

.. math::

  \theta = arc tan \left(\frac{\Delta h_b}{b}\right)

  \rho = \sqrt{\Delta h_b^2 + b^2}


Indoor Communications (I1238)
-----------------------------

In this case the model considers the ITU P.1238, which includes losses due to type of building (i.e., residential, office and commercial).
The analytical expression is given in the following.

.. math::

  L_\mathrm{total} = 20\log f + N\log d + L_f(n)- 28 [dB]

where:

  :math:`N = \left\{ \begin{array}{lll} 28 & residential \\ 30 & office \\ 22 & commercial\end{array} \right.` : power loss coefficient [dB]

  :math:`L_f = \left\{ \begin{array}{lll} 4n & residential \\ 15+4(n-1) & office \\ 6+3(n-1) & commercial\end{array} \right.`

  :math:`n` : number of floors between base station and mobile (:math:`n\ge 1`)

  :math:`f` : frequency [MHz]

  :math:`d` : distance (where :math:`d > 1`) [m]



External Walls Penetration Loss (BEL)
-------------------------------------

This component models the penetration loss through walls for indoor to outdoor communications and vice-versa. The values are taken from the [cost231]_ model.

  * Wood ~ 4 dB
  * Concrete with windows (no metallised) ~ 7 dB
  * Concrete without windows ~ 15 dB (spans between 10 and 20 in COST231)
  * Stone blocks ~ 12 dB


Height Gain Model (HG)
-----------------------

This component model the gain due to the fact that the transmitting device is on a floor above the ground. In literature [turkmani]_ this gain has been evaluated as about 2 dB per floor. This gain can be applied to all the indoor to outdoor communications and vice-versa.



Hybrid Model Indoor<->Outdoor
-----------------------------

The pathloss model characterizes the hybrid cases (i.e., when an outdoor node transmit to an indoor one and vice-versa) by adding to the proper model, evaluated according to correspond distance, the external wall penetration loss due to the building (see Section BEL).



Pathloss Logic Model
++++++++++++++++++++


In the following the pseudo-code of the model is presented::

  if (txNode is outdoor)
    then
      if (rxNode is outdoor)
        then
          if (distance > 1 km)
            then
              if (rxNode or txNode is below the rooftop)
                then
                  L = I1411
                else
                  L = OH 
            else
              L = I1411
        else (rxNode is indoor)
          if (distance > 1 km)
            then
              if (rxNode or txNode is below the rooftop)
                L = I1411 + BEL + HG
              else
                L = OH + BEL + HG
            else
              L = I1411 + BEL + HG
  else (txNode is indoor)
    if (rxNode is indoor)
      then
       if (same building)
          then
            L = I1238
          else
            L = I1411 + 2*BEL 
     else (rxNode is outdoor)
      if (distance > 1 km)
        then 
          if (rxNode or txNode is below the rooftop)
                then
                  L = I1411 + BEL + HG
                else
                  L = OH + BEL + HG
        else
          L = I1411 + BEL


Some considerations that apply when the Buildings model is used in an LTE FDD context:

 * in the uplink, the txNode will be an UE, whereas the rxNode will be a 

where ``txNode`` and ``rxNode`` can be one of the elements eNB, SC and UE.
We note that for SC nodes in case that the distance is greater then 1 km, we still consider the I1411 model since it better models the transmissions with antenna below the roof-top level and moreover due to the fact that OH is specifically designed for macro cells and therefore for antennas above the roof-top level. Finally, we introduced a threshold also or SC transmissions (called ``m_itu1411DistanceThreshold``) for pruning the communications between SCs and UEs too far (the default values is fixed to 2 km).


Shadowing Model
+++++++++++++++

The shadowing is modeled according to a log-normal distribution with variable standard deviation as function of the connection characteristics. In the implementation we considered three main possible scenarios which correspond to three standard deviations (i.e., the mean is always 0), in detail:

 * outdoor (``m_shadowingSigmaOutdoor``, defaul value of 7 dB) :math:`\rightarrow X_\mathrm{O} \sim N(\mu_\mathrm{O}, \sigma_\mathrm{O}^2)`.
 * indoor (``m_shadowingSigmaIndoor``, defaul value of 10 dB) :math:`\rightarrow X_\mathrm{I} \sim N(\mu_\mathrm{I}, \sigma_\mathrm{I}^2)`.
 * external walls penetration (``m_shadowingSigmaExtWalls``, default value 5 dB) :math:`\rightarrow X_\mathrm{W} \sim N(\mu_\mathrm{W}, \sigma_\mathrm{W}^2)`

The simulator generates a shadowing value per each active link according to nodes' position the first time the link is used for transmitting. In case of transmissions from outdoor nodes to indoor ones, and vice-versa, the standard deviation (:math:`\sigma_\mathrm{IO}`) has to be calculated as the square root of the sum of the quadratic values of the standard deviatio in case of outdoor nodes and the one for the external walls penetration. This is due to the fact that that the components producing the shadowing are independent of each other; therefore, the variance of a distribution resulting from the sum of two independent normal ones is the sum of the variances. 

.. math::
  
  X \sim N(\mu,\sigma^2) \mbox{ and } Y \sim N(\nu,\tau^2)

  Z = X + Y \sim Z (\mu + \nu, \sigma^2 + \tau^2) 

  \Rightarrow \sigma_\mathrm{IO} = \sqrt{\sigma_\mathrm{O}^2 + \sigma_\mathrm{W}^2}

