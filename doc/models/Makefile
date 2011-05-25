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
	$(SRC)/aodv/doc/aodv.rst \
	$(SRC)/applications/doc/applications.rst \
	$(SRC)/bridge/doc/bridge.rst \
	$(SRC)/click/doc/click.rst \
	$(SRC)/csma/doc/csma.rst \
	$(SRC)/dsdv/doc/dsdv.rst \
	$(SRC)/mpi/doc/distributed.rst \
	$(SRC)/energy/doc/energy.rst \
	$(SRC)/emu/doc/emu.rst \
	$(SRC)/tap-bridge/doc/tap.rst \
	$(SRC)/mesh/doc/mesh.rst \
	$(SRC)/lte/doc/lte.rst \
	$(SRC)/propagation/doc/propagation.rst \
	$(SRC)/network/doc/network-overview.rst \
	$(SRC)/network/doc/packets.rst \
	$(SRC)/network/doc/sockets-api.rst \
	$(SRC)/network/doc/simple.rst \
	$(SRC)/internet/doc/internet-stack.rst \
	$(SRC)/internet/doc/ipv4.rst \
	$(SRC)/internet/doc/ipv6.rst \
	$(SRC)/internet/doc/routing-overview.rst \
	$(SRC)/internet/doc/tcp.rst \
	$(SRC)/olsr/doc/olsr.rst \
	$(SRC)/openflow/doc/openflow-switch.rst \
	$(SRC)/point-to-point/doc/point-to-point.rst \
	$(SRC)/wifi/doc/wifi.rst \
	$(SRC)/wimax/doc/wimax.rst \
	$(SRC)/uan/doc/uan.rst \
	$(SRC)/stats/doc/statistics.rst \
	$(SRC)/netanim/doc/animation.rst \
	$(SRC)/flow-monitor/doc/flow-monitor.rst \

# list all model library figure files that need to be copied to 
# $SOURCETEMP/figures.  Either a .dia or both a .pdf and .png
SOURCEFIGS = \
	figures/testbed.dia \
	figures/emulated-channel.dia \
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
	$(SRC)/lte/doc/lte-transmission.png \
	$(SRC)/lte/doc/lte-transmission.pdf \
        $(SRC)/uan/doc/auvmobility-classes.dia \
	$(SRC)/netanim/doc/animation-dumbbell.png \
	$(SRC)/netanim/doc/animation-dumbbell.pdf \

# specify figures for build process (all dia figures)
IMAGES_EPS = \
	$(FIGURES)/testbed.eps \
	$(FIGURES)/emulated-channel.eps \
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
        $(FIGURES)/auvmobility-classes.eps \

# rescale figures as necessary
$(FIGURES)/testbed.pdf_width = 5in
$(FIGURES)/emulated-channel.pdf_width = 6in
$(FIGURES)/node.pdf_width = 5in
$(FIGURES)/packet.pdf_width = 4in
$(FIGURES)/buffer.pdf_width = 15cm
$(FIGURES)/sockets-overview.pdf_width = 10cm
$(FIGURES)/internet-node-send.pdf_width = 5in
$(FIGURES)/internet-node-recv.pdf_width = 5in
$(FIGURES)/routing.pdf_width = 6in
$(FIGURES)/routing-specialization.pdf_width = 5in
$(FIGURES)/snir.pdf_width = 3in
$(FIGURES)/lte-transmission.pdf_width = 3in
$(FIGURES)/auvmobility-classes.pdf_width = 10cm

IMAGES_PNG = ${IMAGES_EPS:.eps=.png}
IMAGES_PDF = ${IMAGES_EPS:.eps=.pdf}

IMAGES = $(IMAGES_EPS) $(IMAGES_PNG) $(IMAGES_PDF)

%.eps : %.dia; $(DIA) -t eps $< -e $@
%.png : %.dia; $(DIA) -t png $< -e $@
%.pdf : %.eps; $(EPSTOPDF) $< -o=$@; if test x$($@_width) != x; then TMPFILE=`mktemp`; ./rescale-pdf.sh $($@_width) $@ $${TMPFILE} && mv $${TMPFILE} $@; fi

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
	@rm -rf $(SOURCETEMP)
	@mkdir -p $(SOURCETEMP) 
	@mkdir -p $(FIGURES) 
	@cp -r $(SOURCES) $(SOURCETEMP)
	@cp -r $(SOURCEFIGS) $(FIGURES)

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


