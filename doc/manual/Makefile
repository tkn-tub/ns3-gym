TEXI2HTML = texi2html
TEXI2PDF = texi2dvi --pdf
EPSTOPDF = epstopdf
DIA = dia
CONVERT = convert
CSS = --css-include=manual.css
SPLIT = --split section

DIA_SOURCES = \
	internet-node-send.dia \
	internet-node-recv.dia \
	packet.dia \
	node.dia \
	buffer.dia \
	sockets-overview.dia \
	testbed.dia \
	emulated-channel.dia

DIA_EPS = ${DIA_SOURCES:.dia=.eps}
DIA_PNG = ${DIA_SOURCES:.dia=.png}
DIA_PDF = ${DIA_SOURCES:.dia=.pdf}

all:  images html split-html pdf

images:
	cd figures/; $(DIA) -t png $(DIA_SOURCES) 
	cd figures/; $(DIA) -t eps $(DIA_SOURCES) 
	cd figures/; $(foreach FILE,$(DIA_EPS),$(EPSTOPDF) $(FILE);)

html: images
	$(TEXI2HTML) ${CSS} manual.texi

split-html: images
	$(TEXI2HTML) ${CSS} ${SPLIT} manual.texi

pdf: images
	$(TEXI2PDF) manual.texi

figures-clean:
	cd figures/; rm -rf $(DIA_EPS); rm -rf $(DIA_PNG); rm -rf $(DIA_PDF)

clean: figures-clean
	rm -rf manual.aux manual.cp manual.cps manual.fn manual.ky manual.pg manual.tp manual.vr manual.toc manual.log manual.pdf manual.html manual/ 
