EPSTOPDF = epstopdf
DIA = dia
SEQDIAG = seqdiag
CONVERT = convert -density 250


SOURCE = source
FIGURES = $(SOURCE)/figures

# specify dia figures from which .png and .pdf figures need to be built

IMAGES_DIA = \
	$(FIGURES)/epc-data-flow-dl.dia \
	$(FIGURES)/epc-data-flow-ul.dia \
	$(FIGURES)/epc-profiling-scenario.dia \
	$(FIGURES)/epc-topology.dia \
	$(FIGURES)/eutran-profiling-scenario.dia \
	$(FIGURES)/ff-example.dia \
	$(FIGURES)/ff-mac-saps.dia \
	$(FIGURES)/lte-arch-data-rrc-pdcp-rlc.dia \
	$(FIGURES)/lte-enb-architecture.dia \
	$(FIGURES)/lte-epc-e2e-data-protocol-stack.dia \
	$(FIGURES)/lte-interference-test-scenario.dia \
	$(FIGURES)/lte-ue-architecture.dia


# specify eps figures from which .png and .pdf figures need to be built

IMAGES_EPS = \
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
	$(FIGURES)/lte-rlc-data-retx-ul.eps 


# rescale pdf figures as necessary
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
$(FIGURES)/lte-interference-test-scenario.pdf_width = 3in
$(FIGURES)/epc-topology.pdf_width = 4in
$(FIGURES)/lte-arch-data-rrc-pdcp-rlc.pdf_width = 3in
$(FIGURES)/lte-epc-e2e-data-protocol-stack.pdf_width = 15cm
$(FIGURES)/ff-mac-saps.pdf_width = 5in
$(FIGURES)/ff-example.pdf_width = 5in
$(FIGURES)/lte-rlc-implementation-model.pdf_width = 20in
$(FIGURES)/lte-rlc-data-txon-dl.pdf_width = 10cm
$(FIGURES)/lte-rlc-data-txon-ul.pdf_width = 10cm
$(FIGURES)/lte-rlc-data-retx-ul.pdf_width = 10cm
$(FIGURES)/lte-phy-interference.pdf_width = 12cm
$(FIGURES)/auvmobility-classes.pdf_width = 10cm


IMAGES_SEQDIAG = \
	$(FIGURES)/lte-phy-interference.seqdiag \
	$(FIGURES)/helpers.seqdiag

IMAGES_NOBUILD = $(FIGURES)/fading_pedestrian.png \
	$(FIGURES)/fading_vehicular.png \
	$(FIGURES)/fading_urban_3kmph.png \
	$(FIGURES)/fading_pedestrian.pdf \
	$(FIGURES)/fading_vehicular.pdf \
	$(FIGURES)/fading_urban_3kmph.pdf \
	$(FIGURES)/MCS_1_4.pdf \
	$(FIGURES)/MCS_1_4.png \
	$(FIGURES)/MCS_5_8.pdf \
	$(FIGURES)/MCS_5_8.png \
	$(FIGURES)/MCS_9_12.pdf \
	$(FIGURES)/MCS_9_12.png \
	$(FIGURES)/MCS_13_16.pdf \
	$(FIGURES)/MCS_13_16.png \
	$(FIGURES)/MCS_17_20.pdf \
	$(FIGURES)/MCS_17_20.png \
	$(FIGURES)/MCS_21_24.pdf \
	$(FIGURES)/MCS_21_24.png \
	$(FIGURES)/MCS_25_28.pdf \
	$(FIGURES)/MCS_25_28.png \
	$(FIGURES)/MCS_29_29.pdf \
	$(FIGURES)/MCS_29_29.png \
	$(FIGURES)/MCS_2_test.png \
	$(FIGURES)/MCS_2_test.pdf \
	$(FIGURES)/MCS_12_test.png \
	$(FIGURES)/MCS_12_test.pdf \
	$(FIGURES)/MCS_16_test.png \
	$(FIGURES)/MCS_16_test.pdf \
	$(FIGURES)/lte-phy-interference.png \
	$(FIGURES)/lte-phy-interference.pdf \
	$(FIGURES)/helpers.png \
	$(FIGURES)/helpers.pdf \
	$(FIGURES)/miesm_scheme.pdf \
	$(FIGURES)/miesm_scheme.png \
	${IMAGES_SEQDIAG:.seqdiag=.png} \
	${IMAGES_SEQDIAG:.seqdiag=.pdf}

IMAGES_BUILD = \
	${IMAGES_DIA:.dia=.eps} \
	${IMAGES_DIA:.dia=.png} \
	${IMAGES_DIA:.dia=.pdf} \
	${IMAGES_EPS:.eps=.png} \
	${IMAGES_EPS:.eps=.pdf} 



IMAGES = $(IMAGES_NOBUILD) $(IMAGES_BUILD)

%.eps : %.dia; $(DIA) -t eps $< -e $@
%.png : %.dia; $(DIA) -t png $< -e $@
%.png : %.seqdiag; $(SEQDIAG) -Tpng -o $@ $< 
%.png : %.eps; $(CONVERT) $< $@
%.pdf : %.seqdiag
	$(SEQDIAG) -Tpdf -o $@ $<
	if test x$($@_width) != x; then ./rescale-pdf.sh $($@_width) $@ ; fi
%.pdf : %.eps
	$(EPSTOPDF) $< -o=$@
	if test x$($@_width) != x; ./rescale-pdf.sh $($@_width) $@ ; fi


%.png : %.eps; $(CONVERT) $< $@
%.pdf : %.eps
	$(EPSTOPDF) $< -o=$@
	if test x$($@_width) != x; then ./rescale-pdf.sh $($@_width) $@ ; fi

