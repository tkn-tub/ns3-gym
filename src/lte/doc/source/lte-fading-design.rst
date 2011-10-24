.. include:: replace.txt


++++++++++++++++++++++++++++++++++++++
 Trace Fading Loss Model Documentation
++++++++++++++++++++++++++++++++++++++


Jakes Fading Trace Model
++++++++++++++++++++++++

Fading Traces Model Description
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The fading model is based on the one developed during the GSoC 2010 [Piro2011]. The main characteristic of this model is the fact that the fading evaluation during simulation run-time is based on per-calculated traces. This is done for limiting the computational complexity of the simulator. On the other hand, it needs huge structures for storing the traces; therefore, a trade-off between the number of possible parameters and the memory occupancy has to be found. The most important ones are:

 * users' speed: relative speed between users (affects the Doppler frequency, which in turns affects the time-variance property of the fading)
 * number of taps (and relative power): number of multiple paths considered, which affects the frequency property of the fading.
 * time granularity of the trace: sampling time of the trace.
 * frequency granularity of the trace: number of values in frequency to be evaluated.
 * length of trace: ideally large as the simulation time, might be reduced by windowing mechanism.
 * number of users: number of independent traces to be used (ideally one trace per user).

Respect to the mathematical channel propagation model, we suggest the one provided by the ``rayleighchan`` function of Matlab since it provides a well accepted channel modelization both in time and frequency domain, more information following the link:

http://www.mathworks.es/help/toolbox/comm/ug/a1069449399.html#bq5zk36

The simulator provides a matlab script (``/lte/model/JakesTraces/fading-trace-generator.m``) for generating traces based on the format used by the simulator. 
In detail, the channel object created with the rayleighchan function is used to filter a discrete-time impulse signal in order to obtain the channel impulse response. The filtering is repeated for different TTI, thus yielding subsequent time-correlated channel responses (one per TTI). The channel response is then processed with the ``pwelch`` function for obtaining its power spectral density values, which are then saved in a file with the proper format compatible with the simulator model. For sake of clarity, the names of the trace and of the file created can be completed with user defined tag for distinguish them. The file produced is a standard c header file with the definition of the vector containing the trace. This file has to be included in the ``JakesFadingLossModel`` class implementation and the code has to be updated for managing the generated array with the fading trace according to the parameters combination defined by the user at the generation.

Since the number of variable it is pretty high, generate traces considering all of them might produce a high number of traces of huge size. On this matter, we considered the following assumptions of the parameters based on the 3GPP fading propagation conditions (see TS 36.104 Annex B.2):

 * users' speed: consider the most common typical value

   * 0 and 3 kmph for pedestrian scenarios
   * 30 and 60 kmph for vehicular scenarios
   * 0, 3, 30 and 60 for urban scenarios

 * number of taps: use the three models presented in Annex B.2 of TS 36.104.
 * time granularity: 1 ms (as the simulator granularity in time).
 * frequency granularity: per RB basis (which implies 100 RBs, as the simulator granularity in frequency).
 * length of the trace: the simulator includes the windowing mechanism implemented during the GSoC 2011, which consists of picking up a window of the trace each window length in a random fashion.
 * number of users: users share the same fading trace, but the windows are independent among users which randomly pick up their starting point.

According to the parameters we considered, the following formula express in detail the dimension:

.. math::
 T_{SIZE} = Sample_{size} \times RB_{NUM} \times \frac{T_{total}}{T_{sample}} \times Speed_{NUM} \times Scenarios_{NUM} \mbox{ [bytes]}

where :math:`Sample_{size}` is the size in bytes of the sample (e.g., 8 in case of double precision, 4 in case of float precision), :math:`RB_{NUM}` is the number of RB or set of RBs to be considered, :math:`T_{total}` is the total length of the trace, :math:`T_{sample}` is the sampling period (1 ms for having a new sample each subframe), :math:`Speed_{NUM}` is the number of users relative speeds considered and :math:`Scenarios_{NUM}` is the number of scenarios (i.e., different taps configurations).
According to the formula we have that a typical single channel realization (i.e., independent RB traces at a given speed and given set of number of taps with one sample per ms/TTI)) implies the usage of 8,000,000 bytes (7.6294 MB) considering the precision of double (:math:`1\times10^{-308}` to :math:`1\times10^{308}`). This size can be halved by considering float numbers instead of double, reducing the precision to :math:`1\times10^{-37}` to :math:`1\times10^{37}` (and possibly increasing the computational complexity due to the cast operation between float and double, which is what is used in the simulator; this is compilator dependent and can be evaluated after implementation ``TBD``).

A typical set of traces for a simulation will result therefore in:

 * :math:`RB_{NUM}`: 100
 * :math:`T_{total}`: 10 secs
 * :math:`T_{sample}`: 0.001 secs (1 ms as a subframe)
 * :math:`Speed_{NUM}`: 1 speed per scenarios (e.g. 3 kmph for pedestrian)
 * :math:`Scenarios_{NUM}`: 1 (pedestrian)

which results in 8,000,000 bytes (~8 MB) of traces with double precision.

Fading Traces Generation
~~~~~~~~~~~~~~~~~~~~~~~~

Thanks to the matlab script provided with the code (``/lte/model/JakesTraces/fading-trace-generator.m``) it is possible to generate traces according to specific simulation scenarios. The script already includes the typical taps configurations for three 3GPP scenarios (i.e., pedestrian, vehicular and urban); however users can introduce their specific configurations. The list of the configurable parameters is provided in the following:

 * ``fc`` : the frequency in use (it affects the computation of the dopples speed).
 * ``v_km_h`` : the speed of the users
 * ``traceDuration`` : the duration in seconds of the total length of the trace.
 * ``numRBs`` : the number of the resource block to be evaluated. 
 * ``tag`` : the tag to be applied to the file generated.

The file generated is formatted in a matrix fashion by putting each RBs temporal fading trace samples in different rows.

Fading Traces Usage
~~~~~~~~~~~~~~~~~~~

The proper set of the trace parameters in the simulation is of paramount importance for the correct usage of the trace itself within the simulator.
The list of the parameters to be configured are:

 * ``TraceFilename`` : the name of the trace to be loaded (absolute path o relative one according to the execution point of the script).
 * ``TraceLength`` : the trace duration in seconds.
 * ``SamplesNum`` : the number of samples.
 * ``WindowSize`` : the size of the fading sampling window in seconds.

It is important to highlight that the sampling interval of the fading trace has to me at most of 1 ms or greater and in the latter case it has to be an integer multiple of 1 ms in order to be correctly processed by the fading module. 

In order to activate the fading module (by default disactivated) it has to be explicity specified in the simulation script::

  Ptr<LenaHelper> lena = CreateObject<LenaHelper> ();
  lena->SetFadingModel("ns3::TraceFadingLossModel");

While, for setting the parameters::

  lena->SetFadingModelAttribute ("TraceFilename", StringValue ("src/lte/model/FadingTraces/fading_trace_EPA_3kmph.fad"));
  lena->SetFadingModelAttribute ("TraceLength", TimeValue (Seconds (10.0)));
  lena->SetFadingModelAttribute ("SamplesNum", UintegerValue (10000));
  lena->SetFadingModelAttribute ("WindowSize", TimeValue (Seconds (0.5)));
  lena->SetFadingModelAttribute ("RbNum", UintegerValue (100));

Apart of ``TraceFilename``, the parameters have already a default value according to the standard format generated (i.e., the same values used in the example above).



