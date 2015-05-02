.. include:: replace.txt
.. highlight:: cpp

Propagation
-----------

The |ns3| propagation module defines two generic interfaces, namely :cpp:class:`PropagationLossModel`
and :cpp:class:`PropagationDelayModel`, for the modeling of respectively propagation loss and propagation delay.

PropagationLossModel
********************

Propagation loss models calculate the Rx signal power considering the Tx signal power and the
mutual Rx and Tx antennas positions.

A propagation loss model can be "chained" to another one, making a list. The final Rx power 
takes into account all the chained models. In this way one can use a slow fading and a fast 
fading model (for example), or model separately different fading effects.

The following propagation delay models are implemented:

* Cost231PropagationLossModel
* FixedRssLossModel
* FriisPropagationLossModel
* ItuR1411LosPropagationLossModel
* ItuR1411NlosOverRooftopPropagationLossModel
* JakesPropagationLossModel
* Kun2600MhzPropagationLossModel
* LogDistancePropagationLossModel
* MatrixPropagationLossModel
* NakagamiPropagationLossModel
* OkumuraHataPropagationLossModel
* RandomPropagationLossModel
* RangePropagationLossModel
* ThreeLogDistancePropagationLossModel
* TwoRayGroundPropagationLossModel

Other models could be available thanks to other modules, e.g., the ``building`` module.

Each of the available propagation loss models of ns-3 is explained in
one of the following subsections.

FriisPropagationLossModel
=========================

This model implements the Friis propagation loss model. This model was first described in [friis]_.
The original equation was described as:

.. math::

  \frac{P_r}{P_t} = \frac{A_r A_t}{d^2\lambda^2}

with the following equation for the case of an isotropic antenna with no heat loss:
 
.. math::

  A_{isotr.} = \frac{\lambda^2}{4\pi}

The final equation becomes:

.. math::

  \frac{P_r}{P_t} = \frac{\lambda^2}{(4 \pi d)^2}

Modern extensions to this original equation are:

.. math::

  P_r = \frac{P_t G_t G_r \lambda^2}{(4 \pi d)^2 L}

With:

  :math:`P_t` : transmission power (W)
  
  :math:`P_r` : reception power (W)
  
  :math:`G_t` : transmission gain (unit-less)
  
  :math:`G_r` : reception gain (unit-less)
  
  :math:`\lambda` : wavelength (m)
  
  :math:`d` : distance (m)
  
  :math:`L` : system loss (unit-less)

In the implementation, :math:`\lambda` is calculated as 
:math:`\frac{C}{f}`, where :math:`C = 299792458` m/s is the speed of light in
vacuum, and :math:`f` is the frequency in Hz which can be configured by
the user via the Frequency attribute.

The Friis model is valid only for propagation in free space within
the so-called far field region, which can be considered
approximately as the region for :math:`d > 3 \lambda`.
The model will still return a value for :math:`d > 3 \lambda`, as
doing so (rather than triggering a fatal error) is practical for
many simulation scenarios. However, we stress that the values
obtained in such conditions shall not be considered realistic. 

Related with this issue, we note that the Friis formula is
undefined for :math:`d = 0`, and results in 
:math:`P_r > P_t` for :math:`d < \lambda / 2 \sqrt{\pi}`.

Both these conditions occur outside of the far field region, so in
principle the Friis model shall not be used in these conditions. 
In practice, however, Friis is often used in scenarios where accurate
propagation modeling is not deemed important, and values of 
:math:`d = 0` can occur.

To allow practical use of the model in such
scenarios, we have to 1) return some value for :math:`d = 0`, and
2) avoid large discontinuities in propagation loss values (which
could lead to artifacts such as bogus capture effects which are
much worse than inaccurate propagation loss values). The two issues
are conflicting, as, according to the Friis formula, 
:math:`\lim_{d \to 0}  P_r = +\infty`;
so if, for :math:`d = 0`, we use a fixed loss value, we end up with an infinitely large
discontinuity, which as we discussed can cause undesirable
simulation artifacts.

