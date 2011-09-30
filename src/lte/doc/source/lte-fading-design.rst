.. include:: replace.txt


++++++++++++++++++++++++++++++++++++++
 Trace Fading Loss Model Documentation
++++++++++++++++++++++++++++++++++++++


GSoC Model
++++++++++

The fading model of GSoC [Piro2011] is based on the Jakes Simulator. In order to limit the computational complexity, the fading is evaluated run-time by query pre-calculated traces. In detail, the simulator includes a set of fading traces 3 seconds long sampled at 1 ms for different values of speeds (i.e., 0, 3, 30, 60, 120 Kmph) and different number of signaling paths (e.g., 6, 8, 10 and 12). The traces are internally managed by loading a window (with default value of 0.5 seconds) picked up with a start instant uniformly distributed in the whole duration of a trace. The trace to be used is selected according to users' relative speed and the number of path are uniformly picked up among the available ones each window update. This implies that the traces are generated according to a specific  speed and number of taps and then shared among users and RBs at the same time (by randomly select the beginning of the window).
The Jakes Simulator used for generating the traces is a proprietary implementation of the algorithm proposed by Jakes done in Matlab which is not part of the official GSoC release.

Pros.

 * Reduce size of the traces (one trace shared among users and RBs)
 * Already included in LENA

Cons.

 * The frequency selectivity is modeled by randomly pick up the window in the traces (i.e., there is no correlation between adjacent RBs); this modelization does not have reference in literature
 * the total length of the trace might be low (3 seconds) for generating independent windows which might be potentially 100 RBs x (no. of users x no. of BSs)
 * the number of path used is randomly pick up each window actualization (0.5 seconds); therefore it changes during simulation run-time each window.


Proposed Model
++++++++++++++

We suggest the implementation of a similar model relaxing some assumptions. In detail, we propose a new trace format and trace management procedure. Regarding the model of the channel, we suggest the one provided by the ``rayleighchan`` function of Matlab since it provides a well accepted channel modelization both in time and frequency domain with the parameters we are considering, more information following the link:

http://www.mathworks.es/help/toolbox/comm/ug/a1069449399.html#bq5zk36

We note that, this solution allows the realization of a matlab simulation script for allowing users the generation of new traces based on specific scenario tailored needs.

Analyzing the possible channel parameters, we identified the following ones as representative for accurately simulating the channel in the simulator:

 * granularity in frequency, three options are available

   * one value for each RB
   * one value per group of RBs (size to be dimensioned)
   * the same trace for all RBs but using a random offset in the trace (as done in GSoC module)

 * length of the trace: ideally large as the simulation time, might be reduced by applying the window mechanism of GSoC
 * granularity in time: the sampling time (1 ms in GSoC) of the fading might be tuned according scenario characteristics (i.e., channel low or high variable in time)
 * speed values: speed affects the channel by varying the Doppler shift, a specific speed needs the generation of its correspondent traces
 * number of set of taps: number of different path of the signal (scenario dependent)

Since the number of variable it is pretty high, generate traces considering all of them might produce a high number of traces of huge size. The following formula express in detail the dimension:

.. math::
 T_{SIZE} = 8 \times RB_{NUM} \times \frac{T_{total}}{T_{sample}} \times DopplerShift_{NUM} \times |Taps_{NUM}|\mbox{ [bytes]}

where :math:`RB_{NUM}` is the number of RB or set of RBs to be considered, :math:`T_{total}` is the total length of the trace, :math:`T_{sample}` is the sampling period (1 ms for having a new sample each subframe), :math:`DopplerShift_{NUM}` is the number of Doppler frequencies to be used and :math:`Taps_{NUM}` is the number of taps.
According to the formula we have that a typical single channel realization (i.e., independent RB traces at a given speed and given set of number of taps with one sample per ms/TTI) implies the usage of 8,000,000 bytes (7.6294 MB) considering the precision of double (:math:`1\times10^{-308}` to :math:`1\times10^{308}`). This size can be halved by considering float numbers instead of double, reducing the precision to :math:`1\times10^{-37}` to :math:`1\times10^{37}` (and possibly increasing the computational complexity due to the cast operation between float and double, which is what is used in the simulator; this is compilator dependent and can be evaluated after implementation).
The number of taps seems to be limited to 7 in case of pedestrian scenario and 9 in case of vehicular and urban model, according to Annex B.2 of 3GPP TS 36.104. This implies, that we need at most 3 different taps configurations, which are static during the simulation.

A typical set of traces for a simulation will result therefore in:

 * :math:`RB_{NUM}`: 100
 * :math:`T_{total}`: 10 secs
 * :math:`T_{sample}`: 0.001 secs (1 ms as a subframe)
 * :math:`DopplerShift_{NUM}`: 0, 10 and 50 Hz for modeling (static, pedestrian a slow vehicle)
 * :math:`Taps_{NUM}`: 3

which results in 72,000,000 bytes (~6.86 MB) of traces with double precision.
If instead of using a single trace for all UEs (with random offset UE) we want to have one independent channel trace for each UE, we need to multiply that size by the desired number of independent traces.