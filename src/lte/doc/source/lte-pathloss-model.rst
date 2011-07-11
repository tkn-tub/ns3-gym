.. include:: replace.txt


+++++++++++++++++++++++++++
 Pathloss Model Documentation
+++++++++++++++++++++++++++


Overview
********

The pathloss model included in the lte module is obtained through a combination of several well known pathloss models in order to mimic heterogeneous scenarios. In fact, we are interested in modeling different environmental scenarios such as urban, suburban and open areas. Moreover, indoor and outdoor communication has to be included since HeNB might be installed either within building and either outside. In case of indoor communication, the model has to consider also the type of building in outdoor <-> indoor communication according to some general criteria such as the wall penetration losses of the common materials; moreover it includes some general configuration for the internal walls in indoor communications. Finally, the frequency also represent an important parameter since it spans from 600 MHz up to 2600 MHz according to [TS36.101]_.

Description of the Model
******************************

The naming used in the following will be:

 * User equipment::  UE
 * small cell BS (e.g., pico, femto): SC
 * BS -> eNB

Both UEs and SC might be either indoor and outdoor.

The model provides the following pathloss link computations:

  * BS <-> UE (indoor and outdoor)
  * SC (indoor and outdoor) <-> UE (indoor and outdoor)
 
The model will not include the following pathloss link computations:

  * UE <-> UE
  * BS <-> BS
  * BS <-> SC
  * SC <-> SC

In the following we present the link pathloss models included.

Okumura Hata (OH)
-----------------

This model is used to model open area pathloss for long distance (i.e., > 1 Km). In order to include all the possible frequencies usable by LTE we need both the standard OH model and the COST231 one; in fact, the standard one is designed for frequencies ranging from 150 MHz to 1500 MHz, the COST231 one for the 1500 MHz up to 2000 MHz and [paper] for the one at 2.6G Hz. Another important aspect is the scenarios considered by the models, in fact the all models are originally designed for urban scenario and then only the standard one and the COST231 are extended to suburban, while only the standard one has been extended to open areas. Therefore, the model cannot cover all scenarios at all frequencies. In the following we detail the models adopted.



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

  C_\mathrm{H} = 0.8 + (1.1\log{f} - 0.7)h_\mathrm[M] -1.56\log{f}

and for large cities

.. math::
  C = \left\{\begin{array}{ll} 8.29 (\log{(1.54h_\mathrm[M])})^2 -1.1 & \mbox{if } 150\leq f\leq 200 \\ 3.2(\log{(11.75h_\mathrm[M])})^2 -4.97 & \mbox{if } 200<f\leq 1500\end{array} \right.

There extension for the standard OH in suburban is

.. math::

  L_\mathrm{SU} = L_\mathrm{U} - 2 \left(\log{\frac{f}{28}}\right)^2 - 5.4

where

  :math:`L_\mathrm{U}` : pathloss in urban areas

The extension for the standard OH in open area is

.. math::

  L_\mathrm{O} = L_\mathrm{U} - 4.70 (\log{f})^2 + 18.33\log{f} - 40.94


I did not find info on how to extend the COST231 to open area (for suburban it seems that we can just impose C = 0).


While for 2600 MHz in literature we found that the COST231 model can be used with the assumption that the loss due to the higher frequency is compensated by the increase in the antenna gain. However, there is a formula coming from an empirical evaluation for urban area [pl26ghz]_:

.. math::

  L = 36 + 26\log{d}


Short Range Communications ITU-R P.1411 (I1411)
---------------------------------------

This is model from the short range outdoor communication in the frequency range 300 MHz to 100 GHz. It is divided in LOS and NLoS models and NLoS is split in roof-tops and canyons. The model implemented will consider the LoS canyon configuration, in order to proper model the pathloss between the HeNB and the surrounding users which represent the most critical scenario at this distance. However, this will produce results a bit optmistic in terms of far nodes, where probably a NLoS model should been more conservative. In the following we give the expressions of this model.

This model provides an upper and lower bound resectively according to the following formulas

.. math::

  L_\mathrm{LoS,l} = L_\mathrm{bp} + \left\{\begin{array}{ll} 20\log{\frac{d}{R_\mathrm{bp}}} & \mbox{for $d \le R_\mathrm{bp}$} \\ 40\log{\frac{d}{R_\mathrm{bp}}} & \mbox{for $d > R_\mathrm{bp}$}\end{array} \right.

.. math::

  L_\mathrm{LoS,u} = L_\mathrm{bp} + 20 + \left\{\begin{array}{ll} 25\log{\frac{d}{R_\mathrm{bp}}} & \mbox{for $d \le R_\mathrm{bp}$} \\ 40\log{\frac{d}{R_\mathrm{bp}}} & \mbox{for $d > R_\mathrm{bp}$}\end{array} \right.

where the breackpoint distance is given by

.. math::

  R_\mathrm{bp} \approx \frac{4h_\mathrm{b}h_\mathrm{m}}{\lambda}

and the above parameters are

  :math:`\lambda` : wavelength [m]

  :math:`h_\mathrm{b}` : eNB height above the ground [m]

  :math:`h_\mathrm{m}` : UE height above the ground [m]

  :math:`d` : distance [m]


Indoor Communications (I1238)
-----------------------------

In this case the model considers the ITU P.1238, which includes losses due to type of building (i.e., residential, office and commercial).
The analytical expression is given in the following.

.. math::

  L_\mathrm{total} = 20\log f + N\log d - 28 [dB]

where:

  :math:`N = \left\{ \begin{array}{lll} 28 & residential \\ 30 & office \\ 22 & commercial\end{array} \right.` : power loss coefficient [dB]

  :math:`f` : frequency [MHz]

  :math:`d` : distance (where :math:`d > 1`) [m]



External Walls Penetration Loss (BEL)
-------------------------------------

From COST231 we have:

  * Wood ~ 4 dB
  * Concrete with windows (no metallised) ~ 7 dB
  * Concrete without windows 10-20 dB


Hybrid Model Indoor<->Outdoor
-----------------------------

The pathloss model characterizes the hybrid cases (i.e., when an outdoor node transmit to an indoor one and viceversa) by adding to the proper model, evaluated according to their distance, the external wall penetration loss due to the building.


Pathloss Logic Model
-------------------


In the following the pseudo-code of the model is presented::

  if (txNode is outdoor)
    then
      if (rxNode is outdoor)
        then
          if (distance > 1 km)
            then
              if (rxNode or txNode is SC)
                then
                  L = I1411
                else
                  L = OH 
            else
              L = I1411
        else (rxNode is indoor)
          if (distance > 1 km)
            then 
              L = OH + BEL
            else
              L = I1411 + BEL
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
          if (rxNode or txNode is SC)
                then
                  L = I1411 + BEL
                else
                  L = OH + BEL
        else
          L = I1411 + BEL


where `txNode` and `rxNode` can be one of the elements eNB, SC and UE.
We note that for SC nodes in case that the distance is greater then 1 km, we still consider the I1411 model since it better models the tranmissions with antenna below the roof-top level and moreover due to the fact that OH is specifically designed for macro cells and therefore for antennas above the roof-top level.