To avoid these artifact, this implementation of the Friis model
provides an attribute called MinLoss which allows to specify the
minimum total loss (in dB) returned by the model. This is used in
such a way that 
:math:`P_r` continuously increases for :math:`d \to 0`, until
MinLoss is reached, and then stay constant; this allow to
return a value for :math:`d = 0` and at the same time avoid
discontinuities. The model won't be much realistic, but at least
the simulation artifacts discussed before are avoided. The default value of
MinLoss is 0 dB, which means that by default the model will return 
:math:`P_r = P_t` for :math:`d <= \lambda / 2 \sqrt{\pi}`.
We note that this value of :math:`d` is outside of the far field
region, hence the validity of the model in the far field region is
not affected.


TwoRayGroundPropagationLossModel
================================

This model implements a Two-Ray Ground propagation loss model ported from NS2

The Two-ray ground reflection model uses the formula

.. math::

  P_r = \frac{P_t * G_t * G_r * (H_t^2 * H_r^2)}{d^4 * L}

The original equation in Rappaport's book assumes :math:`L = 1`.
To be consistent with the free space equation, :math:`L` is added here.

:math:`H_t` and :math:`H_r` are set at the respective nodes :math:`z` coordinate plus a model parameter
set via SetHeightAboveZ.

The two-ray model does not give a good result for short distances, due to the
oscillation caused by constructive and destructive combination of the two
rays. Instead the Friis free-space model is used for small distances. 

The crossover distance, below which Friis is used, is calculated as follows:

.. math::

  dCross = \frac{(4 * \pi * H_t * H_r)}{\lambda}

In the implementation,  :math:`\lambda` is calculated as 
:math:`\frac{C}{f}`, where :math:`C = 299792458` m/s is the speed of light in
vacuum, and :math:`f` is the frequency in Hz which can be configured by
the user via the Frequency attribute.


LogDistancePropagationLossModel
===============================

This model implements a log distance propagation model.

The reception power is calculated with a so-called
log-distance propagation model:

.. math::

  L = L_0 + 10 n \log(\frac{d}{d_0})

where:

  :math:`n` : the path loss distance exponent
  
  :math:`d_0` : reference distance (m)
  
  :math:`L_0` : path loss at reference distance (dB)
  
  :math:`d` :  - distance (m)
  
  :math:`L` : path loss (dB)

When the path loss is requested at a distance smaller than
the reference distance, the tx power is returned.

ThreeLogDistancePropagationLossModel
====================================

This model implements a log distance path loss propagation model with three distance
fields. This model is the same as ns3::LogDistancePropagationLossModel
except that it has three distance fields: near, middle and far with
different exponents.

Within each field the reception power is calculated using the log-distance
propagation equation:

.. math::

  L = L_0 + 10 \cdot n_0 \log_{10}(\frac{d}{d_0})

Each field begins where the previous ends and all together form a continuous function.

There are three valid distance fields: near, middle, far. Actually four: the
first from 0 to the reference distance is invalid and returns txPowerDbm.

.. math::

  \underbrace{0 \cdots\cdots}_{=0} \underbrace{d_0 \cdots\cdots}_{n_0} \underbrace{d_1 \cdots\cdots}_{n_1} \underbrace{d_2 \cdots\cdots}_{n_2} \infty 

Complete formula for the path loss in dB:


.. math::

  \displaystyle L =
  \begin{cases}
  0 & d < d_0 \\
  L_0 + 10 \cdot n_0 \log_{10}(\frac{d}{d_0}) & d_0 \leq d < d_1 \\
  L_0 + 10 \cdot n_0 \log_{10}(\frac{d_1}{d_0}) + 10 \cdot n_1 \log_{10}(\frac{d}{d_1}) & d_1 \leq d < d_2 \\
  L_0 + 10 \cdot n_0 \log_{10}(\frac{d_1}{d_0}) + 10 \cdot n_1 \log_{10}(\frac{d_2}{d_1}) + 10 \cdot n_2 \log_{10}(\frac{d}{d_2})& d_2 \leq d
  \end{cases}

