EPSTOPDF = epstopdf
DIA = dia
CONVERT = convert

SRC = ../../src
# Temporary source directory, for build
SOURCETEMP = source-temp
FIGURES = $(SOURCETEMP)/figures

# list all model library .rst files that need to be copied to $SOURCETEMP
SOURCES = \
	source/conf.py \
	source/_static \
	source/index.rst \
	source/replace.txt \
	source/organization.rst \
	source/internet-models.rst \
	source/network.rst \
	source/emulation-overview.rst \
	$(SRC)/antenna/doc/source/antenna.rst \
	$(SRC)/antenna/doc/source/antenna-design.rst \
	$(SRC)/antenna/doc/source/antenna-user.rst \
	$(SRC)/antenna/doc/source/antenna-testing.rst \
	$(SRC)/aodv/doc/aodv.rst \
	$(SRC)/applications/doc/applications.rst \
	$(SRC)/bridge/doc/bridge.rst \
	$(SRC)/brite/doc/brite.rst \
	$(SRC)/buildings/doc/source/buildings.rst \
	$(SRC)/buildings/doc/source/buildings-design.rst \
	$(SRC)/buildings/doc/source/buildings-user.rst \
	$(SRC)/buildings/doc/source/buildings-testing.rst \
	$(SRC)/buildings/doc/source/buildings-references.rst \
	$(SRC)/click/doc/click.rst \
	$(SRC)/csma/doc/csma.rst \
	$(SRC)/dsdv/doc/dsdv.rst \
	$(SRC)/dsr/doc/dsr.rst \
	$(SRC)/emu/doc/emu.rst \
	$(SRC)/mpi/doc/distributed.rst \
	$(SRC)/energy/doc/energy.rst \
	$(SRC)/fd-net-device/doc/fd-net-device.rst \
	$(SRC)/tap-bridge/doc/tap.rst \
	$(SRC)/mesh/doc/mesh.rst \
	$(SRC)/lte/doc/source/lte.rst \
	$(SRC)/lte/doc/source/lte-user.rst \
	$(SRC)/lte/doc/source/lte-design.rst \
	$(SRC)/lte/doc/source/lte-testing.rst \
	$(SRC)/lte/doc/source/lte-profiling.rst \
	$(SRC)/lte/doc/source/lte-references.rst \
	$(SRC)/propagation/doc/propagation.rst \
	$(SRC)/network/doc/network-overview.rst \
	$(SRC)/network/doc/packets.rst \
	$(SRC)/network/doc/error-model.rst \
	$(SRC)/network/doc/sockets-api.rst \
	$(SRC)/network/doc/simple.rst \
	$(SRC)/network/doc/queue.rst \
	$(SRC)/internet/doc/internet-stack.rst \
	$(SRC)/internet/doc/ipv4.rst \
	$(SRC)/internet/doc/ipv6.rst \
	$(SRC)/internet/doc/routing-overview.rst \
	$(SRC)/internet/doc/tcp.rst \
	$(SRC)/internet/doc/codel.rst \
	$(SRC)/mobility/doc/mobility.rst \
	$(SRC)/olsr/doc/olsr.rst \
	$(SRC)/openflow/doc/openflow-switch.rst \
	$(SRC)/point-to-point/doc/point-to-point.rst \
	$(SRC)/wifi/doc/wifi.rst \
	$(SRC)/wimax/doc/wimax.rst \
	$(SRC)/uan/doc/uan.rst \
	$(SRC)/topology-read/doc/topology.rst \
	$(SRC)/spectrum/doc/spectrum.rst \
	$(SRC)/stats/doc/adaptor.rst \
	$(SRC)/stats/doc/aggregator.rst \
	$(SRC)/stats/doc/collector.rst \
	$(SRC)/stats/doc/data-collection-helpers.rst \
	$(SRC)/stats/doc/data-collection-overview.rst \
	$(SRC)/stats/doc/data-collection.rst \
	$(SRC)/stats/doc/probe.rst \
	$(SRC)/stats/doc/scope-and-limitations.rst \
	$(SRC)/stats/doc/statistics.rst \
	$(SRC)/netanim/doc/animation.rst \
	$(SRC)/flow-monitor/doc/flow-monitor.rst \
	$(SRC)/wave/doc/wave.rst \
	$(SRC)/sixlowpan/doc/sixlowpan.rst \
	$(SRC)/lr-wpan/doc/lr-wpan.rst \

