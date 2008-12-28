TEXI2HTML = texi2html
TEXI2PDF = texi2dvi --pdf
EPSTOPDF = epstopdf
DIA = dia
CONVERT = convert
CSS = --css-include=manual.css
SPLIT = --split section

FIGURES = figures
VPATH = $(FIGURES)

IMAGES_EPS = \
	$(FIGURES)/internet-node-send.eps \
	$(FIGURES)/internet-node-recv.eps \
	$(FIGURES)/packet.eps \
	$(FIGURES)/node.eps \
	$(FIGURES)/buffer.eps \
	$(FIGURES)/sockets-overview.eps \
	$(FIGURES)/testbed.eps \
	$(FIGURES)/emulated-channel.eps \
	$(FIGURES)/snir.eps

IMAGES_PNG = ${IMAGES_EPS:.eps=.png}
IMAGES_PDF = ${IMAGES_EPS:.eps=.pdf}

IMAGES = $(IMAGES_EPS) $(IMAGES_PNG) $(IMAGES_PDF)

CHAPTERS = \
	manual.texi \
	attributes.texi \
	callbacks.texi \
	csma.texi \
	emulation.texi \
	node.texi \
	objects.texi \
	other.texi \
	output.texi \
	packets.texi \
	point-to-point.texi \
	random.texi \
	realtime.texi \
	routing.texi \
	sockets.texi \
	statistics.texi \
	troubleshoot.texi \
	wifi.texi

%.eps : %.dia; $(DIA) -t eps $< -e $@
%.png : %.dia; $(DIA) -t png $< -e $@
%.pdf : %.eps; $(EPSTOPDF) $< -o=$@

all:  $(IMAGES) manual.pdf manual.html manual/manual.html

manual.pdf: $(IMAGES) $(CHAPTERS)
	$(TEXI2PDF) manual.texi

manual.html: $(IMAGES) $(CHAPTERS)
	$(TEXI2HTML) ${CSS} manual.texi

manual/manual.html: $(IMAGES) $(CHAPTERS)
	$(TEXI2HTML) ${CSS} ${SPLIT} manual.texi

figures-clean:
	rm -rf $(IMAGES)

clean: 	figures-clean
	rm -rf manual.aux manual.cp manual.cps manual.fn manual.ky manual.pg 
	rm -rf manual.tp manual.vr manual.toc manual.log manual.pdf manual.html manual/ 