where:

  
  :math:`d_0, d_1, d_2` : three distance fields (m)
  
  :math:`n_0, n_1, n_2` : path loss distance exponent for each field (unitless)
  
  :math:`L_0` : path loss at reference distance (dB)
  
  :math:`d` :  - distance (m)
  
  :math:`L` : path loss (dB)

When the path loss is requested at a distance smaller than the reference
distance :math:`d_0`, the tx power (with no path loss) is returned. The
reference distance defaults to 1m and reference loss defaults to
:cpp:class:`FriisPropagationLossModel` with 5.15 GHz and is thus :math:`L_0` = 46.67 dB.

JakesPropagationLossModel
=========================

ToDo
````

RandomPropagationLossModel
==========================

The propagation loss is totally random, and it changes each time the model is called.
As a consequence, all the packets (even those between two fixed nodes) experience a random
propagation loss.

NakagamiPropagationLossModel
============================

This propagation loss model implements Nakagami-m fast fading propagation loss model.

The Nakagami-m distribution is applied to the power level. The probability density function is defined as

.. math::

  p(x; m, \omega) = \frac{2 m^m}{\Gamma(m) \omega^m} x^{2m - 1} e^{-\frac{m}{\omega} x^2} = 2 x \cdot p_{\text{Gamma}}(x^2, m, \frac{m}{\omega})
  
with :math:`m` the fading depth parameter and :math:`\omega` the average received power.

It is implemented by either a :cpp:class:`GammaRandomVariable` or a :cpp:class:`ErlangRandomVariable`
random variable.