# list all model library figure files that need to be copied to 
# $SOURCETEMP/figures.  For each figure to be included in all
# documentation formats (html, latex...) the following formats are supported: 
#  1) a single .dia file (preferred option, because it can be edited)
#  2) a single .eps file
#  3) both a .pdf and .png file
SOURCEFIGS = \
	figures/testbed.dia \
	figures/emulated-channel.dia \
	$(SRC)/antenna/doc/source/figures/antenna-coordinate-system.dia	\
	$(SRC)/network/doc/packet.dia \
	$(SRC)/network/doc/node.dia \
	$(SRC)/network/doc/buffer.dia \
	$(SRC)/network/doc/sockets-overview.dia \
	$(SRC)/internet/doc/internet-node-send.dia \
	$(SRC)/internet/doc/internet-node-recv.dia \
	$(SRC)/internet/doc/routing.dia \
	$(SRC)/internet/doc/routing-specialization.dia \
	$(SRC)/wifi/doc/WifiArchitecture.dia \
	$(SRC)/wifi/doc/snir.dia \
	$(SRC)/wimax/doc/WimaxArchitecture.dia \
	$(SRC)/lte/doc/source/figures/epc-ctrl-arch.dia \
	$(SRC)/lte/doc/source/figures/epc-data-flow-dl.dia \
	$(SRC)/lte/doc/source/figures/epc-data-flow-ul.dia \
	$(SRC)/lte/doc/source/figures/epc-profiling-scenario.dia \
	$(SRC)/lte/doc/source/figures/epc-topology.dia \
	$(SRC)/lte/doc/source/figures/epc-topology-x2-enhanced.dia \
	$(SRC)/lte/doc/source/figures/eutran-profiling-scenario.dia \
	$(SRC)/lte/doc/source/figures/ff-example.dia \
	$(SRC)/lte/doc/source/figures/ff-mac-saps.dia \
	$(SRC)/lte/doc/source/figures/lte-arch-enb-data.dia \
	$(SRC)/lte/doc/source/figures/lte-arch-enb-ctrl.dia \
	$(SRC)/lte/doc/source/figures/lte-arch-ue-data.dia \
	$(SRC)/lte/doc/source/figures/lte-arch-ue-ctrl.dia \
	$(SRC)/lte/doc/source/figures/lte-enb-phy.dia \
	$(SRC)/lte/doc/source/figures/lte-ue-phy.dia \
	$(SRC)/lte/doc/source/figures/lte-epc-x2-handover-seq-diagram.dia \
	$(SRC)/lte/doc/source/figures/lte-epc-e2e-data-protocol-stack.dia \
	$(SRC)/lte/doc/source/figures/lte-interference-test-scenario.dia \
	$(SRC)/lte/doc/source/figures/lte-subframe-structure.dia \
	$(SRC)/lte/doc/source/figures/lte-epc-x2-interface.dia \
	$(SRC)/lte/doc/source/figures/lte-harq-architecture.dia \
	$(SRC)/lte/doc/source/figures/lte-harq-processes-scheme.dia \
	$(SRC)/lte/doc/source/figures/ue-meas-consumer.dia \
	$(SRC)/lte/doc/source/figures/ue-meas-piecewise-motion.dia \
	$(SRC)/lte/doc/source/figures/ue-meas-piecewise-a1.dia \
	$(SRC)/lte/doc/source/figures/ue-meas-piecewise-a1-hys.dia \
	$(SRC)/lte/doc/source/figures/lte-cell-selection-timeline.dia \
	$(SRC)/lte/doc/source/figures/lte-cell-selection-scenario.dia \
	$(SRC)/lte/doc/source/figures/lte-handover-target-scenario.dia \
	$(SRC)/lte/doc/source/figures/lena-dual-stripe.eps \
	$(SRC)/lte/doc/source/figures/lte-mcs-index.eps \
	$(SRC)/lte/doc/source/figures/lenaThrTestCase1.eps \
	$(SRC)/lte/doc/source/figures/lenaThrTestCase2.eps \
	$(SRC)/lte/doc/source/figures/runningTime10s.eps \
	$(SRC)/lte/doc/source/figures/epcRunningTime.eps \
	$(SRC)/lte/doc/source/figures/propagationModel.eps \
	$(SRC)/lte/doc/source/figures/simulationTime.eps \
	$(SRC)/lte/doc/source/figures/epcSimulationTime.eps \
	$(SRC)/lte/doc/source/figures/epcEutranRunningTime.eps \
	$(SRC)/lte/doc/source/figures/profiling-memory.eps \
	$(SRC)/lte/doc/source/figures/lte-rlc-implementation-model.eps \
	$(SRC)/lte/doc/source/figures/lte-rlc-data-txon-dl.eps \
	$(SRC)/lte/doc/source/figures/lte-rlc-data-retx-dl.eps \
	$(SRC)/lte/doc/source/figures/lte-rlc-data-txon-ul.eps \
	$(SRC)/lte/doc/source/figures/lte-rlc-data-retx-ul.eps \
	$(SRC)/lte/doc/source/figures/lte-epc-x2-entity-saps.eps \
	$(SRC)/lte/doc/source/figures/lte-strongest-cell-handover-algorithm.eps \
	$(SRC)/lte/doc/source/figures/lte-phy-interference.pdf \
	$(SRC)/lte/doc/source/figures/lte-phy-interference.png \
	$(SRC)/lte/doc/source/figures/helpers.pdf \
	$(SRC)/lte/doc/source/figures/helpers.png \
	$(SRC)/lte/doc/source/figures/mac-random-access-contention.pdf \
	$(SRC)/lte/doc/source/figures/mac-random-access-contention.png \
	$(SRC)/lte/doc/source/figures/mac-random-access-noncontention.pdf \
	$(SRC)/lte/doc/source/figures/mac-random-access-noncontention.png \
	$(SRC)/lte/doc/source/figures/rrc-connection-establishment.pdf \
	$(SRC)/lte/doc/source/figures/rrc-connection-establishment.png \
	$(SRC)/lte/doc/source/figures/rrc-connection-reconfiguration.pdf \
	$(SRC)/lte/doc/source/figures/rrc-connection-reconfiguration.png \
	$(SRC)/lte/doc/source/figures/rrc-connection-reconfiguration-handover.pdf \
	$(SRC)/lte/doc/source/figures/rrc-connection-reconfiguration-handover.png \
	$(SRC)/lte/doc/source/figures/nas-attach.pdf \
	$(SRC)/lte/doc/source/figures/nas-attach.png \
	$(SRC)/lte/doc/source/figures/lte-enb-rrc-states.pdf \
	$(SRC)/lte/doc/source/figures/lte-enb-rrc-states.png \
	$(SRC)/lte/doc/source/figures/lte-ue-rrc-states.pdf \
	$(SRC)/lte/doc/source/figures/lte-ue-rrc-states.png \
	$(SRC)/lte/doc/source/figures/fading_pedestrian.pdf \
	$(SRC)/lte/doc/source/figures/fading_pedestrian.png \
	$(SRC)/lte/doc/source/figures/fading_vehicular.pdf \
	$(SRC)/lte/doc/source/figures/fading_vehicular.png \
	$(SRC)/lte/doc/source/figures/fading_urban_3kmph.pdf \
	$(SRC)/lte/doc/source/figures/fading_urban_3kmph.png \
	$(SRC)/lte/doc/source/figures/fr-enhanced-fractional-frequency-reuse-scheme.png \
	$(SRC)/lte/doc/source/figures/fr-enhanced-fractional-frequency-reuse-scheme.pdf \
	$(SRC)/lte/doc/source/figures/fr-full-frequency-reuse-scheme.png \
	$(SRC)/lte/doc/source/figures/fr-full-frequency-reuse-scheme.pdf \
	$(SRC)/lte/doc/source/figures/fr-hard-frequency-reuse-scheme.png \
	$(SRC)/lte/doc/source/figures/fr-hard-frequency-reuse-scheme.pdf \
	$(SRC)/lte/doc/source/figures/fr-soft-fractional-frequency-reuse-scheme.png \
	$(SRC)/lte/doc/source/figures/fr-soft-fractional-frequency-reuse-scheme.pdf \
	$(SRC)/lte/doc/source/figures/fr-soft-frequency-reuse-scheme-v1.png \
	$(SRC)/lte/doc/source/figures/fr-soft-frequency-reuse-scheme-v1.pdf \
	$(SRC)/lte/doc/source/figures/fr-soft-frequency-reuse-scheme-v2.png \
	$(SRC)/lte/doc/source/figures/fr-soft-frequency-reuse-scheme-v2.pdf \
	$(SRC)/lte/doc/source/figures/fr-strict-frequency-reuse-scheme.png \
	$(SRC)/lte/doc/source/figures/fr-strict-frequency-reuse-scheme.pdf \
	$(SRC)/lte/doc/source/figures/ffr-distributed-scheme.png \
	$(SRC)/lte/doc/source/figures/ffr-distributed-scheme.pdf \
	$(SRC)/lte/doc/source/figures/lte-fr-soft-1-rem.png \
	$(SRC)/lte/doc/source/figures/lte-fr-soft-1-rem.pdf \
	$(SRC)/lte/doc/source/figures/lte-ffr-soft-2-spectrum-trace.png \
	$(SRC)/lte/doc/source/figures/lte-ffr-soft-2-spectrum-trace.pdf \
	$(SRC)/lte/doc/source/figures/lte-fr-hard-1-rem.png \
	$(SRC)/lte/doc/source/figures/lte-fr-hard-1-rem.pdf \
	$(SRC)/lte/doc/source/figures/lte-fr-hard-2-rem.png \
	$(SRC)/lte/doc/source/figures/lte-fr-hard-2-rem.pdf \
	$(SRC)/lte/doc/source/figures/lte-fr-hard-3-rem.png \
	$(SRC)/lte/doc/source/figures/lte-fr-hard-3-rem.pdf \
	$(SRC)/lte/doc/source/figures/MCS_1_4.pdf \
	$(SRC)/lte/doc/source/figures/MCS_1_4.png \
	$(SRC)/lte/doc/source/figures/MCS_5_8.pdf \
	$(SRC)/lte/doc/source/figures/MCS_5_8.png \
	$(SRC)/lte/doc/source/figures/MCS_9_12.pdf \
	$(SRC)/lte/doc/source/figures/MCS_9_12.png \
	$(SRC)/lte/doc/source/figures/MCS_13_16.pdf \
	$(SRC)/lte/doc/source/figures/MCS_13_16.png \
	$(SRC)/lte/doc/source/figures/MCS_17_20.pdf \
	$(SRC)/lte/doc/source/figures/MCS_17_20.png \
	$(SRC)/lte/doc/source/figures/MCS_21_24.pdf \
	$(SRC)/lte/doc/source/figures/MCS_21_24.png \
	$(SRC)/lte/doc/source/figures/MCS_25_28.pdf \
	$(SRC)/lte/doc/source/figures/MCS_25_28.png \
	$(SRC)/lte/doc/source/figures/MCS_29_29.pdf \
	$(SRC)/lte/doc/source/figures/MCS_29_29.png \
	$(SRC)/lte/doc/source/figures/MCS_2_test.png \
	$(SRC)/lte/doc/source/figures/MCS_2_test.pdf \
	$(SRC)/lte/doc/source/figures/MCS_12_test.png \
	$(SRC)/lte/doc/source/figures/MCS_12_test.pdf \
	$(SRC)/lte/doc/source/figures/MCS_16_test.png \
	$(SRC)/lte/doc/source/figures/MCS_16_test.pdf \
	$(SRC)/lte/doc/source/figures/miesm_scheme.pdf \
	$(SRC)/lte/doc/source/figures/miesm_scheme.png \
	$(SRC)/lte/doc/source/figures/lte-dl-power-control.png \
	$(SRC)/lte/doc/source/figures/lte-dl-power-control.pdf \
	$(SRC)/lte/doc/source/figures/lte-ffr-scheduling.png \
	$(SRC)/lte/doc/source/figures/lte-ffr-scheduling.pdf \
	$(SRC)/lte/doc/source/figures/lte-handover-campaign-rem.pdf \
	$(SRC)/lte/doc/source/figures/lte-handover-campaign-rem.png \
	$(SRC)/lte/doc/source/figures/lte-legacy-handover-algorithm.pdf \
	$(SRC)/lte/doc/source/figures/lte-legacy-handover-algorithm.png \
	$(SRC)/uan/doc/auvmobility-classes.dia \
	$(SRC)/stats/doc/Stat-framework-arch.png \
	$(SRC)/stats/doc/Wifi-default.png \
	$(SRC)/stats/doc/dcf-overview.dia \
	$(SRC)/stats/doc/dcf-overview-with-aggregation.dia \
	$(SRC)/stats/doc/file-example.png \
	$(SRC)/stats/doc/gnuplot-aggregator.png \
	$(SRC)/stats/doc/gnuplot-example.png \
	$(SRC)/stats/doc/gnuplot-helper-example.png \
	$(SRC)/stats/doc/seventh-packet-byte-count.png \
	$(SRC)/netanim/doc/figures/PacketStatistics.png \
	$(SRC)/netanim/doc/figures/PacketStatistics.pdf \
	$(SRC)/netanim/doc/figures/NetAnim_3_105.png \
	$(SRC)/netanim/doc/figures/NetAnim_3_105.pdf \
	$(SRC)/netanim/doc/figures/Trajectory.png \
	$(SRC)/netanim/doc/figures/Trajectory.pdf \
	$(SRC)/netanim/doc/figures/NodeCountersChart.png \
	$(SRC)/netanim/doc/figures/NodeCountersChart.pdf \
	$(SRC)/netanim/doc/figures/NodeCountersTable.png \
	$(SRC)/netanim/doc/figures/NodeCountersTable.pdf \
	$(SRC)/netanim/doc/figures/RoutingTables.png \
	$(SRC)/netanim/doc/figures/RoutingTables.pdf \
	$(SRC)/netanim/doc/figures/PacketTimeline.png \
	$(SRC)/netanim/doc/figures/PacketTimeline.pdf \
	$(SRC)/spectrum/doc/spectrum-channel-phy-interface.png \
	$(SRC)/spectrum/doc/spectrum-channel-phy-interface.pdf \
	$(SRC)/spectrum/doc/spectrum-analyzer-example.eps \
	$(SRC)/lr-wpan/doc/lr-wpan-arch.dia \
	$(SRC)/lr-wpan/doc/lr-wpan-data-example.dia \
	$(SRC)/lr-wpan/doc/lr-wpan-primitives.dia \
	$(SRC)/lr-wpan/doc/802-15-4-ber.eps \
	$(SRC)/lr-wpan/doc/802-15-4-per-sens.eps \
	$(SRC)/lr-wpan/doc/802-15-4-psr-distance.eps

