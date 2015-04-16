.. include:: replace.txt

.. _Propagation:

###########
Propagation
###########

The |ns3| propagation module defines two generic interfaces, namely ``PropagationLossModel`` and ``PropagationDelayModel``, for the modeling of respectively propagation loss and propagation delay.


++++++++++++++++++++
PropagationLossModel
++++++++++++++++++++


Each of the available propagation loss models of ns-3 is explained in
one of the following subsections.



FriisPropagationLossModel
++++++++++++++++++++++++++++++++++++


TwoRayGroundPropagationLossModel
++++++++++++++++++++++++++++++++


LogDistancePropagationLossModel
+++++++++++++++++++++++++++++++

ThreeLogDistancePropagationLossModel
++++++++++++++++++++++++++++++++++++

JakesPropagationLossModel
+++++++++++++++++++++++++

PropagationLossModel
++++++++++++++++++++

RandomPropagationLossModel
++++++++++++++++++++++++++

NakagamiPropagationLossModel
++++++++++++++++++++++++++++

FixedRssLossModel
+++++++++++++++++

MatrixPropagationLossModel
++++++++++++++++++++++++++

RangePropagationLossModel
+++++++++++++++++++++++++




OkumuraHataPropagationLossModel
+++++++++++++++++++++++++++++++

This model is used to model open area pathloss for long distance (i.e., > 1 Km). In order to include all the possible frequencies usable by LTE we need to consider several variants of the well known Okumura Hata model. In fact, the original Okumura Hata model [hata]_ is designed for frequencies ranging from 150 MHz to 1500 MHz, the COST231 [cost231]_ extends it for the frequency range from 1500 MHz to 2000 MHz. Another important aspect is the scenarios considered by the models, in fact the all models are originally designed for urban scenario and then only the standard one and the COST231 are extended to suburban, while only the standard one has been extended to open areas. Therefore, the model cannot cover all scenarios at all frequencies. In the following we detail the models adopted.



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




ItuR1411LosPropagationLossModel
+++++++++++++++++++++++++++++++

This model is designed for Line-of-Sight (LoS) short range outdoor communication in the frequency range 300 MHz to 100 GHz.  This model provides an upper and lower bound respectively according to the following formulas

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


ItuR1411NlosOverRooftopPropagationLossModel
+++++++++++++++++++++++++++++++++++++++++++

This model is designed for Non-Line-of-Sight (LoS) short range outdoor communication over rooftops in the frequency range 300 MHz to 100 GHz. This model includes several scenario-dependent parameters, such as average street width, orientation, etc. It is advised to set the values of these parameters manually (using the ns-3 attribute system) according to the desired scenario. 

In detail, the model is based on [walfisch]_ and [ikegami]_, where the loss is expressed as the sum of free-space loss (:math:`L_{bf}`), the diffraction loss from rooftop to street (:math:`L_{rts}`) and the reduction due to multiple screen diffraction past rows of building (:math:`L_{msd}`). The formula is:

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



Kun2600MhzPropagationLossModel
++++++++++++++++++++++++++++++

This is the empirical model for the pathloss at 2600 MHz for urban areas which is described in [kun2600mhz]_. The model is as follows. Let :math:`d` be the distance between the transmitter and the receiver in meters; the pathloss :math:`L` in dB is calculated as:

.. math::

  L = 36 + 26\log{d}




+++++++++++++++++++++
PropagationDelayModel
+++++++++++++++++++++


The following propagation delay models are implemented:

PropagationDelayModel
+++++++++++++++++++++

RandomPropagationDelayModel
+++++++++++++++++++++++++++

ConstantSpeedPropagationDelayModel
++++++++++++++++++++++++++++++++++





.. [hata] M.Hata, "Empirical formula for propagation loss in land mobile radio
   services", IEEE Trans. on Vehicular Technology, vol. 29, pp. 317-325, 1980

.. [cost231] “Digital Mobile Radio: COST 231 View on the Evolution Towards 3rd Generation Systems”, Commission of the European Communities, L-2920, Luxembourg, 1989

.. [walfisch]  J.Walfisch and H.L. Bertoni, "A Theoretical model of UHF propagation in urban environments," in IEEE Trans. Antennas Propagat., vol.36, 1988, pp.1788- 1796

.. [ikegami] F.Ikegami, T.Takeuchi, and S.Yoshida, "Theoretical prediction of mean field strength for Urban Mobile Radio", in IEEE Trans. Antennas Propagat., Vol.39, No.3, 1991

.. [kun2600mhz] Sun Kun, Wang Ping, Li Yingze, "Path loss models for suburban scenario at 2.3GHz, 2.6GHz and 3.5GHz",
   in Proc. of the 8th International Symposium on Antennas, Propagation and EM Theory (ISAPE),  Kunming,  China, Nov 2008.