Like in :cpp:class:ThreeLogDistancePropagationLossModel`, the :math:`m` parameter is varied
over three distance fields:

.. math::

  \underbrace{0 \cdots\cdots}_{m_0} \underbrace{d_1 \cdots\cdots}_{m_1} \underbrace{d_2 \cdots\cdots}_{m_2} \infty

For :math:`m = 1` the Nakagami-m distribution equals the Rayleigh distribution. Thus
this model also implements Rayleigh distribution based fast fading.

FixedRssLossModel
=================

This model sets a constant received power level independent of the transmit power.

The received power is constant independent of the transmit power; the user
must set received power level.  Note that if this loss model is chained to other loss
models, it should be the first loss model in the chain. 
Else it will disregard the losses computed by loss models that precede it in the chain. 

MatrixPropagationLossModel
==========================

The propagation loss is fixed for each pair of nodes and doesn't depend on their actual positions.
This model shoud be useful for synthetic tests. Note that by default the propagation loss is 
assumed to be symmetric.

RangePropagationLossModel
=========================

This propagation loss depends only on the distance (range) between transmitter and receiver.

The single MaxRange attribute (units of meters) determines path loss.
Receivers at or within MaxRange meters receive the transmission at the
transmit power level. Receivers beyond MaxRange receive at power
-1000 dBm (effectively zero).

OkumuraHataPropagationLossModel
===============================

This model is used to model open area pathloss for long distance (i.e., > 1 Km). 
In order to include all the possible frequencies usable by LTE we need to consider 
several variants of the well known Okumura Hata model. In fact, the original Okumura 
Hata model [hata]_ is designed for frequencies ranging from 150 MHz to 1500 MHz, 
the COST231 [cost231]_ extends it for the frequency range from 1500 MHz to 2000 MHz. 
Another important aspect is the scenarios considered by the models, in fact the all 
models are originally designed for urban scenario and then only the standard one and 
the COST231 are extended to suburban, while only the standard one has been extended 
to open areas. Therefore, the model cannot cover all scenarios at all frequencies. 
In the following we detail the models adopted.

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


The literature lacks of extensions of the COST231 to open area (for suburban it seems that 
we can just impose C = 0); therefore we consider it a special case fo the suburban one.


Cost231PropagationLossModel
===========================

ToDo
````

ItuR1411LosPropagationLossModel
===============================

This model is designed for Line-of-Sight (LoS) short range outdoor communication in the 
frequency range 300 MHz to 100 GHz.  This model provides an upper and lower bound 
respectively according to the following formulas

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
===========================================

This model is designed for Non-Line-of-Sight (LoS) short range outdoor communication over 
rooftops in the frequency range 300 MHz to 100 GHz. This model includes several scenario-dependent 
parameters, such as average street width, orientation, etc. It is advised to set the values of 
these parameters manually (using the ns-3 attribute system) according to the desired scenario. 

In detail, the model is based on [walfisch]_ and [ikegami]_, where the loss is expressed 
as the sum of free-space loss (:math:`L_{bf}`), the diffraction loss from rooftop to 
street (:math:`L_{rts}`) and the reduction due to multiple screen diffraction past 
rows of building (:math:`L_{msd}`). The formula is:

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


The multiple screen diffraction loss depends on the BS antenna height relative to the building 
height and on the incidence angle. The former is selected as the higher antenna in the communication 
link. Regarding the latter, the "settled field distance" is used for select the proper model; 
its value is given by

.. math::

  d_{s} = \frac{\lambda d^2}{\Delta h_{b}^2}

with

  :math:`\Delta h_b = h_b - h_m`

Therefore, in case of :math:`l > d_s` (where `l` is the distance over which the building extend), 
it can be evaluated according to

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
==============================

This is the empirical model for the pathloss at 2600 MHz for urban areas which is described in [kun2600mhz]_. 
The model is as follows. Let :math:`d` be the distance between the transmitter and the receiver 
in meters; the pathloss :math:`L` in dB is calculated as:

.. math::

  L = 36 + 26\log{d}


PropagationDelayModel
*********************

The following propagation delay models are implemented:

* ConstantSpeedPropagationDelayModel
* RandomPropagationDelayModel

ConstantSpeedPropagationDelayModel
==================================

In this model, the signal travels with constant speed.
The delay is calculated according with the trasmitter and receiver positions.
The Euclidean distance between the Tx and Rx antennas is used.
Beware that, according to this model, the Earth is flat.

RandomPropagationDelayModel
===========================

The propagation delay is totally random, and it changes each time the model is called.
All the packets (even those between two fixed nodes) experience a random delay.
As a consequence, the packets order is not preserved. 


References
**********

.. [friis] Friis, H.T., "A Note on a Simple Transmission Formula," Proceedings of the IRE , vol.34, no.5, pp.254,256, May 1946

.. [hata] M.Hata, "Empirical formula for propagation loss in land mobile radio
   services", IEEE Trans. on Vehicular Technology, vol. 29, pp. 317-325, 1980

.. [cost231] “Digital Mobile Radio: COST 231 View on the Evolution Towards 3rd Generation Systems”, Commission of the European Communities, L-2920, Luxembourg, 1989

.. [walfisch]  J.Walfisch and H.L. Bertoni, "A Theoretical model of UHF propagation in urban environments," in IEEE Trans. Antennas Propagat., vol.36, 1988, pp.1788- 1796

.. [ikegami] F.Ikegami, T.Takeuchi, and S.Yoshida, "Theoretical prediction of mean field strength for Urban Mobile Radio", in IEEE Trans. Antennas Propagat., Vol.39, No.3, 1991

.. [kun2600mhz] Sun Kun, Wang Ping, Li Yingze, "Path loss models for suburban scenario at 2.3GHz, 2.6GHz and 3.5GHz",
   in Proc. of the 8th International Symposium on Antennas, Propagation and EM Theory (ISAPE),  Kunming,  China, Nov 2008.