# specify figures from which .png and .pdf figures need to be
# generated (all dia and eps figures)
IMAGES_EPS = \
	$(FIGURES)/testbed.eps \
	$(FIGURES)/emulated-channel.eps \
	$(FIGURES)/antenna-coordinate-system.eps \
	$(FIGURES)/packet.eps \
	$(FIGURES)/node.eps \
	$(FIGURES)/buffer.eps \
	$(FIGURES)/sockets-overview.eps \
	$(FIGURES)/internet-node-send.eps \
	$(FIGURES)/internet-node-recv.eps \
	$(FIGURES)/routing.eps \
	$(FIGURES)/routing-specialization.eps \
	$(FIGURES)/WifiArchitecture.eps \
	$(FIGURES)/snir.eps \
	$(FIGURES)/WimaxArchitecture.eps \
	$(FIGURES)/dcf-overview.eps \
	$(FIGURES)/dcf-overview-with-aggregation.eps \
	$(FIGURES)/epc-ctrl-arch.eps \
	$(FIGURES)/epc-data-flow-dl.eps \
	$(FIGURES)/epc-data-flow-ul.eps \
	$(FIGURES)/epc-profiling-scenario.eps \
	$(FIGURES)/epc-topology.eps \
	$(FIGURES)/epc-topology-x2-enhanced.eps \
	$(FIGURES)/eutran-profiling-scenario.eps \
	$(FIGURES)/ff-example.eps \
	$(FIGURES)/ff-mac-saps.eps \
	$(FIGURES)/lte-arch-enb-data.eps \
	$(FIGURES)/lte-arch-enb-ctrl.eps \
	$(FIGURES)/lte-arch-ue-data.eps \
	$(FIGURES)/lte-arch-ue-ctrl.eps \
	$(FIGURES)/lte-enb-phy.eps \
	$(FIGURES)/lte-ue-phy.eps \
	$(FIGURES)/lte-epc-e2e-data-protocol-stack.eps \
	$(FIGURES)/lte-interference-test-scenario.eps \
	$(FIGURES)/lte-subframe-structure.eps \
	$(FIGURES)/lte-epc-x2-interface.eps \
	$(FIGURES)/lte-harq-architecture.eps \
	$(FIGURES)/lte-harq-processes-scheme.eps \
	$(FIGURES)/ue-meas-consumer.eps \
	$(FIGURES)/ue-meas-piecewise-motion.eps \
	$(FIGURES)/ue-meas-piecewise-a1.eps \
	$(FIGURES)/ue-meas-piecewise-a1-hys.eps \
	$(FIGURES)/lte-cell-selection-timeline.eps \
	$(FIGURES)/lte-cell-selection-scenario.eps \
	$(FIGURES)/lte-handover-target-scenario.eps \
	$(FIGURES)/lena-dual-stripe.eps \
	$(FIGURES)/lte-mcs-index.eps \
	$(FIGURES)/lenaThrTestCase1.eps \
	$(FIGURES)/lenaThrTestCase2.eps \
	$(FIGURES)/runningTime10s.eps \
	$(FIGURES)/epcRunningTime.eps \
	$(FIGURES)/propagationModel.eps \
	$(FIGURES)/simulationTime.eps \
	$(FIGURES)/epcSimulationTime.eps \
	$(FIGURES)/epcEutranRunningTime.eps \
	$(FIGURES)/profiling-memory.eps \
	$(FIGURES)/lte-rlc-implementation-model.eps \
	$(FIGURES)/lte-rlc-data-txon-dl.eps \
	$(FIGURES)/lte-rlc-data-retx-dl.eps \
	$(FIGURES)/lte-rlc-data-txon-ul.eps \
	$(FIGURES)/lte-rlc-data-retx-ul.eps \
	$(FIGURES)/lte-epc-x2-handover-seq-diagram.eps \
	$(FIGURES)/lte-epc-x2-entity-saps.eps \
	$(FIGURES)/lte-strongest-cell-handover-algorithm.eps \
	$(FIGURES)/auvmobility-classes.eps \
	$(FIGURES)/spectrum-analyzer-example.eps \
	$(FIGURES)/lr-wpan-primitives.eps \
	$(FIGURES)/lr-wpan-data-example.eps \
	$(FIGURES)/lr-wpan-arch.eps \
	$(FIGURES)/802-15-4-ber.eps \
	$(FIGURES)/802-15-4-per-sens.eps \
	$(FIGURES)/802-15-4-psr-distance.eps 