# You can set these variables from the command line.
SPHINXOPTS    =
SPHINXBUILD   = sphinx-build
PAPER         =
BUILDDIR      = build

# Internal variables.
PAPEROPT_a4     = -D latex_paper_size=a4
PAPEROPT_letter = -D latex_paper_size=letter
ALLSPHINXOPTS   = -d $(BUILDDIR)/doctrees $(PAPEROPT_$(PAPER)) $(SPHINXOPTS) $(SOURCE) 

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

clean:
	-rm -rf $(BUILDDIR)/*
	-rm -f $(IMAGES_BUILD)

frag: pickle
	@if test ! -d $(BUILDDIR)/frag; then mkdir $(BUILDDIR)/frag; fi
	pushd $(BUILDDIR)/frag && ../../pickle-to-xml.py ../pickle/index.fpickle  > navigation.xml && popd
	cp -r $(BUILDDIR)/pickle/_images $(BUILDDIR)/frag

html:  $(IMAGES) 
	$(SPHINXBUILD) -b html $(ALLSPHINXOPTS) $(BUILDDIR)/html
	@echo
	@echo "Build finished. The HTML pages are in $(BUILDDIR)/html."

dirhtml:  $(IMAGES)
	$(SPHINXBUILD) -b dirhtml $(ALLSPHINXOPTS) $(BUILDDIR)/dirhtml
	@echo
	@echo "Build finished. The HTML pages are in $(BUILDDIR)/dirhtml."

singlehtml:  $(IMAGES)
	$(SPHINXBUILD) -b singlehtml $(ALLSPHINXOPTS) $(BUILDDIR)/singlehtml
	@echo
	@echo "Build finished. The HTML page is in $(BUILDDIR)/singlehtml."

pickle:  $(IMAGES)
	$(SPHINXBUILD) -b pickle $(ALLSPHINXOPTS) $(BUILDDIR)/pickle
	@echo
	@echo "Build finished; now you can process the pickle files."

json:  $(IMAGES)
	$(SPHINXBUILD) -b json $(ALLSPHINXOPTS) $(BUILDDIR)/json
	@echo
	@echo "Build finished; now you can process the JSON files."

htmlhelp:  $(IMAGES)
	$(SPHINXBUILD) -b htmlhelp $(ALLSPHINXOPTS) $(BUILDDIR)/htmlhelp
	@echo
	@echo "Build finished; now you can run HTML Help Workshop with the" \
	      ".hhp project file in $(BUILDDIR)/htmlhelp."

qthelp:  $(IMAGES)
	$(SPHINXBUILD) -b qthelp $(ALLSPHINXOPTS) $(BUILDDIR)/qthelp
	@echo
	@echo "Build finished; now you can run "qcollectiongenerator" with the" \
	      ".qhcp project file in $(BUILDDIR)/qthelp, like this:"
	@echo "# qcollectiongenerator $(BUILDDIR)/qthelp/ns-3.qhcp"
	@echo "To view the help file:"
	@echo "# assistant -collectionFile $(BUILDDIR)/qthelp/ns-3.qhc"

devhelp:  $(IMAGES)
	$(SPHINXBUILD) -b devhelp $(ALLSPHINXOPTS) $(BUILDDIR)/devhelp
	@echo
	@echo "Build finished."
	@echo "To view the help file:"
	@echo "# mkdir -p $$HOME/.local/share/devhelp/ns-3"
	@echo "# ln -s $(BUILDDIR)/devhelp $$HOME/.local/share/devhelp/ns-3"
	@echo "# devhelp"

epub:  $(IMAGES)
	$(SPHINXBUILD) -b epub $(ALLSPHINXOPTS) $(BUILDDIR)/epub
	@echo
	@echo "Build finished. The epub file is in $(BUILDDIR)/epub."

latex:  $(IMAGES)
	$(SPHINXBUILD) -b latex $(ALLSPHINXOPTS) $(BUILDDIR)/latex
	@echo
	@echo "Build finished; the LaTeX files are in $(BUILDDIR)/latex."
	@echo "Run \`make' in that directory to run these through (pdf)latex" \
	      "(use \`make latexpdf' here to do that automatically)."

latexpdf:  $(IMAGES)
	$(SPHINXBUILD) -b latex $(ALLSPHINXOPTS) $(BUILDDIR)/latex
	@echo "Running LaTeX files through pdflatex..."
	make -C $(BUILDDIR)/latex all-pdf
	@echo "pdflatex finished; the PDF files are in $(BUILDDIR)/latex."

text:  $(IMAGES)
	$(SPHINXBUILD) -b text $(ALLSPHINXOPTS) $(BUILDDIR)/text
	@echo
	@echo "Build finished. The text files are in $(BUILDDIR)/text."

man:  $(IMAGES)
	$(SPHINXBUILD) -b man $(ALLSPHINXOPTS) $(BUILDDIR)/man
	@echo
	@echo "Build finished. The manual pages are in $(BUILDDIR)/man."

changes:  $(IMAGES)
	$(SPHINXBUILD) -b changes $(ALLSPHINXOPTS) $(BUILDDIR)/changes
	@echo
	@echo "The overview file is in $(BUILDDIR)/changes."

linkcheck:  $(IMAGEs)
	$(SPHINXBUILD) -b linkcheck $(ALLSPHINXOPTS) $(BUILDDIR)/linkcheck
	@echo
	@echo "Link check complete; look for any errors in the above output " \
	      "or in $(BUILDDIR)/linkcheck/output.txt."

doctest:  $(IMAGES)
	$(SPHINXBUILD) -b doctest $(ALLSPHINXOPTS) $(BUILDDIR)/doctest
	@echo "Testing of doctests in the sources finished, look at the " \
	      "results in $(BUILDDIR)/doctest/output.txt."

