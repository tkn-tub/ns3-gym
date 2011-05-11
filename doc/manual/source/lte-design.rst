.. include:: replace.txt


++++++++++++++++++++++++++
 LTE Design Documentation
++++++++++++++++++++++++++

-----------------------------------------------------------------------------

* the new eutra stack diagram with all SAPs, differentiated between eNB and UE
* the explanation of the new example program


-----------------------------------------------------------------------------

Radio Resource Management and Packet Scheduling
###############################################

For how the ns-3 LTE module handles Radio Resource Management and Packet Scheduling at the eNB, please see the :ref:`ff-mac-sched-api`.


Physical layer
##############

A :cpp:class:`ns3::LtePhy` class models the LTE PHY layer. 

Basic functionalities of the PHY layer are: (i) transmit packets coming from the device to the channel; (ii) receive packets from the channel; (ii) evaluate the quality of the channel from the Signal To Noise ratio of the received signal; and (iii) forward received packets to the device.

Both the PHY and channel have been developed extending :cpp:class:`ns3::SpectrumPhy` and 
:cpp:class:`ns3::SpectrumChannel` classes, respectively, which are provided by the Spectrum Framework [1]_.
 
The module implements an FDD channel access. In FDD channel access, downlink and uplink 
transmissions work together in the time but using a different set of frequencies.  
Since DL and UL are indipendent between them, the PHY is composed by couple of 
:cpp:class:`ns3::LteSpectrumPhy` object, one for the downlink and one for the uplink.
The :cpp:class:`ns3::LtePhy` stores and manages both downlink and uplink 
:cpp:class:`ns3::LteSpectrumPhy` elements. 

In order to customize all physical functionalities for both UE and eNB devices, dedicated 
classes have been inherited from ones described before. In particular, 
:cpp:class:`ns3::LteUePhy` and :cpp:class:`ns3::LteEnbPhy` classes, inherited from 
the :cpp:class:`ns3::LtePhy` class, implement the PHY layer for the UE and the 
eNB, respectively. 

The figure below shows how UE and eNB can exchange packets through the considered PHY layer.

.. _lte-transmission:

.. figure:: figures/lte-transmission.png

    DL and UL transmision on the LTE network

For the downlink, when the eNB whants to send packets, it calls the ``StartTx`` function to 
send them into the downlink channel. Then, the downlink channel delivers the burst 
of packets to all the :cpp:class:`ns3::UeLteSpectrumPhy` attached to it, handling the 
``StartRx`` function. 
When the UE receives packets, it executes the following tasks:

* compute the SINR for all the sub channel used in the downlink

* create and send CQI feedbacks

* forward all the received packets to the MAC layer through the PHY SAP.

The uplink works similary.

Propagation Loss Models
#######################

A proper propagation loss model has been developed for the LTE E-UTRAN interface (see [2]_ and [3]_).
It is used by the PHY layer to compute the loss due to the propagation. 

The LTE propagation loss model is composed by 4 different models (shadowing, multipath, 
penetration loss and path loss) [2]_:

* Pathloss: :math:`PL = 128.1 + (37.6 * log10 (R))`, where R is the distance between the 
  UE and the eNB in Km.

* Multipath: Jakes model

* PenetrationLoss: 10 dB

* Shadowing: log-normal distribution (mean=0dB, standard deviation=8dB)

Every time that the ``LteSpectrumPHY::StartRx ()`` function is called, the 
``SpectrumInterferenceModel`` is used to computed the SINR, as proposed in [3]_. Then, 
the network device uses the AMC module to map the SINR to a proper CQI and to send it 
to the eNB using the ideal control channel.


References
----------

.. [1] N. Baldo and M. Miozzo, Spectrum-aware Channel and PHY layer modeling for ns3, Proceedings 
       of ICST NSTools 2009, Pisa, Italy. The framework is designed to simulate only data 
       transmissions. For the transmission of control messages (such as CQI feedback, PDCCH, 
       etc..) will be used an ideal control channel). 

.. [2] 3GPP TS 25.814 ( http://www.3gpp.org/ftp/specs/html-INFO/25814.htm ) 

.. [3] Giuseppe Piro, Luigi Alfredo Grieco, Gennaro Boggia, and Pietro Camarda", A Two-level 
       Scheduling Algorithm for QoS Support in the Downlink of LTE Cellular Networks", Proc. of 
       European Wireless, EW2010, Lucca, Italy, Apr., 2010 ( draft version is available on 
       http://telematics.poliba.it/index.php?option=com_jombib&task=showbib&id=330 )

.. [4] 3GPP R1-081483 (available on 
       http://www.3gpp.org/ftp/tsg_ran/WG1_RL1/TSGR1_52b/Docs/R1-081483.zip )