# rescale pdf figures as necessary
$(FIGURES)/testbed.pdf_width = 5in
$(FIGURES)/emulated-channel.pdf_width = 6in
$(FIGURES)/antenna-coordinate-system.pdf_width = 7cm	
$(FIGURES)/node.pdf_width = 5in
$(FIGURES)/packet.pdf_width = 4in
$(FIGURES)/buffer.pdf_width = 15cm
$(FIGURES)/sockets-overview.pdf_width = 10cm
$(FIGURES)/internet-node-send.pdf_width = 5in
$(FIGURES)/internet-node-recv.pdf_width = 5in
$(FIGURES)/routing.pdf_width = 6in
$(FIGURES)/routing-specialization.pdf_width = 5in
$(FIGURES)/snir.pdf_width = 3in
$(FIGURES)/lte-interference-test-scenario.pdf_width = 3in
$(FIGURES)/epc-ctrl-arch.pdf_width = 8cm
$(FIGURES)/epc-topology.pdf_width = 4in
$(FIGURES)/epc-topology-x2-enhanced.pdf_width = 14cm
$(FIGURES)/lte-arch-data-rrc-pdcp-rlc.pdf_width = 3in
$(FIGURES)/lte-epc-e2e-data-protocol-stack.pdf_width = 15cm
$(FIGURES)/ff-mac-saps.pdf_width = 5in
$(FIGURES)/ff-example.pdf_width = 5in
$(FIGURES)/lte-arch-enb-data.pdf_width = 6cm 
$(FIGURES)/lte-arch-enb-ctrl.pdf_width = 10cm
$(FIGURES)/lte-arch-ue-data.pdf_width = 6cm
$(FIGURES)/lte-arch-ue-ctrl.pdf_width = 10cm
$(FIGURES)/lte-rlc-implementation-model.pdf_width = 20in
$(FIGURES)/lte-rlc-data-txon-dl.pdf_width = 10cm
$(FIGURES)/lte-rlc-data-txon-ul.pdf_width = 10cm
$(FIGURES)/lte-rlc-data-retx-ul.pdf_width = 10cm
$(FIGURES)/lte-phy-interference.pdf_width = 12cm
$(FIGURES)/lte-subframe-structure.pdf_width = 2in
$(FIGURES)/mac-random-access-contention.pdf_width = 10cm
$(FIGURES)/mac-random-access-noncontention.pdf_width = 15cm
$(FIGURES)/helpers.pdf_width = 8cm
$(FIGURES)/auvmobility-classes.pdf_width = 10cm
$(FIGURES)/spectrum-analyzer-example.pdf_width = 15cm
$(FIGURES)/lr-wpan-primitives.pdf_width = 3in
$(FIGURES)/lr-wpan-arch.pdf_width = 2in

