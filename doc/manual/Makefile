TEXI2HTML = texi2html
TEXI2PDF = texi2dvi --pdf
EPSTOPDF = epstopdf
DIA = dia
CONVERT = convert
CSS = --css-include=manual.css
SPLIT = --split section

FIGURES = figures
DOC_FIGURES = ..
VPATH = $(FIGURES)

IMAGES_EPS = \
	$(FIGURES)/internet-node-send.eps \
	$(FIGURES)/internet-node-recv.eps \
	$(FIGURES)/packet.eps \
	$(FIGURES)/node.eps \
	$(FIGURES)/buffer.eps \
	$(FIGURES)/sockets-overview.eps \
	$(FIGURES)/software-organization.eps \
	$(FIGURES)/routing.eps \
	$(FIGURES)/routing-specialization.eps \
	$(FIGURES)/testbed.eps \
	$(FIGURES)/emulated-channel.eps \
	$(FIGURES)/snir.eps \
	$(DOC_FIGURES)/WifiArchitecture.eps \
	$(DOC_FIGURES)/WimaxArchitecture.eps

$(FIGURES)/node.pdf_width = 5in
$(FIGURES)/testbed.pdf_width = 5in
$(FIGURES)/emulated-channel.pdf_width = 6in
$(FIGURES)/internet-node-send.pdf_width = 5in
$(FIGURES)/internet-node-recv.pdf_width = 5in
$(FIGURES)/software-organization.pdf_width = 5in
$(FIGURES)/packet.pdf_width = 4in
$(FIGURES)/buffer.pdf_width = 15cm
$(FIGURES)/routing.pdf_width = 6in
$(FIGURES)/routing-specialization.pdf_width = 5in
$(FIGURES)/sockets-overview.pdf_width = 10cm
$(FIGURES)/snir.pdf_width = 3in
IMAGES_PNG = ${IMAGES_EPS:.eps=.png}
IMAGES_PDF = ${IMAGES_EPS:.eps=.pdf}

IMAGES = $(IMAGES_EPS) $(IMAGES_PNG) $(IMAGES_PDF)

CHAPTERS = \
	manual.texi \
	animation.texi \
	attributes.texi \
	bridge.texi \
	callbacks.texi \
	csma.texi \
	emulation.texi \
	emu.texi \
	flow-monitor.texi \
	helpers.texi \
	internet.texi \
	ipv4.texi \
	ipv6.texi \
	log.texi \
	manual.texi \
	mesh.texi \
	names.texi \
	new-models.texi \
	node.texi \
	objects.texi \
	other.texi \
	output.texi \
	packets.texi \
	point-to-point.texi \
	python.texi \
	random.texi \
	realtime.texi \
  distributed.texi \
	routing.texi \
	simple.texi \
	sockets.texi \
	statistics.texi \
	tap.texi \
	tcp.texi \
	tracing.texi \
	troubleshoot.texi \
	wifi.texi

%.eps : %.dia; $(DIA) -t eps $< -e $@
%.png : %.dia; $(DIA) -t png $< -e $@
%.pdf : %.eps; $(EPSTOPDF) $< -o=$@; if test x$($@_width) != x; then TMPFILE=`mktemp`; ./rescale-pdf.sh $($@_width) $@ $${TMPFILE} && mv $${TMPFILE} $@; fi

all:  $(IMAGES) version manual.pdf manual.html manual/manual.html

manual.pdf: version $(IMAGES) $(CHAPTERS)
	$(TEXI2PDF) manual.texi

manual.html: version $(IMAGES) $(CHAPTERS)
	$(TEXI2HTML) ${CSS} manual.texi

manual/manual.html: version $(IMAGES) $(CHAPTERS)
	$(TEXI2HTML) ${CSS} ${SPLIT} --output manual manual.texi

figures-clean:
	rm -rf $(IMAGES)

version:
	echo -n "ns-" > VERSION-PREFIX; cat VERSION-PREFIX ../../VERSION > VERSION; rm -rf VERSION-PREFIX

texi-clean: 	
	rm -rf manual.aux manual.cp manual.cps manual.fn manual.ky manual.pg manual.tp
	rm -rf manual.vr manual.toc manual.log manual.pdf manual.html manual/ VERSION 

clean:	figures-clean texi-clean