IMAGES_PNG = ${IMAGES_EPS:.eps=.png}
IMAGES_PDF = ${IMAGES_EPS:.eps=.pdf}

IMAGES = $(IMAGES_EPS) $(IMAGES_PNG) $(IMAGES_PDF)

RESCALE = ../../utils/rescale-pdf.sh

%.eps : %.dia; $(DIA) -t eps $< -e $@
%.png : %.dia; $(DIA) -t png $< -e $@
%.png : %.eps; $(CONVERT) $< $@
%.pdf : %.eps
	$(EPSTOPDF) $< -o=$@
	@if test x$($@_width) != x; then $(RESCALE) $($@_width) $@ ; fi

# You can set these variables from the command line.
SPHINXOPTS    =
SPHINXBUILD   = sphinx-build
PAPER         =
BUILDDIR      = build

# Internal variables.
PAPEROPT_a4     = -D latex_paper_size=a4
PAPEROPT_letter = -D latex_paper_size=letter
ALLSPHINXOPTS   = -d $(BUILDDIR)/doctrees $(PAPEROPT_$(PAPER)) $(SPHINXOPTS) $(SOURCETEMP) 

.PHONY: help clean html dirhtml singlehtml pickle json htmlhelp qthelp devhelp epub latex latexpdf text man changes linkcheck doctest

.NOTPARALLEL:

help:
	@echo "Please use \`make <target>' where <target> is one of"
	@echo "  html       to make standalone HTML files"
	@echo "  dirhtml    to make HTML files named index.html in directories"
	@echo "  singlehtml to make a single large HTML file"
	@echo "  pickle     to make pickle files"
	@echo "  json       to make JSON files"
	@echo "  htmlhelp   to make HTML files and a HTML help project"
	@echo "  qthelp     to make HTML files and a qthelp project"
	@echo "  devhelp    to make HTML files and a Devhelp project"
	@echo "  epub       to make an epub"
	@echo "  latex      to make LaTeX files, you can set PAPER=a4 or PAPER=letter"
	@echo "  latexpdf   to make LaTeX files and run them through pdflatex"
	@echo "  text       to make text files"
	@echo "  man        to make manual pages"
	@echo "  changes    to make an overview of all changed/added/deprecated items"
	@echo "  linkcheck  to check all external links for integrity"
	@echo "  doctest    to run all doctests embedded in the documentation (if enabled)"

copy-sources:  $(SOURCES)
	@mkdir -p $(SOURCETEMP) 
	@mkdir -p $(FIGURES) 
	@cp -r -p $(SOURCES) $(SOURCETEMP)
	@cp -r -p $(SOURCEFIGS) $(FIGURES)

clean:
	-rm -rf $(BUILDDIR)/*
	-rm -rf $(SOURCETEMP)

frag: pickle
	@if test ! -d $(BUILDDIR)/frag; then mkdir $(BUILDDIR)/frag; fi
	pushd $(BUILDDIR)/frag && ../../pickle-to-xml.py ../pickle/index.fpickle  > navigation.xml && popd
	cp -r $(BUILDDIR)/pickle/_images $(BUILDDIR)/frag

html: copy-sources $(IMAGES) 
	$(SPHINXBUILD) -b html $(ALLSPHINXOPTS) $(BUILDDIR)/html
	@echo
	@echo "Build finished. The HTML pages are in $(BUILDDIR)/html."

dirhtml: copy-sources $(IMAGES)
	$(SPHINXBUILD) -b dirhtml $(ALLSPHINXOPTS) $(BUILDDIR)/dirhtml
	@echo
	@echo "Build finished. The HTML pages are in $(BUILDDIR)/dirhtml."

singlehtml: copy-sources $(IMAGES)
	$(SPHINXBUILD) -b singlehtml $(ALLSPHINXOPTS) $(BUILDDIR)/singlehtml
	@echo
	@echo "Build finished. The HTML page is in $(BUILDDIR)/singlehtml."

pickle: copy-sources $(IMAGES)
	$(SPHINXBUILD) -b pickle $(ALLSPHINXOPTS) $(BUILDDIR)/pickle
	@echo
	@echo "Build finished; now you can process the pickle files."

json: copy-sources $(IMAGES)
	$(SPHINXBUILD) -b json $(ALLSPHINXOPTS) $(BUILDDIR)/json
	@echo
	@echo "Build finished; now you can process the JSON files."

htmlhelp: copy-sources $(IMAGES)
	$(SPHINXBUILD) -b htmlhelp $(ALLSPHINXOPTS) $(BUILDDIR)/htmlhelp
	@echo
	@echo "Build finished; now you can run HTML Help Workshop with the" \
	      ".hhp project file in $(BUILDDIR)/htmlhelp."

qthelp: copy-sources $(IMAGES)
	$(SPHINXBUILD) -b qthelp $(ALLSPHINXOPTS) $(BUILDDIR)/qthelp
	@echo
	@echo "Build finished; now you can run "qcollectiongenerator" with the" \
	      ".qhcp project file in $(BUILDDIR)/qthelp, like this:"
	@echo "# qcollectiongenerator $(BUILDDIR)/qthelp/ns-3.qhcp"
	@echo "To view the help file:"
	@echo "# assistant -collectionFile $(BUILDDIR)/qthelp/ns-3.qhc"

devhelp: copy-sources $(IMAGES)
	$(SPHINXBUILD) -b devhelp $(ALLSPHINXOPTS) $(BUILDDIR)/devhelp
	@echo
	@echo "Build finished."
	@echo "To view the help file:"
	@echo "# mkdir -p $$HOME/.local/share/devhelp/ns-3"
	@echo "# ln -s $(BUILDDIR)/devhelp $$HOME/.local/share/devhelp/ns-3"
	@echo "# devhelp"

epub: copy-sources $(IMAGES)
	$(SPHINXBUILD) -b epub $(ALLSPHINXOPTS) $(BUILDDIR)/epub
	@echo
	@echo "Build finished. The epub file is in $(BUILDDIR)/epub."

latex: copy-sources $(IMAGES)
	$(SPHINXBUILD) -b latex $(ALLSPHINXOPTS) $(BUILDDIR)/latex
	@echo
	@echo "Build finished; the LaTeX files are in $(BUILDDIR)/latex."
	@echo "Run \`make' in that directory to run these through (pdf)latex" \
	      "(use \`make latexpdf' here to do that automatically)."

latexpdf: copy-sources $(IMAGES)
	$(SPHINXBUILD) -b latex $(ALLSPHINXOPTS) $(BUILDDIR)/latex
	@echo "Running LaTeX files through pdflatex..."
	make -C $(BUILDDIR)/latex all-pdf
	@echo "pdflatex finished; the PDF files are in $(BUILDDIR)/latex."

text: copy-sources $(IMAGES)
	$(SPHINXBUILD) -b text $(ALLSPHINXOPTS) $(BUILDDIR)/text
	@echo
	@echo "Build finished. The text files are in $(BUILDDIR)/text."

man: copy-sources $(IMAGES)
	$(SPHINXBUILD) -b man $(ALLSPHINXOPTS) $(BUILDDIR)/man
	@echo
	@echo "Build finished. The manual pages are in $(BUILDDIR)/man."

changes: copy-sources $(IMAGES)
	$(SPHINXBUILD) -b changes $(ALLSPHINXOPTS) $(BUILDDIR)/changes
	@echo
	@echo "The overview file is in $(BUILDDIR)/changes."

linkcheck: copy-sources $(IMAGEs)
	$(SPHINXBUILD) -b linkcheck $(ALLSPHINXOPTS) $(BUILDDIR)/linkcheck
	@echo
	@echo "Link check complete; look for any errors in the above output " \
	      "or in $(BUILDDIR)/linkcheck/output.txt."

doctest: copy-sources $(IMAGES)
	$(SPHINXBUILD) -b doctest $(ALLSPHINXOPTS) $(BUILDDIR)/doctest
	@echo "Testing of doctests in the sources finished, look at the " \
	      "results in $(BUILDDIR)/doctest/output.txt."


